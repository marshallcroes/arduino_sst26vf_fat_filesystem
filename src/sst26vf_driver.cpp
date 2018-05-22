/* 
    Copyright (c) 2018, Marshall Croes

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include "sst26vf_driver.h"

namespace sst26vf {
namespace detail {

//------ SPI handler ------

template <typename T>
static void spi_handler::write(const T& self, char data)
{
        write(self, data, 1);
}

template <typename T>
static void spi_handler::write(const T& self, char* data, uint16_t len)
{
        byte c;

        self.m_spi->beginTransaction(SPISettings(F_CPU, MSBFIRST, SPI_MODE0));
        while (len--) {
                c = *data;
                data++;
                self.m_spi->transfer(c);
        }

        self.m_spi->endTransaction();
}

template <typename T>
static void spi_handler::read(const T& self, uint8_t* data)
{
        read(self, data, 1);
}

template <typename T>
static void spi_handler::read(const T& self, uint8_t* data, uint16_t len)
{
        uint8_t x = 0;

        self.m_spi->beginTransaction(SPISettings(F_CPU, MSBFIRST, SPI_MODE0));
        while (len--) {
                x = self.m_spi->transfer(SST26VF_CMD_NOP);
                *data = x;
                data++;
        }

        self.m_spi->endTransaction();
}
} // namespace detail
using detail::spi_handler;


//------ Flash drive ------

flash_driver::flash_driver(uint8_t ss)
        : m_ss(ss),
          m_spi(&SPI)
{
        digitalWrite(m_ss, HIGH);
        pinMode(m_ss, OUTPUT);
}

bool flash_driver::begin()
{
        m_spi->begin();
}

uint8_t flash_driver::read_status()
{
        digitalWrite(m_ss, LOW);
        spi_handler::write(*this, SST26VF_CMD_RDSR);

        uint8_t status;
        spi_handler::read(*this, &status);
        digitalWrite(m_ss, HIGH);

        return status;
}

bool flash_driver::write_enable(bool enable)
{
        digitalWrite(m_ss, LOW);
        spi_handler::write(*this,
                enable ? SST26VF_CMD_WREN : SST26VF_CMD_WRDI);
        digitalWrite(m_ss, HIGH);

        asm __volatile__ ("nop");

        return true;
}

bool flash_driver::wait_until_ready(uint32_t timeout)
{
        uint8_t status;

        // Read status bit to check if busy
        while (timeout > 0) {
                status = read_status()
                        & SST26VF_STAT_BUSY | SST26VF_STAT_BUSY2;
                if (!status)
                        return false;

                asm __volatile__ ("nop");
                timeout--;
        }

        return true;
}

uint32_t flash_driver::write_buffer(uint32_t addr, uint8_t* buf, uint32_t len)
{
        // If the data is only on one page we can take a shortcut
        if ((addr % m_page_size) + len <= m_page_size)
                return write_page(addr, buf, len);

        // Block spans multiple pages
        uint32_t bytes_written = 0;
        uint32_t buff_offset = 0;

        while(len > 0) {
                // Figure out how many bytes need to be written to this page
                uint32_t bytes_to_write = m_page_size - (addr % m_page_size);

                // Write the current page
                uint32_t results = write_page(addr, (buf + buf_offset), bytes_to_write);
                bytes_written += results;

                // Abort if we returned an error
                if (!results)
                        return bytes_written;

                // Adjust address and len, and buffer offset
                addr += bytes_to_write;
                len  -= bytes_to_write;

                buf_offset += bytes_to_write;

                // If the next page is the last one, write it and exit
                // if (len <= m_page_size) {
                //         // Write the last frame and then quit
                //         results = write_page(addr, (buf + buf_offset), len);
                //         bytes_written += results;

                //         // Abort if we returned an error
                //         if (!results)
                //                 return bytes_written;

                //         // set len to zero to gracefully exit loop
                //         len = 0;
                // }
        }

        // Return the number of bytes written
        return bytes_written;
}

uint32_t flash_driver::write_page(uint32_t addr, uint8_t* buf, uint32_t len)
{
        // Make sure the address is valid
        if (addr >= SST26VF_MAX_ADDR)
                return 0;

        // Make sure that the supplied data is no larger than the page size
        if (len > m_page_size)
                return 0;

        // Make sure that the data won't wrap around to the beginning of the sector
        if ((addr % m_page_size) + len > m_page_size)
                return 0;

        // Wait until the device is ready or a timeout occurs
        if (wait_until_ready())
                return 0;

        // Make sure the chip is write enabled
        write_enable(true);

        // Make sure the write enable latch is actually set
        uint8_t status = read_status() & SPIFLASH_STAT_WRTEN;
        if (!status)
                // Throw a write protection error (write enable latch not set)
                return 0;

        digitalWrite(m_ss, LOW);

        // Send page progam command, 24-bit address
        uint32_t cmd_list[] = {
                SST26VF_CMD_PP,
                (addr >> 16) & 0xff,
                (addr >> 8)  & 0xff,
                (addr)       & 0xff
        };

        // set lower 8 bits to 0 to avoid wrapping
        if (len == m_page_size)
                cmd_list[3] = 0;

        spi_handler::write(*this, &cmd_list, 4);

        // Transfer data
        spi_handler::write(*this, &buf, len);

        // Write only occurs after the CS line is de-asserted
        digitalWrite(m_ss, HIGH);

        // Wait until the device is ready or a timeout occurs
        if (wait_until_ready(5))
                return 0;

        return len;
}

uint32_t flash_driver::read_buffer(uint32_t addr, uint8_t* buf, uint32_t len)
{
        // Make sure the address is valid
        if (addr >= m_total_size)
                return 0;

        // Wait until the device is ready or exit if timeout occurs
        if (wait_until_ready())
                return 0;

        // Send the read data command
        uint8_t cmd_list[] = {
                SST26VF_CMD_READ,
                (addr >> 16) & 0xff,
                (addr >> 8)  & 0xff,
                (addr)       & 0xff,
        };

        digitalWrite(m_ss, LOW);
        spi_handler::write(*this, &cmd_list, 4);

        if ((addr+len) > m_total_size) {
                len = m_total_size - addr;
        }

        // Fill response buffer
        spi_handler::read(*this, &buf, len);

        digitalWrite(m_ss, HIGH);

        // Return bytes read
        return len;
}

bool flash_driver::erase_chip()
{
        if (wait_until_ready()) return false;

        write_enable(true);

        uint8_t status = read_status() & SST26VF_STAT_WEL;
        if (!status) return false;

        digitalWrite(m_ss, LOW);
        spi_handler::write(*this, SST26VF_CMD_CE);
        digitalWrite(m_ss, HIGH);

        if (wait_until_ready(12000)) return false;

        return true;
}

bool flash_driver::erase_sector(uint32_t sector_num)
{
        if (wait_until_ready()) return false;

        write_enable(true);

        uint8_t status = read_status() & SST26VF_STAT_WEL;
        if (!status) return falsle;

        uint32_t addr = sector_num * SST26VF_SECTOR_SIZE;
        uint8_t cmd_list[] = {
                SST26VF_CMD_SE,
                (addr >> 16) & 0xff,
                (addr >> 8)  & 0xff,
                (addr)       & 0xff,
        };

        digitalWrite(m_ss, LOW);
        spi_handler::write(*this, &cmd_list, 4);
        digitalWrite(m_ss, HIGH);

        wait_until_ready(500);

        return true;
}

bool flash_driver::erase_block(uint32_t block_num)
{
        if (wait_until_ready()) return false;

        write_enable(true);

        uint8_t status = read_status() & SST26VF_STAT_WEL;
        if (!status) return falsle;

        // Has 72 variance size blocks (8*8KB, 2*32KB, 62*64KB)
        // See datasheet for memory map
        uint32_t addr = sector_num * SST26VF_SECTOR_SIZE;
        if (block_num < 4) {
                addr = block_num * SST26_BLOCK_S_SIZE;
        } else if (block_num == 4 || block_num == 67) {
                addr = 4 * SST26VF_BLOCK_S_SIZE + SST26VF_BLOCK_M_SIZE;
                addr += (block_num - 66) * 62 * SST26VF_BLOCK_L_SIZE;
        } else if (block_num > 4 && block_num < 67) {
                addr = 4 * SST26VF_BLOCK_S_SIZE + SST26VF_BLOCK_M_SIZE;
                addr += (block_num - 4) * SST26VF_BLOCK_L_SIZE;
        } else if (block_num > 67) {
                addr = (block_num - 64) * SST26VF_BLOCK_S_SIZE;
                addr += 2 * SST26VF_BLOCK_M_SIZE;
                addr += 62 * SST26VF_BLOCK_L_SIZE;
        }

        uint8_t cmd_list[] = {
                SST26VF_CMD_SE,
                (addr >> 16) & 0xff,
                (addr >> 8)  & 0xff,
                (addr)       & 0xff,
        };

        digitalWrite(m_ss, LOW);
        spi_handler::write(*this, &cmd_list, 4);
        digitalWrite(m_ss, HIGH);

        wait_until_ready(2000);

        return true;
}

// --------- Debug functions -----------

// void flash_drive::get_id(uint8_t* buffer)
{
        //
// }

void flash_driver::get_manufacturer_info(uint8_t* mfr_id,uint8_t* dev_type, uint8_t* dev_id)
{
        digitalWrite(m_ss, LOW);
        spi_handler::write(*this, SST26VF_CMD_JEDECID); 

        uint8_t buf[3];
        spi_handler::read(*this, &buf, 3);

        *mfr_id = buf[0];
        *dev_type = buf[1];
        *dev_id = buf[2];

        digitalWrite(m_ss, HIGH);
}
} // namespace sst26vf






















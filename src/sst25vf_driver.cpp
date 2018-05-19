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

#include "sst25vf_driver.h"

using namespace sst25vf;

//------ SPI handler ------

template <typename T>
static void flash_drive::spi_handler::write(const T& self, char data)
{
        spi_handler::write(self, data, 1);
}

template <typename T>
static void flash_drive::spi_handler::write(const T& self, char* data, uint16_t len)
{
        byte c;

        self.m_spi->beginTransaction(SPISettings(SPIFLASH_SPI_SPEED, MSBFIRST, SPI_MODE0));
        while (length--) {
                c = *data;
                data++;
                self.m_spi->transfer(c);
        }

        self.m_spi->endTransaction();
}

template <typename T>
static void flash_drive::spi_handler::read(const T& self, uint8_t* data)
{
        spi_handler::read(self, data, 1);
}

template <typename T>
static void flash_drive::spi_handler::read(const T& self, uint8_t* data, uint16_t len)
{
        uint8_t x = 0;

        self.m_spi->beginTransaction(SPISettings(SPIFLASH_SPI_SPEED, MSBFIRST, SPI_MODE0));
        while (length--) {
                x = _spi->transfer(SST25VF_CMD_NOP);
                *data = x;
                data++;
        }

        self.m_spi->endTransaction();
}

//------ Flash drive ------

flash_drive::flash_drive(uint8_t ss)
        : m_ss(ss)
{
        digitalWrite(m_ss, HIGH);
        pinMode(m_ss, OUTPUT);
}

bool flash_drive::begin()
{
        m_spi->begin();
}

uint32_t flash_drive::read_buffer(uint32_t addr, uint8_t* buf, uint32_t len)
{
        // Make sure the address is valid
        if (addr >= m_total_size)
                return 0;

        // Wait until the device is ready or exit if timeout occurs
        if (wait_until_ready())
                return 0;

        // Send the read data command
        uint8_t cmd_list[] = {
                SST25VF_CMD_READ,
                (addr >> 16) & 0xff,
                (addr >> 8)  & 0xff,
                addr,
        };

        digitalWrite(m_ss, LOW);
        spi_handler::write(this, cmd_list, 4);

        if ((addr+len) > m_total_size) {
                len = m_total_size - addr;
        }

        // Fill response buffer
        spi_handler::read(this, buf, len);

        digitalWrite(m_ss, HIGH);

        // Return bytes read
        return len;
}

uint32_t flash_drive::write_buffer(uint32_t addr, uint8_t* buf, uint32_t len)
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

uint32_t flash_drive::write_page(uint32_t addr, uint8_t* buf, uint32_t len)
{
        // page size is 256 bytea
        uint8_t status;

        // Make sure the address is valid
        if (addr >= W25Q16BV_MAXADDRESS)
                return 0;

        // Make sure that the supplied data is no larger than the page size
        if (len > pagesize)
                return 0;

        // Make sure that the data won't wrap around to the beginning of the sector
        if ((address % pagesize) + len > pagesize)
                return 0;

        // Wait until the device is ready or a timeout occurs
        if (WaitForReady())
                return 0;

        // Make sure the chip is write enabled
        WriteEnable (1);

        // Make sure the write enable latch is actually set
        status = readstatus();
        if (!(status & SPIFLASH_STAT_WRTEN))
                // Throw a write protection error (write enable latch not set)
                return 0;

        digitalWrite(_ss, LOW);

        if (addrsize == 24) {
                // Send page write command (0x02) plus 24-bit address
                spiwrite(W25Q16BV_CMD_PAGEPROG);      // 0x02
                spiwrite((address >> 16) & 0xFF);     // address upper 8
                spiwrite((address >> 8) & 0xFF);      // address mid 8
                if (len == pagesize)
                        // If len = 256 bytes, lower 8 bits must be 0 (see datasheet 11.2.17)
                        spiwrite(0);
                else
                        spiwrite(address & 0xFF);           // address lower 8
        } else if (addrsize == 16) {
                // Send page write command (0x02) plus 16-bit address
                spiwrite(W25Q16BV_CMD_PAGEPROG);      // 0x02
                spiwrite((address >> 8) & 0xFF);     // address upper 8
                spiwrite((address) & 0xFF);      // address lower 8
        } else {
                return 0;
        }

        // Transfer data
        spiwrite(buffer, len);

        // Write only occurs after the CS line is de-asserted
        digitalWrite(_ss, HIGH);

        if (! fastquit) {
        // Wait until the device is ready or a timeout occurs
                if (WaitForReady(5)) {
                        return 0;
                }
        }

        return len;
}





















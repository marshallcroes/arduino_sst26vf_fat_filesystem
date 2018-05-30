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

#ifndef SST26VF_DRIVER_H__
#define SST26VF_DRIVER_H__

#include <Arduino.h>
#include <SPI.h>

#include "sst26vf_config.h"
#include "sst26vf_instructions.h"

namespace sst26vf {

namespace detail {

        // forward declare
        struct spi_handler;

} // namespace detail

class flash_driver {
        SPIClass* m_spi = &SPI;

        uint8_t m_ss = 0;

        const uint32_t m_page_size = SST26VF_PAGE_SIZE;
        const uint32_t m_num_pages = SST26VF_NUM_PAGES;
        const uint32_t m_total_size = SST26VF_MAX_ADDR;
        // const uint32_t m_addr_size;

    public:
        flash_driver() = default;
        flash_driver(uint8_t ss);
        flash_driver(flash_driver&& other) = default;

        flash_driver& operator=(const flash_driver& other) = delete;
        flash_driver& operator=(flash_driver&& other);

        /*!
         * Initialize spi and begin communication with flash chip.
         */
        bool begin();

        //------ Erase functions ------

        /*!
         * Erase all data on chip.
         *
         * return      true if succeed false otherwise
         */
        bool erase_chip();

        /*!
         * Erase data in specify sector.
         *
         * param       sector number
         *
         * return      true if succeed false otherwise
         */
        bool erase_sector(uint32_t sector_num);

        /*!
         * Erase all data from the specific block. Blocks vary in sizes.
         *
         * param       block number
         *
         * return      true if succeed false otherwise
         */
        bool erase_block(uint32_t block_num);

        //------ Write functions ------

        uint32_t write_page(uint32_t addr, uint8_t* buf, uint32_t len);

        uint32_t write_buffer(uint32_t addr, uint8_t* buf, uint32_t len);

        //------ Read functions ------

        uint32_t read_buffer(uint32_t addr, uint8_t* buf, uint32_t len);

        //------ Debug functions ------

        /*!
         * Get the manufacturer information from flash chip.
         *
         * param[out]  manufacturer id, should be 0xBF
         * param[out]  device type, should be 0x26
         * param[out]  device id, should be 0x42
         */
        void get_manufacturer_info(uint8_t* mfr_id, uint8_t* dev_type, uint8_t* dev_id);

    private:
        bool write_enable(bool enable);

        uint8_t read_status();

        bool wait_until_ready(uint32_t timeout = 1000);

        friend struct detail::spi_handler;

}; // class flash driver

namespace detail {

        typedef flash_driver Chip;

        /*!
         * SPI class to handle all spi task.
         */
        struct spi_handler {
                static void write(const Chip& self, uint8_t data);

                static void write(const Chip& self, uint8_t* data, uint16_t len);

                static void read(const Chip& self, uint8_t* data);

                static void read(const Chip& self, uint8_t* data, uint16_t len);
        };
} // namespace detail

} // namespace sst26vf

#endif

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

#include "sst26vf_instructions.h"

#define SST26VF_PAGE_SIZE       256   // 4194304 bytes
#define SST26VF_NUM_PAGES       16384 // 256 sizze
#define SST26VF_SECTOR_SIZE     4096
#define SST26VF_BLOCK_S_SIZE    8192
#define SST26VF_BLOCK_M_SIZE    32768
#define SST26VF_BLOCK_L_SIZE    65536


namespace sst26vf {

  namespace detail {
    struct spi_handler {
          template <typename T>
          static void write(const T& self, char data);

          template <typename T>
          static void write(const T& self, char* data, uint16_t len);

          template <typename T>
          static void read(const T& self, uint8_t* data);

          template <typename T>
          static void read(const T& self, uint8_t* data, uint16_t len);
    };
  } // namespace detail

  class flash_driver {
        SPIClass* m_spi;
        // const SPIClass* m_spi = &SPI;

        const uint8_t m_ss;
        // const uint8_t m_clk;
        // const uint8_t m_mosi;
        // const uint8_t m_miso;

        const uint32_t m_page_size;
        const uint32_t m_num_pages;

        const uint32_t m_total_size;
        const uint32_t m_addr_size;

  public:
        flash_driver(uint8_t ss);

        bool begin();

        //------ Erase functions ------

        bool erase_chip();

        bool erase_sector(uint32_t sector_num);

        bool erase_block(uint32_t block_num);

        //------ Write functions ------

        uint32_t write_page(uint32_t addr, uint8_t* buf, uint32_t len);

        uint32_t write_buffer(uint32_t addr, uint8_t* buf, uint32_t len);

        //------ Read functions ------

        uint32_t read_buffer(uint32_t addr, uint8_t* buf, uint32_t len);

        //------ Standard OS functions ------

        size_t read(uint8_t b);

        size_t write(uint8_t b);

        void seek(uint32_t addr);

        //------ Debug functions ------

        // void get_id(uint8_t* buffer);

        void get_manufacturer_info(uint8_t* mfr_id, uint8_t* dev_type, uint8_t* dev_id);

  private:
        bool write_enable(bool enable);

        // uint32_t get_addr();

        // uint32_t find_first_empty_addr();

        uint8_t read_status();

        // bool append_data();

        bool wait_until_ready(uint32_t timeout=1000);

        friend struct detail::spi_handler;

  }; // class flash driver

} // namespace sst26vf

#endif

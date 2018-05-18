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

#ifndef SST25VF_DRIVER_H__
#define SST25VF_DRIVER_H__

#include <SPI.h>

#include "sst25vf_instructions.h"

namespace sst25vf {

  class flash_driver {
        const SPIClass* m_spi;

        const uint8_t m_ss;
        const uint8_t m_clk;
        const uint8_t m_mosi;
        const uint8_t m_miso;

        const uint32_t m_page_size;
        const uint32_t m_num_pages;

        const uint32_t m_total_size;
        const uint32_t m_addr_size;

  public:
        bool begin();

        //------ Erase functions ------

        bool erase_chip();

        bool erase_sector();

        bool erase_block();

        //------ Write functions ------

        uint32_t write_page();

        uint32_t write_buffer();

        //------ ... ------
        // TODO: finish writing class interface

  };

}

#endif

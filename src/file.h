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

#ifndef SST26VF_FILE_H__
#define SST26VF_FILE_H__

#include <Arduino.h>

#include "utility/ff.h"
#include "utility/diskio.h"

namespace sst26vf {

class file {
    public:
        file()
            : m_opened(false)
            , m_file({ 0 })
            , m_file_info({ 0 })
        {
        }

        file(const char* filepath, uint8_t mode = FA_READ);

        size_t write(uint8_t val) { return write(&val, 1); }
        size_t write(const uint8_t* buf, size_t size);

        int read();
        int read(void* buf, uint16_t nbyte);

        int peek();

        void flush() { f_sync(&m_file); }

        bool seek(uint32_t pos) { return f_lseek(&m_file, pos); }

        uint32_t position() { return f_tell(&m_file); }

        uint32_t size() { return f_size(&m_file); }

        void close();

        operator bool() { return m_opened; }

        char* name() { return m_file_info.fname; }

    private:
        bool m_opened;
        FIL m_file;
        FILINFO m_file_info;
};

} // namespace sst26vf

#endif

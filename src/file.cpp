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

#include "file.h"

namespace sst26vf {

file::file(const char* filepath, uint8_t mode)
        : m_opened(true)
        , m_file({ 0 })
        , m_file_info({ 0 })
{
        // Check if file exists!
        if (f_stat(filepath, &m_file_info) != FR_OK) {
                if ((mode & FA_READ) != FR_OK) {
                        m_opened = false;
                        return; // error
                }
        }

        FRESULT fr = f_open(&m_file, filepath, mode);
        if (fr != FR_OK) {
                m_opened = false; // failed to open file in specified mode
        }
}

size_t file::write(const uint8_t* buf, size_t size)
{
        // Write shit here! bitch!
        UINT bw = 0;
        if (f_write(&m_file, buf, size, &bw) != FR_OK){
                return 0;
        }

        return bw;
}

int file::read()
{
        UINT br = 0;
        uint8_t buf[] = { 0 };

        if (f_read(&m_file, buf, 1, &br) != FR_OK) {
                return -1;
        }

        if (br != 1) {
                return -1;
        }

        return buff[0];
}

int file::read(void* buf, uint16_t n_byte)
{
        UINT br = 0;
        if (f_read(&m_file, buf, n_byte, &br) != FR_OK) {
                return 0;
        }

        return br;
}

int file::peek()
{
        int c = read();
        if (c != -1) {
                f_lseek(&m_file, f_tell(&m_file) - 1);
        }

        return c;
}

void file::close()
{
        f_close(&m_file);
        m_opened = false;
}

} // namespace sst26vf

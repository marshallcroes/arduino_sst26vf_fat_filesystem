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

#include "sst26vf_filesystem.h"

// TODO: Make a singleton class for binding to FAT functions
namespace sst26vf {

bool filesystem::exist(const char* file_path)
{
        return f_stat(file_path, nullptr) == FR_OK;
}

bool filesystem::mount()
{
        // Mount the filesystem.
        FRESULT res = f_mount(&m_filesystem, "", 1);

        if (res != FR_OK) {
                return false;
        }

        return true;
}

//------- FAT specific functions ---------

DRESULT filesystem::disk_init()
{
        //
}

DRESULT filesystem::disk_status()
{
        //
}

DRESULT filesystem::disk_read(BYTE* buf, DWORD sector, UINT count)
{
        //
}

DRESULT filesystem::disk_write(const BYTE* buf, DWORD sector, UINT count)
{
        //
}

DRESULT filesystem::disk_ioctl(BYTE cmd, void* buff)
{
        //
}

} // namespace sst26vf

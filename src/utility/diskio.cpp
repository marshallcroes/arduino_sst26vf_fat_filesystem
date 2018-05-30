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

#include "diskio.h"

#include "filesystem.h"

DSTATUS disk_status (BYTE pdrv [[maybe_unused]])
{
        auto& fs = sst26vf::filesystem::instance();
	return fs.disk_status();
}

DSTATUS disk_initialize (BYTE pdrv [[maybe_unused]])
{
        auto& fs = sst26vf::filesystem::instance();
	return fs.disk_init();
}

DRESULT disk_read (BYTE pdrv [[maybe_unused]], BYTE *buff, DWORD sector, UINT count)
{
        auto& fs = sst26vf::filesystem::instance();
	return fs.disk_read(buff, sector, count);
}

DRESULT disk_write (BYTE pdrv [[maybe_unused]], const BYTE *buff, DWORD sector, UINT count)
{
        auto& fs = sst26vf::filesystem::instance();
	return fs.disk_write(buff, sector, count);
}

DRESULT disk_ioctl (BYTE pdrv [[maybe_unused]], BYTE cmd, void *buff)
{
        auto& fs = sst26vf::filesystem::instance();
	return fs.disk_ioctl(cmd, buff);
}


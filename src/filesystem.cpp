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

#include "filesystem.h"

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
        return RES_OK;
}

DRESULT filesystem::disk_status()
{
        return RES_OK;
}

DRESULT filesystem::disk_read(BYTE* buf, DWORD sector, UINT count)
{
        uint32_t addr = fat_sector_address(sector);
        if (!m_disk.read_buffer(addr, buf, count * m_fat_sector_size)) {
                return RES_ERROR;
        }

        return RES_OK;
}

DRESULT filesystem::disk_write(const BYTE* buf, DWORD sector, UINT count)
{
        // Check for memory allocation successfull.
        if (!m_buffer) return RES_ERROR;

        int sector_num = 0;
        while (count) {
                uint32_t addr = fat_sector_address(sector + sector_num);
                uint32_t sector_start = addr & 0xfff000; // Get base

                int available = (sector_start + m_flash_sector_size - addr) / m_fat_sector_size;
                int count_to_write = min((count - sector_num), available);

                if (m_disk.read_buffer(
                        sector_start, m_buffer, m_flash_sector_size) != m_flash_sector_size) {
                        return RES_ERROR;
                }

                uint16_t block_offset = addr & 0x000fff; // Get offset
                memcpy((m_buffer + block_offset),
                       (buf + sector_num * m_fat_sector_size),
                       count_to_write * m_fat_sector_size);

                if (!m_disk.erase_sector(sector_start / m_flash_sector_size)) {
                        return RES_ERROR;
                }

                if (m_disk.write_buffer(sector_start, m_buffer, m_flash_sector_size)
                         != m_flash_sector_size) {
                        return RES_ERROR;
                }

                count -= count_to_write;
                sector_num++;
        }

        return RES_OK;
}

DRESULT filesystem::disk_ioctl(BYTE cmd, void* buff)
{
        switch(cmd) {
        case GET_SECTOR_COUNT: {
                DWORD* count = static_cast<DWORD*>(buff);
                *count = sector_count();
        } break;

        case GET_SECTOR_SIZE: {
                WORD* count = static_cast<WORD*>(buff);
                *count = m_fat_sector_size;
        } break;

        case GET_BLOCK_SIZE: {
                DWORD* count = static_cast<DWORD*>(buff);
                *count = m_flash_sector_size / m_fat_sector_size;
        } break;

        // Not yet supported commands
        case CTRL_SYNC:
        case CTRL_TRIM:
        // So throw invalid parameter instead
        default: return RES_PARERR; break;
        }

        return RES_OK;
}

} // namespace sst26vf

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

#ifndef SST26VF_FILESYSTEM_H__
#define SST26VF_FILESYSTEM_H__

#include <utility>

#include "sst26vf_config.h"
#include "sst26vf_driver.h"

#include "file.h"

#include "utility/ffconf.h"
#include "utility/ff.h"
#include "utility/diskio.h"

namespace sst26vf {

class filesystem {
        filesystem()
                : m_filesystem()
        {
        }

        filesystem(const filesystem&) = delete;

        /*!
         * Calculate how many sectors/blocks are available on flash disk.
         *
         * return       The number of sectors/blocks available on disk
         */
        uint32_t sector_count()
        {
                return (SST26VF_PAGE_SIZE * SST26VF_NUM_PAGES) / m_fat_sector_size;
        }

        /*!
         * Calculate the disk address of the given FAT sector.
         *
         * return       The disk address of specified FAT sector
         */
        uint32_t fat_sector_address(uint32_t sector) { return sector * m_fat_sector_size; }

    public:
        /*!
         * This function creates the only instance of the class filesystem.
         * The instance is deleted when all reference go out of scope.
         *
         * return     filesystem
         */
        static filesystem& instance()
        {
                static filesystem fs;

                return fs;
        }

        /*!
         * Attach flash chip to filesystem.
         *
         * param       moves instance of disk drivers.
         */
        void attach(flash_driver&& disk) { m_disk = std::move(disk); }

        /*!
         * Functions needed for the ChaN FAT module
         */
        DRESULT disk_init();
        DRESULT disk_status();
        DRESULT disk_read(BYTE* buf, DWORD sector, UINT count);
        DRESULT disk_write(const BYTE* buf, DWORD sector, UINT count);
        DRESULT disk_ioctl(BYTE cmd, void* buff);

        /*!
         * Check if file exist
         *
         * param        The full path to the file including file name
         *
         * return       true if file exist, false otherwise
         */
        bool exist(const char* file_path);
        bool exist(const String& file_path)
        {
                return exist(file_path.c_str());
        }

        /*!
         * Erases the file from the disk
         *
         * param        The full path to the file
         *
         * return       true if successfull, false otherwise.
         */
        bool remove(const char* file_path);
        bool remove(const String& file_path)
        {
                return remove(file_path.c_str());
        }

        /*!
         * Open a file from the filesystem
         *
         * param        The full path to the file
         *
         * return       A file handler to manipulate the open file.
         */
        file open(const char* file_path);
        file open(const String& file_path)
        {
                return open(file_path.c_str());
        }

        /*!
         * Mount file system to disk
         *
         * return       true on succes, false otherwise
         */
        bool mount();

    private:
        flash_driver m_disk;
        FATFS m_filesystem;

        const int m_fat_sector_size = FF_MAX_SS; // FAT sector size, usually 512
        const int m_flash_sector_size = SST26VF_SECTOR_SIZE; // Flash sector erase size, usually 4k

        // Allocate a buffer to hold the data of the sector being overridden.
        uint8_t* m_buffer = static_cast<uint8_t*>(malloc(SST26VF_SECTOR_SIZE));
};
} // namespace sst26vf

#endif

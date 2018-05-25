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

#include "sst26vf_driver.h"

#include "utility/ff.h"

namespace sst26vf {
// TODO: Implement filesystem
class file;

class filesystem {
        filesystem();

        filesystem(const filesystem&) = delete;

        /*!
         * Calculate how many sectors/blocks are available on flash disk.
         *
         * return       The number of sectors/blocks available on disk
         */
        uint32_t sector_count()
        {
                return (m_disk.page_size() * m_disk.num_pages()) / m_fat_sector_size;
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
         * param       reference to disk drivers.
         */
        void attach(const flash_driver& disk);

        /*!
         * Functions needed for the ChaN FAT module
         */
        DRESULT disk_init();
        DRESULT disk_status();
        DRESULT disk_read(BYTE* buf, DWORD sector, UINT count);
        DRESULT disk_write(const BYTE* buf, DWORD sector, UINT count);
        DRESULT disk_ioctl(BYTE cmd, void* buff);

        func exist();
        func remove();
        func open();

        // func mount();
        // func unmount();

    private:
        flash_driver& m_disk;
        FATFS m_filesystem;

        const int m_fat_sector_size; // FAT sector size, usually 512
        const int m_flash_sector_size; // Flash sector erase size, usually 4k

        uint8_t* m_buffer;
};
} // namespace sst26vf

#endif

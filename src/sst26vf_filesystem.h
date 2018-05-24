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

namespace sst26vf {
  // TODO: Implement filesystem
  class filesystem {
        filesystem();

        filesystem(const filesystem&) = delete;

  public:

        /*!
             This function creates the only instance of the class filesystem.
             The instance is deleted when all reference go out of scope.

             return     filesystem
         */
        static filesystem& instance()
        {
                static filesystem fs;

                return fs;
        }

        /*!
            Attach flash chip to filesystem.

            param       reference to disk drivers.
         */
        void attach(const flash_driver& disk);
  private:

        flash_driver* m_disk;

  };
} // namespace sst26vf

#endif

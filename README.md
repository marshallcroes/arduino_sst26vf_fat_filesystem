# Sst26vf fat filesystem

A library to access the Sst26vf flash chip family. This includes formatting the drive to support FAT and being able to mount the drive.

## TODO
- [x] Implement driver for flash chip
- [x] Implement filesystems hook
- [x] Implement file hanlder
- [ ] Implement Dir handler
- [ ] Write tests

## Wiring to arduino

```
spi
-----
|1|2|
-----
|3|4|
-----
|5|6|
-----
```

1) miso, connect to so of flash
2) vcc+, connect to vdd of flash
3) sck, connect to sck of flash
4) mosi, connect to si of flash
5) reset, NC
6) gnd, connect to vss of flash

## Test Cases

- Test driver by reading manufacturer id and sucessfully read/write to sector on flash
- Test filesystem by formatting flash and writting a file on the flash

## MIT License

Copyright (c) 2018 Marshall Croes

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

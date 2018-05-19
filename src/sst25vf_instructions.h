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

/*
    Notes: Instruction set for flash chips from Microchip
    Reference: http://ww1.microchip.com/downloads/en/DeviceDoc/20005218E.pdf
 */

#ifndef SST25VF_INSTRUC_H__
#define SST25VF_INSTRUC_H__


// Configuration instructions
#define SST25VF_CMD_NOP             0x00
#define SST25VF_CMD_RSTEN           0x66
#define SST25VF_CMD_RST             0x99
#define SST25VF_CMD_EQIO            0x38
#define SST25VF_CMD_RSTQIO          0xff
#define SST25VF_CMD_RDSR            0x05
#define SST25VF_CMD_WRSR            0x01
#define SST25VF_CMD_RDCR            0x35

// Read instructions
#define SST25VF_CMD_READ            0x03
#define SST25VF_CMD_FREAD           0x0b
#define SST25VF_CMD_SQOR            0x6b
#define SST25VF_CMD_SQIOR           0xeb
#define SST25VF_CMD_SDOR            0x3b
#define SST25VF_CMD_SDIOR           0xbb
#define SST25VF_CMD_SB              0xc0
#define SST25VF_CMD_RBSQI           0x0c
#define SST25VF_CMD_RBSPI           0xec

// Identification instructions
#define SST25VF_CMD_JEDECID         0x9f
#define SST25VF_CMD_QJID            0xaf
#define SST25VF_CMD_SFDP            0x5a

// Write instructions
#define SST25VF_CMD_WREN            0x06
#define SST25VF_CMD_WRDI            0x04
#define SST25VF_CMD_SE              0x20
#define SST25VF_CMD_BE              0xd8
#define SST25VF_CMD_CE              0xc7
#define SST25VF_CMD_PP              0x02
#define SST25VF_CMD_SQPP            0x32
#define SST25VF_CMD_WRSU            0xb0
#define SST25VF_CMD_WRRE            0x30

// Protection instructions
#define SST25VF_CMD_RBPR            0x72
#define SST25VF_CMD_WBPR            0x42
#define SST25VF_CMD_LBPR            0x8d
#define SST25VF_CMD_NVWLDR          0xe8
#define SST25VF_CMD_ULBPR           0x98
#define SST25VF_CMD_RSID            0x88
#define SST25VF_CMD_PSID            0xa5
#define SST25VF_CMD_LSID            0x85

#endif
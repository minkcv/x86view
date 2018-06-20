# x86view
### A bootable utility for poking around on x86 systems
No warranty is provided. See LICENSE.md for more info.

### Building and running on linux
#### Install 
    sudo apt install build-essential qemu nasm gcc-multilib

#### Build
    make

#### Run
Mount `build/image.iso` in virtualization software.
Or run

    make run

to open in QEMU

### Usage
Supported commands are `r` (read), `w` (write), and `help`.
#### R - Read bytes - Usage:
R address [number of bytes]
* address: Memory address in hexadecimal to start reading at.
* [number of bytes]: A nonzero number of bytes to read in hexadecimal. Implicitly 1 if not specified.

#### W - Write bytes - Usage:
W address values
* address: Memory address in hexadecimal to write bytes to.
* values: Bytes to write to the memory address specified. Must be an even number of hex digits.

### Examples
Read a single byte at address 0
    > R 0
    00000000: 53

Read 32 bytes (20 in hex) at address 0
    > R 0 20
    00000000: 53FF 00F0 53FF 00F0 C3E2 00F0 53FF 00F0
    00000010: 53FF 00F0 54FF 00F0 53FF 00F0 53FF 00F0

Read 8 bytes at address 100000

    > R 100000 8
    0010000: BC0C 4010 00EB 0D90

Disassembly of build/image/boot/kernel.bin
    00100000 <code>:
      100000:       bc 0c 40 10 00          mov    $0x10400c,%esp
      100005:       eb 0d                   jmp    100014 <stublet>
      100007:       90                      nop
    ...

Hey look, that's where our program is loaded in memory. You can confirm this by finding 0x00100000 in the link.ld file.

    phys = 0x001000000


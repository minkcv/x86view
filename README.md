# x86view
### A bootable utility for poking around on x86 systems
No warranty is provided. See LICENSE.md for more info. I have run this on one of my physical machines but I assume no responsibility for any damage to your machine. This utility lets you write to ram and jump to arbitrary addresses with no safeguards.

### Building and running on linux
Install Required dependencies:  
Systems with apt should run: `sudo apt install build-essential qemu nasm gcc-multilib xorriso grub-pc-bin`

#### Build and Run
Run `make` to build the utility and create an iso.  
Run `make run` to open in QEMU. Or mount `build/image.iso` in virtualization software. 

### Usage
Supported commands are `R` (read), `W` (write), `J` (jump) and `HELP`.
#### R - Read bytes - Usage:
R address [number of bytes]
* address: Memory address in hexadecimal to start reading at.
* [number of bytes]: A nonzero number of bytes to read in hexadecimal. Implicitly 1 if not specified.

#### W - Write bytes - Usage:
W address values
* address: Memory address in hexadecimal to write bytes to.
* values: Bytes to write to the memory address specified. Must be an even number of hex digits.

#### J - Jump to address
J address
* address: Absolute memory address in hexadecimal to jump to and start executing. 

### Examples
#### Reading
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

These 8 bytes are actually the x86 instructions for this program. You can disassemble this program in linux and see that they match.

Disassembly of build/image/boot/kernel.bin:

    $ objdump -d build/boot.o

    00100000 <code>:
      100000:       bc 0c 40 10 00          mov    $0x10400c,%esp
      100005:       eb 0d                   jmp    100014 <stublet>
      100007:       90                      nop
    ...

The bytes match. You can also find 0x00100000 in the link.ld file.

#### Writing
Write a single byte at address 0

    > R 0 4
    00000000: 53FF 00F0
    > W 0 00
    > R 0 4
    00000000: 00FF 00F0

#### Jumping
Now for the fun part. You can start code execution at any address using the `J` command. 

This is a far, absolute, indirect jump, where the destination is stored in eax. The jump instruction is `FF E0`.

The command:

    > J 100000

Will jump to the beginning of this program, clear the screen, and reprint the welcome message and prompt.

With the combination of the write and jump commands you can write x86 code in hex into memory and execute it. 
Let's write a program in assembly in linux that jumps to absolute address 0x00100000 (which is where the x86view code starts).

Our assembly file:

    ; test.asm
    [BITS 32]
    mov eax, 0x00100000
    jmp eax ; An absolute jump to the address in eax

Now lets assemble it and disassemble it to see the hexadecimal representation:

    $ nasm -f elf -o test.bin test.asm
    $ objdump -d test.bin

    00000000 <.text>:
       0:	b8 00 00 10 00       	mov    $0x100000,%eax
       5:	ff e0                	jmp    *%eax

And finally we will enter it in our machine that has booted x86view. I chose the memory address 0x500 arbitrarily.

    > W 500 B800001000
    > W 505 FFE0
    > J 500

And it clears the screen and prints the welcome message and prompt. Now we know how to enter programs directly into memory as hexadecimal x86 instructions and execute them.



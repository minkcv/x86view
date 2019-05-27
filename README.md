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
* address: Absolute memory address in hex to jump to and start executing. 

#### FIND - Find a sequence of bytes in memory - Usage:
FIND start end sequence
* start: Memory address to start searching at.
* end: Memory address to stop searching at.
* sequence: Sequence of bytes to search for between start and end.

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
    0010000: BC00 5010 00EB 0D90 02BO AD1B 0300 0000

Some of these bytes are actually the x86 instructions for this program. You can disassemble this program in linux and see that they match.

Disassembly of build/image/boot/kernel.bin:

    $ objdump -d build/boot.o

    build/boot.o:     file format elf32-i386

    Disassembly of section .text:

    00000000 <start>:
    0:   bc 00 20 00 00          mov    $0x2000,%esp
    5:   eb 0d                   jmp    14 <stublet>
    7:   90                      nop

    00000008 <mboot>:
     8:   02 b0 ad 1b 03 00       add    0x31bad(%eax),%dh
     e:   00 00                   add    %al,(%eax)
    10:   fb                      sti
    11:   4f                      dec    %edi
    12:   52                      push   %edx
    13:   e4                      .byte 0xe4

The bytes match up after 8 bytes. You can also find 0x00100000 in the link.ld file.

#### Writing
Write a single byte at address 0

    > R 0 4
    00000000: 53FF 00F0
    > W 0 00
    > R 0 4
    00000000: 00FF 00F0

We can also write directly to the screen memory. 
0xB8000 is the start address of the screen memory and 0x01 is a smiley face in the [VGA Standard](https://en.wikipedia.org/wiki/Code_page_437)

    > W B8000 01

Should show a smiley face in the top left corner.

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

#### Finding
It is also possible to modify the x86view code itself. We know the jump command uses the instruction `FF E0` to jump to an address.
We use the Find command to look between 0x100000 and 0x200000 for the bytes `FFE0`:

    > FIND 100000 200000 FFE0
    001009C0, 001057D7, 001058BB

And get 3 results.  
I'll save you time time of checking and tell you that 001009C0 is the address that the jump command calls.
We can use the Write command to replace these bytes with x86 [NOP](https://en.wikipedia.org/wiki/NOP_(code))s (0x90).

    > W 1009C0 9090
    > J 100000

And you will notice that the jump instruction doesn't actually jump us to the address now, it just prints the next prompt.


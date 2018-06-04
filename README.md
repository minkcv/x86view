# x86view
### A bootable image for poking around on x86 systems
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


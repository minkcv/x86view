all: build/image.iso

run: all
	qemu-system-i386 -boot d -cdrom build/image.iso

build/image.iso: build/kernel.bin 
	cp grub.cfg build/image/boot/grub/
	grub-mkrescue -o build/image.iso build/image

build/kernel.bin: build/kernel.o build/boot.o
	ld -T link.ld -o build/image/boot/kernel.bin build/boot.o build/kernel.o -m elf_i386

build/%.o: kernel/%.c
	mkdir -p build/image/boot/grub
	gcc -fno-stack-protector -fno-builtin -nostdinc -O -g -Wall -Ikernel -c -o $@ $^ -m32

build/boot.o: boot/boot.asm
	nasm -f elf -o build/boot.o boot/boot.asm

clean:
	rm -rf build


C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
OBJECTS = $(notdir $(C_SOURCES:.c=.o))

all: clean image.iso

run: all
	qemu-system-i386 -boot d -cdrom build/image.iso

image.iso: kernel.bin 
	cp grub.cfg build/image/boot/grub/
	grub-mkrescue -o build/image.iso build/image

kernel.bin: boot.o $(OBJECTS)
	ld -T link.ld -o build/image/boot/$@ $(addprefix build/,$^) -m elf_i386

%.o: */%.c 
	mkdir -p build/image/boot/grub
	gcc -m32 -fno-stack-protector -fno-builtin -O -Wall -Ikernel:drivers -c $< -o $(addprefix build/, $@)

boot.o: boot/boot.asm
	mkdir -p build/image/boot/grub
	nasm -f elf -o build/boot.o boot/boot.asm

clean:
	rm -rf build


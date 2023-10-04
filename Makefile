Kernel_C_Files := $(shell find src/kernel -name *.c)
Kernel_O_Files := $(patsubst src/kernel/%.c, build/kernel/%.o, $(Kernel_C_Files))

Boot_ASM_Files := $(shell find src/boot -name *.asm)
Boot_O_Files := $(patsubst src/boot/%.asm, build/boot/%.o, $(Boot_ASM_Files))

All_Object_Files := $(Kernel_O_Files) $(Boot_O_Files)

$(Kernel_O_Files): build/kernel/%.o : src/kernel/%.c
	mkdir -p $(dir $@) && \
	i686-elf-gcc -c -I src/kernel -ffreestanding $(patsubst build/kernel/%.o, src/kernel/%.c, $@) -g -o $@

$(Boot_O_Files): build/boot/%.o : src/boot/%.asm
	mkdir -p $(dir $@) && \
	nasm -felf32 $(patsubst build/boot/%.o, src/boot/%.asm, $@) -o $@

.PHONY: build
build: $(Boot_O_Files) $(Kernel_O_Files)
	i686-elf-gcc -T targets/linker.ld -o ./out/fullerene.bin -ffreestanding -O2 -nostdlib $^ -lgcc
	echo "= Checking if multiboot is legit ="
	grub-file --is-x86-multiboot ./out/fullerene.bin
	grub-mkrescue -o fullerene.iso out

.PHONY: clear
clear:
	cd build/
	find . -name "*.o" -type f -delete
	cd out/
	find . -name "*.bin" -type f -delete
	find . -name "*.iso" -type f -delete
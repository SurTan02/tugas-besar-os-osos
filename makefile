
# Makefile
all: diskimage bootloader lib_interrupt stdlib lib kernel shell ls

# Recipes
diskimage:
	dd if=/dev/zero of=out/system.img bs=512 count=2880

bootloader:
	nasm src/asm/bootloader.asm -o out/bootloader
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc
	
kernel:
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	ld86 -o out/kernel -d out/kernel.o out/std_lib.o out/kernel_asm.o out/lib_interrupt.o out/string.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

shell:
	bcc -ansi -c -o out/shell.o src/c/shell.c
	ld86 -o out/shell -d out/shell.o out/lib_interrupt.o out/textio.o out/fileio.o out/message.o out/string.o

stdlib:
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c

lib:
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/message.o src/c/message.c

utility:
	bcc -ansi -c -o out/cd.o src/c/cd.c
	bcc -ansi -c -o out/mkdir.o src/c/mkdir.c
	bcc -ansi -c -o out/cat.o src/c/cat.c
	bcc -ansi -c -o out/cp.o src/c/cp.c
	bcc -ansi -c -o out/mv.o src/c/mv.c

ls:
	bcc -ansi -c -o out/ls.o src/c/ls.c
	ld86 -o out/ls -d out/ls.o out/lib_interrupt.o out/textio.o out/fileio.o out/message.o out/string.o out/std_lib.o

lib_interrupt:
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o

run:
	bochs -f src/config/if2230.config

build-run: all run


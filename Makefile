
all: test.o
	riscv64-unknown-elf-ld -melf32lriscv -T hifive1b.ld -o test test.o
	riscv64-unknown-elf-objcopy test -O ihex test.hex
	rm test

%.o: %.c
	riscv64-unknown-elf-gcc $< -c -g -march=rv32imac -mabi=ilp32 -mcmodel=medany

flash:
	echo -e "loadfile test.hex\n rnh\n exit" | JLinkExe -device FE310 -if JTAG -speed 4000 -jtagconf -1,-1 -autoconnect 1

clean:
	rm *.o *.hex

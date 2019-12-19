
OUTPUT=lofive
OBJS = init.o interrupts.o asm_procs.o 

$(OUTPUT): $(OBJS) hifive1b.h hifive1b.ld
	riscv-sifive-elf-ld -melf32lriscv -T hifive1b.ld -o $(OUTPUT) $(OBJS)
	riscv-sifive-elf-objcopy $(OUTPUT) -O ihex $(OUTPUT).hex

%.o: %.c
	riscv-sifive-elf-gcc $< -c -g -march=rv32imac -mabi=ilp32 -mcmodel=medany

%.o: %.s
	riscv-sifive-elf-as $< -o $@ -march=rv32imac -mabi=ilp32

flash: $(OUTPUT)
	echo -e "loadfile $(OUTPUT).hex\n rnh\n exit" | JLinkExe -device FE310 -if JTAG -speed 4000 -jtagconf -1,-1 -autoconnect 1

clean:
	rm *.o *.hex $(OUTPUT)

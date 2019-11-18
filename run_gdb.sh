riscv-sifive-elf-gdb $1 -ex "set architecture riscv:rv32" -ex "target extended-remote localhost:3333" -ex "monitor allowsimulation 0" -ex "monitor reset" -ex "load"

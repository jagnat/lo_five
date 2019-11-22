

To debug the board using gdb:

- In one terminal, run `jlink_server.sh`
- In another terminal, run `run_gdb.sh lofive`, or another ELF file you are targetting as the first arg
- GDB will automatically connect, flash the binary, and reset the board
- You can set breakpoints with `b` as usual
- When you are ready to run, type `c` and execution will continue from reset
- For a list of SEGGER commands that can be run through GDB with the `monitor` prefix, type `monitor help`

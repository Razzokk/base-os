set prompt \033[36mdbg> \033[m

set output-radix 0x10
set input-radix 0x10

set disassembly-flavor intel

target remote host.docker.internal:1234

define hook-stop
	disassemble $rip
end

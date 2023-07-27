$command=$args[0]

[System.Collections.ArrayList]$new_args=$args

if ($new_args.Count -gt 0)
{
	$new_args.RemoveAt(0)
}

# We want to execute qemu separately on a windows machine
if ($command -eq "qemu")
{
    qemu-system-x86_64 -cdrom ./build/base_os.iso -debugcon stdio -serial file:serial.log $new_args
}
elseif ($command -eq "qemu-dbg")
{
    qemu-system-x86_64 -cdrom ./build/base_os.iso -debugcon stdio -serial file:serial.log -s -S $new_args
}
else
{
    bash bos $args
}

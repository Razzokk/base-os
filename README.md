# Base OS

This repository serves as a basis for getting started with simple OS development.
It is a bare minimum kernel that uses the [Multiboot 2 Specification](https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html) and [Grub](https://de.wikipedia.org/wiki/Grand_Unified_Bootloader) to start in 32-bit mode, enter custom assembly code to enter 64-bit mode and call the `kmain()` function in [kmain.c](./kernel/kmain.c).

Additionally, some utility functions are provided to write to the framebuffer (Textmode-80x25).

## Getting Started

For easier installation, building and running of the operating system two scripts `bos` and `bos.p1` are provided to make this process easier.

You can type

```shell
./bos help
```

to get an overview of which commands are available.

## Installation

To install all the required tools for building run the provided shell scripts (this step can take quiet a while...):

### Linux (bash)

```shell
./bos setup
```

### Windows (powershell)

You need to have WSL 2 enabled and a distro installed (suggested: [Ubuntu 22.04](https://apps.microsoft.com/store/detail/ubuntu-22042-lts/9PN20MSR04DW?hl=de-at&gl=at&rtc=1))

```ps1
.\bos[.ps1] setup
```

Here `[.ps1]` means it is optional.

Then make sure to install [QEMU](https://www.qemu.org/).
It is important that the `qemu-system-x86_64` component is installed since this OS template runs and compiles on x86_64.

## How to build and run

Make sure you followed the steps above for QEMU.
Then you can build the OS with

```shell
./bos make all
```

and after that run it with

```shell
./bos qemu
```

or just simply chain them together:

```shell
./bos make build && ./bos qemu
```

You should than get a `build` directory in the root directory and in it an ISO file called `base_os.iso` which is the file that gets executed by **QEMU**.

## Debugging

For debugging purposes functions/defines inside the [debug.h](./kernel/include/debug.h) header can be used:

Single characters, though this is more or less used for the other functions:

```c
debug_char(<your_char>);
```
String, here you have to specify the maximal length of the string.
Printing also ends when a null byte is hit:

```c
debug(<your_string>, <string_length>);
```

And for convenient reasons a wrapper around `debug` for string literals so that you don't have to specify the string length:

```c
debug_literal(<your_string_literal>);
```

Now by default everything that is printed via the debug functions/defines goes to `stdout` over port `0xE9` defined in the debug header.
This can however be changed to log debug into a `serial.log` file inside the root directory by setting the `DEBUG_PORT` define inside the debug header to `COM1_PORT` which has the value `0x3F8`.

**TIP**: To make your debug statements a bit more expressive in the log use [ANSI Escape Codes](https://en.wikipedia.org/wiki/ANSI_escape_code) to color them (this however only makes sense when you print to stdout):

```c
debug_literal("[\x1b[33mDEBUG\x1b[m]: print some useful information for debugging here\n");
```

produces:

<code>[<span style="color: #C19C00">DEBUG</span>]: print some useful information for debugging here</code>

## Planned

- [x] I/O with `outb` and `inb` instructions
- [x] Debug printing over stdout (requires I/O)
- [ ] Adjusting colors in text mode (requires I/O)
- [x] Printing functions
- [ ] `rdtsc` instruction wrapper
- [ ] Simple kernel memory management (`kmalloc`)
- [ ] Simple pseudo random number generator ([linear congruential generator](https://en.wikipedia.org/wiki/Linear_congruential_generator))
- [ ] Simple interrupts with [PIC](https://wiki.osdev.org/8259_PIC) (timer and keyboard interrupts, requires I/O)
- [ ] Simple lock (spin-lock, with `xchg` instruction)
- [ ] Use docker image for building

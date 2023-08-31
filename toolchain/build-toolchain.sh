#!/usr/bin/env sh

target=x86_64-elf
binutils=binutils-2.40
gcc=gcc-13.2.0

# Install compilers etc.

apk --update add build-base
apk add gmp-dev mpfr-dev mpc1-dev bison flex texinfo nasm

# The important osdev tools

apk add make
apk add grub-bios xorriso
apk add gdb-multiarch

rm -rf /var/cache/apk/*

# Install binutils

cd /opt
wget http://ftp.gnu.org/gnu/binutils/${binutils}.tar.gz
tar -xf ${binutils}.tar.gz
mkdir binutils-build && cd binutils-build
../${binutils}/configure \
	--target=${target} \
	--disable-nls \
	--disable-werror \
	--with-sysroot

make -j 8
make install

# Install gcc

cd /opt
wget http://ftp.gnu.org/gnu/gcc/${gcc}/${gcc}.tar.gz
tar -xf ${gcc}.tar.gz
mkdir gcc-build && cd gcc-build
../${gcc}/configure \
	--target=${target} \
	--disable-nls \
	--enable-languages=c,c++ \
	--without-headers \
	--disable-hosted-libstdcxx

make all-gcc
make all-target-libgcc CFLAGS_FOR_TARGET='-g -O2 -mcmodel=large -mno-red-zone' -j 8
make all-target-libstdc++-v3 CXXFLAGS_FOR_TARGET='-g -O2 -mcmodel=large -mno-red-zone' -j 8
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3

apk del build-base

cd /
rm -rf /opt

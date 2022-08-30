#!/bin/bash

BINUTILS_VERSION=2.39
GCC_VERSION=12.2.0

DIR="$( cd "$( dirname "$0" )" && pwd )"

# Define environment variables

export PREFIX="${DIR}/cross"
export TARGET=x86_64

# Install compilers etc.

sudo apt-get update && \
sudo apt-get install -y wget gcc libgmp3-dev libmpfr-dev libisl-dev \
    libmpc-dev texinfo bison flex make bzip2 patch \
    build-essential xorriso grub-pc-bin grub-common qemu-system

# Download

mkdir -p $PREFIX/src
cd $PREFIX/src
wget https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.gz
wget https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.gz
tar zxf binutils-${BINUTILS_VERSION}.tar.gz
tar zxf gcc-${GCC_VERSION}.tar.gz
rm binutils-${BINUTILS_VERSION}.tar.gz gcc-${GCC_VERSION}.tar.gz
chown -R root:root binutils-${BINUTILS_VERSION}
chown -R root:root gcc-${GCC_VERSION}
chmod -R o-w,g+w binutils-${BINUTILS_VERSION}
chmod -R o-w,g+w gcc-${GCC_VERSION}

# Binutils

cd $PREFIX/src
mkdir build-binutils
cd build-binutils
../binutils-${BINUTILS_VERSION}/configure --target=$TARGET-elf --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j 4
make install

cd $PREFIX/src
rm -rf build-binutils binutils-${BINUTILS_VERSION}

# GCC

cd $PREFIX/src
mkdir build-gcc
cd build-gcc
../gcc-${GCC_VERSION}/configure --target=$TARGET-elf --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make -j 4 all-gcc
make -j 4 all-target-libgcc
make install-gcc
make install-target-libgcc

cd $PREFIX/src
rm -rf build-gcc gcc-${GCC_VERSION}

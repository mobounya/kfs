#!/usr/bin/bash

###
#   This scipt is based on the GCC Cross-Compiler guide in the osdev wiki
#   https://wiki.osdev.org/GCC_Cross-Compiler
###

# Add these env variables to your .bashrc file.

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p $HOME/opt/cross
mkdir -p $HOME/src/
cd $HOME/src/

# Install dependencies for building binutils and gcc.

# Install these to build code from source.
sudo apt-get install build-essential tar

# You need texinfo to build binutils.
sudo apt-get install texinfo -y

# You need (GMP, MPC, MPFR) to build gcc.
sudo apt-get install libgmp3-dev libmpc-dev libmpfr-dev -y

# get source code for binutils (version 2.38) and build it.
wget https://ftp.gnu.org/gnu/binutils/binutils-2.38.tar.gz
tar -xf binutils-2.38.tar.gz
mkdir -p build-binutils

pushd build-binutils
../binutils-2.38/configure --target="$TARGET" --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
popd

# get source code for gcc (version 11.3.0) and build it.
wget https://ftp.gnu.org/gnu/gcc/gcc-11.3.0/gcc-11.3.0.tar.gz
tar -xf gcc-11.3.0.tar.gz
mkdir -p build-gcc

pushd build-gcc
../gcc-11.3.0/configure --target="$TARGET" --prefix="$PREFIX" --disable-nls --enable-languages="c,c++" --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
popd

echo "Successfully installed gcc: "
$HOME/opt/cross/bin/$TARGET-gcc --version

echo "Successfully installed binutils: "
$HOME/opt/cross/bin/$TARGET-ld --version

# Maple

This is a development environment based on the lib maple.

## Starting

You should get the ARM cross-compile tools:

    sudo apt-get install libstdc++-arm-none-eabi-newlib gcc-arm-none-eabi libnewlib-arm-none-eabi binutils-arm-none-eabi

Then, go to the `Maple/skeleton` directory, and check that your board is
OK in the `Makefile` (maple_mini by default).

Then:

    make
    make install

## Installing DFU-UTIL 0.1

First, install lib usb:

    sudo apt-get install libusb-dev

Then, do the following:

    git clone git://git.code.sf.net/p/dfu-util/dfu-util
    cd dfu-util
    git checkout v0.1
    ./autogen.sh
    ./configure
    make

The `dfu-util` tool will be built in the src/ repository, you can add it
in your `$PATH` in your `.bashrc`, like:

    export PATH="$HOME/dfu-util/src:$PATH"



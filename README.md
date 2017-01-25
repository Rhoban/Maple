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

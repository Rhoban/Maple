# Maple

This is a development environment based on the lib maple.

## Using xenial toolchain on Ubuntu 18+

This lib should be used with gcc 4.9, which can be found in xenial toolchain. On Ubuntu 18.04
or 18.10, you can:

* Backup your `/etc/sources.list`,
* Edit your `/etc/sources.list` to have just this line:
  `deb http://fr.archive.ubuntu.com/ubuntu xenial main restricted universe multiverse`
* Update apt: `sudo apt-get update`
* In case you already have it, remove packages:
  `sudo apt-get autoremove gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib`
* Install correct packages:
  `sudo apt-get install gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib`
* Freeze the version of those packages:
  `sudo apt-mark hold gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib`
* Restore your `/etc/sources.list`.

## Starting

You should get the ARM cross-compile tools:

    sudo apt-get install libstdc++-arm-none-eabi-newlib gcc-arm-none-eabi libnewlib-arm-none-eabi binutils-arm-none-eabi

Then, go to the `Maple/skeleton` directory, and check that your board is
OK in the `Makefile` (maple_mini by default).

Then:

    make
    make install

## Installing `dfu-util` 0.1

The old version of dfu-util (0.1) is faster to flash. To install it, first
install libusb:

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



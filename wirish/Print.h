/*
 * Print.h - Base class that provides print() and println()
 * Copyright (c) 2008 David A. Mellis.  All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 *
 * Modified 12 April 2011 by Marti Bolivar <mbolivar@leaflabs.com>
 */

#ifndef _PRINT_H_
#define _PRINT_H_

#include "libmaple_types.h"

enum {
    BYTE = 0,
    BIN  = 2,
    OCT  = 8,
    DEC  = 10,
    HEX  = 16
};

class Print {
public:
    virtual void write(uint8) = 0;
    virtual void write(const char *str);
    virtual void write(void*, uint32);
    void print(char);
    void print(const char[]);
    void print(uint8);
    void print(int);
    void print(unsigned int);
    void print(long);
    void print(unsigned long);
    void print(long long);
    void print(unsigned long long);
    void print(unsigned long long, int);
    void print(double);
    void println(void);
    void println(char);
    void println(const char[]);
    void println(uint8);
    void println(int);
    void println(unsigned int);
    void println(long);
    void println(unsigned long);
    void println(long long);
    void println(unsigned long long);
    void println(unsigned long long, int);
    void println(double);
private:
    void printNumber(unsigned long long, uint8);
    void printFloat(double, uint8);
};

#endif

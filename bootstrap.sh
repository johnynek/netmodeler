#!/bin/sh
aclocal -I config
autoconf -I m4
automake --add-missing
./configure
make


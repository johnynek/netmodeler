#!/bin/sh
aclocal -I config
autoconf
automake --foreign --add-missing
./configure
make


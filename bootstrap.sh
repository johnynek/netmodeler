#!/bin/sh
aclocal -I config
autoconf
automake --add-missing
./configure
make


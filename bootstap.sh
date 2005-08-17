#!/bin/sh
aclocal -I config
autoconf
automake
./configure
make


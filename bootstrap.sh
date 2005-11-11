#!/bin/sh
autoreconf --force --install -I config -I m4
aclocal -I config -I m4
autoconf -I m4
automake --add-missing
./configure
make


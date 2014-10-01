#!/bin/sh

aclocal
autoheader
libtoolize -c
automake -a -c --foreign
autoreconf

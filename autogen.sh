#!/bin/sh

aclocal --force && \
libtoolize --force --copy && \
autoconf --force && \
autoheader --force && \
automake --force-missing --add-missing --copy

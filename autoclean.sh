#!/bin/sh

find . -type f -name Makefile -print0 | xargs -r0 rm -f
find . -type f -name Makefile.in -print0 | xargs -r0 rm -f
rm -f config.* configure aclocal.m4
rm -f install-sh missing mkinstalldirs depcomp ltmain.sh
rm -fR autom4te.cache

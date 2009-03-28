CFLAGS = -g -Wall -O2

all: dump

clean:
	rm -f dump *.o *.txt

check: dump
	for i in *.sfr; do \
	    ./dump $$i | iconv -f cp1251 > `basename $$i .sfr`.txt; \
	done

indent: dump.c sfr.h
	indent -br -brs -nbad -nbfda -nprs -npsl $^

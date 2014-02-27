#
# Just a minimal Makefile for now to install the most basic components
#
# Currently installs the assembler and the app loader library
#
PREFIX?=/usr/local

all:
	cd pru_sw/utils/pasm_source && ./linuxbuild
	cd pru_sw/app_loader/interface && CROSS_COMPILE=$(CROSS_COMPILE) make

install:
	install -m 0755 -d $(DESTDIR)$(PREFIX)/bin
	install -m 0755 pru_sw/utils/pasm $(DESTDIR)$(PREFIX)/bin
	cd pru_sw/app_loader/interface && CROSS_COMPILE=$(CROSS_COMPILE) make install

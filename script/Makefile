BINDIR := $(shell [ -x ../../gfxboot-compile ] && echo ../../ )
GFXBOOT_COMPILE = $(BINDIR)gfxboot-compile
GFXBOOT = $(BINDIR)gfxboot
BFLAGS = -O -v -L ../..

.PHONY: all clean distclean

all: bootlogo

bootlogo: bootlogo.dir/init
	$(GFXBOOT) --archive bootlogo.dir --pack-archive bootlogo

bootlogo.dir/init: xmlboot.bc *.inc
	@rm -rf bootlogo.dir
	@mkdir bootlogo.dir
	$(GFXBOOT_COMPILE) $(BFLAGS) -l bootlogo.log -c $< bootlogo.dir/init

clean:
	rm -rf *~ *.log *.dir bootlogo

distclean: clean


BINDIR := $(shell [ -x ../../gfxboot-compile ] && echo ../../ )
GFXBOOT_COMPILE = $(BINDIR)gfxboot-compile
GFXBOOT_FONT = $(BINDIR)gfxboot-font
GFXBOOT = $(BINDIR)gfxboot
BFLAGS = -O -v -L ../..

.PHONY: all clean distclean

all: bootlogo

bootlogo: font
	@rm -rf bootlogo.dir
	cd ../../script; make
	cp -a ../../script/bootlogo.dir .
	cp ../lernstick_debian6/*jpg *fnt po/*translation xmlboot.config bootlogo.dir
	cp *jpg bootlogo.dir
	$(GFXBOOT) --archive bootlogo.dir --pack-archive bootlogo

font: font_size_10.fnt font_size_12.fnt font_size_15.fnt font_size_15_bold.fnt font_size_20.fnt

font_size_10.fnt:
	$(GFXBOOT_FONT) -v -a 0-65535 -p /usr/share/fonts/truetype/ttf-dejavu/ -f DejaVuSans:size=10 font_size_10.fnt >font.log

font_size_12.fnt:
	$(GFXBOOT_FONT) -v -a 0-65535 -p /usr/share/fonts/truetype/ttf-dejavu/ -f DejaVuSans:size=12 font_size_12.fnt >font.log

font_size_15.fnt:
	$(GFXBOOT_FONT) -v -a 0-65535 -p /usr/share/fonts/truetype/ttf-dejavu/ -f DejaVuSans:size=15 font_size_15.fnt >font.log

font_size_15_bold.fnt:
	$(GFXBOOT_FONT) -v -a 0-65535 -p /usr/share/fonts/truetype/ttf-dejavu/ -f DejaVuSans:size=15:bold=1 font_size_15_bold.fnt >font.log

font_size_20.fnt:
	$(GFXBOOT_FONT) -v -a 0-65535 -p /usr/share/fonts/truetype/ttf-dejavu/ -f DejaVuSans:size=20 font_size_20.fnt >font.log

clean:
	rm -rf *~ *.log *.dir bootlogo *.fnt

test: bootlogo
	gfxboot -p -b isolinux -a bootlogo

distclean: clean


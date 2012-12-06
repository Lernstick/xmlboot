BINDIR := $(shell [ -x ../../gfxboot-compile ] && echo ../../ )
GFXBOOT_COMPILE = $(BINDIR)gfxboot-compile
GFXBOOT_FONT = $(BINDIR)gfxboot-font
GFXBOOT = $(BINDIR)gfxboot
BFLAGS = -O -v -L ../..

.PHONY: all themes clean distclean

all: bootlogo

release: xmlboot.bc font
	@rm -rf bootlogo.dir
	@mkdir bootlogo.dir
	$(GFXBOOT_COMPILE) $(BFLAGS) -l bootlogo.log -c $< bootlogo.dir/init
	$(GFXBOOT) --archive bootlogo.dir --pack-archive bootlogo

bootlogo: xmlboot.bc font
	@rm -rf $@.dir
	@mkdir $@.dir
	$(GFXBOOT_COMPILE) $(BFLAGS) -l $@.log -c $< $@.dir/init
	@cp po/*.translation *.jpg xmlboot.config font*.fnt $@.dir
	$(GFXBOOT) --archive $@.dir --pack-archive $@

font:
	$(GFXBOOT_FONT) -v -a 0-65535 -p /usr/share/fonts/truetype/ttf-dejavu/ -f DejaVuSans:size=10 font_size_10.fnt >font.log
	$(GFXBOOT_FONT) -v -a 0-65535 -p /usr/share/fonts/truetype/ttf-dejavu/ -f DejaVuSans:size=12 font_size_12.fnt >font.log
	$(GFXBOOT_FONT) -v -a 0-65535 -p /usr/share/fonts/truetype/ttf-dejavu/ -f DejaVuSans:size=15 font_size_15.fnt >font.log
	$(GFXBOOT_FONT) -v -a 0-65535 -p /usr/share/fonts/truetype/ttf-dejavu/ -f DejaVuSans:size=15:bold=1 font_size_15_bold.fnt >font.log
	$(GFXBOOT_FONT) -v -a 0-65535 -p /usr/share/fonts/truetype/ttf-dejavu/ -f DejaVuSans:size=20 font_size_20.fnt >font.log

clean:
	rm -rf *~ *.log *.dir bootlogo *.fnt

distclean: clean


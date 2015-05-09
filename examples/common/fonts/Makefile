ifneq ("$(wildcard $(/usr/share/fonts/truetype/ttf-dejavu/))","")
FONT_PATH="/usr/share/fonts/truetype/ttf-dejavu/"
else
FONT_PATH="/usr/share/fonts/truetype/dejavu/"
endif

.PHONY: all clean distclean

all: font

font: font_size_10.fnt font_size_12.fnt font_size_15.fnt font_size_15_bold.fnt font_size_20.fnt

font_size_10.fnt:
	gfxboot-font -v -a 0-65535 -p $(FONT_PATH) -f DejaVuSans:size=10 font_size_10.fnt >font.log

font_size_12.fnt:
	gfxboot-font -v -a 0-65535 -p $(FONT_PATH) -f DejaVuSans:size=12 font_size_12.fnt >font.log

font_size_15.fnt:
	gfxboot-font -v -a 0-65535 -p $(FONT_PATH) -f DejaVuSans:size=15 font_size_15.fnt >font.log

font_size_15_bold.fnt:
	gfxboot-font -v -a 0-65535 -p $(FONT_PATH) -f DejaVuSans:size=15:bold=1 font_size_15_bold.fnt >font.log

font_size_20.fnt:
	gfxboot-font -v -a 0-65535 -p $(FONT_PATH) -f DejaVuSans:size=20 font_size_20.fnt >font.log

clean:
	rm -rf *~ *.log *.fnt

distclean: clean

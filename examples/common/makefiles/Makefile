.PHONY: all clean test distclean

all: bootlogo

bootlogo:
	rm -rf bootlogo.dir
	$(MAKE) -C po
	$(MAKE) -C ../common/fonts
	$(MAKE) -C ../../script
	cp -a ../../script/bootlogo.dir .
	cp *jpg *fnt po/*translation xmlboot.config bootlogo.dir
	gfxboot --archive bootlogo.dir --pack-archive bootlogo

clean:
	rm -rf *~ *.log *.dir *.iso bootlogo
	$(MAKE) -C po clean

test: bootlogo
	gfxboot -p -b isolinux -a bootlogo --save-image test.iso

distclean: clean

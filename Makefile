.PHONY: all clean

all:

clean:
	$(MAKE) -C examples/common/fonts clean
	$(MAKE) -C examples/lernstick_debian6 clean
	$(MAKE) -C examples/lernstick_debian7 clean
	$(MAKE) -C examples/lernstick_debian8 clean
	$(MAKE) -C examples/lernstick_debian8_exam clean
	$(MAKE) -C examples/lernstick_pu_debian6 clean
	$(MAKE) -C examples/lernstick_pu_debian7 clean
	$(MAKE) -C script clean

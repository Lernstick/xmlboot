XMLboot is a script for gfxboot (see http://en.opensuse.org/SDB:Gfxboot)
that supports reading its configuration from  an XML file.

XMLboot supports:
	- a hierarchical menu structure
	- start menu entries
	- option radiobuttons
	- option checkboxes
	- option triggers
	- option screenshots
	- option descriptions
	- internationalization and localization
	- on-the-fly keyboard selection
	- different screen resolutions
	- splash screens for start entries
	- initial timeout

It is designed to also be usable on tablet computers with only
up, down and enter keys.

For splash and icon graphics, only JPG seems to work.
Grayscale mode is not supported, only RGB.
When saving JPGs in GIMP open the detailled options and:
	- progressive mode must be *DISABLED*
	- only 4:2:0 mode is supportted

For compilation you usually need to install some additional packages:
	- itstool
	- translate-toolkit
	- gfxboot-dev

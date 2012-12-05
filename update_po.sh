#!/bin/bash

itstool -o xmlboot.pot xmlboot.config

for i in po/*.po; do

	# update po file
	msgmerge -U $i xmlboot.pot

	# convert po file to null-separated format
	po2csv -i $i | \
	sed '1,1d' | \
	sed -e 's|"\(.*\)","\(.*\)","\(.*\)"|\2\x00\3|' | \
	tr '\n' '\000' | \
	tr -d '\r' > po/$(basename $i).tr
done

rm xmlboot.pot

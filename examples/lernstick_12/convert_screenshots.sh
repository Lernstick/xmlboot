#!/bin/sh
for i in *.png
do
convert "$i" -colorspace YUV -sampling-factor 4:2:0 -scale 200x "$(basename "$i" .png)_200.jpg"
convert "$i" -colorspace YUV -sampling-factor 4:2:0 -scale 300x "$(basename "$i" .png)_300.jpg"
done

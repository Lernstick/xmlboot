#!/bin/sh
for i in *.png
do
convert "$i" -scale 200x "$(basename "$i" .png)_200.jpg"
convert "$i" -scale 300x "$(basename "$i" .png)_300.jpg"
done

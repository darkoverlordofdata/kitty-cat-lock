#!/usr/bin/env bash
#
#  $1 - chached background jpg 
#  $2 - screen width
#  $3 - screen height
#
#	Copy & resize the wallpaper to fit on the device
#
mkdir -p ~/.local/share/catlock/themes

convert $1 -resize $2x$3\! ~/.local/share/catlock/themes/wallpaper.locked.jpg
#
#
if [ -f ~/.iface ]; then

	let center=($2/2)-96
	let top=$3/6

	convert $1 -resize $2x$3\! -fill black -colorize 40% ~/.local/share/catlock/themes/wallpaper.tmp.jpg
	convert ~/.local/share/catlock/themes/wallpaper.tmp.jpg  ~/.local/share/catlock/themes/avatar.png -geometry +$center+$top -composite ~/.local/share/catlock/themes/wallpaper.authorize.jpg
	rm -f ~/.local/share/catlock/themes/wallpaper.tmp.jpg

else

    convert $1 -resize $2x$3 -fill black -colorize 40% ~/.local/share/catlock/themes/wallpaper.authorize.jpg

fi       

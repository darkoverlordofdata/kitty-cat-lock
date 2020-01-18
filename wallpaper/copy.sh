#!/usr/local/bin/bash
#
#  $1 - user data dir, usually ~/.local/share
#  $2 - chached background jpg 
#  $3 - screen width
#  $4 - screen height
#
#	Copy & resize the wallpaper to fit on the device
#
convert $2 -resize $3x$4 $1/metalock/themes/badabing/bg.jpg
#
#	copy to modal background, using avatar (192x192) if available 
#
if [ -f $1/metalock/themes/badabing/avatar.png ]; then

    let center = ($3/2)-96
    let top = $4/6

    convert $2 -resize $3x$4 -fill black -colorize 40% $1/metalock/themes/badabing/box1.jpg
    convert $1/metalock/themes/badabing/box1.jpg  $1/metalock/themes/badabing/avatar.png -geometry +$center+$top -composite $1/metalock/themes/badabing/box.png
    rm -f $1/metalock/themes/badabing/box1.jpg

else

    convert $2 -resize $3x$4 -fill black -colorize 40% $1/metalock/themes/badabing/box.jpg

fi
# install notes



## PINs

This makes it more like my smart phone or windows desktop. I use the same pin on all devices. You can give someone the pin without revealing your password. Without a pin, the return key will unlock the screen.

to lock with a pin: 
catlock --pin 12345


the original metalock requires to be install with these permissions:
```
chmod 755 /usr/bin/metalock
chmod u+s /usr/bin/metalock
```
Since swiching to PIN, this step is removed from catlock. 
Also removed flag -a --as_user.


## Wallpaper Integration

I use my badabing wallpaper to manage this, but it's not required.

imagemagick is required
```
sudo apt install imagemagick
```

set wallpaper theme using wallpaper/copy.sh. On my laptop, this is:
```
bash wallpaper/copy.sh ~/Pictures/badabing.jpg 1366 768
```

Run catlock as:
```
catlock --theme wallpaper
```

if you have an avatar png named ~/.iface, it will be used for an authorize screen.
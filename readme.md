# catlock

## A cat proof screen lock 

###  What is cat proof?

Screen lock programs can get pretty complex, as they can allow you to do things such as adjust your volume or disable wifi without unlocking your screen.

But Bal jumps on, lays on, and attacks my keyboard. She has, among other things, filled my hard drive with screen captures in chromeos, frozen mate screenlock, leading to a hard reboot, and even crashed metalock, letting her do further evil things to my computer,

All of this cool functionality has been stripped out of catlock - it does 2 things:

* display the date and time
* accept your <strike>password</strike> pin

Bal has 'paw-tested' catlock, and has not been able to do anything evil to my computer for a while.

based on: http://chiselapp.com/user/beyert/repository/metalock/index


## Usage

    -h / --help               help (this)
    -V / --version            version information
    -s / --scrot              take screen pics
    -p / --pin                pin to unlock screen
    -c / --calendar           calendar app, such as "orage"
    -v n / --verbosity n      verbosity level (default: 0)
    -f name / --font name     X11 font name 
    -t name / --theme name    theme name (default: badabing)    

## install:
```
./configure
cd build
make
sudo make install
```
for standalone use:
```
bash wallpaper/copy.sh ~/Pictures/MyWallpaper.jpg screen_width screen_height
```

Then add to keybindings, example .config/openbox/rc.xml. 
```
<keybind key="W-l">
    <action name="Execute">
    <command>catlock --pin 12345 --theme wallpaper</command>
    </action>
</keybind>
```


## known issues

* segmentation fault when specifying a calendar

    example:
    catlock --calendar /home/<user>/.local/share/catlock/us_en.ics

## stop screen blank on freebsd:
    xset -dpms
    xset s off


![Screenshot](https://github.com/darkoverlordofdata/kitty-cat-lock/raw/master/assets/0.png "Screenshot")

![Screenshot](https://github.com/darkoverlordofdata/kitty-cat-lock/raw/master/assets/1.png "Screenshot")

![Screenshot](https://github.com/darkoverlordofdata/kitty-cat-lock/raw/master/assets/2.png "Screenshot")
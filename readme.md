# catlock

## A cat proof screen lock 

###  What is cat proof?

Screen lock programs can get pretty complex, as they can allow you to do things such as adjust your volume or disable wifi without unlocking your screen.

But Bal jumps on, lays on, and attacks my keyboard. She has, among other things, filled my hard drive with screen captures in chromeos, frozen mate screenlock, leading to a hard reboot, and even crashed metalock, letting her do further evil things to my computer,

All of this cool functionality has been stripped out of catlock - it does 2 things:

* display the date and time
* accept your <strike>password</strike> pin

Bal has 'paw-tested' catlock, and has not been able to do anything evil to my computer for a while.


## Usage

    -h / --help               help (this)
    -V / --version            version information
    -o / --one_time           exit input loop on ESC
    -s / --scrot              take screen pics
    -a / --as_user            user name
    -c / --calendar           calendar app, such as "orage"
    -v n / --verbosity n      verbosity level (default: 0)
    -f name / --font name     X11 font name 
    -t name / --theme name    theme name (default: badabing)    

## install:
    ./configure
    cd build
    make
    sudo make install


Then add to .config/openbox/rc.xml
```
<keybind key="W-l">
    <action name="Execute">
    <command>catlock</command>
    </action>
</keybind>
```

## testing: 

    sudo ./catlock -a <user>


## known issues

This project is a wip and prototype for my github project catlock. But it is also my daily driver for my FreeBSD desktop, so it should be working. Still, there are some rough spots.

* change from password to pin

    This makes it more like my smart phone or windows desktop. I use the same pin on all devices. You can give someone the pin without revealing your password.

    Incomplete change. Current hardcoded pin is '420420'. Requires password.
    You can change the pin in src/application.c line 333. 
    planned change is new --pin flag.

* image /home/<user>/.local/share/catlock/themes/badabing.locked.jpg does not exist

    this program doesn't set up the environment. You need a folder "~/.local/share/catlock/themes" with 2 images (they can be the same) - locked.jpg and authorize.jpg, png should also work. They should be large enough to cover your screen. On my computer, these images are maintained by another program, 'badabing', but I can also do this manually.

* segmentation fault when specifying a calendar

    example:
    catlock -c /home/<user>/.local/share/catlock/us_en.ics


* flags not working:

    -a or --as_user
        gets the 'image ... does not exist' error.

## stop screen blank on freebsd:
xset -dpms
xset s off

## todo

display today's holiday along with date:

    sudo ./catlock -a <name> -c orage


![Screenshot](https://github.com/darkoverlordofdata/kitty-cat-lock/raw/master/assets/0.png "Screenshot")

![Screenshot](https://github.com/darkoverlordofdata/kitty-cat-lock/raw/master/assets/1.png "Screenshot")

![Screenshot](https://github.com/darkoverlordofdata/kitty-cat-lock/raw/master/assets/2.png "Screenshot")
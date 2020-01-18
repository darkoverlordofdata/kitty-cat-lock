# metalock version and author email address
VERSION=	0.8.1
EMAIL=	"beyert AT SYMBOL fastmail DOT SYMBOL net"

# Customize below to fit your system

# paths (on some xservers, X11PREFIX should be set to /usr/X11R6)
LOCALBASE=	/usr/local
PREFIX=	${LOCALBASE}
X11PREFIX=	${LOCALBASE}

X11INC=	${X11PREFIX}/include
X11LIB=	${X11PREFIX}/lib

DEFAULT_THEME=	Blue_Marble

# compiler and linker (clang or gcc is supported)
CC=	clang

# Linux probably needs this
USE_SHADOW=	no

# This variable does not apply to FreeBSD, it might to other BSDs, though.
USE_BSD_AUTH=	no

# On OpenBSD and Darwin disable this, all others most likely need this enabled
USE_CRYPT=	yes

# Optional libraries
#
# If you care about security, Imlib2 is the biggest risk, followed by Xft.
# Ultragetopt is not a security risk.
USE_ULTRAGETOPT=	yes
USE_IMLIB2=	yes
USE_XFT=	yes

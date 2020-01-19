# catlock - enhanced screen locker
# © 2012 Timothy Beyer
# © 2006-2007 Anselm R. Garbe, Sander van Dijk

include config.mk

# .if ${USE_IMLIB2} == "yes"
OPT_LIBS+=	-lImlib2
OPT_CPPFLAGS+=	-DWITH_IMLIB2
# .endif

# .if ${USE_XFT} == "yes"
OPT_LIBS+=	-lXft
OPT_CPPFLAGS+=	-DWITH_XFT
OPT_INCS+=	"-I${LOCALBASE}/include/freetype2"
# .endif

# Linux probably needs this
# .if ${USE_SHADOW} == "yes"
# OPT_CPPFLAGS+=	-DWITH_SHADOW_H
# .endif

# This variable does not apply to FreeBSD, it might to other BSDs, though.
# .if ${USE_BSD_AUTH} == "yes"
# OPT_CPPFLAGS+=	-DWITH_BSD_AUTH
# .endif

# On OpenBSD and Darwin disable this, all others most likely need this enabled
# .if ${USE_CRYPT} == "yes"
CRYPT=	-lcrypt
# .endif

# includes and libs
INCS=	-I. -I${X11INC} -I${LOCALBASE}/include ${OPT_INCS} \
	-I${LOCALBASE}/include/X11
LIBS=	-lc ${CRYPT} -L${X11LIB} -L${LOCALBASE}/lib -lX11 -lXext -lXpm \
	${OPT_LIBS}

# flags
CPPFLAGS=	-DVERSION=\"${VERSION}\" \
			-DPREFIX=\"${PREFIX}\" \
			-DLOCALBASE=\"${LOCALBASE}\" \
			-DEMAIL=\"${EMAIL}\" \
			-DDEFAULT_THEME=\"${DEFAULT_THEME}\" \
			${OPT_CPPFLAGS}

.if ${CC} == "clang"
# -pedantic disabled because of annoying warnings
CFLAGS=	-Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS=	${LIBS}
.elif ${CC} == "gcc"
# -pedantic disabled because of annoying warning:
# Xfuncproto.h:134:24: warning: ISO C does not permit named variadic macros
CFLAGS=	-std=c99 -Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS=	-s ${LIBS}
.else
CFLAGS=	-std=c99 -Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS=	-s ${LIBS}
.endif

SRC=	main.c application.c helpers.c
HDR=	helper_fns.h
OBJ=	${SRC:.c=.o}

all: options build postclean

options:
	@echo catlock build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"
	@echo "OBJ      = ${OBJ}"
	@echo

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

prebuild:
	@echo building catlock

postclean:
	@echo "finished building catlock"
	@echo
	@rm -f ${OBJ}

build: prebuild ${OBJ}
	@echo CC -o $@
	@echo "Obj: ${OBJ}"
	@${CC} -o catlock ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f catlock ${OBJ} catlock-${VERSION}.tar.bz2 \

dist: clean
	@echo "creating dist tarball catlock-${VERSION}"
	@cp -p ../LICENSE ../README catlock-${VERSION}
	@cp -pR Makefile config.mk ${SRC} ${HDR} catlock-${VERSION}/src
	@tar -cf catlock-${VERSION}.tar catlock-${VERSION}
	@bzip2 catlock-${VERSION}.tar
	@mv catlock-${VERSION}.tar.bz2 ../dist/
	@rm -rf catlock-${VERSION}


install: all
	@echo installing catlock
	@echo installing executable file to ${PREFIX}/bin
	@mkdir -p ${PREFIX}/bin
	@cp -f catlock ${PREFIX}/bin
	@chmod 755 ${PREFIX}/bin/catlock
	@chmod u+s ${PREFIX}/bin/catlock
	@echo "finished installing catlock"

uninstall:
	@echo removing executable file from ${PREFIX}/bin

.PHONY: all options clean dist install uninstall

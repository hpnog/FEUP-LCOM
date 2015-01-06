# Makefile for the proj driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	interface.c proj.c vbe.c video_gr.c timer.c keyboard.c bitmap.c mouse.c game.c rtc.c #add all files

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>

clean:
	rm -rf *o *d ${PROG}
# $Id$

PROG = funiq
SRCS = funiq.c mem.c hash.c
MAN = funiq.1
CFLAGS = -Wall

.include <bsd.prog.mk>

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = 
LDLIBS = -lX11 -linput -lm `pkg-config --cflags libinput` `pkg-config --libs libinput`

SRC = logger.c analyser.c main.c
OBJ = ${SRC:.c=.o}

main: ${SRC}
	${CC} ${SRC} ${CFLAGS} ${LDLIBS} -o main -g
clean:
	${RM} ${OBJ}   # remove object files
	${RM} main     # remove main program

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lm
LDFLAGS = 
LDLIBS = -lX11 -linput -lSDL2_image `pkg-config --cflags libinput sdl2` `pkg-config --libs libinput sdl2`

SRC = utils/screenSize.c logger.c utils/sdl.c utils/scaleColor.c analyser.c main.c
OBJ = ${SRC:.c=.o}

main: ${SRC}
	${CC} ${SRC} ${CFLAGS} ${LDLIBS} -o main -g
clean:
	${RM} ${OBJ}   # remove object files
	${RM} main     # remove main program

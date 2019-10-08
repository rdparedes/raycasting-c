PROGRAM = Raycast
FILES.c = trigonometry.cpp player.cpp raycast.cpp
FILES.h = config.hpp trigonometry.hpp player.hpp
FILES.o = ${FILES.c:.cpp=.o}

CC 		= g++
LDLIBS 	= -lSDL2 -lSDL2_image

all: 	${PROGRAM}

${PROGRAM}: ${FILES.o}
	${CC} -o $@ ${FILES.o} ${LDLIBS}

trigonometry.o: ${FILES.h}
player.o: 		${FILES.h}
raycast.o: 		${FILES.h}

RM_FR = rm -rf

clean:
	${RM_FR} ${FILES.o} ${PROGRAM}
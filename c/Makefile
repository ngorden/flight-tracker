CC		:= gcc
CFLAGS	:= -Wall -Werror -Wextra -Wpedantic -std=gnu99
LFLAGS	:= -lm

EXE 	:= tracker
BIN		:= bin/
OBJ		:= ${BIN}obj/
SRC		:= ./

SRCS	:= $(wildcard ${SRC}*.c)
OBJS	:= $(patsubst ${SRC}%.c,${OBJ}%.o,${SRCS})

debug: CFLAGS += -g -O0 -DDEBUG
debug: all

release: CFLAGS += -O2 -DNDEBUG
release: all

all: ${OBJ} ${EXE}

clean: 
	${RM} ${OBJS}

${EXE}: ${OBJS}
	${CC} ${CFLAGS} ${LFLAGS} $^ -o ${BIN}$@

${OBJ}%.o: ${SRC}%.c
	${CC} ${CFLAGS} -c $< -o $@

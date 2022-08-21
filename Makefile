CC = clang
CFLAGS = -g -O2 -mavx2 -mlzcnt
DEPS = 
OBJ = strlen.o

%.o: %.c ${DEPS}
	${CC} ${CFLAGS} -c -o $@ $<

strlen: ${OBJ} 
	${CC} ${CFLAGS} -o $@.out $^



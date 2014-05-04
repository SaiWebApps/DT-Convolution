OPTFLAGS=-O3 -msse3
FILE=dt_conv.c

all:
	gcc ${OPTFLAGS} ${FILE}

baseline:
	gcc ${FILE}

clean:
	rm -rf a.out

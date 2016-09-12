CFLAGS+=	-Wall -g
BIN=		pipe socketpair unixdgram1 unixdgram2 inetdgram1 inetdgram2 \
			inetstream1 inetstream2 inetstream3 unixstream1 unixstream2

all: ${BIN}

clean:
	rm -f ${BIN}

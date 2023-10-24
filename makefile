CFLAGS=-Wall -Werror -Wpedantic -std=c89 -D_GNU_SOURCE

all: main.c ship/ship.c port/port.c common/sync/sync.c common/message/message.c common/utils/utils.c
	gcc ${CFLAGS} -o bin/main main.c
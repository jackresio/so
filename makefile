CC = gcc
CFLAGS=-Wall -Werror -Wpedantic -std=c89 -D_GNU_SOURCE
MSG = src/common/message/message.c
SYNC = src/common/sync/sync.c
UTILS = src/common/utils/config.c src/common/utils/utils.c

all: obj/main.o obj/port.o obj/ship.o obj/utils.o obj/msg.o obj/sync.o obj/config.o src/common/utils/map.c
	$(CC) $(CFLAGS) obj/port.o obj/msg.o obj/sync.o obj/util.o
	$(CC) $(CFLAGS) obj/ship.o obj/msg.o obj/sync.o obj/util.o
	$(CC) $(CFLAGS) obj/main.o obj/msg.o obj/sync.o obj/util.o

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -c $^ -o obj/main.o

obj/port.o: src/port/port.c
	$(CC) $(CFLAGS) -c $^ -o obj/port.o

obj/ship.o: src/ship/ship.c
	$(CC) $(CFLAGS) -c $^ -o obj/ship.o

obj/utils.o: $(UTILS)
	$(CC) $(CFLAGS) -c $^
	mv config.o utils.o map.o obj

obj/msg.o: $(MSG)
	$(CC) $(CFLAGS) -c $^ -o obj/msg.o

obj/sync.o: $(SYNC)
	$(CC) $(CFLAGS) -c $^ -o obj/sync.o


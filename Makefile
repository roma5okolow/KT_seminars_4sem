CC = gcc
CFLAGS = -Wall -Werror
PSWD_FLAGS = -lpam -lpam_misc

all:
	$(CC) $(CFLAGS) main/client.c log/log.c broadcast/broadcast.c tcp-udp/tcp-udp.c pswd/pswd.c -o client $(PSWD_FLAGS)
	$(CC) $(CFLAGS) main/server.c log/log.c broadcast/broadcast.c tcp-udp/tcp-udp.c -o server
CC ?= gcc
CFLAGS ?= -Wall -Werror
ipath = $()

stlib = $(realpath lib/mylib.a)

inc_path = $(realpath include)

all: src 
    echo $(inc_path)

install: src
    cp src/executable
    cp lib/mylib.so /home

uninstall:
    rm -rf /home...

.PHONY src
src: make -C src

.PHONY lib
lib: make -C lib

clean: make clean -C src
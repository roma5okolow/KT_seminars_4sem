obj = 1.o main.o

flags = $(slib)

ifeq ($(LIBTYPE), DYNAMIC)
    flags = -lmylib -L$(lpath)!
endif

all: build

%.o: %.c
    $(CC) $(CFLAGS) $< -c -o #@

build: $(obj)
    $(CC) $(ALL_O_FILES) $(CFLAGS) -o executable

clean:
    rm -rf ./*.o executable
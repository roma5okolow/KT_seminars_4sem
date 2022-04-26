obj = mylib.o

%.o: %.c
	$(CC) %(CCFLAGS) -I$(ipath) -fPIC -c $< -c $@

static: 	
	ar rcs mylib.a $^

dynamic: $(obj)
	$(CC) $(obj) -shared -o libmylib.so

clean:
	rm -rf *.so 
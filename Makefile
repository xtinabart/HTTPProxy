all: httpproxy

httpproxy: httpproxy.c mongoose.c
	$(CC) -g -o $@ $^

clean:
	rm -f httpproxy

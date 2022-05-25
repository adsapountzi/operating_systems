all: 
	gcc -Wall -g bbfs.c log.c myfslib.c -fsanitize=address -lcrypto -D_FILE_OFFSET_BITS=64 -lfuse\
		`pkg-config fuse --cflags --libs` \
		`pkg-config libssl --cflags --libs` -o bbfs

clean:
	rm -rf ./bbfs
	rm -r ../example/.realStorage
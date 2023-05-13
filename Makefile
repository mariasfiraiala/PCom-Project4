CC=g++
CFLAGS=-Wall -Werror -Wno-error=unused-variable -g

client: client.cpp requests.cpp helpers.cpp buffer.cpp parser.cpp commands.cpp
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o client

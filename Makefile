CC=g++
CCFLAGS=-Wall -Werror -Wno-error=unused-variable -g

client: client.cpp requests.cpp helpers.cpp buffer.cpp parser.cpp commands.cpp
	$(CC) -o $@ $^ $(CCFLAGS)

clean:
	rm -f *.o client

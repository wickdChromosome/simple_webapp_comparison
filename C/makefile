CCFLAGS=

all: bin/server

debug: CCFLAGS += -DDEBUG -g
debug: bin/server

bin/server:  
	mkdir -p bin
	cd src && \
	$(CC) $(CCFLAGS) server.c -o ../bin/server

clean:
	rm bin/server

run:
	./bin/server

rt:
	rm bin/server
	cd src && \
	$(CC) $(CCFLAGS) server.c -o ../bin/server
	./bin/server


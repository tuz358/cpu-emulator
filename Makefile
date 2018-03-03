CC      = g++
CFLAGS  = -Wall
DEST    = /usr/local/bin
OBJS    = main.o emulator.o instructions.o memory.o
PROGRAM = cpu-emulator

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(PROGRAM)
	make clean

.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f ./*.o

install: $(PROGRAM)
	install -s $(PROGRAM) $(DEST)

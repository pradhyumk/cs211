OUTPUT=interpret
OBJECTS=interpret.o
CFLAGS=-g -Wall -fsanitize=address -std=c99
LFLAGS=-lm

%.o: %.c %.h
	gcc $(CFLAGS) -c $<

%.o: %.c
	gcc $(CFLAGS) -c $<

all: $(OUTPUT)

interpret: $(OBJECTS)
	gcc $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -f *.o $(OUTPUT)

OUTPUT=first
CFLAGS=-g -Wall -fsanitize=address -std=c99
LDFLAGS=-lm

%: %.c %.h
	gcc $(CFLAGS) -o $@ $< $(LDFLAGS)

%: %.c
	gcc $(CFLAGS) -o $@ $< $(LDFLAGS)

all: $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)

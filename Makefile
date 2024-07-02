CC = gcc
CFLAGS = -Wall -Wextra -std=c99

EXEC1 = rainy

.PHONY: all clean

all: $(EXEC1) 

$(EXEC1): weather.c
	$(CC) $(CFLAGS) weather.c -o $(EXEC1)

clean:
	rm -f $(EXEC1) $(EXEC2)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

EXEC1 = weather
EXEC2 = weather2

.PHONY: all clean

all: $(EXEC1) $(EXEC2)

$(EXEC1): weather.c
	$(CC) $(CFLAGS) weather.c -o $(EXEC1)

$(EXEC2): weather2.c
	$(CC) $(CFLAGS) weather2.c -o $(EXEC2)

clean:
	rm -f $(EXEC1) $(EXEC2)
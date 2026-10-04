CC     = gcc
CFLAGS = -Wall -Wextra -g
TARGET = charlog
OBJS   = main.o keys.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c keys.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
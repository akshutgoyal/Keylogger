CC     = gcc
CFLAGS = -Wall -Wextra -g
TARGET = keylogger
OBJS   = main.o keys.o logger.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c keys.h logger.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

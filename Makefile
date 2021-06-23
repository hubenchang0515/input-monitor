CFLAGS := -W -Wall -Wextra -O3 -std=c11

TARGET := input-monitor
OBJS := main.o input_event.o monitor.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

main.o: input_event.h monitor.h
input_event.o: input_event.h input_event_code_name.h
monitor.o: monitor.h

clean:
	rm -f $(OBJS)
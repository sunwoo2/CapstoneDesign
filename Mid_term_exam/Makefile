
CC = gcc
CCFLAGS = -W -Wall -O2 -D_REENTRANT
LFLAGS = -pthread -lpthread
OBJS = Mid-term.o
TARGET = $(OBJS:.o=)

print-%: ; @echo $*=$($*)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $(LFLAGS) -o $@ $^ 

clean:
	@echo "Deleting"
	rm -rf $(TARGET) $(OBJS)

run:
	@echo "Executing"
	./$(TARGET)

.c.o:
	$(CC) -c $(CCFLAGS) $<

help:
	@echo "make clean"
	@echo "make run"

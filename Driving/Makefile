
CC = gcc
CCFLAGS = -D_REENTRANT
LFLAGS = -lpigpio -lrt -pthread -lpthread
OBJS = driving.o
TARGET = $(OBJS:.o=)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $(LFLAGS) -o $@ $^ 

run:
	./$(TARGET)

clean:
	rm -rf $(TARGET) $(OBJS)

.c.o:
	$(CC) -c $(CCFLAGS) $<

CC = clang
CFLAGS = -g -mavx2 -mlzcnt -O2

TARGET = testing

all = $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET).out $(TARGET).c

clean:
	rm -rf *.out

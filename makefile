TARGET=projektenigma
MYLIB=enigma
CC=gcc
CFLAGS=-g -Wall
LDFLAGS= -L. -lm -l$(MYLIB)
all: $(TARGET) 
$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(TARGET).o: lib$(MYLIB).so $(MYLIB).h
lib$(MYLIB).so : $(MYLIB).c
	${CC} $^ ${CFLAGS} -shared -fPIC -L. -o $@
enigma.o: enigma.c 
	$(CC) $(CFLAGS) -c $@ 
#$(MYLIB).c: $(MYLIB).h
clean:
	rm -f core *.o $(TARGET) *.so

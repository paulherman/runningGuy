CC=g++
CFLAGS=-c -O2
LDFLAGS=-lIrrlicht -lm
SOURCES=main.cpp engine.cpp configuration.cpp statePlaying.cpp stateMenu.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=runningGuy

all: $(SOURCES) $(EXECUTABLE)
	rm *.o

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o runningGuy


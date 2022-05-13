# Configuration
CC=gcc
CFLAGS=-std=c99
LDFLAGS=
SOURCES=proj3.c
EXECUTABLE=proj3

# Create list of objects corresponding to sources
OBJECTS=$(SOURCES:.c=.o)

# Compile sources, link executable, and clean up
all: $(SOURCES) $(EXECUTABLE) clean

# Handle all C source files
.c:
	$(CC) $(CFLAGS) -o $@ $<

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Clean up after build
clean:
	rm *.o

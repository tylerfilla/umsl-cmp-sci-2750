# Configuration
CC=gcc
CFLAGS=-std=c99
LDFLAGS=
SOURCES=proj2.c
EXECUTABLE=proj2

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

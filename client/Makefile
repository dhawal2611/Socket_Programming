# Define compiler and flags
CC = gcc
CFLAGS = -Wall -g

CUR_DIR = $(CURDIR)

# Define the output directory
BIN_DIR = $(CUR_DIR)/../build

# Define the list of targets (the full paths to the executables)
TARGETS = $(BIN_DIR)/client #$(BIN_DIR)/client

# Default target: build all executables
all: $(TARGETS)

# Rule for the first client
$(BIN_DIR)/client: client.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Rule for the second client
#$(BIN_DIR)/client2: client2.c | $(BIN_DIR)
#	$(CC) $(CFLAGS) -o $@ $<

# Clean target to remove the binaries and the bin folder
clean:
	rm -rf $(TARGETS)
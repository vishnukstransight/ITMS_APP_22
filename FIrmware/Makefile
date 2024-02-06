# Compiler and flags
CC = gcc
CFLAGS = -Wall -pthread -lm -lgps 
LDFLAGS = -pthread -lm -lgps -lcjson -lncurses 
INC = -Iext_lib/ipc -Isrc -Isrc/tasks -Isrc/system_init -Iext_lib/apps/ts_vlt_app_0022 -Iext_lib/user_led

# Directories containing source files
SRC_DIRS = src ext_lib

# Recursively find all C source files in the specified directories
SRCS = $(shell find $(SRC_DIRS) -name '*.c')

# Object files will be placed in a separate build directory
OBJ_DIR = build
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Name of the executable
TARGET = app

# Default target (build the executable)
all: $(TARGET)
	@echo "Build completed at: $(shell date)"
# Rule to build the executable from object files
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Rule to compile C source files into object files
$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Clean completed at: $(shell date)"

# PHONY targets to avoid conflicts with file names
.PHONY: all clean
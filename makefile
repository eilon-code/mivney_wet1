# Compiler settings
CC = gcc
CXX = g++

# Directories
DIR = code/
PIRATE_DIR = $(DIR)pirate/
SHIP_DIR = $(DIR)ship/
TREE_DIR = $(DIR)search_tree/
BUILD_DIR = build/

# Source files
MAIN_SRC = $(DIR)main24b1.cpp
OCEAN_SRC = $(DIR)pirates24b1.cpp
TREE_SRC = $(TREE_DIR)avl_tree.h
PIRATE_SRC = $(PIRATE_DIR)pirate.cpp
SHIP_SRC = $(SHIP_DIR)ship.cpp

# Object files
MAIN_OBJ = $(BUILD_DIR)main.o
OCEAN_OBJ = $(BUILD_DIR)ocean.o
TREE_OBJ = $(BUILD_DIR)tree.o
PIRATE_OBJ = $(BUILD_DIR)pirate.o
SHIP_OBJ = $(BUILD_DIR)ship.o

# All object files
OBJS = $(MAIN_OBJ) $(OCEAN_OBJ) $(PIRATE_OBJ) $(SHIP_OBJ)

# Executable name
EXEC = prog

# Flags
DEBUG_FLAG = # Leave empty, assign -g for debug
COMP_FLAG = -std=c++11 -Wall -Werror
INCLUDE_FLAGS = -I$(DIR) -I$(PIRATE_DIR) -I$(SHIP_DIR) -I$(TREE_DIR)

# Default target
$(EXEC): $(OBJS)
	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $(BUILD_DIR)$@

# Main object file
$(MAIN_OBJ): $(MAIN_SRC) $(DIR)wet1util.h | $(BUILD_DIR)
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $(INCLUDE_FLAGS) $< -o $@

# Ocean object file
$(OCEAN_OBJ): $(OCEAN_SRC) $(DIR)pirates24b1.h $(DIR)wet1util.h $(TREE_SRC) | $(BUILD_DIR)
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $(INCLUDE_FLAGS) $< -o $@

# Pirate object file
$(PIRATE_OBJ): $(PIRATE_SRC) $(PIRATE_DIR)pirate.h $(PIRATE_DIR)pirate_rank.h $(SHIP_DIR)ship.h | $(BUILD_DIR)
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $(INCLUDE_FLAGS) $< -o $@

# Ship object file
$(SHIP_OBJ): $(SHIP_SRC) $(SHIP_DIR)ship.h $(TREE_SRC) $(PIRATE_DIR)pirate.h | $(BUILD_DIR)
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $(INCLUDE_FLAGS) $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean build directory
clean:
	@rm -rf $(BUILD_DIR)

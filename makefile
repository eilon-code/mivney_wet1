CC = gcc
CXX = g++
DIR = code/
BUILD_DIR = build/

MAIN = $(BUILD_DIR)main.o
PIRATES = $(BUILD_DIR)pirates.o
OBJS = $(MAIN) $(PIRATES)

EXEC = prog
DEBUG_FLAG = # now empty, assign -g for debug
COMP_FLAG = -std=c++11 -Wall -Werror

$(EXEC): $(OBJS)
	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $(BUILD_DIR)$@

$(MAIN): $(DIR)main24b1.cpp $(DIR)wet1util.h | $(BUILD_DIR)
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $< -o $@

$(PIRATES): $(DIR)pirates24b1.cpp $(DIR)pirates24b1.h $(DIR)wet1util.h | $(BUILD_DIR)
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $< -o $@

$(BUILD_DIR):
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

clean:
	@if exist "$(BUILD_DIR)" rmdir /S /Q "$(BUILD_DIR)"

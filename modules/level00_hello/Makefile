ROOT:=../..
BIN_DIR:= $(ROOT)/bin

TARGET:= $(BIN_DIR)/level00_hello
SRC:= main.cpp

CXX:= g++
CXXFLAGS:= -std=c++17 -g -O2 -Wall -Wextra -MMD -MP

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(BIN_DIR)/level00_hello
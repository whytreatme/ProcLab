#============= 全局设置 ========================

ROOT := .
SRC_DIR := $(ROOT)/src
BIN_DIR := $(ROOT)/BIN_DIR

CXX := g++
CXXFLAGS := -std=c++17 -Wall _wextra -O2

#============= 自动搜集所有.cpp 文件 ===================
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
TARGETS := $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%, $(SRCS))

.PHONY: all clean run 

#======== 默认规则：编译所有文件 ================
all: $(TARGETS)
	@echo "All targets built."

# =========== 通用编译规则 ===============
$(BIN_DIR)/%: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "[OK] $@ built."

#============ 运行单个程序 =============
run:
	@if[ -z "$(prog)" ]; then \
		echo "Usage: make run prog=<name>";\
	 else \
	 		$(BIN_DIR)/$(prog);\
	 fi

clean:
	rm -f $(BIN_DIR)/*
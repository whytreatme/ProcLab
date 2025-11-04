#============= 全局设置 ========================
ROOT     := .
SRC_DIR  := $(ROOT)/src
BIN_DIR  := $(ROOT)/bin

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

#============= 自动搜集所有 .cpp 文件 ===================
SRCS    := $(wildcard $(SRC_DIR)/*.cpp)
TARGETS := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%,$(SRCS))

.PHONY: all clean run run-%

#======== 默认规则：编译所有文件 ================
all: $(TARGETS)
	@echo "All targets built."

# =========== 通用编译规则 ===============
$(BIN_DIR)/%: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "[OK] $@ built."

#============ 运行：两种方式 =============
# 方式A：make run prog=supervisor_min
run:
	@if [ -z "$(prog)" ]; then \
		echo "Usage: make run prog=<name>"; \
	else \
		"$(BIN_DIR)/$(prog)"; \
	fi

# 方式B：make run-supervisor_min
run-%: $(BIN_DIR)/%
	@$(BIN_DIR)/$*

#============ 清理 =======================
clean:
	@rm -f $(BIN_DIR)/* 2>/dev/null || true
	@rmdir $(BIN_DIR) 2>/dev/null || true

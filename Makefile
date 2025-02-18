# 编译器 & 编译选项
CC = g++
CFLAGS = -Wall -Iinclude
CFLAGS_CHECK = -Wall -Iinclude -fsanitize=address -fno-omit-frame-pointer

# 目录定义
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
OBJ_DIR = build
TEST_DIR = test

# 代码文件 & 目标文件
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

TARGET = $(BIN_DIR)/main

# 默认编译 & 运行主程序
all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ===============================
# 运行测试
# ===============================
# `make test TEST=<文件名>` 运行单个测试
# `make test` 运行所有测试
# `make test TEST=<文件名> ASAN=1` 运行带 ASan 的测试
TEST_SRC_FILES = $(wildcard $(TEST_DIR)/*.cpp)

ifeq ($(ASAN),1)
	TEST_CFLAGS = $(CFLAGS_CHECK)
	TEST_SUFFIX = _asan
else
	TEST_CFLAGS = $(CFLAGS)
	TEST_SUFFIX =
endif

.PHONY: test

test: 
ifeq ($(TEST),)  # 如果未指定 TEST 变量，运行所有测试
	@echo "Running all tests..."
	@mkdir -p $(BIN_DIR)
	@for file in $(TEST_SRC_FILES); do \
		test_exec=$(BIN_DIR)/$$(basename $$file .cpp)$(TEST_SUFFIX); \
		$(CC) $(TEST_CFLAGS) $$file -o $$test_exec; \
		echo "Running $$test_exec"; \
		./$$test_exec; \
	done
else  # 如果指定了 TEST 变量，运行指定的测试
	@mkdir -p $(BIN_DIR)
	@echo "Compiling and running test: $(TEST)"
	$(CC) $(TEST_CFLAGS) $(TEST_DIR)/$(TEST) -o $(BIN_DIR)/$(basename $(TEST))$(TEST_SUFFIX)
	@./$(BIN_DIR)/$(basename $(TEST))$(TEST_SUFFIX)
endif

# ===============================
# 运行 AddressSanitizer (ASan) 版本的主程序
# ===============================
check: $(TARGET)_asan

$(TARGET)_asan: $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS_CHECK) $^ -o $@
	./$(TARGET)_asan

# ===============================
# 清理编译文件
# ===============================
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# LuduScript Makefile
# 支持多平台构建的简单Makefile

# 编译器设置
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude
LDFLAGS := 

# 目录设置
SRCDIR := src
INCDIR := include
BUILDDIR := build
BINDIR := bin
OBJDIR := $(BUILDDIR)/obj

# 目标文件名
TARGET := luduscript

# 检测操作系统
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    RM := del /Q
    MKDIR := mkdir
    RMDIR := rmdir /S /Q
    PATHSEP := \\
else
    RM := rm -f
    MKDIR := mkdir -p
    RMDIR := rm -rf
    PATHSEP := /
endif

# 源文件（排除legacy文件）
SOURCES := $(filter-out %legacy.cpp, $(wildcard $(SRCDIR)/*.cpp))
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPENDS := $(OBJECTS:.o=.d)

# 默认目标
.PHONY: all clean install test help

all: $(BINDIR)/$(TARGET)

# 创建可执行文件
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# 编译对象文件
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# 创建目录
$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(BINDIR):
	$(MKDIR) $(BINDIR)

# 包含依赖文件
-include $(DEPENDS)

# 清理构建文件
clean:
ifeq ($(OS),Windows_NT)
	-$(RMDIR) $(BUILDDIR) 2>nul || true
	-$(RMDIR) $(BINDIR) 2>nul || true
else
	$(RMDIR) $(BUILDDIR) $(BINDIR)
endif

# 安装（简单版本）
install: $(BINDIR)/$(TARGET)
	@echo "Installing $(TARGET)..."
ifeq ($(OS),Windows_NT)
	@echo "Please manually copy $(BINDIR)$(PATHSEP)$(TARGET) to your desired location"
else
	cp $(BINDIR)/$(TARGET) /usr/local/bin/
endif

# 测试
test: $(BINDIR)/$(TARGET)
	@echo "Running tests..."
	$(BINDIR)/$(TARGET) scripts/generate_poker_deck.gen --output output/test_poker.json
	$(BINDIR)/$(TARGET) scripts/generate_werewolf_cards.gen --output output/test_werewolf.json
	@echo "Tests completed!"

# 调试版本
debug: CXXFLAGS += -g -DDEBUG
debug: $(BINDIR)/$(TARGET)

# 发布版本
release: CXXFLAGS += -O2 -DNDEBUG
release: $(BINDIR)/$(TARGET)

# 帮助信息
help:
	@echo "LuduScript Makefile"
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build files"
	@echo "  debug    - Build debug version"
	@echo "  release  - Build optimized release version"
	@echo "  install  - Install the executable"
	@echo "  test     - Run basic functionality tests"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Usage examples:"
	@echo "  make           # Build the project"
	@echo "  make clean     # Clean build files"
	@echo "  make release   # Build optimized version"
	@echo "  make test      # Run tests"
#!/bin/bash
# LuduScript Unix/Linux构建脚本
# 使用CMake或Makefile构建项目

set -e  # 遇到错误时退出

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"
BIN_DIR="$PROJECT_DIR/bin"

echo "LuduScript 构建脚本"
echo "=================="

# 显示帮助信息
show_help() {
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  clean    - 清理构建文件"
    echo "  cmake    - 使用CMake构建"
    echo "  make     - 使用Makefile构建"
    echo "  test     - 运行测试"
    echo "  install  - 安装到系统"
    echo "  help     - 显示此帮助信息"
    echo ""
    echo "默认使用CMake构建（如果可用），否则使用Makefile"
}

# 清理构建文件
clean_build() {
    echo "清理构建文件..."
    rm -rf "$BUILD_DIR" "$BIN_DIR"
    echo "清理完成！"
}

# 运行测试
run_tests() {
    echo "运行测试..."
    if [ -f "$BIN_DIR/luduscript" ]; then
        "$BIN_DIR/luduscript" scripts/generate_poker_deck.script --output output/test_poker.json
        "$BIN_DIR/luduscript" scripts/generate_werewolf_cards.script --output output/test_werewolf.json
        echo "测试完成！"
    else
        echo "错误: 找不到可执行文件，请先构建项目"
        exit 1
    fi
}

# CMake构建
cmake_build() {
    echo "使用CMake构建..."
    
    # 创建构建目录
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    # 配置项目
    echo "配置项目..."
    cmake .. -DCMAKE_BUILD_TYPE=Release
    
    # 构建项目
    echo "构建项目..."
    cmake --build . --config Release
    
    echo "CMake构建完成！"
    echo "可执行文件位于: $BUILD_DIR/bin/luduscript"
}

# Makefile构建
make_build() {
    echo "使用Makefile构建..."
    cd "$PROJECT_DIR"
    make release
    echo "Make构建完成！"
    echo "可执行文件位于: $BIN_DIR/luduscript"
}

# 安装到系统
install_system() {
    echo "安装到系统..."
    if [ -f "$BIN_DIR/luduscript" ]; then
        sudo cp "$BIN_DIR/luduscript" /usr/local/bin/
        echo "安装完成！luduscript已安装到/usr/local/bin/"
    elif [ -f "$BUILD_DIR/bin/luduscript" ]; then
        sudo cp "$BUILD_DIR/bin/luduscript" /usr/local/bin/
        echo "安装完成！luduscript已安装到/usr/local/bin/"
    else
        echo "错误: 找不到可执行文件，请先构建项目"
        exit 1
    fi
}

# 检查命令是否存在
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# 主逻辑
case "$1" in
    clean)
        clean_build
        ;;
    help)
        show_help
        ;;
    test)
        run_tests
        ;;
    install)
        install_system
        ;;
    cmake)
        if ! command_exists cmake; then
            echo "错误: CMake不可用"
            exit 1
        fi
        cmake_build
        ;;
    make)
        if ! command_exists make; then
            echo "错误: Make不可用"
            exit 1
        fi
        make_build
        ;;
    "")
        # 默认构建
        if command_exists cmake; then
            echo "使用CMake构建..."
            cmake_build
        elif command_exists make; then
            echo "使用Makefile构建..."
            make_build
        else
            echo "错误: 没有找到可用的构建系统（CMake或Make）"
            echo "请安装CMake或Make，或者手动编译源文件"
            exit 1
        fi
        ;;
    *)
        echo "未知选项: $1"
        show_help
        exit 1
        ;;
esac

echo ""
echo "构建脚本执行完成！"
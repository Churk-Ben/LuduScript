# LuduScript

一个专为游戏卡牌生成设计的领域特定语言（DSL）。LuduScript 允许您使用简洁的脚本语法来定义和生成各种类型的游戏卡牌，如扑克牌、狼人杀卡牌等。

## 特性

- 🎯 **专门为游戏卡牌设计** - 内置对卡牌属性、套装、数值等概念的支持
- 📝 **简洁的语法** - 易于学习和使用的脚本语言
- 🔄 **灵活的输出格式** - 支持JSON等多种输出格式
- 🎲 **丰富的内置函数** - 提供随机数生成、数组操作等实用功能
- 🏗️ **模块化架构** - 清晰的词法分析、语法分析和解释执行分离
- 🔧 **跨平台支持** - 支持Windows、Linux和macOS

## 快速开始

### 构建项目

#### 使用CMake（推荐）

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake .. -DCMAKE_BUILD_TYPE=Release

# 构建
cmake --build . --config Release
```

#### 使用Makefile

```bash
# 构建发布版本
make release

# 或者构建调试版本
make debug
```

#### 使用构建脚本

**Windows:**

```cmd
build.bat
```

**Linux/macOS:**

```bash
chmod +x build.sh
./build.sh
```

### 运行示例

构建完成后，您可以运行预置的示例脚本：

```bash
# 生成扑克牌
./bin/luduscript scripts/generate_poker_deck_en.gen --output output/poker_cards.json

# 生成狼人杀卡牌
./bin/luduscript scripts/generate_werewolf_cards.gen --output output/werewolf_cards.json
```

## 项目结构

```text
LuduScript/
├── src/                    # 源代码文件
│   ├── main.cpp           # 主程序入口
│   ├── lexer.cpp          # 词法分析器
│   ├── parser.cpp         # 语法分析器
│   ├── parser_expr.cpp    # 表达式解析
│   ├── interpreter.cpp    # 解释器核心
│   ├── interpreter_stmt.cpp # 语句执行
│   └── ast.cpp            # 抽象语法树
├── include/               # 头文件
│   ├── lexer.h
│   ├── parser.h
│   ├── interpreter.h
│   ├── ast.h
│   └── nlohmann/          # JSON库
├── scripts/               # 示例脚本
│   ├── generate_poker_deck_en.gen
│   └── generate_werewolf_cards.gen
├── examples/              # 更多示例和测试文件
├── output/                # 输出文件目录
├── build/                 # 构建文件（生成）
├── docs/                  # 文档
├── CMakeLists.txt         # CMake构建配置
├── Makefile              # Make构建配置
├── build.bat             # Windows构建脚本
├── build.sh              # Unix/Linux构建脚本
└── README.md             # 本文件
```

## 内置函数

- `output(value)` - 输出值到结果
- `print(value)` - 打印值到控制台
- `random(min, max)` - 生成指定范围内的随机数
- `shuffle(array)` - 随机打乱数组
- `length(array)` - 获取数组长度
- `push(array, value)` - 向数组添加元素
- `pop(array)` - 从数组移除最后一个元素

## 数据类型

- **数字** - 整数和浮点数
- **字符串** - 文本数据
- **布尔值** - true/false
- **数组** - 有序元素集合
- **对象** - 键值对集合
- **null** - 空值

## 开发

### 添加新功能

1. 在相应的头文件中声明新功能
2. 在对应的源文件中实现
3. 更新解释器以支持新语法
4. 添加测试用例

### 调试

使用调试版本进行开发：

```bash
# CMake调试构建
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

# 或使用Makefile
make debug
```

## 贡献

欢迎提交问题报告和功能请求！如果您想贡献代码：

1. Fork 本项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 许可证

本项目采用 Apache 2.0 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 更新日志

### v1.0.0

- 初始版本发布
- 基本的脚本语言功能
- 扑克牌和狼人杀卡牌生成示例
- 跨平台构建支持
- 模块化代码架构

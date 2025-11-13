# LuduScript

一个专为游戏卡牌生成设计的领域特定语言（DSL）。LuduScript 允许您使用简洁的脚本语法来定义和生成各种类型的游戏卡牌，如扑克牌、狼人杀卡牌等。

> 当前为 LuduScript.Gen. 敬请期待 LuduScript.Lds 完全体!

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

#### Windows平台

```cmd
# 使用PowerShell或命令提示符
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

#### Linux/macOS平台

```bash
# 创建构建目录并构建
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### 运行示例

构建完成后，您可以运行预置的示例脚本：

```bash
# 运行基本语法示例
./bin/luduscript examples/in/e1.gen

# 生成扑克牌
./bin/luduscript examples/in/poker.gen

# 使用输出重定向保存结果
./bin/luduscript examples/in/poker.gen --output output/poker_cards.json
```

## 语法示例

> test


见 [语法规范](docs/syntax.md)

## 项目结构

```text
LuduScript/
├── src/                   # 源代码文件
│   ├── main.cpp          # 主程序入口
│   ├── lexer.cpp         # 词法分析器
│   ├── parser.cpp        # 语法分析器
│   ├── parser_expr.cpp   # 表达式解析
│   ├── interpreter.cpp   # 解释器核心
│   ├── interpreter_stmt.cpp # 语句执行
│   ├── ast.cpp           # 抽象语法树
│   └── ludus_legacy/     # 遗留代码
│       └── LuduScript.cpp
├── include/              # 头文件
│   ├── lexer.h
│   ├── parser.h
│   ├── interpreter.h
│   ├── ast.h
│   └── nlohmann/         # JSON库
│       └── json.hpp
├── examples/             # 示例和测试文件
│   ├── in/              # 输入示例文件
│   │   ├── e1.gen       # 基本语法示例
│   │   ├── e2.gen       # 更多示例...
│   │   ├── ...
│   │   └── poker.gen    # 扑克牌生成示例
│   └── out/             # 输出结果文件
│       ├── e1.json
│       ├── e2.json
│       ├── ...
│       └── poker.json
├── docs/                # 文档
│   └── syntax.md        # 语法规范文档
├── build/               # 构建文件（生成）
├── CMakeLists.txt       # CMake构建配置
├── LICENSE              # 许可证文件
├── .gitignore           # Git忽略文件配置
└── README.md            # 本文件
```

## 功能特性

### 当前支持的功能

#### 核心语言特性

- **变量声明和赋值** - 支持 `num`、`str`、`bool` 类型的变量
- **对象创建** - 使用 `obj("类名", id)` 语法创建结构化对象
- **控制流** - 支持 `if/else` 条件语句和 `for` 循环
- **表达式计算** - 支持算术运算、逻辑运算和比较运算
- **作用域管理** - 支持嵌套作用域和变量查找
- **JSON输出** - 自动将对象序列化为JSON格式

#### 数据类型

- **数字** - 整数和浮点数，自动类型推断
- **字符串** - 支持转义字符的文本数据
- **布尔值** - `true`/`false` 逻辑值
- **对象** - 键值对集合，自动输出到结果

#### 语法特性

- **复杂初始化块** - 变量可以使用包含控制流的代码块初始化
- **嵌套作用域** - 支持在初始化块中声明临时变量
- **条件表达式** - 在初始化块中使用条件逻辑
- **循环结构** - 支持带步长的for循环：`for(变量, 起始值, 结束值[, 步长])`

#### 内置函数（开发中）

- `output(value)` - 输出值到结果
- `print(value)` - 打印值到控制台  
- `random(min, max)` - 生成指定范围内的随机数
- `shuffle(array)` - 随机打乱数组
- `length(array)` - 获取数组长度
- `push(array, value)` - 向数组添加元素
- `pop(array)` - 从数组移除最后一个元素

#### 高级特性（规划中）

- **数组支持** - 原生数组类型和操作
- **函数定义** - 用户自定义函数
- **模块系统** - 代码模块化和导入
- **错误处理** - 异常处理机制

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
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug

# 运行调试版本
./bin/luduscript_d examples/in/e1.gen
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

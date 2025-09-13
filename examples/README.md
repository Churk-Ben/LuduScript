# LuduScript 示例文件

本目录包含了各种 LuduScript 语言的示例脚本，帮助您学习和理解语言的特性。

## 示例文件列表

### 基础语法示例

#### `basic_syntax.script`

**用途:** 演示 LuduScript 的基本语法特性
**内容:**

- 变量声明和基本数据类型
- 数组和对象操作
- 条件语句和循环
- 基本的输入输出

**运行方式:**

```bash
./bin/luduscript examples/basic_syntax.script --output output/basic_example.json
```

#### `test_simple.script`

**用途:** 简单的卡牌创建示例
**内容:**

- 基本对象创建
- 属性访问和输出
- 适合初学者的入门示例

**运行方式:**

```bash
./bin/luduscript examples/test_simple.script --output output/simple_card.json
```

### 进阶功能示例

#### `functions_and_loops.script`

**用途:** 演示函数定义和各种循环结构
**内容:**

- 函数定义和调用
- 递归函数
- for 循环和 while 循环
- 数组遍历
- 条件筛选

**运行方式:**

```bash
./bin/luduscript examples/functions_and_loops.script --output output/functions_demo.json
```

#### `random_and_arrays.script`

**用途:** 随机数生成和数组操作的综合示例
**内容:**

- 随机数生成（浮点数和整数）
- 数组操作（添加、删除、洗牌）
- 权重随机选择算法
- 游戏道具生成系统

**运行方式:**

```bash
./bin/luduscript examples/random_and_arrays.script --output output/random_demo.json
```

#### `testfile.script`

**用途:** 高级卡牌生成系统示例
**内容:**

- 复杂的卡牌属性系统
- 条件逻辑和分支处理
- 数据分类和统计
- 多类型卡牌生成

**运行方式:**

```bash
./bin/luduscript examples/testfile.script --output output/advanced_cards.json
```

### 其他文件

#### `test_file.txt`

**用途:** 文本格式的简单示例
**内容:** 演示如何在单行中创建和输出对象

#### `test.json`

**用途:** JSON 格式的示例输出
**内容:** 展示 LuduScript 输出的标准 JSON 格式

## 学习路径建议

### 初学者路径

1. **`test_simple.script`** - 了解基本概念
2. **`basic_syntax.script`** - 学习核心语法
3. **`functions_and_loops.script`** - 掌握函数和控制流

### 进阶路径

1. **`random_and_arrays.script`** - 学习随机数和数组操作
2. **`testfile.script`** - 理解复杂的业务逻辑
3. **查看 `scripts/` 目录** - 学习实际应用案例

## 运行所有示例

您可以使用以下脚本批量运行所有示例：

**Windows (PowerShell):**

```powershell
# 创建输出目录
New-Item -ItemType Directory -Force -Path "output/examples"

# 运行所有示例
./bin/luduscript.exe examples/basic_syntax.script --output output/examples/basic_syntax.json
./bin/luduscript.exe examples/test_simple.script --output output/examples/test_simple.json
./bin/luduscript.exe examples/functions_and_loops.script --output output/examples/functions_and_loops.json
./bin/luduscript.exe examples/random_and_arrays.script --output output/examples/random_and_arrays.json
./bin/luduscript.exe examples/testfile.script --output output/examples/testfile.json
```

**Linux/macOS (Bash):**

```bash
# 创建输出目录
mkdir -p output/examples

# 运行所有示例
./bin/luduscript examples/basic_syntax.script --output output/examples/basic_syntax.json
./bin/luduscript examples/test_simple.script --output output/examples/test_simple.json
./bin/luduscript examples/functions_and_loops.script --output output/examples/functions_and_loops.json
./bin/luduscript examples/random_and_arrays.script --output output/examples/random_and_arrays.json
./bin/luduscript examples/testfile.script --output output/examples/testfile.json
```

## 自定义示例

您可以基于这些示例创建自己的 LuduScript 脚本：

1. 复制一个现有示例作为模板
2. 修改变量、函数和逻辑
3. 使用 `luduscript` 命令运行您的脚本
4. 查看生成的 JSON 输出

## 常见问题

**Q: 示例运行失败怎么办？**
A: 确保您已经成功构建了项目，并且可执行文件位于 `bin/` 或 `build/bin/` 目录中。

**Q: 如何修改示例？**
A: 直接编辑 `.script` 文件，然后重新运行即可看到效果。

**Q: 输出文件在哪里？**
A: 默认输出到 `output/` 目录，您可以使用 `--output` 参数指定其他位置。

## 更多资源

- 查看 `docs/SYNTAX.md` 了解完整的语法参考
- 查看 `docs/API.md` 了解所有内置函数
- 查看 `scripts/` 目录了解实际应用案例

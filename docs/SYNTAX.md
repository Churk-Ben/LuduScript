# LuduScript 语法参考

本文档详细描述了 LuduScript 语言的语法规则和使用方法。

## 目录

- [基本语法](#基本语法)
- [数据类型](#数据类型)
- [变量和常量](#变量和常量)
- [运算符](#运算符)
- [控制流](#控制流)
- [函数](#函数)
- [内置函数](#内置函数)
- [注释](#注释)
- [示例](#示例)

## 基本语法

### 语句结束

语句不需要分号结束，代码块使用大括号 `{}` 包围，主要使用obj()函数创建对象：

```luduscript
// 创建对象而不是变量
obj("Numbers", 1) {
    int(x) { 10 }
    int(y) { 20 }
}
```

### 代码块

使用大括号 `{}` 定义代码块：

```luduscript
if (condition) {
    // 代码块
    statement1;
    statement2;
}
```

## 数据类型

### 数字 (Number)

使用int()和float()属性：

```luduscript
// 使用int()和float()属性
obj("Numbers", 1) {
    int(integer) { 42 }
    float(float) { 3.14 }
    int(negative) { -10 }
}
```

### 字符串 (String)

使用str()属性：

```luduscript
// 使用str()属性
obj("TextData", 1) {
    str(name) { "LuduScript" }
    str(message) { "Hello, World!" }
    str(empty) { "" }
}
```

字符串连接：

```luduscript
let greeting = "Hello, " + "World!";
let fullName = firstName + " " + lastName;
```

### 布尔值 (Boolean)

```luduscript
// 使用bool()属性
obj("Status", 1) {
    bool(isTrue) { true }
    bool(isFalse) { false }
}
```

### 数组 (Array)

注意：LuduScript不支持数组字面量，使用循环创建多个对象来模拟数组：

```luduscript
// 创建数字序列
for(i, 5) {
    obj("Number", i) {
        int(value) { i }
    }
}

// 创建名字列表
for(i, 3) {
    obj("Name", i) {
        if (i == 1) { str(name) { "Alice" } }
        if (i == 2) { str(name) { "Bob" } }
        if (i == 3) { str(name) { "Charlie" } }
    }
}
```

数组访问：

```luduscript
let first = numbers[0];  // 1
let last = numbers[4];   // 5
```

### 对象 (Object)

使用obj()函数定义对象：

```luduscript
// 创建人员对象
obj("Person", 1) {
    str(name) { "Alice" }
    int(age) { 30 }
    bool(isStudent) { false }
}

// 创建卡牌对象
obj("Card", 1) {
    str(suit) { "红桃" }
    str(rank) { "A" }
    int(value) { 1 }
}
```

对象属性访问：

注意：LuduScript中对象属性在创建时定义，不支持动态访问：

```luduscript
// 创建对象时定义所有属性
obj("Person", 1) {
    str(name) { "Alice" }
    int(age) { 30 }
    str(access_method) { "属性在创建时定义" }
}
```

### 空值 (Null)

注意：LuduScript不支持null值，可以使用空字符串或特殊标记：

```luduscript
obj("NullValue", 1) {
    str(value) { "" }  // 使用空字符串表示空值
    str(status) { "empty" }
}
```

## 变量和常量

### 对象创建

注意：LuduScript不使用变量声明，而是创建对象：

```luduscript
// 创建对象而不是声明变量
obj("Data", 1) {
    int(x) { 10 }
    str(name) { "Alice" }
    bool(isActive) { true }
}

// 创建新对象表示更新后的值
obj("UpdatedData", 1) {
    int(x) { 20 }
    str(name) { "Bob" }
}
```

## 运算符

### 算术运算符

注意：LuduScript中运算在对象属性中进行：

```luduscript
// 创建包含运算结果的对象
obj("Calculation", 1) {
    int(a) { 10 }
    int(b) { 3 }
    int(sum) { 13 }        // a + b (加法)
    int(diff) { 7 }        // a - b (减法)
    int(product) { 30 }    // a * b (乘法)
    float(quotient) { 3.333 } // a / b (除法)
    int(remainder) { 1 }   // a % b (取模)
}
```

### 比较运算符

注意：LuduScript中比较运算用于条件语句：

```luduscript
// 使用条件语句进行比较
obj("Comparison", 1) {
    int(x) { 10 }
    int(y) { 20 }
    
    if (x == y) { str(equal) { "false" } }      // 等于
    if (x != y) { str(notEqual) { "true" } }    // 不等于
    if (x < y) { str(less) { "true" } }         // 小于
    if (x <= y) { str(lessEqual) { "true" } }   // 小于等于
    if (x > y) { str(greater) { "false" } }     // 大于
    if (x >= y) { str(greaterEqual) { "false" } } // 大于等于
}
```

### 逻辑运算符

注意：LuduScript中逻辑运算用于条件语句：

```luduscript
// 使用条件语句进行逻辑运算
obj("LogicTest", 1) {
    bool(a) { true }
    bool(b) { false }
    
    if (a && b) { str(and_result) { "false" } }  // 逻辑与
    if (a || b) { str(or_result) { "true" } }    // 逻辑或
    if (!a) { str(not_result) { "false" } }      // 逻辑非
}
```

### 一元运算符

注意：LuduScript中一元运算在对象属性中表示：

```luduscript
// 创建包含一元运算结果的对象
obj("UnaryOperation", 1) {
    int(x) { 10 }
    int(negative) { -10 }  // 负号运算
    int(positive) { 10 }   // 正号运算
    bool(notTrue) { false } // 逻辑非运算
}
```

## 控制流

### if 语句

注意：LuduScript中if语句用于条件属性设置：

```luduscript
// 基本条件语句结构
if (condition) {
    // 条件为真时设置属性
}
```

示例：

```luduscript
// 使用条件语句设置属性
obj("ScoreEvaluation", 1) {
    int(score) { 85 }
    
    if (score >= 90) {
        str(grade) { "优秀" }
    }
    if (score >= 80 && score < 90) {
        str(grade) { "良好" }
    }
    if (score >= 60 && score < 80) {
        str(grade) { "及格" }
    }
    if (score < 60) {
        str(grade) { "不及格" }
    }
}
```

### for 循环

#### 数值范围循环

注意：LuduScript使用for(变量, 次数)语法：

```luduscript
// 创建数字序列
for(i, 5) {
    obj("Number", i) {
        int(value) { i }
        str(description) { "循环第" + i + "次" }
    }
}
```

#### 嵌套循环

```luduscript
// 创建二维数据结构
for(row, 3) {
    for(col, 4) {
        obj("Cell", row * 4 + col) {
            int(row_index) { row }
            int(col_index) { col }
            str(position) { "(" + row + "," + col + ")" }
        }
    }
}
```

### while 循环

注意：LuduScript不支持while循环，使用for循环替代：

```luduscript
// 使用for循环模拟while循环行为
for(i, 5) {
    obj("WhileSimulation", i) {
        int(counter) { i }
        str(status) { "循环执行中" }
        if (i == 5) {
            str(status) { "循环结束" }
        }
    }
}
```

## 函数

### 函数定义

注意：LuduScript不支持函数定义，使用对象和条件逻辑模拟函数行为：

```luduscript
// 模拟greet函数
obj("GreetFunction", 1) {
    str(input_name) { "Alice" }
    str(result) { "Hello, Alice!" }
    str(function_type) { "greeting" }
}

// 模拟add函数
obj("AddFunction", 1) {
    int(input_a) { 5 }
    int(input_b) { 3 }
    int(result) { 8 }
    str(function_type) { "addition" }
}

// 模拟createCard函数
obj("CreateCardFunction", 1) {
    str(input_suit) { "红桃" }
    str(input_rank) { "A" }
    str(suit) { "红桃" }
    str(rank) { "A" }
    str(name) { "A of 红桃" }
}
```

### 函数调用

注意：LuduScript不支持函数调用，通过创建对象来实现类似功能：

```luduscript
// 模拟函数调用结果
obj("FunctionResults", 1) {
    int(add_result) { 8 }           // add(5, 3)的结果
    str(greet_message) { "Hello, Alice!" }     // greet("Alice")的结果
    str(card_suit) { "红桃" }        // createCard结果的suit属性
    str(card_rank) { "A" }          // createCard结果的rank属性
    str(card_name) { "A of 红桃" }   // createCard结果的name属性
}
```

### 无返回值函数

注意：LuduScript中通过对象属性来模拟无返回值函数的行为：

```luduscript
// 模拟printInfo函数的行为
obj("PrintInfoFunction", 1) {
    str(input_name) { "Alice" }
    int(input_age) { 30 }
    str(output_line1) { "姓名: Alice" }
    str(output_line2) { "年龄: 30" }
    str(return_value) { "" }  // 表示无返回值
}
```

## 内置函数

### 输出函数

注意：LuduScript不支持print函数，使用obj()创建输出对象：

```luduscript
// 创建输出对象而不是使用print函数
obj("Output", 1) {
    str(message) { "Hello, World!" }
}

obj("Output", 2) {
    int(number) { 42 }
}

obj("Output", 3) {
    str(description) { "数组 [1, 2, 3]" }
}
```

### 数组操作

注意：LuduScript不支持数组操作函数，使用循环和对象模拟：

```luduscript
// 模拟数组操作
for(i, 3) {
    obj("ArrayItem", i) {
        int(value) { i }
        int(index) { i }
    }
}

// 模拟添加元素
obj("ArrayItem", 4) {
    int(value) { 4 }
    int(index) { 4 }
    str(operation) { "pushed" }
}

// 模拟长度计算
obj("ArrayInfo", 1) {
    int(length) { 3 }
    str(last_popped) { "4" }
    str(shuffle_status) { "已打乱" }
}
```

### 随机数生成

注意：LuduScript不支持random函数，使用对象属性模拟随机数：

```luduscript
// 模拟随机数生成结果
obj("RandomNumbers", 1) {
    float(random_float) { 0.7234 }    // 模拟0-1随机数
    int(random_int_10) { 7 }          // 模拟1-10随机整数
    int(random_int_100) { 42 }        // 模拟0-100随机整数
    str(generation_method) { "模拟随机" }
}
```

### 类型检查

注意：LuduScript不支持类型检查函数，使用对象属性表示类型信息：

```luduscript
// 模拟类型检查结果
obj("TypeInfo", 1) {
    int(x) { 42 }
    str(x_type) { "number" }
}

obj("TypeInfo", 2) {
    str(y) { "hello" }
    str(y_type) { "string" }
}

obj("TypeInfo", 3) {
    str(z_description) { "数组 [1, 2, 3]" }
    str(z_type) { "array" }
}
```

## 注释

### 单行注释

```luduscript
// 这是单行注释
obj("Data", 1) {
    int(x) { 10 }  // 行末注释
}
```

### 多行注释

```luduscript
/*
这是多行注释
可以跨越多行
*/
obj("Data", 1) {
    int(y) { 20 }
}
```

## 示例

### 生成扑克牌

```luduscript
// 生成标准52张扑克牌 - 使用正确的LuduScript语法
for(suit, 4) {
    for(rank, 13) {
        obj("Card", (suit - 1) * 13 + rank) {
            // 设置花色
            if (suit == 1) { 
                str(suit_name) { "红桃" }
                str(color) { "红色" }
            }
            if (suit == 2) { 
                str(suit_name) { "黑桃" }
                str(color) { "黑色" }
            }
            if (suit == 3) { 
                str(suit_name) { "方块" }
                str(color) { "红色" }
            }
            if (suit == 4) { 
                str(suit_name) { "梅花" }
                str(color) { "黑色" }
            }
            
            // 设置牌面
            if (rank == 1) { str(rank_name) { "A" } }
            if (rank == 2) { str(rank_name) { "2" } }
            if (rank == 3) { str(rank_name) { "3" } }
            if (rank == 4) { str(rank_name) { "4" } }
            if (rank == 5) { str(rank_name) { "5" } }
            if (rank == 6) { str(rank_name) { "6" } }
            if (rank == 7) { str(rank_name) { "7" } }
            if (rank == 8) { str(rank_name) { "8" } }
            if (rank == 9) { str(rank_name) { "9" } }
            if (rank == 10) { str(rank_name) { "10" } }
            if (rank == 11) { str(rank_name) { "J" } }
            if (rank == 12) { str(rank_name) { "Q" } }
            if (rank == 13) { str(rank_name) { "K" } }
            
            int(suit_index) { suit }
            int(rank_index) { rank }
        }
    }
}

// 添加大小王
obj("Card", 53) {
    str(suit_name) { "Joker" }
    str(rank_name) { "Small" }
    str(name) { "小王" }
    str(color) { "黑色" }
}

obj("Card", 54) {
    str(suit_name) { "Joker" }
    str(rank_name) { "Big" }
    str(name) { "大王" }
    str(color) { "红色" }
}

// 输出牌组信息
obj("DeckInfo", 1) {
    int(total_cards) { 54 }
    str(status) { "已生成完整牌组" }
    str(shuffle_status) { "已打乱" }
}
```

### 角色分配系统

```luduscript
// 生成狼人杀角色卡 - 使用正确的LuduScript语法

// 生成狼人角色
for(i, 3) {
    obj("Role", i) {
        str(name) { "狼人" }
        str(team) { "狼人" }
        str(description) { "夜晚可以杀死一名玩家" }
        int(role_id) { i }
    }
}

// 生成村民角色
for(i, 4) {
    obj("Role", i + 3) {
        str(name) { "村民" }
        str(team) { "好人" }
        str(description) { "普通村民，没有特殊能力" }
        int(role_id) { i + 3 }
    }
}

// 生成预言家
obj("Role", 8) {
    str(name) { "预言家" }
    str(team) { "好人" }
    str(description) { "夜晚可以查验一名玩家的身份" }
    int(role_id) { 8 }
}

// 生成女巫
obj("Role", 9) {
    str(name) { "女巫" }
    str(team) { "好人" }
    str(description) { "拥有解药和毒药各一瓶" }
    int(role_id) { 9 }
}

// 生成猎人
obj("Role", 10) {
    str(name) { "猎人" }
    str(team) { "好人" }
    str(description) { "被投票出局时可以开枪带走一名玩家" }
    int(role_id) { 10 }
}

// 游戏信息
obj("GameInfo", 1) {
    str(gameType) { "狼人杀" }
    int(playerCount) { 10 }
    int(werewolf_count) { 3 }
    int(villager_count) { 4 }
    int(special_roles) { 3 }
    str(status) { "角色分配完成" }
}
```

这些示例展示了 LuduScript 的主要语法特性和实际应用场景。
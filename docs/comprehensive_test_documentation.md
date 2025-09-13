# LuduScript 全面语法测试文档

## 概述

本文档详细说明了 `comprehensive_test.gen` 测试文件的内容、预期执行结果以及对解释器行为的详细说明。该测试文件全面覆盖了 LuduScript 的所有语法特性，包括正常情况、边界情况和错误处理。

## 测试文件结构

测试文件 `comprehensive_test.gen` 包含 32 个主要测试用例，按以下类别组织：

1. **基本数据类型测试** (测试 1-3)
2. **复杂变量声明测试** (测试 4)
3. **算术操作符测试** (测试 5-7)
4. **比较操作符测试** (测试 8-9)
5. **逻辑操作符测试** (测试 10-11)
6. **条件语句测试** (测试 12-15)
7. **循环语句测试** (测试 16-22)
8. **对象创建测试** (测试 23-27)
9. **边界情况测试** (测试 28-30)
10. **复杂综合测试** (测试 31-32)

## 详细测试用例说明

### 1. 基本数据类型测试

#### 测试 1: 数值类型
```lud
num(test_int) { 42 }                    // 整数
num(test_float) { 3.14159 }             // 浮点数
num(test_negative) { -100 }             // 负数
num(test_zero) { 0 }                    // 零值
num(test_large) { 999999999 }           // 大数值
num(test_default_num)                   // 默认值测试
```

**预期结果:**
- `test_int` = 42
- `test_float` = 3.14159
- `test_negative` = -100
- `test_zero` = 0
- `test_large` = 999999999
- `test_default_num` = 0 (默认值)

**解释器行为:**
- 支持整数和浮点数的存储
- 正确处理负数
- 未提供初始值时使用默认值 0

#### 测试 2: 字符串类型
```lud
str(test_string) { "Hello World" }      // 基本字符串
str(test_empty) { "" }                  // 空字符串
str(test_escape) { "Line1\nLine2\tTab\"Quote\\Backslash" }  // 转义字符
str(test_chinese) { "你好世界" }         // 中文字符串
str(test_default_str)                   // 默认值测试
```

**预期结果:**
- `test_string` = "Hello World"
- `test_empty` = ""
- `test_escape` = "Line1\nLine2\tTab\"Quote\\Backslash"
- `test_chinese` = "你好世界"
- `test_default_str` = "" (默认值)

**解释器行为:**
- 正确处理 UTF-8 编码的中文字符
- 支持转义字符：\n, \t, \", \\
- 未提供初始值时使用空字符串作为默认值

#### 测试 3: 布尔类型
```lud
bool(test_true) { true }                // 真值
bool(test_false) { false }              // 假值
bool(test_default_bool)                 // 默认值测试
```

**预期结果:**
- `test_true` = true
- `test_false` = false
- `test_default_bool` = false (默认值)

**解释器行为:**
- 支持布尔字面量 true 和 false
- 未提供初始值时使用 false 作为默认值

### 2. 复杂变量声明测试

#### 测试 4: 语句块变量声明
```lud
num(complex_calc) {
    num(a) { 10 }
    num(b) { 20 }
    num(c) { a + b * 2 }  // 最后一个变量作为值
}
```

**预期结果:**
- `complex_calc` = 50 (10 + 20 * 2)

**解释器行为:**
- 在变量声明的语句块中可以定义多个变量
- 最后一个定义的变量的值作为外层变量的值
- 支持在语句块内进行复杂计算
- 正确处理运算优先级 (乘法优先于加法)

### 3. 算术操作符测试

#### 测试 5: 基本算术运算
```lud
num(add_test) { 10 + 5 }                // 加法
num(sub_test) { 10 - 3 }                // 减法
num(mul_test) { 6 * 7 }                 // 乘法
num(div_test) { 20 / 4 }                // 除法
num(mod_test) { 17 % 5 }                // 取模
```

**预期结果:**
- `add_test` = 15
- `sub_test` = 7
- `mul_test` = 42
- `div_test` = 5
- `mod_test` = 2

**解释器行为:**
- 支持所有基本算术运算符
- 除法结果根据操作数类型确定 (整数除法或浮点除法)
- 取模运算正确处理余数

#### 测试 6: 运算优先级
```lud
num(precedence1) { 2 + 3 * 4 }          // 应为14
num(precedence2) { (2 + 3) * 4 }        // 应为20
num(precedence3) { 10 - 6 / 2 }         // 应为7
num(precedence4) { (10 - 6) / 2 }       // 应为2
```

**预期结果:**
- `precedence1` = 14 (乘法优先)
- `precedence2` = 20 (括号改变优先级)
- `precedence3` = 7 (除法优先)
- `precedence4` = 2 (括号改变优先级)

**解释器行为:**
- 遵循标准数学运算优先级
- 乘法和除法优先于加法和减法
- 括号可以改变运算优先级

#### 测试 7: 一元操作符
```lud
num(negative_test) { -42 }              // 负号
num(double_negative) { -(-10) }         // 双重负号
bool(not_test) { !true }                // 逻辑非
bool(not_false) { !false }              // 逻辑非假值
```

**预期结果:**
- `negative_test` = -42
- `double_negative` = 10
- `not_test` = false
- `not_false` = true

**解释器行为:**
- 支持一元负号操作符
- 支持逻辑非操作符
- 正确处理嵌套的一元操作符

### 4. 比较操作符测试

#### 测试 8: 数值比较
```lud
bool(equal_test) { 5 == 5 }             // 相等
bool(not_equal_test) { 5 != 3 }         // 不等
bool(less_test) { 3 < 5 }               // 小于
bool(greater_test) { 7 > 4 }            // 大于
bool(less_equal_test) { 5 <= 5 }        // 小于等于
bool(greater_equal_test) { 6 >= 6 }     // 大于等于
```

**预期结果:**
- 所有比较结果均为 true

**解释器行为:**
- 支持所有比较操作符
- 正确处理数值比较
- 比较结果返回布尔值

#### 测试 9: 字符串比较
```lud
bool(str_equal) { "abc" == "abc" }      // 字符串相等
bool(str_not_equal) { "abc" != "def" }  // 字符串不等
```

**预期结果:**
- `str_equal` = true
- `str_not_equal` = true

**解释器行为:**
- 支持字符串的相等和不等比较
- 字符串比较区分大小写

### 5. 逻辑操作符测试

#### 测试 10-11: 逻辑运算
```lud
bool(and_true) { true && true }         // 逻辑与 - 真
bool(and_false) { true && false }       // 逻辑与 - 假
bool(or_true) { false || true }         // 逻辑或 - 真
bool(or_false) { false || false }       // 逻辑或 - 假
bool(complex_logic1) { (5 > 3) && (2 < 4) }  // 复合条件
```

**预期结果:**
- `and_true` = true
- `and_false` = false
- `or_true` = true
- `or_false` = false
- `complex_logic1` = true

**解释器行为:**
- 支持逻辑与 (&&) 和逻辑或 (||) 操作符
- 支持短路求值 (如果可能)
- 可以组合比较操作符和逻辑操作符

### 6. 条件语句测试

#### 测试 12-15: 条件控制流
```lud
// if语句
num(if_result) { 0 }
if(5 > 3) {
    num(if_result) { 100 }
}

// if-else语句
num(if_else_result) { 0 }
if(2 > 5) {
    num(if_else_result) { 1 }
} else {
    num(if_else_result) { 2 }
}

// if-elif-else语句
num(grade) { 85 }
str(grade_letter) { "" }
if(grade >= 90) {
    str(grade_letter) { "A" }
} elif(grade >= 80) {
    str(grade_letter) { "B" }
} elif(grade >= 70) {
    str(grade_letter) { "C" }
} else {
    str(grade_letter) { "F" }
}
```

**预期结果:**
- `if_result` = 100 (条件为真，执行if块)
- `if_else_result` = 2 (条件为假，执行else块)
- `grade_letter` = "B" (85分对应B等级)

**解释器行为:**
- 支持 if、elif、else 条件语句
- 正确评估条件表达式
- 只执行匹配条件的代码块
- 支持嵌套条件语句

### 7. 循环语句测试

#### 测试 16-22: 各种循环模式
```lud
// 基本for循环 (1参数) - 从1到5
num(sum1) { 0 }
for(i, 5) {
    num(sum1) { sum1 + i }
}

// for循环 (2参数) - 从3到7
num(sum2) { 0 }
for(j, 3, 7) {
    num(sum2) { sum2 + j }
}

// for循环 (3参数) - 从2到10，步长2
num(sum3) { 0 }
for(k, 2, 10, 2) {
    num(sum3) { sum3 + k }
}
```

**预期结果:**
- `sum1` = 15 (1+2+3+4+5)
- `sum2` = 25 (3+4+5+6+7)
- `sum3` = 30 (2+4+6+8+10)

**解释器行为:**
- 支持三种for循环参数模式
- 1参数：从1到指定数值
- 2参数：从起始值到结束值
- 3参数：从起始值到结束值，指定步长
- 循环变量在每次迭代中更新

#### Break和Continue测试
```lud
// break语句测试
num(break_sum) { 0 }
for(n, 1, 100) {
    if(n > 5) {
        break
    }
    num(break_sum) { break_sum + n }
}

// continue语句测试
num(continue_sum) { 0 }
for(p, 1, 10) {
    if(p % 2 == 0) {
        continue
    }
    num(continue_sum) { continue_sum + p }
}
```

**预期结果:**
- `break_sum` = 15 (1+2+3+4+5，在n=6时break)
- `continue_sum` = 25 (1+3+5+7+9，跳过偶数)

**解释器行为:**
- break语句立即退出当前循环
- continue语句跳过当前迭代的剩余部分，进入下一次迭代
- 正确处理循环控制语句的作用域

### 8. 对象创建测试

#### 测试 23-27: 对象生成
```lud
// 基本对象创建
obj("Card", 1001) {
    str(name) { "火球术" }
    num(cost) { 3 }
    num(damage) { 4 }
    bool(is_spell) { true }
}
```

**预期JSON输出:**
```json
{
    "class": "Card",
    "id": 1001,
    "name": "火球术",
    "cost": 3,
    "damage": 4,
    "is_spell": true
}
```

**解释器行为:**
- 自动添加 class 和 id 字段
- 支持在对象中定义各种类型的属性
- 所有对象输出到JSON数组中
- 支持在循环中批量创建对象

### 9. 边界情况测试

#### 测试 28-30: 边界值处理
```lud
// 零值和边界值
num(zero_div_safe) { 10 / 1 }           // 避免除零
num(large_calc) { 999999 + 1 }          // 大数计算
num(small_calc) { 1 - 999999 }          // 负数结果

// 空循环
num(empty_loop_sum) { 0 }
for(empty, 5, 3) {  // 起始值大于结束值
    num(empty_loop_sum) { empty_loop_sum + 1 }  // 不应执行
}

// 单次循环
num(single_loop_result) { 0 }
for(single, 5, 5) {
    num(single_loop_result) { 42 }
}
```

**预期结果:**
- `zero_div_safe` = 10
- `large_calc` = 1000000
- `small_calc` = -999998
- `empty_loop_sum` = 0 (循环不执行)
- `single_loop_result` = 42 (执行一次)

**解释器行为:**
- 正确处理大数值计算
- 当循环起始值大于结束值时，循环不执行
- 当起始值等于结束值时，循环执行一次
- 避免除零等错误情况

### 10. 复杂综合测试

#### 测试 31-32: 实际应用场景

**游戏卡牌生成系统:**
- 生成4个稀有度等级的卡牌
- 每个稀有度生成3张卡牌
- 根据稀有度计算基础费用和属性
- 预期生成12个GameCard对象

**装备强化系统:**
- 生成3种装备类型
- 每种装备有5个强化等级
- 跳过+0装备，限制最高等级
- 预期生成15个Equipment对象

**解释器行为:**
- 支持复杂的嵌套循环和条件语句
- 正确处理变量作用域
- 支持在对象属性中进行复杂计算
- 能够处理实际游戏开发中的数据生成需求

## 测试覆盖范围

### 语法特性覆盖

✅ **基本数据类型**
- 数值类型 (整数、浮点数、负数)
- 字符串类型 (基本字符串、转义字符、中文)
- 布尔类型 (true/false)
- 默认值处理

✅ **变量声明**
- 简单变量声明
- 语句块变量声明
- 复杂表达式作为初始值

✅ **表达式和操作符**
- 算术操作符 (+, -, *, /, %)
- 比较操作符 (==, !=, <, >, <=, >=)
- 逻辑操作符 (&&, ||, !)
- 一元操作符 (-, !)
- 运算优先级和括号

✅ **控制流语句**
- if语句
- if-else语句
- if-elif-else语句
- 嵌套条件语句

✅ **循环语句**
- for循环 (1参数、2参数、3参数)
- 嵌套循环
- break语句
- continue语句
- 循环中的条件语句

✅ **对象创建**
- 基本对象创建
- 使用变量作为属性值
- 循环创建多个对象
- 条件创建对象
- 复杂属性计算

✅ **注释**
- 行注释 (//)

### 测试场景覆盖

✅ **正常情况**
- 所有语法特性的标准用法
- 典型的游戏开发场景
- 复杂的嵌套结构

✅ **边界情况**
- 零值和大数值
- 空字符串
- 单次循环
- 空循环 (起始值大于结束值)
- 默认值处理

✅ **复杂场景**
- 多层嵌套的控制结构
- 复杂的表达式计算
- 实际游戏数据生成场景
- 变量作用域测试

### 错误处理覆盖

⚠️ **需要额外测试的错误情况**
- 除零错误
- 类型不匹配
- 未定义变量引用
- 语法错误
- 无限递归

## 预期输出格式

解释器执行测试文件后，应生成包含以下对象的JSON数组：

1. **Card对象** (1个) - ID: 1001
2. **Item对象** (1个) - ID: 1002
3. **Weapon对象** (5个) - ID: 2001-2005
4. **Monster对象** (3个) - ID: 3001-3003
5. **Hero对象** (3个) - ID: 4001-4003
6. **GameCard对象** (12个) - ID: 5101-5403
7. **Equipment对象** (15个) - ID: 6101-6305
8. **TestComplete对象** (1个) - ID: 9999

**总计预期对象数量:** 41个对象

## 测试执行建议

1. **运行测试:** 使用LuduScript解释器执行 `comprehensive_test.gen`
2. **验证输出:** 检查生成的JSON数组是否包含预期的41个对象
3. **检查变量值:** 验证各个测试变量是否具有预期的值
4. **性能测试:** 记录执行时间，确保解释器性能合理
5. **错误处理:** 尝试修改测试文件引入错误，验证错误处理机制

## 结论

这个全面的测试文件覆盖了LuduScript的所有主要语法特性，包括：
- 所有数据类型和变量声明方式
- 完整的表达式和操作符集合
- 所有控制流语句
- 对象创建和属性定义
- 复杂的实际应用场景

通过执行这个测试文件，可以全面验证LuduScript解释器的正确性、完整性和健壮性。测试结果将为解释器的进一步开发和优化提供重要的参考依据。
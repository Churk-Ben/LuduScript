# LuduScript API 参考

本文档详细描述了 LuduScript 的内置函数和API。

## 目录

- [输出函数](#输出函数)
- [数组操作](#数组操作)
- [字符串操作](#字符串操作)
- [数学函数](#数学函数)
- [随机数生成](#随机数生成)
- [类型检查](#类型检查)
- [实用工具](#实用工具)

## 输出函数

### output(value)

将值添加到最终输出结果中。这是脚本的主要输出方式。

**参数:**

- `value` - 要输出的值，可以是任何类型

**返回值:** 无

**示例:**

```luduscript
// 创建并输出对象
obj("Message", 1) {
    str(message) { "Hello, World!" }
}

// 输出多个对象
for(i, 3) {
    obj("Number", i) {
        int(value) { i }
    }
}

obj("Status", 1) {
    str(message) { "完成" }
}
```

### print(value)

将值打印到控制台，主要用于调试。

**参数:**

- `value` - 要打印的值

**返回值:** 无

**示例:**

```luduscript
// 注意：print函数在LuduScript中不可用
// 使用obj()创建调试对象
obj("Debug", 1) {
    str(message) { "调试信息" }
}

obj("Debug", 2) {
    int(value) { 42 }
}
```

## 数组操作

### length(array)

获取数组的长度。

**参数:**

- `array` - 目标数组

**返回值:** 数组的长度（数字）

**示例:**

```luduscript
// 注意：LuduScript不支持数组字面量和length函数
// 使用循环创建多个对象来模拟数组
for(i, 5) {
    obj("ArrayItem", i) {
        int(value) { i }
        int(index) { i }
    }
}
```

### push(array, value)

向数组末尾添加一个元素。

**参数:**

- `array` - 目标数组
- `value` - 要添加的值

**返回值:** 新的数组长度

**示例:**

```luduscript
// 注意：LuduScript不支持push函数和数组操作
// 使用循环创建对象序列
for(i, 3) {
    obj("Item", i) {
        int(value) { i }
    }
}

// 添加新项目
obj("Item", 4) {
    int(value) { 4 }
}

obj("Item", 5) {
    str(value) { "hello" }
}
```

### pop(array)

移除并返回数组的最后一个元素。

**参数:**

- `array` - 目标数组

**返回值:** 被移除的元素，如果数组为空则返回 null

**示例:**

```luduscript
// 注意：LuduScript不支持pop函数和数组操作
// 使用条件逻辑创建对象
for(i, 4) {
    obj("Item", i) {
        int(value) { i }
        if (i <= 2) {
            str(status) { "保留" }
        }
        if (i > 2) {
            str(status) { "移除" }
        }
    }
}
```

### shuffle(array)

随机打乱数组中元素的顺序。

**参数:**

- `array` - 要打乱的数组

**返回值:** 无（直接修改原数组）

**示例:**

```luduscript
// 注意：LuduScript不支持shuffle函数和数组操作
// 创建卡牌对象，使用随机条件模拟打乱
for(i, 4) {
    obj("Card", i) {
        if (i == 1) { str(rank) { "A" } }
        if (i == 2) { str(rank) { "K" } }
        if (i == 3) { str(rank) { "Q" } }
        if (i == 4) { str(rank) { "J" } }
        int(position) { i }
    }
}
```

### slice(array, start, end)

返回数组的一个片段（子数组）。

**参数:**

- `array` - 源数组
- `start` - 起始索引（包含）
- `end` - 结束索引（不包含），可选

**返回值:** 新的数组片段

**示例:**

```luduscript
// 注意：LuduScript不支持slice函数和数组操作
// 使用条件逻辑创建子集
for(i, 5) {
    obj("Original", i) {
        int(value) { i }
    }
}

// 创建第一个子集 (索引1-2)
for(i, 2) {
    obj("Part1", i) {
        int(value) { i + 1 }
    }
}

// 创建第二个子集 (索引2-4)
for(i, 3) {
    obj("Part2", i) {
        int(value) { i + 2 }
    }
}
```

### indexOf(array, value)

查找值在数组中的索引位置。

**参数:**

- `array` - 要搜索的数组
- `value` - 要查找的值

**返回值:** 值的索引位置，如果未找到则返回 -1

**示例:**

```luduscript
// 注意：LuduScript不支持indexOf函数和数组操作
// 使用条件逻辑查找元素
for(i, 3) {
    obj("Fruit", i) {
        if (i == 1) { str(name) { "apple" } }
        if (i == 2) { str(name) { "banana" } }
        if (i == 3) { str(name) { "cherry" } }
        int(index) { i }
    }
}

// 查找结果对象
obj("SearchResult", 1) {
    str(target) { "banana" }
    int(found_index) { 2 }
}

obj("SearchResult", 2) {
    str(target) { "orange" }
    int(found_index) { -1 }
}
```

## 字符串操作

### substring(string, start, length)

提取字符串的子串。

**参数:**

- `string` - 源字符串
- `start` - 起始位置
- `length` - 子串长度（可选）

**返回值:** 子字符串

**示例:**

```luduscript
// 注意：LuduScript不支持substring函数和字符串操作
// 使用对象属性模拟字符串操作
obj("Text", 1) {
    str(original) { "Hello, World!" }
    str(part1) { "Hello" }
    str(part2) { "World!" }
    int(start_index) { 0 }
    int(end_index) { 5 }
}
```

### toUpperCase(string)

将字符串转换为大写。

**参数:**

- `string` - 要转换的字符串

**返回值:** 大写字符串

**示例:**

```luduscript
// 注意：LuduScript不支持toUpperCase函数
// 使用对象属性模拟大小写转换
obj("TextCase", 1) {
    str(original) { "hello world" }
    str(uppercase) { "HELLO WORLD" }
}
```

### toLowerCase(string)

将字符串转换为小写。

**参数:**

- `string` - 要转换的字符串

**返回值:** 小写字符串

**示例:**

```luduscript
// 注意：LuduScript不支持toLowerCase函数
// 使用对象属性模拟大小写转换
obj("TextCase", 1) {
    str(original) { "HELLO WORLD" }
    str(lowercase) { "hello world" }
}
```

### split(string, delimiter)

使用分隔符分割字符串。

**参数:**

- `string` - 要分割的字符串
- `delimiter` - 分隔符

**返回值:** 分割后的字符串数组

**示例:**

```luduscript
// 注意：LuduScript不支持split函数和数组操作
// 使用循环创建分割后的对象
obj("SplitResult", 1) {
    str(original) { "apple,banana,cherry" }
    str(separator) { "," }
}

for(i, 3) {
    obj("Part", i) {
        if (i == 1) { str(value) { "apple" } }
        if (i == 2) { str(value) { "banana" } }
        if (i == 3) { str(value) { "cherry" } }
    }
}
```

## 数学函数

### abs(number)

返回数字的绝对值。

**参数:**

- `number` - 输入数字

**返回值:** 绝对值

**示例:**

```luduscript
// 注意：LuduScript不支持abs函数
// 使用条件逻辑模拟绝对值
obj("MathResult", 1) {
    int(original) { -5 }
    int(absolute) { 5 }
}
```

### floor(number)

向下取整。

**参数:**

- `number` - 输入数字

**返回值:** 向下取整后的整数

**示例:**

```luduscript
// 注意：LuduScript不支持floor函数
// 使用对象属性模拟向下取整
obj("MathResult", 1) {
    float(original) { 3.7 }
    int(floored) { 3 }
}
```

### ceil(number)

向上取整。

**参数:**

- `number` - 输入数字

**返回值:** 向上取整后的整数

**示例:**

```luduscript
// 注意：LuduScript不支持ceil函数
// 使用对象属性模拟向上取整
obj("MathResult", 1) {
    float(original) { 3.2 }
    int(ceiled) { 4 }
}
```

### round(number)

四舍五入。

**参数:**

- `number` - 输入数字

**返回值:** 四舍五入后的整数

**示例:**

```luduscript
// 注意：LuduScript不支持round函数
// 使用对象属性模拟四舍五入
obj("MathResult", 1) {
    float(original) { 3.6 }
    int(rounded) { 4 }
}
```

### min(a, b, ...)

返回参数中的最小值。

**参数:**

- `a, b, ...` - 要比较的数字

**返回值:** 最小值

**示例:**

```luduscript
// 注意：LuduScript不支持min函数
// 使用对象属性模拟最小值计算
obj("MathResult", 1) {
    int(value1) { 5 }
    int(value2) { 3 }
    int(value3) { 8 }
    int(value4) { 1 }
    int(minimum) { 1 }
}
```

### max(a, b, ...)

返回参数中的最大值。

**参数:**

- `a, b, ...` - 要比较的数字

**返回值:** 最大值

**示例:**

```luduscript
// 注意：LuduScript不支持max函数
// 使用对象属性模拟最大值计算
obj("MathResult", 1) {
    int(value1) { 5 }
    int(value2) { 3 }
    int(value3) { 8 }
    int(value4) { 1 }
    int(maximum) { 8 }
}
```

## 随机数生成

### random()

生成 0 到 1 之间的随机浮点数。

**参数:** 无

**返回值:** 0 到 1 之间的随机数

**示例:**

```luduscript
// 注意：LuduScript不支持random函数
// 使用对象属性模拟随机数生成
obj("RandomResult", 1) {
    float(random_float) { 0.7234 }  // 模拟0-1随机数
}
```

### random(max)

生成 0 到 max 之间的随机整数。

**参数:**

- `max` - 最大值（不包含）

**返回值:** 0 到 max-1 之间的随机整数

**示例:**

```luduscript
// 注意：LuduScript不支持random函数
// 使用对象属性模拟随机整数生成
obj("RandomResult", 1) {
    int(dice) { 3 }  // 模拟0-5随机整数
}
```

### random(min, max)

生成 min 到 max 之间的随机整数。

**参数:**

- `min` - 最小值（包含）
- `max` - 最大值（包含）

**返回值:** min 到 max 之间的随机整数

**示例:**

```luduscript
// 注意：LuduScript不支持random函数
// 使用对象属性模拟范围随机数生成
obj("RandomResult", 1) {
    int(dice) { 4 }     // 模拟1-6随机整数
    int(score) { 73 }   // 模拟0-100随机整数
}
```

### randomChoice(array)

从数组中随机选择一个元素。

**参数:**

- `array` - 要选择的数组

**返回值:** 随机选中的元素

**示例:**

```luduscript
// 注意：LuduScript不支持randomChoice函数和数组操作
// 使用条件逻辑模拟随机选择
for(i, 4) {
    obj("Color", i) {
        if (i == 1) { str(name) { "红" } }
        if (i == 2) { str(name) { "绿" } }
        if (i == 3) { str(name) { "蓝" } }
        if (i == 4) { str(name) { "黄" } }
    }
}

obj("RandomChoice", 1) {
    str(chosen) { "蓝" }  // 模拟随机选择结果
}
```

## 类型检查

### typeof(value)

返回值的类型。

**参数:**

- `value` - 要检查的值

**返回值:** 类型字符串（"number", "string", "boolean", "array", "object", "null"）

**示例:**

```luduscript
// 注意：LuduScript不支持typeof函数
// 使用对象属性模拟类型检查
obj("TypeCheck", 1) {
    int(value) { 42 }
    str(type) { "number" }
}

obj("TypeCheck", 2) {
    str(value) { "hello" }
    str(type) { "string" }
}

obj("TypeCheck", 3) {
    str(description) { "array with 3 elements" }
    str(type) { "array" }
}
```

### isNumber(value)

检查值是否为数字。

**参数:**

- `value` - 要检查的值

**返回值:** 布尔值

**示例:**

```luduscript
// 注意：LuduScript不支持isNumber函数
// 使用对象属性模拟数字类型检查
obj("TypeCheck", 1) {
    int(test_value) { 42 }
    bool(is_number) { true }
}

obj("TypeCheck", 2) {
    str(test_value) { "42" }
    bool(is_number) { false }
}
```

### isString(value)

检查值是否为字符串。

**参数:**

- `value` - 要检查的值

**返回值:** 布尔值

**示例:**

```luduscript
// 注意：LuduScript不支持isString函数
// 使用对象属性模拟字符串类型检查
obj("TypeCheck", 1) {
    str(test_value) { "hello" }
    bool(is_string) { true }
}

obj("TypeCheck", 2) {
    int(test_value) { 42 }
    bool(is_string) { false }
}
```

### isArray(value)

检查值是否为数组。

**参数:**

- `value` - 要检查的值

**返回值:** 布尔值

**示例:**

```luduscript
// 注意：LuduScript不支持isArray函数和数组字面量
// 使用对象属性模拟数组类型检查
obj("TypeCheck", 1) {
    str(test_description) { "array [1, 2, 3]" }
    bool(is_array) { true }
}

obj("TypeCheck", 2) {
    str(test_value) { "hello" }
    bool(is_array) { false }
}
```

## 实用工具

### range(start, end, step)

生成数字范围数组。

**参数:**

- `start` - 起始值
- `end` - 结束值（不包含）
- `step` - 步长（可选，默认为1）

**返回值:** 数字数组

**示例:**

```luduscript
// 注意：LuduScript不支持range函数和数组操作
// 使用for循环创建数字序列
for(i, 5) {
    obj("Number", i) {
        int(value) { i - 1 }  // 0, 1, 2, 3, 4
    }
}

// 创建范围1-9步长2的数字序列
for(i, 5) {
    obj("RangeNumber", i) {
        int(value) { i * 2 - 1 }  // 1, 3, 5, 7, 9
    }
}
```

### repeat(value, count)

重复值指定次数，返回数组。

**参数:**

- `value` - 要重复的值
- `count` - 重复次数

**返回值:** 包含重复值的数组

**示例:**

```luduscript
// 注意：LuduScript不支持repeat函数
// 使用循环创建重复内容
for(i, 5) {
    obj("Zero", i) {
        int(value) { 0 }
        int(occurrence) { i }
    }
}

for(i, 3) {
    obj("Hello", i) {
        str(text) { "hello" }
        int(occurrence) { i }
    }
}
```

### keys(object)

获取对象的所有键。

**参数:**

- `object` - 目标对象

**返回值:** 包含所有键的数组

**示例:**

```luduscript
// 注意：LuduScript不支持keys函数和对象字面量
// 使用对象属性模拟键值对
obj("Person", 1) {
    str(name) { "Alice" }
    int(age) { 30 }
    str(city) { "Beijing" }
}

// 创建键列表对象
for(i, 3) {
    obj("Key", i) {
        if (i == 1) { str(name) { "name" } }
        if (i == 2) { str(name) { "age" } }
        if (i == 3) { str(name) { "city" } }
    }
}
```

### values(object)

获取对象的所有值。

**参数:**

- `object` - 目标对象

**返回值:** 包含所有值的数组

**示例:**

```luduscript
// 注意：LuduScript不支持values函数和对象字面量
// 使用对象属性模拟键值对
obj("Person", 1) {
    str(name) { "Alice" }
    int(age) { 30 }
    str(city) { "Beijing" }
}

// 创建值列表对象
obj("Value", 1) {
    str(name_value) { "Alice" }
}

obj("Value", 2) {
    int(age_value) { 30 }
}

obj("Value", 3) {
    str(city_value) { "Beijing" }
}
```

## 使用示例

### 综合示例：卡牌游戏工具

```luduscript
// 注意：LuduScript不支持函数定义、数组字面量等JavaScript语法
// 以下是使用正确LuduScript语法的卡牌游戏示例

// 创建标准扑克牌deck
for(suit, 4) {
    for(rank, 13) {
        obj("Card", (suit - 1) * 13 + rank) {
            // 设置花色
            if (suit == 1) { str(suit_name) { "♠" } }
            if (suit == 2) { str(suit_name) { "♥" } }
            if (suit == 3) { str(suit_name) { "♦" } }
            if (suit == 4) { str(suit_name) { "♣" } }
            
            // 设置牌面和值
            if (rank == 1) { 
                str(rank_name) { "A" }
                int(value) { 1 }
            }
            if (rank >= 2 && rank <= 10) {
                int(rank_value) { rank }
                int(value) { rank }
            }
            if (rank == 11) {
                str(rank_name) { "J" }
                int(value) { 11 }
            }
            if (rank == 12) {
                str(rank_name) { "Q" }
                int(value) { 12 }
            }
            if (rank == 13) {
                str(rank_name) { "K" }
                int(value) { 13 }
            }
            
            int(suit_index) { suit }
            int(rank_index) { rank }
            str(card_id) { "card_" + suit + "_" + rank }
        }
    }
}

// 创建4个玩家，每人发5张牌
for(player, 4) {
    for(card, 5) {
        obj("PlayerHand", player * 5 + card) {
            int(player_id) { player }
            int(card_position) { card }
            
            // 模拟发牌结果（实际游戏中会是随机的）
            if (player == 1 && card == 1) {
                str(suit_name) { "♠" }
                str(rank_name) { "A" }
                int(value) { 1 }
            }
            if (player == 1 && card == 2) {
                str(suit_name) { "♥" }
                str(rank_name) { "K" }
                int(value) { 13 }
            }
            // ... 更多卡牌分配逻辑
            
            str(status) { "已发牌" }
        }
    }
}

// 游戏状态输出
obj("GameState", 1) {
    str(game_type) { "扑克牌游戏" }
    int(total_cards) { 52 }
    int(player_count) { 4 }
    int(cards_per_player) { 5 }
    int(remaining_cards) { 32 }  // 52 - 4*5 = 32
    str(status) { "发牌完成" }
}

// 创建游戏统计
for(player, 4) {
    obj("PlayerStats", player) {
        int(player_id) { player }
        int(hand_size) { 5 }
        str(status) { "准备就绪" }
    }
}
```

这个API参考涵盖了 LuduScript 的所有内置函数和常用操作模式。

# ADK User Guide-Advanced

> 建议在阅读本进阶版指南前，首先阅读Lite版指南。

[TOC]

## Raw IO

本ADK提供RAW IO模式用于AI的数据交互，致力于提供和原始stdio统一的使用体验，减少各位选手在编程语言上的学习成本。与Judger的通信由ADK实现，AI可以利用ADK发送或接受本模式下的数据。

### 输入数据

1. **（游戏配置）** 第一行为四个整数，分别代表
   
    1. 场地长度
    2. 场地宽度
    3. 总回合数
   4. 我方玩家编号（0为先手，1为后手）
   
   如`32 32 1024 0`代表：游戏场地大小为32x32，共进行1024回合，您为先手。
2. **（道具列表）** 第二行为一个整数`N`，代表一共`N`个道具
3. 接下来的`N`行，每一行有5个整数，代表道具的参数`x` `y` `time` `type` `param`。第`N`行表示的道具的`id`为行数（从0开始）。

   如：
    ```
    5
    31 24 1 3 1024
    11 19 1 3 1024
    26 17 1 2 1024
    29 17 2 2 1024
    27 16 2 1 2
    ```
4. **（玩家操作）** 接下来双方玩家将会轮流进行操作，操作列表的格式双方相同，为：
    - 若干行*操作*，以一行单独的`0`终结，表示**这一回合**的所有操作结束。
    - 每行的*操作*包含两个整数，第一个整数为操作的`type`，
        1. 若`type == 1`，即代表移动操作，则第二个整数代表移动的方向`direction`
        2. 若`type == 2`，即代表使用道具，则第二个整数代表要使用的道具的`id`
    - 操作的顺序应和自己的蛇的结算顺序相同，这个顺序参见游戏规则，并需要玩家自己维护。

   如：
    ```
    1 1
    1 2
    2 10
    0
    ```
   代表，第一条蛇向上移动，第二条蛇向左移动，第三条蛇使用`id`为10的道具。
5. **（游戏结束）** 若游戏结束，那么下一行数据将以`-1`开始，以与玩家操作的`0` `1` `2`区分。这一行数据包含五个整数，分别为
    - `-1`
    - 获胜方（0为我方，1为对方）
    - 获胜原因
        - `0`：正常结束
        - `1`：对方失去连接或对方使用了不合法的操作导致失败
        - `2`：其他未知问题导致游戏异常结束
    - 我方得分
    - 对方得分 如`-1 0 0 100 99`代表：游戏正常结束，我方以100:99的比分击败对手。

### 输出数据

1. **（玩家操作）** 输出数据只需要我方玩家的每回合的操作列表。操作列表已经在上面定义过了。

### 示例

虽然实际上游戏的运行需要两个AI、一个游戏逻辑、一个裁判程序互相通信来进行，但是Raw IO模式会让AI开发者像做程序设计题目一样，通过读写输入输出本地的文件来进行游戏，只不过是通过ADK提供的输入输出函数进行读写。

下面是一个完整的例子，分别为输入和输出文件。需要注意的是，并不是真的存在`input.txt`或`output.txt`这类的文件，这只是模拟的例子。

还应注意，`//`开头的行只是本文档中的注释，并不会、也不应当出现在实际读写的数据中。

`input.txt`

```
// Game Config
32 32 1024 0
// Generated Items
5
31 24 1 3 1024
11 19 1 3 1024
26 17 1 2 1024
29 17 2 2 1024
27 16 2 1 2
// Player 1 Round 0
1 2
0
// Player 1 Round 1
1 2
0
// Player 1 Round 2
1 1
0
// Player 1 Round 3
1 1
0
// ... And Many Rounds
// ...
// ...
// Game Result
-1 0 0 100 99
```

`output.txt`

```
// Player 0 Round 0
1 0
0
// Player 0 Round 1
1 0
0
// Player 0 Round 2
1 3
0
// Player 0 Round 3
1 3
0
// ... And Many Rounds
// ...
// ...
```

虽然在示例中，输入数据`input.txt`看起来已经包含了对方的全部操作数据。但实际上，在己方没有输出所有应当的操作之前，对方是不会行动的，这时试图读取数据也只是会阻塞程序。你可以将其看作一个**强制在线**的题目。

下面展示了对于上述数据的正确读写顺序，相应的数据段参见上面的注释：

1. 读取`Game Config`
2. 读取`Generated Items`
3. 因为我方为先手方，因此现在需要输出`Player 0 Round 0`
4. 然后才能读取对手的操作`Player 1 Round 0`
5. 双方轮流进行操作，即先输出`Player 0 Round x`，再读取`Player 1 Round x`
6. 若读取到`-1`开头的数据行，即为`Game Result`，游戏结束

### API

#### 引入ADK

为了简化使用流程（尤其对于C++），两个语言的ADK均为单文件，将其与你自己的程序文件放在同一个文件夹，然后在代码中对其引用即可。

`Python`

```python
import adk
```

`C++`

```c++
#include <adk.hpp>
```

#### 初始化客户端

可以将其看作打开输入、输出文件的过程。客户端`Client`类的对象可以看作文件指针。构造时无需任何参数。

`Python`

```python
client = adk.Client()
```

`C++`

```c++
// Static
adk::Client client();

// Dynamic
// If you are using pointers, remember to use -> instead of . in the following docs.
auto client = new adk::Client();
```

当然，如果不想写模块名或命名空间，也可以通过下面的方式进行调用：

`Python`

```python
from adk import Client

# ...

client = Client()
```

`C++`

```c++
#include <adk.hpp>
using namespace adk;

// ...

Client client();
```

#### 读取数据

`read_line`可以直接读取一行：

`Python`

```python
line = client.read_line()
```

`C++`

```c++
std::string line = client.read_line()
```

---

`next_int`可以读取下一个整数：

```python
a_int = client.next_int()
```

`C++`

```c++
int a_int = client.next_int()
```

---

对于C++选手来说，可以使用类似`iostream`的方法读取数据：

`C++`

```c++
string line;
client >> line;

int a_int;
client >> a_int;
```

#### 写入数据

`write_line`可以写入一行字符串，会自动在后面写入换行符。若没有参数，则会直接写入换行符：

`Python & C++`

```python
client.write_line("1 2")
```

---

`write_int`可以写入一个整数，会在后面自动添加空格：

`Python & C++`

```python
client.write_int(1)
```

---

对于C++选手来说，可以使用类似`iostream`的方法写入数据：

**注意：**

- 输出字符串的行为与`write_line`一致，即不需要`client << std::endl`等代码输入换行符
- 输出整数的行为与`write_int`一致，即不需要写入额外的空格了

`C++`

```c++
string line;
client << line;

int a_int;
client << a_int;
```

## 游戏状态维护

本ADK除去通信交互部分，也提供了游戏局面维护逻辑代码，可减少选手的代码负担，直接调用现成代码即可。

### 基础类型

#### Item 道具类

- `x: int` x坐标
- `y: int` y坐标
- `id: int` 道具编号
- `time: int` 道具生成的时间
- `type: int` 道具类型，`1`代表分裂道具，`2`代表融化射线    PS: 长度道具不会被保存，直接存入长度银行
- `param: int` 道具参数，代表道具的有效时间
- `gotten_time: int` 道具被蛇获取的时间（若道具仍在地图中未被蛇获取，为-1）
- `item_num: int` 静态成员，用于分配道具编号
- `__init__(self, x, y, time, type, param)` 初始化道具，id会自动分配

#### GameConfig 游戏配置类

- `length: int` 地图的长度
- `width: int` 地图的宽度
- `max_round: int` 最大回合上限
- `random_seed: int` 随机数种子
- `__init__(self, length, width, max_round)` 初始化游戏配置

#### Snake 蛇类

- `coor_list: List[Tuple[int, int]]` 一个顺序（蛇头 -> 蛇尾）包含所有坐标的列表，列表中元素为包含两个`int`的元组，第一个为`x`坐标，第二个为`y`坐标
- `item_list: List[Item]` 当前蛇所有的道具列表
- `id: int` 蛇的编号
- `length_bank: int` 长度银行中保存的长度
- `camp: int` 蛇归属于玩家0/1
- `snake_num: int` 静态成员，用于分配蛇的编号
- `__init__(self, coor_list, item_list, camp, id=-1)` 初始化道具，若无指定id会自动分配
- `get_len(self) -> int ` 返回当前蛇的长度
- `get_coor(self) -> List[Tuple[int, int]]`：返回当前蛇从头到尾坐标构成的列表
- `add_item(self, item:Item)` 为当前蛇增添道具`item`，若为长度道具则直接加入长度银行
- `get_item(self, id: int)` 获取当前蛇的编号为`id`的道具，若无返回`None`
- `delete_item(self, id: int)` 删除当前蛇编号为`id`的道具

#### Map 地图类

- `wall_map: List[List[int]]` -1 / 0 / 1 代表无墙 / 0号选手的墙 / 1号选手的墙

- `snake_map: List[List[int]]` -1 / id 代表无 / id号蛇占据

- `item_map: List[List[int]]` -1 / id 代表无 / id号道具占据

- `item_list: List[Item]`现存所有道具的列表

- `length: int` 地图长度

- `width: int` 地图宽度

- `__init__(self, item_list: List[Item], config: GameConfig)`  

  ​	根据游戏配置与生成的道具列表初始化地图，并设置蛇起始位置`(0, width - 1)`及`(length - 1, 0)`

- `get_map_item(self, id: int) -> Item` 返回当前编号为id的道具

- `set_wall(self, coor_list: List[Tuple[int, int]]), camp: int, type: int)`  

  ​	`type == 1`时将`coor_list`中所有坐标设为玩家`camp`的墙，`type == -1`时将`coor_list`中的墙摧毁

- `add_map_item(self, item: Item)` 将道具`item`加到地图中
- `delete_map_item(self, id: int)` 删除地图中的编号为`id`的道具
- `add_map_snake(self, coor_list:List[Tuple[int, int]], id: int)`  将编号为`id`的蛇加入地图中，坐标区域为`coor_list`
- `delete_map_item(self, coor_list:List[Tuple[int, int]])` 删除地图中原来坐标区域为`coor_list`的蛇

#### Context 上下文类

- `snake_list: List[Snake]` 包含现存所有蛇，同一个玩家的蛇按照操作顺序排序。
- `game_map: Map` 现在的游戏地图，为自定义类`Map`，具体内容见后。
- `turn: int` 当前回合数
- `current_player: int` 当前玩家编号
- `auto_growth_round: int` 自动增长回合数，默认为8
- `max_round: int` 最大回合数
- `__init__(self, config: GameConfig)`  根据游戏配置初始化上下文，并将初始两条蛇加入`snake_list`
- `get_map(self) -> Map` 返回当前地图
- `get_snake_count(self, camp: int) -> int` 返回当前`camp`选手蛇的数量
- `get_snake(self, id: int) -> Snake` 返回当前编号为id的蛇
- `add_snake(self, snake:Snake, index: int)` 在`snake_list`下标为`index`处插入新蛇，并加入地图中
- `delete_snake(self, id: int) -> int` 删除编号为`id`的蛇

#### Graph 固化计算

该类实际实现了floodfill用于计算固化围成的区域，使用方法如下：

`__init__(self, bound, h, w)` 初始化固化边界、地图长宽

`calc() -> List` 计算固化区域，返回固化区域（不包括蛇本身）

#### Operation 操作类

- `type: int` 操作类型 1移动，2分裂，3融化
- `snake: int` 蛇编号
- `direction: int` 方向 0 : x轴正向，1 : y轴正向, 2 : x轴负向, 3 : y轴负向，若无则为-1
- `item_id: int` 道具编号，若无则为-1

#### RoundInfo 回合信息类

- `ItemSnake = {"item": int, "snake": int}`
- `ExtraLength = {"snake": int, "el": int}`
- `gotten_item: List[ItemSnake]` 生成在蛇上的道具（除长度道具）
- `expired_item: List[ItemSnake]` 过期道具
- `invalid_item: List[ItemSnake]` 无效道具（废弃）
- `extra_length: List[ExtraLength]` 额外长度

### 核心类Controller以及逻辑处理流程

`Controller`类为逻辑处理的核心，以下是其包含的成员属性与方法：

- `ctx: Context` 游戏上下文信息

- `map: Map` 游戏地图信息

- `player: int`当前玩家

- `next_snake: int` 待操作的蛇的编号，没有则为-1

- `current_snake_list: List[Tuple(Snake, boolean)]` 处理下一条蛇时的辅助数组，复制`ctx.snake_list`中的蛇，但删除死亡的蛇时通过元组中的布尔值标记是否死亡，不直接在列表中删除

- `__init__(self, ctx: Context)` 通过已初始化的上下文信息初始化Controller

- `round_preprocess(self) -> RoundInfo` 处理回合开始信息，并返回`RoundInfo`类型的数据

- `find_first_snake(self) -> int` 查询初始的蛇（用于处理自动增长）

- `find_next_snake(self) -> int` 返回下一条需操作的蛇在`current_snake_list`数组中的编号 

- `next_player(self) -> int` 切换到下一个玩家

- `delete_snake(self, s_id: int)` 删除编号为`s_id`的蛇，会调用`ctx`中的`delete_snake`方法

- `round_init(self)` 玩家行动前的预处理，初始化`current_snake_list`并寻找第一条待操作的蛇

- `apply(self, op: List[int]) -> Processed_Operation ` `op`格式为与ADK交互的格式，一个包含2个整数的列表，转换为`Operation`类调用`apply_single`处理操作，寻找下一条蛇并返回操作附加信息

- `apply_single(self, snake: int, op: Operation) -> Processed_Operation` 处理在`current_snake_list`中下标为`snake`的蛇的操作并返回附加信息

  根据不同操作，分别调用`move`/ `split`/ `fire`函数处理移动、分裂、融化

- `get_item(self, snake: Snake, item_id: int)` 为蛇`snake`增加编号为`item_id`的道具

#### 实例化Controller

游戏开始时，AI读入游戏配置、道具列表后可实例化`Controller`类型，调用构造函数，参数为实例化的`Context`对象。如下就是一段初始化代码：

```python
config = GameConfig(width=height, length=width, max_round=round_count)
ctx = Context(config=config)
ctx.game_map = Map(item_list, config=config)
controller = Controller(ctx)
```

#### 回合信息处理

当每个回合开始时，即轮到先手玩家进行操作前，需要调用`controller.round_preprocess`方法进行回合的预处理，处理生成在蛇上的道具、道具的过期等。

#### 玩家操作预处理

每个玩家开始操作前，还需调用`controller.round_init`方法预处理，查找第一条蛇。

#### 应用操作

玩家每进行一次操作，需要调用`controller.apply`方法处理当前操作并寻找下一条蛇。

#### 玩家切换

一个玩家结束后，需要调用`controller.next_player`方法切换换到下一个玩家。

#### 框架示例

大致的逻辑处理流程框架如下：

```python
while playing:
    if current_player == 0:
        controller.round_preprocess()
    controller.round_init()
    if player == current_player:  # 玩家的回合
        while controller.next_snake != -1: # 还有蛇需要操作
            op = judge(...) # AI计算当前操作
            c.write_int(op[0]) # 输出当前操作
            c.write_int(op[1])
            controller.apply(op) # 应用当前操作并寻找下一条蛇
        controller.next_player() # 切换玩家
        c.write_int(0)
    else:
        while True:
            line = c.read_line() # 读取对方操作
            if line.startswith('0'): # 操作是否结束
                controller.next_player() # 切换玩家
                break 
            if line.startswith('-1'): # 游戏是否结束
                playing = False
                ...
                break
            op = list(map(int, line.split(' '))) 
            controller.apply(op) # 引用对手操作
    current_player = 1 - current_player # 切换玩家
```

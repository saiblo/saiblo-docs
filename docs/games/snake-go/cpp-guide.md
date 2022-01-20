# C++ ADK User Guide

## 0.预备工作

请先前往Saiblo官网下载ADK包，确认内含`adk.hpp`、`main.cpp`、`CMakeList.txt/Makefile`。

`adk.hpp`为你的AI提供的功能有：与Judger进行通讯，发送你的操作、读取对手的操作等；游戏状态维护，提供逻辑维护代码，能为你处理玩家的操作并提供当前局面信息。

`main.cpp`为一个已经实现的简易AI的代码，其中的框架可以沿用至你的AI程序中，或者你也可以直接在其上进行修改。

`CMakeList.txt`与`Makefile`负责将你的AI与ADK进行编译和链接，请选择其中之一与`adk.hpp`和`main.cpp`共同压缩为zip压缩包，并在saiblo平台选择对应语言创建AI并提交。注意，如果你修改了`main.cpp`的文件名或新创建文件编写你的AI，请在`CMakeList.txt`或`Makefile`中同步修改。

## 1.开始！

首先来一波`include`：

```c++
#include "adk.hpp"
```

再写一个`main`函数：

```c++
int main(int argc, char** argv) {
  SnakeGoAI run(argc, argv);
}
```

注意如果你要在`main`里面做其他的初始化工作，请一定要在`SnakeGoAI run(argc, argv);`这一行之前写，要不然你的AI就真的开始`run`起来了。

最后实现两个需要的函数：

```c++
Operation make_your_decision( const Snake &snake_to_operate, const Context &ctx ) {}

void game_over( int gameover_type, int winner, int p0_score, int p1_score ) {}
```

顾名思义，第一个函数代表决定`snake_to_operate`的操作`Operation`，可用的操作有：

| 操作类型         | 返回值     |
| ---------------- | ---------- |
| 向右移动         | OP_RIGHT   |
| 向上移动         | OP_UP      |
| 向左移动         | OP_LEFT    |
| 向下移动         | OP_DOWN    |
| 分裂             | OP_SPLIT   |
| 使用融化射线道具 | OP_RAILGUN |

所以你可以直接写一个

```c++
Operation make_your_decision( const Snake &snake_to_operate, const Context &ctx ) {
  return OP_UP;
}
```

这样你的所有蛇都会~~莫名其妙地~~向上跑，这就是一个合法的最小AI。

第二个函数是汇报游戏结束后的输赢与得分情况，各个参数顾名思义。

这两个函数的参数名称可以随意改变，如果你觉得`snake_to_operate`太长的话。

ADK会帮你处理建立stdio或socket通讯、读取游戏配置、读取物品列表、读取与处理你和对手的操作、维护游戏数据等操作，`Context ctx`内应该有你需要的一切信息，具体的使用方法请参考第三节`API Reference`。

## 2.样例AI的优化方向

> 供参考

- 第8-11行：何时使用融化射线？考虑使用后的收益
- 第14-23行：何时分裂更优？考虑分裂时长度以及其它因素
- 第77-111行：如何才能找到最近的道具的最优解，考虑安全性、可能的收益以及寻找的范围
- 第115-140行：怎样使蛇固化后围成的区域能够较大？在何处、以怎样的路径固化？
- 更多其他策略，如主动去围对手、从对手处逃跑

## 3. API Reference

### 3.0 结构体是啥

~~其实你可以翻一下程序设计基础的课件~~

为了更加清楚的组织一大堆状态和数据，我们使用结构体来进行层次化的组织，你可以使用`{}` 内依次提供数据值来创建一个结构体，如：

```c++
Coord c0 = {0, 1};
```

创建了一个`x`坐标为0，`y`坐标为1的`Coord`、即“坐标”结构体。

然后你就可以使用`.`来读取和修改内部的信息：

```c++
int x0 = c0.x; // x0 == 0
c0.y += 2;     // c0.y == 3
```

当然为了确保ADK内数据的正确性，提供给大家的结构体一般都只支持读取哦～

### 3.1 Coord结构体

简单的包含`x`和`y`的结构体，并且重载了`==` 用于方便地判断相等，如：

```c++
Coord c0, c1;
c0 == c1;
```

等价于

```c++
c0.x == c1.x && c0.y == c1.y;
```

### 3.2 Snake结构体

对于`Snake snake`对象，支持以下操作：

- `snake.length()`：长度
- `snake[i]`：从蛇头开始，第`i`个位置的坐标，下标从零开始
  - 进一步可以获得`x` `y`坐标：`snake[0].x`
- `snake.coord_list`：坐标列表，是一个`std::vector<Coord>` 
- `snake.id`：编号
- `snake.length_bank`：长度银行
- `snake.camp`：归属的玩家
- `snake.railgun_item`：拥有的融化射线道具
  - 若没有融化射线，则这个道具的`id`小于零，即`snake.railgun_item.id < 0`为`ture`代表没有融化射线
- `s1 == s2`：可以判断两条蛇是否是同一条，其实就是包装了判断其id是否相等

### 3.3 Item结构体

一个道具`Item item`包含以下数据：

- `item.x` `item.y`：坐标
- `item.id`：编号
- `item.time`：道具生成的时间/回合数
- `item.type`：类型，0为长度道具，2为融化射线~~（不要问1是啥，问就是游戏平衡性调整）~~
- `item.param`：参数，只对于长度道具有效，代表蛇吃了以后变长/给长度银行增加的数量

### 3.4 TwoDimArray

顾名思义，二维数组，和你想的用法一模一样：

```c++
TwoDimArray<int> arr {3, 3, 0};
arr[0][1] = 1;
int k = arr[2][2];
```

第一行是构造一个二维数组，三个数字分别代表第一维长度，第二维长度和初始值。

支持拷贝赋值/构造，~~说人话~~就是把它赋值给另一个二维数组代表把内部元素都复制一遍：

```c++
TwoDimArray<int> arr1 = arr;
```

> 不清楚那个尖括号是啥？可以理解为尖括号里面是什么类型那么数组的元素就是什么类型，比如上面的都是`int`的二维数组，`TwoDimArray<double>` 就是`double` 的二维数组

### 3.5 Context结构体

对于一个游戏上下文环境`Context ctx`来说，支持以下操作（注意都带着一个小括号哦）：

- `ctx.wall_map()`：`int`二维数组，当前回合的墙的分布情况，-1 / 0 / 1 代表无墙 / 0号选手的墙 / 1号选手的墙
- `ctx.snake_map()`：`int`二维数组，当前回合的蛇的分布情况，-1 / id 代表无 / id号蛇占据
- `ctx.item_map()`：`int` 二维数组，当前回合的道具分布情况，-1 / id 代表无 / id号道具占据
- `ctx.item_list()`：`Item`的`std::vector`，所有道具的列表
- `ctx.snake_list_0()` `ctx.snake_list_1()`：`Snake`的`std::vector`，分别为0号玩家和1号玩家的蛇的列表
- `ctx.length()` `ctx.width()`：地图的长度和宽度
- `ctx.current_round()` `ctx.max_round()`：当前回合与最大回合
- `ctx.current_player()`：当前正在操作的玩家
- `ctx.my_snakes()` `ctx.opponents_snakes()`：获取我方蛇的列表和对方蛇的列表，免去了选手通过`current_player()`判断的麻烦
- `ctx.find_item(item_id)` `ctx.find_snake(snake_id)`：通过`id`获取道具`Item`结构体或蛇`Snake`结构体
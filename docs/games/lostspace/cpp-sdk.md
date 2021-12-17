# C++ AI SDK 文档

## 1. 选手提交说明

### 选手必须的填写函数

**play**

当选手回合开始后，会执行一次 `play` 函数。选手需要在该函数中操控 `client` 完成**一回合的决策、行动**。

NOTE：完成所有的决策和操作后调用`finish`结束回合。

```cpp
void play(AI_Client& client);
```

参数：

- `client`：`AI_Client&`，参与游戏的 AI 类，在其中处理了数据的发送与接收。

### 选手可以填写的函数

选手可以填写以下函数，用于处理回合外，自己的陷阱被触发时的处理部分。这两个函数会在回合外，接收到陷阱触发/摧毁消息时被自动调用。

NOTE: 由于陷阱被触发后立刻摧毁，所以可以只填写一个。

```cpp
void _trigger_trap(std::string trap, Pos pos);
void _destroy_trap(std::string trap, Pos pos);
```

参数：

* `trap`：被触发的陷阱类型
* `pos`：被触发的位置

### 提交说明

使用如下指令，将`ai_client.cpp`, `ai_client.h`, `main.cpp`, `Makefile`, `mapconf2.map`, 文件夹`jsoncpp`，和所需的其他文件一同打包交至saiblo。你可以修改`Makefile`中对应的部分，以使用以下指令更方便的完成打包操作（我们在Makefile中给出了样例ai的打包命令示例）：

```bash
make ai.zip
```

## 2. AI_Client

代表游戏中的一个 AI，提供各种行动和信息查询接口。

### 2.1 构造函数

---

```cpp
AI_Client::AI_Client(char* map_config_path);
AI_Client::AI_Client(const AI_Client& another_client);
```

1. 由地图文件构造：`map_config_path`：`char*`，地图文件的路径，默认网站评测时是sdk文件夹下的mapconf2.map，本地评测时默认是逻辑源码文件夹下的mapconf2.map。
2. 拷贝构造。

### 2.2 pos和node的转换

---

[`Pos`](#pos)和[`Node`](#node)的定义见下。

#### pos2node

```c++
Node& AI_Client::pos2node(const Pos& p);
```

参数：

- `p`：[`const Pos&`](#pos)，待查点的坐标。

返回值：[`Node&`](#node)，坐标 `p` 对应的地图节点。当坐标`p`不合法时，抛出`runtime_error`。

#### node2pos

```c++
Pos AI_Client::node2pos(const Node& n);
```

参数：

- `n`：[`const Node&`](#node)，待查的地图节点。

返回值：[`Pos`](#pos)，节点 `n` 对应的坐标。

### 2.3 查询函数

---
#### get_state
```c++
int get_state() const;
```

返回值： 当前的回合数

#### get_escape_pos

```c++
Pos AI_Client::get_escape_pos() const;
```

返回值：[`Pos`](#pos)，最终逃生点的坐标。

#### get_spawn_pos

```c++
Pos AI_Client::get_spawn_pos(int num) const;
```

参数：

- `num`：`int`，待查玩家的 ID（0/1/2/3）.

返回值：[`Pos`](#pos)，ID 为 `num` 的玩家的出生点坐标。

#### get_player_pos

```c++
Pos AI_Client::get_player_pos() const;
```

返回值：[`Pos`](#pos)，玩家当前的坐标。

#### get_others_pos

```c++
Pos AI_Client::get_others_pos(int id) const;
```

参数：

- `id`：`int`，待查玩家的 ID.

返回值：[`Pos`](#pos)，ID 为 `id` 的玩家的当前坐标。

**注**：当前玩家的坐标请使用 [`get_player_pos`](#get_player_pos) 获取。

#### get_player_hp

```c++
int AI_Client::get_player_hp() const;
```

返回值：`int`，玩家当前的血量。

#### get_others_hp

```c++
int AI_Client::get_others_hp(int id) const;
```

参数：

- `id`：`int`，待查玩家的 ID.

返回值：`int`，ID 为 `id` 的玩家的当前血量。

**注**：当前玩家的血量请使用 [`get_player_hp`](#get_player_hp) 获取。

#### get_neighbors

```c++
std::vector<Pos> AI_Client::get_neighbors(const Pos &p, bool consider_elevator);
```

参数：

- `p`：[`const Pos&`](#pos)，待查点的坐标。
- `consider_elevator`：`bool`，电梯连通性，`true` 表示考虑上下层相连，**默认**为 `false`.

返回值：[`std::vector<Pos>`](#pos)，与坐标 `p` 相邻节点的坐标列表。

**注**：仅返回可达点的坐标，即相邻且未被缩圈的点。

#### find_shortest_path

```c++
std::vector<Pos> AI_Client::find_shortest_path(const Pos &s, const Pos &t);
```

参数：

- `s`：[`const Pos&`](#pos)，路径起点坐标。
- `t`：[`const Pos&`](#pos)，路径终点坐标。

返回值：[`std::vector<Pos>`](#pos)，起点 `s` 到终点 `t` 的一条最短路径，列表中的坐标按从 `s` 到 `t` 顺序排列。

复杂度：线性。

**注**：若 `s` 与 `t` 不连通，则返回空 `std::vector`.

#### get_check_cd

```c++
int AI_Client::get_check_cd() const;
```

返回值：`int`，探查行动的技能冷却剩余回合数。

#### get_tools

```c++
Tool AI_Client::get_tools() const;
```

返回值：[`Tool`](#tool)，玩家持有的道具。[`Tool`](#tool)的定义见下。

#### get_keys

```c++
std::vector<int> AI_Client::get_keys() const;
```

返回值：`std::vector<int>`，玩家持有的钥匙列表。

#### get_others_keys

```c++
std::vector<int> AI_Client::get_others_keys(int id) const;
```

参数：

- `id`：`int`，待查玩家的 ID.

返回值：`std::vector<int>`，ID 为 `id` 的玩家持有的钥匙列表。

**注**：当前玩家持有的钥匙请使用 [`get_keys`](#get_keys) 获取。

#### get_id

```c++
int AI_Client::get_id() const;
```

返回值：`int`，当前玩家的 ID.

#### judge_if_can_pick

```cpp
std::pair<bool, std::vector<std::string>> judge_if_can_pick(const Node& n);
```

参数：

- `n`：[`const Node&`](#node)，待查的地图节点。

返回值：`std::pair<bool, std::vector<std::string>>>`，若该节点上有物品可拾取，则 `.first` 为 `true`；`.second` 表示物品信息，包含：

- `"Materials"`：物资点。
- `"Box"`：玩家死亡后的掉落物。
- 两个都无则vector为空

### 2.4 进行操作

---

#### move

```c++
bool AI_Client::move(const Pos& tar);
```

参数：

- `tar`：[`const Pos&`](#pos)，目标位置的坐标，玩家移动一步，到达 `tar`.

返回值：`bool`，若操作合法，则返回 `true`.

#### attack

```c++
bool AI_Client::attack(const Pos& pos, int playerid);
```

参数：

- `pos`：[`const Pos&`](#pos)，攻击的目标位置。
- `playerid`：`int`，对 `pos` 处 ID 为 `playerid` 的玩家发动攻击。

返回值：`bool`，若操作合法，则返回 `true`.

#### inspect_materials

```c++
bool AI_Client::inspect_materials(const std::string& type);
```

拾取物资点中的物资。

参数：

- `type`：`const std::string&`，要拾取物品的种类，合法取值为[`ToolType`](#tooltype)中的字符串：

返回值：若拾取成功，则返回 `true`.

#### inspect_box

```cpp
bool inspect_box();
```

拾取掉落物。

返回值：操作是否合法。

#### place_trap

```c++
bool AI_Client::place_trap(const std::string& type);""
```

参数：

- `type`：`const std::string&`，在当前位置放置一个 `type` 类型的陷阱，`type` 取值范围为：
  - `"LandMine"`：地雷
  - `"Sticky"`：粘弹（`ToolType.Slime`)

返回值：`bool`，若操作合法，则返回 `true`.

#### use_kit

```c++
bool AI_Client::use_kit();
```

使用一个医疗包。

返回值：`bool`，若操作合法，则返回 `true`.

#### detect

```c++
bool AI_Client::detect(const Pos& tar);
```

参数：

- `tar`：[`const Pos&`](#pos)，要探查的坐标（合法的坐标与自己所在的坐标相邻，且不可跨层）。

返回值：`bool`，若发现陷阱，则将其摧毁，并返回 `true`。但如果该点处的陷阱是自己的，则不将其摧毁，但返回`true`。若未发现陷阱，则返回`false`。

**注**：若操作非法，则无任何效果，并抛出`runtime_error`异常。

#### get_key

```c++
bool AI_Client::get_key();
```

与当前位置的钥匙机互动，下一回合将获得对应钥匙。

返回值：`bool`，若操作合法，则返回 `true`.

#### escape

```cpp
bool escape(bool to_escape); 
```

进行逃离。

参数：

* `to_escape`：为`true`表示进入逃生等待状态，为`false`表示中断逃生等待。

返回值：`bool`，若操作合法，则返回 `true`.

#### blink

```c++
bool AI_Client::blink(Pos target);
```

参数：

- `target`：[`Pos&`](#pos)，目标位置的坐标，玩家闪现一步距离（不可跨层闪现），到达 `target`.

返回值：`bool`，若操作合法，则返回 `true`.

#### finish

```c++
void AI_Client::finish();
```

结束本回合。

### 2.5 得到拷贝

---

#### get_player_copy

```cpp
Player get_player_copy(int id);
```

参数：

* id: 要得到的player的id，可以为自己

返回值：该player的拷贝值。

#### get_copy

```cpp
std::pair<AI_Client*, std::vector<Player*>> get_copy();
```

得到一份完整的`AI_Client`的拷贝，并返回该拷贝对应的私有变量。

返回值：返回一个`AI_Client`的拷贝，和该拷贝中所有`Player`的指针列表。指针列表中按照`id`顺序，依次存放了该拷贝中，**包括自己在内**的所有玩家的Player实例。

**Tips**：修改`pos2node`的返回值中的信息可以修改当前`AI_Client`的节点信息。

### 2.6 主进程函数

---

#### run

```c++
void AI_Client::run();
```

用于回合制循环。

---

## 类型别名

### Pos

```c++
using Pos = std::tuple<int, int, int>;
```

坐标三元组 $(x, y, z)$.

### Trapbag

```c++
using Trapbag = std::map<std::string, std::vector<std::vector<int>>>;
```

- `key_type`：陷阱类型：
  - `"LandMine"`：地雷
  - `"Sticky"`：粘弹
- `mapped_type`：二维数组，该类型陷阱道具的使用信息
  - `[0][]`：大小为 $1$ 的列表，剩余陷阱道具的数量。
  - `[1][]`：大小为 $1$ 的列表，已放置的陷阱道具的数量。
  - `[i][]`：$i > 1$，大小为 $3$ 的列表，已放置陷阱道具的坐标 $[x, y, z]$.

### Nodeset

```c++
using Nodeset = std::vector<std::vector<std::vector<Node*>>>;
```

三维数组，用于存储地图，三个维度依次对应 $x, y, z$.

---

## 常量

### LAYER

```c++
constexpr int LAYER = 3;
```

地图的层数。

### INF

```c++
constexpr int INF = 0x3f3f3f3f;
```

相较于游戏中所有可能出现的合法数值，等效于 $+\infty$.

### ToolType

```cpp
// 道具类型
struct {
    std::string LandMine = "LandMine";  // 地雷
    std::string Slime = "Sticky";       // 粘弹，策划案中的官方名称为Slime，游戏逻辑中对应为Sticky
    std::string MedKit = "Kit";         // 医疗包，官方名称为MedKit，逻辑中为Kit
    std::string Blink ="Transport";     // 闪现，官方名称为Blink，逻辑中为Transport
} const ToolType;

```

### InterpropsType

```cpp
// 交互道具类型
struct {
    std::string KeyMachine = "KeyMachine";
    std::string Materials = "Materials";
    std::string EscapeCapsule = "EscapeCapsule";
    std::string Elevator = "Elevator";
    std::string Box = "Box";
} const InterpropsType;
```

## 枚举量

### STATUS

```c++
enum STATUS { ALIVE, DEAD, ESCAPED, SKIP, WAIT_FOR_ESCAPE, AI_ERROR};
```

玩家的游戏状态。

- `ALIVE`：存活
- `DEAD`：死亡
- `ESCAPED`：逃离成功
- `SKIP`：回合跳过（踩中粘弹，或者与钥匙机交互后）
- `WAIT_FOR_ESCAPE`：逃生等待状态
- `AI_ERROR`：ai错误

### MapReaderStatus

```c++
enum MapReaderStatus { NODE, EDGE, MATERIAL, HMATERIAL, DOOR, ELEVATOR, BLOCK };
```

读取地图时，表示当前数据段的类型。

- `NODE`：节点
- `EDGE`：边
- `MATERIAL`：陷阱道具
- `HMATERIAL`：医疗包
- `ELEVATOR`：电梯
- `BLOCK`：功能区

---

## 结构体

### Edge

```c++
struct Edge {
  Pos stpos, edpos;
  bool elevator_only;
  Edge() = default;
  Edge(const Pos& s, const Pos& t);
};
```

地图中的边。

- `stpos`, `edpos`：[`Pos`](#pos)，起点、终点。
- `elevator_only`：`bool`，是否为用于电梯的虚边，**默认**为 `false`.
- 构造函数：

  1. 默认构造。
  2. 由端点构造：

     - `s`：[`const Pos&`](#pos)，起点坐标。
     - `t`：[`const Pos&`](#pos)，终点坐标。

### Node

```c++
struct Node {
  Pos pos;           
  std::vector<int> playerid;  
  std::vector<std::string> interprops;  
  std::vector<Edge*> edges;   
  bool box = false;           
  bool has_materials = false;   
  int block_turn = INF;           
  bool able = true;                 
  Node() = default;
  Node(const Pos& p) : pos(p) {}
  Node(const Node& another_node);
};
```

地图中的节点。

- `pos`：[`Pos`](#pos)，节点的坐标。
- `playerid`：`std::vector<int>`，位于该节点的玩家 ID 列表。
- `interprops`：`std::vector<std::string>`，交互道具类型：
  - `"Materials"`：物资点
  - `"KeyMachine"`：钥匙机
  - `"EscapeCapsule"`: 逃生舱
- `edges`：[`std::vector<Edge*>`](#edge)，由该点出发的边的列表。
- `box`：`bool`，若该节点上有玩家死亡后的凋落物，则为 `true`，**默认**为 `false`.
- `has_materials`：`bool`，若该节点上有物资点，则为 `true`，**默认**为 `false`.
- `block_turn`：`int`，第 `block_turn` 回合时该节点将被缩圈，**默认**为 [`INF`](#inf).
- `able`：`bool`，若该节点已经被缩圈，则为 `false`，**默认**为 `true`.
- 构造函数：
  1. 默认构造。
  2. 由坐标构造：
     - `p`：[`const Pos&`](#pos)，节点坐标。
  3. 拷贝构造。

### Tool

```c++
struct Tool {
  int kits_num;
  int blinks_num;
  Trapbag trapbag;
};
```

一个玩家的所有道具。

- `kits_num`：`int`，医疗包数量。
  -`blinks_num`:`int`，闪现道具数量。
- `trapbag`：[`Trapbag`](#trapbag)，陷阱的物品栏。

### Player

```c++
struct Player {  
  int hp = 200;   
  STATUS status = ALIVE;  
  int id = -1;       
  bool has_entered_escape_layer = false;   
  std::vector<int> keys;  
  Pos pos;            
  Tool toolbag;         
  Player() = default;
  Player(const Player& another_player) = default;
};
```

玩家类。

- `hp`：`int`，血量。
- `status`：[`STATUS`](#status)，玩家的游戏状态。
- `id`：`int`，玩家的 ID.
- `has_entered_escape_layer`: 仅对于别的玩家有意义，对于自己是无意义的，表示是否曾经到达逃生舱那层。
- `keys`：`std::vector<int>`，持有钥匙的列表。
- `pos`：[`Pos`](#pos)，当前坐标。
- `toolbag`：[`Tool`](#tool)，持有的道具。
- 构造函数：
  1. 默认构造。
  2. 拷贝构造。

### Map

```cpp
// 只展示成员变量
struct Map {
  Nodeset nodeset;
  std::vector<Edge> edgeset;
};
```

地图类。

- `nodeset`：[`Nodeset`](#nodeset)，地图中节点的数组。
- `edgeset`：[`std::vector<Edge>`](#edge)，地图中边的数组，**默认容量**为 $1000$.

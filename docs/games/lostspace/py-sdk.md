# Python SDK 手册

## 选手填写函数

需要选手填写的 SDK 函数为`play`一个函数。

**play**

选手在回合内进行的所有操作。

## 选手可调用函数

### 三元组说明（重要）

下文中所有三元组坐标为`list[int]` 的形式，规定为：`[层数, x, y]`。

### get_my_num

**函数原型**

```python
def get_my_num(self): ...
```

获得自己的编号，返回值为`int`。

### get_my_pos

**函数原型**

```python
def get_my_pos(self): ...
```

获得自己的位置，返回值为三元组。

### get_my_hp

**函数原型**

```python
def get_my_hp(self): ...
```

获得自己的血量，返回值为`int`。
### get_other_pos

**函数原型**

```python
def get_other_pos(self,player_id): ...
```

获得别人的位置，返回值为三元组。

### get_my_hp

**函数原型**

```python
def get_other_hp(self,player_id): ...
```

获得别人的血量，返回值为`int`。

### get_keys

**函数原型**

```python
def get_keys(self): ...
```

获得当前拿到的钥匙，返回值为`list[int]`。

### get_neighbors

**函数原型**

```python
def get_neighbors(self, pos): ...
```

获得某节点的相邻点集，`pos`为节点坐标三元组，返回值为视野内相邻点集的三元组列表。

### move

**函数原型**

```python
def move(self, pos): ...
```

移动，`pos`为节点坐标三元组，返回值为`dict`，格式如下：

```python
{
    "type": "action",
    "success": true,      //操作是否成功
    "hp": 100,       //剩余血量
}
```

### attack

**函数原型**

```python
def attack(self, pos, player_id): ...
```

攻击，`pos`为节点坐标三元组，`player_id`为攻击目标（`int`），无返回值。

### interact

**函数原型**

```python
def interact(self, tool_type): ...
```

与场景道具（除了门和箱子）交互，`tool_type`为道具类型字符串，可取值如下：

```python
"Console"
"EscapeCapsule"
"KeyMachine"
```

返回值为`dict`，格式如下：

```python
{
    "type": "action",
    "success": true,      //操作是否成功
}
```





### view_box

**函数原型**

```python
def view_box(self, box_type, tool_type):
```

检视箱子，`box_type`为掉落池/物资点字符串，可取值如下：

```python
"Box"
"Materials"
```
若box_type为“Materials”，则第三个参数为拿取的物品，可取值如下：
```python
"Landmine"
"Kit"
"Transport"
"Sticky"
```

返回值为`dict`，格式如下：

```python
{
    "type": "action",
    "success": true,      //操作是否成功
    "content": { "LandMine": 0, "Sticky": 0, "Spine": 0, "Alert": 0, "Kit": 0 } //如果成功，返回内容
}
```



### put_trap

**函数原型**

```python
def put_trap(self, trap_type):
```

放置陷阱，`trap_type`为陷阱类型字符串，可取值如下：

```python
"LandMine"
"Sticky"
```

### use_tool

**函数原型**

```python
def use_tool(self, tool_type):
```

使用道具，`tool_type`为道具类型字符串，可取值如下：

```python
"Kit"
"Transport"
```


### end_turn

**函数原型**

```python
def end_turn(self):
```

结束回合，无返回值。

### get_landmine_pos

**函数原型**

```python
def get_landmine_pos(self):
```

获得当前地雷位置坐标，返回值为三元组列表。

### get_sticky_pos

**函数原型**

```python
def get_sticky_pos(self):
```

获得当前粘弹位置坐标，返回值为三元组列表。

### get_spawn_pos

**函数原型**

```python
def get_spawn_pos(self,x):
```

获得编号id选手的出生位置坐标，返回值为三元组列表。

### get_escape_pos

**函数原型**

```python
def get_escape_pos(self):
```

获得逃生舱位置坐标，返回值为三元组列表。

## 预定义类

### Node

地图节点类，用于存储地图中一个节点的信息，其成员变量如下：

#### `pos`

- ```python
  self.pos: list[int] = ...
  ```

- 该节点的坐标，类型为上文提到的坐标三元组。

#### `interprops`

- ```python
  self.interprops: list[str] = ...
  ```

- 该节点存在的交互道具，存储形式为交互道具名称的字符串组成的列表。

#### `player`

- ```python
  self.player: list[int] = ...
  ```

- 该节点上的玩家编号列表。

### Edge

地图边类，存储地图中边的信息，其成员变量如下：

#### `stpos`

- ```python
  self.stpos: list[int] = ...
  ```

- 边的起点坐标，类型为上文提到的坐标三元组。

#### `edpos`

- ```python
  self.edpos: list[int] = ...
  ```

- 边的终点坐标，类型为上文提到的坐标三元组。

### Tool

工具类，存储工具的相关信息，这里工具包括陷阱和医疗包，其成员变量如下：

#### `landmine_number`

- ```python
  self.landmine_number: list[int] = ...
  ```

- 地雷数目二元列表，第一个元素代表未放置的地雷个数，第二个元素代表已经放置的地雷个数。

#### `landmine_pos`

- ```python
  self.landmine_pos: list[list[int]] = ...
  ```

- 地雷放置的位置列表。列表的每一个元素类型均为坐标三元组，分别代表一个已经放置的地雷的坐标。

#### `sticky_number`

- ```python
  self.sticky_number: list[int] = ...
  ```

- 粘弹数目二元列表，同地雷。

#### `sticky_pos`

- ```python
  self.sticky_pos: list[list[int]] = ...
  ```

- 粘弹放置的位置列表，同地雷。

#### `kit`

- ```python
  self.kit: int = ...
  ```

- 医疗包个数。

#### `transport`

- ```python
  self.transport: int = ...
  ```

- 闪现个数。

### Player

玩家类，存储一个玩家的相关信息，成员变量如下：

#### `id`

- ```python
  self.id: int = ...
  ```

- 玩家的 `id` 。

#### `status`

- ```python
  self.status: int = ...
  ```

- 玩家状态，0 代表活着， 1 代表死亡， 2 代表已经逃离。

#### `hp`

- ```python
  self.hp: int = ...
  ```

- 玩家的血量。

#### `keys`

- ```python
  self.keys: list[int] = ...
  ```

- 玩家拿到的钥匙列表，列表中的整数代表一个钥匙对应的玩家的编号。

#### `tools`

- ```python
  self.tools: Tool = ...
  ```

- 玩家的工具包，类型为上边定义的 `Tool` 类。

### View

视野类，存储玩家的视野信息，成员变量如下：

#### `self_nodes`

- ```python
  self.self_nodes: list[Node] = ...
  ```

- 玩家周围的节点信息。




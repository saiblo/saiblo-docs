# 通讯格式说明

**由于未及时更新，本说明仅供选手参考， 本说明中与实际通讯格式不一致之处请以实际通讯格式为准（由judger输出）！**

## 一. 通讯流程

### 1. 回合开始阶段

对于每一小回合，逻辑向该回合行动的玩家发送包含自己的血量、武器充能状态、自己的行动力、陷阱状态、道具持有状态、其他人状态情况等内容的回合开始消息：

【**后文中的方向总与如下规定一致**】

```json
{
    "type": "roundbegin",  
    "state": 111, //当前的大回合数
    "inturn": 1,  //处于回合中的玩家的id 
    "status": 0, //自己的状态，0活着，1死了，2逃了，3回合跳过，在为非0的时候回合自动终止，但仍然收到此消息
    "hp": 100,  //hp
    "keys": [1, 2, 3],  //拿到的钥匙
    "tools": { 
        "LandMine":[<int:该类陷阱的没放置的个数>, <int:已被放置的陷阱的个数>, pos1, pos2 ... posn],  
        "Sticky": [...],
        "Kit": 1,  //持有量
        "Transport": 2,
    },
    "others": [ 
        { "player_id": 1, "status": 0, "keys": [1], "hp": 200},
        //status: 0活着，1死了，2逃了， keys中存储钥匙对应的玩家序号
        ...
    ]
}
```

对于播放器，在上述内容的基础上，还增加了如下信息（注：仍然是一个包，把下面的dict update进上面的dict就行）：

```json
{
   ...
   "attack": [0, 1],   // 可以attack的玩家序列
   "move":   [true, false, false, true, true, false, true, false],  // 八个方向上的点是否可达
   "detect": true,  // 是否可以进行detect
   "interprops": ["Box", "KeyMachine"]   // 当前节点上的交互道具列表
}
```

其中，8个方向顺序规定为：`[(0,1,0), (0,-1,0), (1,0,0), (-1,0,0), (1,-1,0), (-1,1,0), (1,1,0), (-1,-1,0)]`，门的四个方向的顺序就是前四个的顺序。

逻辑向该回合不行动的玩家发送如下信息：

```json
{
    "type": "roundbegin",
    "state": 111, 
    "inturn": 2
}
```

### 回合中

回合中，用户向逻辑发送行动和询问消息，逻辑对用户消息进行回复。具体格式见 **二、用户向逻辑发送信息**
回合结束时，用户应当向逻辑发送结束消息。

### 回合外信息

在别人的回合中，逻辑将给用户发出如下消息：

1. 其他玩家死亡消息（包括自己）
2. 其他玩家逃离消息
3. 其他玩家从视野内经过
4. 其他玩家得到钥匙
5. 自己的陷阱被触发

这些消息将存储于buffer中，可供ai查询。回合外消息格式见 **三、逻辑向玩家发送消息**

## 二、用户向逻辑发送信息

### 用户行动消息

对于用户回合内所有行动消息，有如下模板：

```json
{
    "type": "action",
    "action": [<行动名称>, <参数>]
}
```

行动消息发出之后，逻辑将向用户报告操作是否成功。对于逻辑的回复，有如下模板：

```json
{
    "type": "action",
    "success": true,      //操作是否成功
    ...                   //其余内容，未说明则不含其余内容
}
```

1. 移动
```json
{ 
    "type": "action",
    "action": ["move", [1, 1, 1]] 
    //三元组坐标规定为：(x, y, 层数)
}
```
逻辑的回复为：
```json
{
    "type": "action",
    "success": true,      //操作是否成功
    "hp": 100,    //如果成功，则hp为多少
    "status": 3   // 返回自身状态，如果为3，则该回合自动结束
}
```
在特殊情况下，会添加如下两种消息(注：仍然是一个包，把下面的dict update进上面的dict就行)，并且两种消息可以同时添加。 
如果出现了上下楼，则在该回复的基础上添加了视野更新信息:
```json
{
   // view是list of list，每一项是一个节点的信息：[节点坐标，是否有box，上面的玩家(这项可以没有)]
   "view": [ [[1,2,0], true, [1]], ... ]
   
}
```
如果玩家使用播放器：
```json
{
    ...
   "attack": [0, 1],   // 可以attack的玩家序列
   "move":   [true, false, false, true, true, false, true, false],  // 八个方向上的点是否可达,方向规定与回合开始消息一致
   "detect": true,  // 是否可以进行detect
   "interprops": ["Box", "KeyMachine"]   // 当前节点上的交互道具列表
}
```
2. 攻击
```json
{
    "type": "action",
    "action": ["attack", pos, <int:playerid>]  
}
```
3. 与场景道具交互
```json
{
    "type": "action",
    "action": ["interact", "KeyMachine"]
    //格式为["interact", <交互道具类型>]，第二项与对应的类名一致
    //特别的，如果要与逃生舱交互的话，则格式为 ["interact", "EscapeCapsule", bool]，bool为True表示进入逃生倒计时，False表示终止计时
}
```
回复为:
```json
{
    "type": "action",
    "success": true      //操作是否成功
}
```
4. 与箱子交互。当检视箱子时，发送如下消息：
```json
{
    "type": "action",
    "action": ["interact", "Materials", <str:type>]
}
```
其中，第二项为Box则为掉落池，第二项为Materials则为物资点。对于Box，不含第三项，而对于Material，第三项可为Transport, Kit, LandMine, Sticky。
回复如下：
```json
{
   "type": "action",
   "success": true,      //操作是否成功
   "keys": [1, 2, 3]  // 如果成功，且目标为box，则返回现有钥匙
}
```
对于Box，回复如下：
```json
{
    "type": "action",
    "success": true,      //操作是否成功
}
```
5. 放置陷阱
```json
{
    "type": "action",
    "action": ["trap", "Sticky"]
    //格式为["trap", <陷阱类型>]，第二项与对应类名一致
}
```
6. 使用道具。对于医疗包为：
```json
{
    "type": "action",
    "action": ["tool", "Kit"]
    //格式为["tool", <道具类型>]
}
```
医疗包的返回为：
```json
{
   "type": "action",
   "success": true,      //操作是否成功
   "hp": <hp:int>        //血量
}
```
对于闪现为：
```json
{
   "type": "action",
   "action": ["tool", "Transport", <pos:list>]
}
```
回复和`move`一致
7. 使用探查技能
```json
{
    "type": "action", 
    "action": ["detect", pos]
}
```
回复模板为
```json
{
    "type": "action",
    "success": true,      //操作是否成功
    "has_trap": true     //是否有陷阱
}
```

### 回合结束信息

```json
{
    "type": "finish"
}
```

## 三、逻辑向玩家发送消息

逻辑向玩家发送消息时，实际上是先将满足如下模板的消息发送个judger，然后再由judger转发给各个玩家，因此上述提到的回复模板为玩家最终接收到的消息，逻辑实际发送的消息如下：

```json5
//NOTE：如下消息表示向 player0 发送 消息0， 并向 player1 发送 消息1
//      不是向 player0和1 群发 消息0和1
{
    "state": 233,    //表示回合数，state>0时表示正常回合
    "listen": [0],     //本回合只接收0号玩家的信息
    "player": [0, 1],   //表示发送对象
    "content": ["<消息0>", "<消息1>"]   //每一个消息都满足上文提到的通信模板
}
```

### 回合外消息

对于回合外消息的描述见 **一. 通讯流程**

回合外消息的总体模板(玩家接收到的消息）如下：

```json
{
    "type": "offround",
    "state": 233,   //大回合数
    "playerid": 1,  //是在哪个玩家的小回合中发生的
    "content": [<str:类别>, m1, m2 .. mn]
}
```

1. 其他玩家死亡消息（包括自己）
```json
{
    "type": "offround",
    "state": 233,   //大回合数
    "playerid": 1,  //是在哪个玩家的小回合中发生的
    "content": ["died", 2] //第二个数为谁死了
}
```
2. 其他玩家逃离消息
```json
{
    "type": "offround",
    "state": 233,   //大回合数
    "playerid": 1,  //是在哪个玩家的小回合中发生的，逃离必在自己的小回合内发生
    "content": ["escaped"]
}
```
3. 视野消息。玩家相关的视野消息。其他玩家在视野内移动
```json
{
    "type": "offround",
    "state": 233,   //大回合数
    "playerid": 1,  //是在哪个玩家的小回合中发生的
    "content": ["see", "pos_update", p, <int:playerid>]   //意味着该玩家的位置更新了，如果走出了视野则 p = [-1, -1, -1]
}
```
如果是在视野内的其他行为，则为
```json
{
    "type": "offround",
    "state": 233,   //大回合数
    "playerid": 1,  //是在哪个玩家的小回合中发生的
    "content": ["see",<str:行为名>, pos, <int:playerid> ]  
    //playerid：做出此行为的玩家的序号  行为名： "attack" （在视野内攻击，不知道可不可以知道攻击方向），"regenerate" （在视野内复活）
}
```
场景信息。暂时认为只能看见门的开关和控制台导致的视野变化。关于门的开关：
```json
{
    "type": "offround",
    "state": 233,   //大回合数
    "playerid": 1,  //是在哪个玩家的小回合中发生的
    "content": ["see", "interprops_status_update", pos, <str:场景道具名称>, <str:status>]
    //对于门而言 就是 ["see", "interprops_status_update" , [[1,1,1], [1,2,1]], "Door", "open" / "close"] 其中倒数第三项为门所在边的端点
}
```
4. 其他玩家得到钥匙
```json
{
    "type": "offround",
    "state": 233,   //大回合数
    "playerid": 1,  //是在哪个玩家的小回合中发生的，得到钥匙必须在自己的小回合发生
    "content": ["getkey", <list:playerids>]  //得到了哪个人的钥匙
}
```
5. 自己的陷阱被触发
```json
{
    "type": "offround",
    "state": 233,   //大回合数
    "playerid": 1,  //是在哪个玩家的小回合中发生的
    "content": ["trap_triggered", <str:陷阱类型>, pos]  //pos为陷阱位置
}
```
6. 自己的陷阱被摧毁
```json
{
      "type": "offround",
      "state": 233,   //大回合数
      "playerid": 1,  //是在哪个玩家的小回合中发生的
      "content": ["trap_destoryed", <str:陷阱类型>, pos] //pos为陷阱位置
}
```
7. hp变化
```json
{
    "type": "offround",
    "state": 233,   //大回合数
    "playerid": 1,  //是在哪个玩家的小回合中发生的
    "content": ["hp_update", <hp:int>]
}
```
8. 有玩家到达三层
```json
{
   "type": "offround",
   "state": 233,   //大回合数
   "playerid": 1,  //是在哪个玩家的小回合中发生的
   "content": ["player_enter_top_layer"]
}
```

### 回合内状态更新消息

1. 其他玩家死亡消息
```json
{
    "type": "other_death",
    "playerid": 1  //哪个玩家死了
}
```
2. 逃脱
```json
{
    "type": "escaped"
}
```
3. 自己死亡
```json
{
   "type": "death", 
   "box": true  //掉包则为true，否则为false
}
```

### 初始化消息

在游戏开始时，逻辑会告知ai其id和出生点位置。
```json
{
    "type": "id",
    "id": <id:int>,
    "birth_pos": [0, 0]  // 自己的出生点坐标，这里略去了层数，是二元组
}
```

## 四、逻辑仅向judger发送的信息

### 初始化信息

```json
//以下设置可能更改
{
    "state": 0, 
    "time": 3,     //默认时间限制
    "length": 1024  //单条消息的最大长度
}
```

### 游戏结算信息

```json
{
    "state": -1, 
    "end_info":{
        "0": <score0>,
        "1": <score1>,
        "2": <score2>,
        "3": <score3>
    }
}
```

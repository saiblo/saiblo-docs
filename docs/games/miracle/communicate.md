# 游戏逻辑通信协议

游戏逻辑会将当前局面信息转换成json格式转发给judger，之后再转发给玩家SDK，由SDK进行解析。

玩家收到的信息格式为$6+n$，前$6$位表示json串的长度。可以参考ai_client中的read_opt、update_game_info函数。

玩家传操作时传的信息格式为$4+n$，前$4$位表示json串的长度。json的格式可参考ai_client中init、summon、move、attack、use、end_round函数。

接收到的信息的具体json格式如下

### 总体格式

```json
{
    "map": {
        "units": [
            [3, 0, 0, 2, 1, 2, 2, [3, 4], 3, 4, [0, 0, 0], 1, 0, 1, 0, 0, 1, 1] //一个单位
        ], 
        "barracks": [-1, -1, -1, -1], //驻扎点占领情况，-1为未占领，否则值为玩家id
        "miracles": [30, 30]//双方神迹血量，0号玩家在前
    }, 
    "players": [
        [//player 0
            [//携带神器
                [0, 3, 8, 12, 12, 2, 0, [0, 0, 0]]//神器状态
            ], 
            1, //剩余法力值
            1, //最大法力值
            [//生物容量
                [0, 2, []], 
                [2, 3, []]
            ], 
            [3]//本回合召唤的单位列表
        ], [//player 1。下面同上
            [], 
            2, 
            2, 
            [], 
            []
        ]
    ],
    "camp": 0,
    "round": 0,
}
```

注：以上信息不一定是合法局面的结果，仅供参考

### 生物格式

```json
[
    3, //生物id，从3开始
    0, //生物阵营，同玩家编号
    0, //生物类型，0号为弓箭手，具体类型名对照见后面
    2, //法力值花费
    1, //攻击力
    2, //血量上限
    2, //血量
    [3, 4], //攻击范围
    3, //行动力
    4, //死亡后的冷却时间
    [0, 0, 0], //位置
    1, //等级
    0, //飞行
    1, //对空
    0, //迅捷
    0, //圣盾
    1, //能攻击
    1  //能移动
]
```

### 神器格式

```json
[
    0, //神器id
    3, //神器类型，3为风神，对照见后
    8, //法力值消耗
    12, //使用后冷却时间
    12, //冷却时间
    2, //神器状态，2为冷却中，对照见后
    0, //神器目标类型，0为对坐标，对照见后
    [0, 0, 0] //上次使用地点，未使用过则为[-1,-1,-1]
]
```

### 生物容量格式

```json
[
    0, //生物类型，同生物的对照关系
    2, //还能召唤几个
    [] //在冷却中的生物还需要多久冷却好，例如[1,2]则一个需要一回合一个需要二回合
]
```

### 对照表

具体对照表可以参看游戏逻辑中Data.json，逻辑根据此文件执行转换

#### 生物

```json
"UnitNameParsed": {
        "Archer": 0,
        "Swordsman": 1,
        "BlackBat": 2,
        "Priest": 3,
        "VolcanoDragon": 4,
        "Inferno": 5,
        "FrostDragon": 6
    }
```

#### 神器

```json
"ArtifactNameParsed": {
        "HolyLight": 0,
        "SalamanderShield": 1,
        "InfernoFlame": 2,
        "WindBlessing": 3
    }
```

#### 神器状态

```json
"ArtifactStateParsed": {
        "Ready": 0,
        "In Use": 1,
        "Cooling Down":2
    }
```

#### 神器目标类型

```json
"ArtifactTargetParsed": {
        "Pos": 0,
        "Unit": 1
    }
```

## Pick

logic -> client："Pick"

```json
{
    "Action": "Pick",
    "RemainFishs": [1, 2, 3, 4, 5, 6, 7, 12],
    "FirstMover": 1
}
```

发送该名玩家剩余可选鱼编号，以及这轮该名玩家是否是先手（若为1则是先手）。

client -> logic："Pick"

```json
{
    "Action": "Pick",
    "ChooseFishs": [1, 2, 3, 12],
    "ImitateFish": 4
}
```

发送选择的四条鱼的编号。如果选择的鱼包括拟态鱼，选择中将拟态鱼的编号视为12，并额外发送拟态鱼模仿鱼的编号。

## Assert

logic -> client: "Assert"

```json
{
    "Action": "Assert",
    "EnemyAssert": AssertInfo,
    "EnemyAction": ActionInfo,
    "MyAction": ActionInfo,
    "GameInfo": GameInfo
}
```

发送当前游戏局面信息，对方上回合的断言信息和行动信息，我方上回合的行动信息。

client -> logic:

```json
{
    "Action": "Assert",
    "Pos": 2,
    "ID": 1
}
```

选择断言，发送断言的位置和断言的内容。

```json
{
    "Action": "Null"
}
```

选择不断言，发送不断言的指示。

## Action

logic -> client: "Action"

```json
{
    "Action": "Action",
    "AssertReply": AssertInfo,
    "GameInfo": GameInfo
}
```

发送当前游戏局面，以及该玩家本回合的断言信息。

client -> logic:

```json
{
	"Action": "Action",
    "Type": 1,
    "MyPos": 3,
    "MyList": [0],
    "EnemyList": [1, 3]
}
```

选择发动主动技能，发送我方行动鱼的位置，选择的我方及敌方鱼目标的列表。

```json
{
    "Action": "Action",
    "Type": 0,
    "MyPos": 2,
    "EnemyPos": 0
}
```

选择发动普通攻击，发送我方行动鱼的位置，选择的敌方鱼的位置。

## Finish

logic -> client: "Finish"

```json
{
    "Action": "Finish",
    "Result": "Win"/"Lose",
    "isStateLimitExceed": true/false,
    "MyAssert": AssertInfo
}
```

**我方断言阶段**结束游戏，发送我方本轮输赢结果（Win表示赢），本轮是否是由于到达回合数上限结束（true表示是），我方本回合的断言信息。

```json
{
    "Action": "Finish",
    "Result": "Win"/"Lose",
    "isStateLimitExceed": true/false,
    "MyAction": ActionInfo
}
```

**我方行动阶段**结束游戏，发送我方本轮输赢结果（Win表示赢），本轮是否是由于到达回合数上限结束（true表示是），我方本回合的行动信息。

```json
{
    "Action": "Finish",
    "Result": "Win"/"Lose",
    "isStateLimitExceed": true/false,
    "EnemyAssert": AssertInfo,
    "MyAction": ActionInfo
}
```

**敌方断言阶段**结束游戏，发送我方本轮输赢结果（Win表示赢），本轮是否是由于到达回合数上限结束（true表示是），敌方本回合的断言信息，和我方上回合的行动信息。

```json
{
    "Action": "Finish",
    "Result": "Win"/"Lose",
    "isStateLimitExceed": true/false,
    "EnemyAssert": AssertInfo,
    "EnemyAction": ActionInfo,
    "MyAction": ActionInfo
}
```

**敌方行动阶段**结束游戏，发送我方本轮输赢结果（Win表示赢），本轮是否是由于到达回合数上限结束（true表示是），敌方本回合的断言及行动信息，我方上回合的行动信息。

（建议）client -> logic:

```json
{
    "Action": "Finish"
}
```

## Info

**GameInfo**的格式:

```json
{
    "EnemyFish": [-1, 1, 12, -1],
    "EnemyHP": [0, 100, 150, 50],
    "MyFish": [1, -2, 10, 12],
    "MyHP": [150, 150, 100, 100],
    "MyATK": [100, 170, 100, 240]
}
```

包括敌方鱼的编号、敌方鱼的血量、我方鱼的编号、我方鱼的血量和我方鱼的攻击力，以上信息均根据位置号排列（MyHp的第0项就是我方0号鱼的血量）。

敌方未暴露鱼的编号会是-1，只有被断言出来的鱼才会给出真实编号。

我方未暴露鱼的编号会是真实编号，被断言出来则编号是真实编号的相反数。

所以涉及拟态鱼的编号都是12。

我方或敌方鱼在死亡时，血量会是0。

**AssertInfo**的格式:

```json
{
	"AssertPos": 1,
    "AssertContent": 6,
    "AssertResult": true
}
```

包括断言位置号、断言内容（断言这个位置是哪个编号的鱼）以及断言结果（成功还是失败）。

如果断言阶段没有断言，AssertInfo为`Json::nullValue`。

**ActionInfo**的格式:

```json
{
    "ActionFish": 1,
    "skill": {
        "targets":
        [
            {
                "pos": pos,
                "isEnemy": true/false,
                "value": value
            },
            // ...
        ],
        "type": type,
        "isSkill": true/false
    },
    "hit": [
        {
            "target": target,
            "isEnemy": true/false,
            "value": value,
            "traceable": traceable,
            "time": time
        },
        // ...
    ],
    "passive": [
        {
            "type": type,
            "source": source,
            "isEnemy": true/false,
            "value": value,
            "time": time
        },
        // ...
    ]
}
```

其中 `skill` 描述主动行为（包括主动技能和普通攻击），`hit` 是伤害信息列表（记录所有受伤结算），`passive` 为该次行动触发的被动技能或延时效果列表（记录所有被动技能或延时效果结算），`ActionFish` 表示做出行动的鱼的位置。

- `skill`：

  | `key`     | `value`                                                      |
  | --------- | ------------------------------------------------------------ |
  | `type`    | 字符串类型，表示主动行动类型                                 |
  | `isSkill` | `boolean`类型，为true表示这是一次主动技能，为false表示这是一次普通攻击 |
  | `targets` | 数组类型，行动作用目标的列表，其中的每个元素有 `pos` 属性，为 `number` 类型，描述目标的位置；`isEnemy`属性，为`boolean`类型，描述目标是否属于敌方阵营（相对于**收信者**而非**行动者**，true表示属于敌方阵营）；`value` 属性为 `number` 类型，描述受到的期望伤害值 |

  | `type`           | 描述     | `targets[].value`       |
  | ---------------- | -------- | ----------------------- |
  | `"aoe"`          | 群体伤害 | 该目标期望受到的伤害值  |
  | `"infight"`      | 伤害队友 | 队友期望受到的伤害值    |
  | `"crit"`         | 暴击     | 该目标期望受到的伤害值  |
  | `"subtle"`       | 无作为   | `targets` 为空数组 `[]` |
  | `"normalattack"` | 普通攻击 | 该目标期望受到的伤害值  |

- `hit`：数组类型，其中每个元素为一个对象类型，表示一个实际伤害事件（一次受伤结算）

  | `key`       | `value`                                                      |
  | ----------- | ------------------------------------------------------------ |
  | `target`    | `number` 类型，表示受伤目标的位置                            |
  | `isEnemy`   | `boolean`类型，表示受伤目标是否属于敌方阵营（相对于**收信者**而非**行动者**，true表示属于敌方阵营） |
  | `value`     | `number` 类型，表示受到的实际伤害值                          |
  | `traceable` | `boolean` 类型，当且仅当存在伤害来源，也即该伤害由直接攻击（普通攻击、aoe 或暴击型主动技能）造成时，为 `true` |
  | `time`      | `number` 类型，表示该事件的时间戳，即结算中的顺序（对于每次行动，时间戳从**1**开始**连续**计数，任意两个事件时间戳不同，保证`hit`中按时间戳单调顺序排列元素 ） |

- `passive`：数组类型，其中每个元素为一个对象类型，表示一个被动技能或延时效果触发事件(一次被动或效果结算)

  | `key`     | `value`                                                      |
  | --------- | ------------------------------------------------------------ |
  | `type`    | 字符串类型，被动技能类型                                     |
  | `source`  | `number` 类型，**引发**该事件的目标的位置（不等于发动该事件的鱼的位置，如护友反伤型被动） |
  | `isEnemy` | `boolean`类型，表示**引发**该事件的目标是否属于敌方阵营（相对于**收信者**而非**行动者**，true表示属于敌方阵营） |
  | `value`   | `number` 类型，与被动技能或延时效果有关的值                  |
| `time`    | `number` 类型，表示该事件的时间戳，即结算中的顺序（对于每次行动，时间戳从**1**开始**连续**计数，任意两个事件时间戳不同，保证`passive`中按时间戳单调顺序排列元素 ） |
  
  | `type`      | 描述     | `value`                            |
  | ----------- | -------- | ---------------------------------- |
  | `"counter"` | 膨胀反伤 | 无意义                             |
  | `"deflect"` | 承伤     | 无意义                             |
  | `"reduce"`  | 减伤     | 减伤比例（闪避是0，减伤buff是0.3） |
  | `"heal"`    | 回血     | 实际回血量（考虑了和400取min）     |
  | `"explode"` | 爆炸     | 无意义                             |


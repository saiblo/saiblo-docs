#  Uno示例游戏说明

本游戏是一个简单**回合制、无触发**的游戏。具体游戏规则与输入规则如下



#### 启动游戏

在原目录下已编写好makefile, 在终端中输入make指令可生成main.exe, 启动游戏时, 需**利用命令行参数传入游戏人数**, 具体格式为 **./main n** , 其中n为一正整数, 代表选手数量。建议游玩人数2~8人



#### 开始局面

游戏开始时会为每位参赛者发出7张卡牌, 并随机指定一个起始玩家, 同时从剩余牌库中抽取一张作为牌顶牌。同时并输出$2\times n$ 行信息, 如以下格式

```
add players
player 0 added
...
add players
player n-1 added
```



#### 回合局面信息格式

##### 输出

在每回合开始时会输出当前牌局信息, 共$1+2 \times n+5$ 行 , 如以下格式

```
Message
player(id=0) has cards:
CARD_0 CARD_1 ... 
...
player(id=n-1) has cards:
CARD_0 CARD_1 ...
Current Card: 
CARD
Current Turn:
id
Start Turn
```

Message是游戏开始 / 上回合结束的反馈信息

CARD为卡牌信息, 为 $COLOR\_num$ 的格式。其中, 保证$COLOR$为red、green、blue、yellow、none的其中一种  , 代表当前牌的颜色 (或上位选手打出变色牌所指定的颜色) ; $num$为0~9的数字或是**+2** (抽兩張卡), **R** (轉向卡),  **S** (Skip跳过), **W** (变色卡) , **W+4** (变色抽四张卡) 的其中一种, 代表当前牌的数字 ;  

id为当前回合应该要出牌的选手编号。



##### 输入

此时, 每回合游戏会从裁判程序, 依照选手行动接收n行信息, 每行如以下格式

```
rank [COLOR]
```

第 i 行的rank为一个整数, 代表者**编号 i-1 的选手想要打出的卡牌在其牌组的秩**;  COLOR的参数不一定要加, 为四种颜色的一种, 当且仅当选手打出为W、W+4的牌时才会生效, 并指定当前牌局的颜色 (若未输入则随机选取一种颜色) **空行代表本回合不出牌**

请注意, 只有编号为 id 的该行 (轮到该选手行动时) 会进行游戏逻辑判定 ; 若接收到空行、输入rank不合法、输入牌不符合规则 , 则罚牌一张并轮到下一位选手行动



#### 游戏范例

可能的信息输出如下 :

```
Start Game!
player(id=0) has cards:
red_0 blue_9 blue_R none_W red_6 none_W+4 yellow_S
player(id=1) has cards:
blue_+2 yellow_2 yellow_9 green_2 yellow_8 blue_1 yellow_2
player(id=2) has cards:
red_+2 green_3 green_9 blue_6 blue_0 yellow_3 red_S
Current Card:
green_3
Current Turn:
0
Start Turn
```

意味着当前为编号0号选手的行动 , 牌顶卡绿3。 可能的选手行动输入如下 :

```
5 yellow

2
```

代表选手0号出了第5张卡 (万能+4) 且将牌局变为黄色。此时为0号选手的回合, 只有第1行有效 , 第3行输入无效。

回合结束后的信息如下 :

```
End Turn
player(id=0) has cards:
red_0 blue_9 blue_R none_W red_6 yellow_S
player(id=1) has cards:
blue_+2 yellow_2 yellow_9 green_2 yellow_8 blue_1 yellow_2 green_7 none_W green_1 green_8
player(id=2) has cards:
red_+2 green_3 green_9 blue_6 blue_0 yellow_3 red_S
Current Card:
yellow_W+4
Current Turn:
2
Start Turn
```

可看到因为0号选手出了+4 卡, 1号选手回合被跳过且罚牌四张, 而指定当前牌局颜色为黄色。此时2号玩家只能出黄色牌或+4卡。假如他选择了0号牌 ( 红色不合法) , 则本回合结束后可能的局面如下 : 

```
Obey the rule!End Turn
player(id=0) has cards:
red_0 blue_9 blue_R none_W red_6 yellow_S 
player(id=1) has cards:
blue_+2 yellow_2 yellow_9 green_2 yellow_8 blue_1 yellow_2 green_7 none_W green_1 green_8 none_W+4
player(id=2) has cards:
red_+2 green_3 green_9 blue_6 blue_0 yellow_3 red_S red_+2 
Current Card:
yellow_W+4
Current Turn:
0
Start Turn
```

因为输入不合法 , 2号选手被罚牌一张, 当前牌顶牌不变, 轮到0号选手出牌。



#### 游戏结束

当玩家出了最后一张牌 (且不是功能牌时) , 游戏结束程序终止。此时输出 ```player i wins ! ```。可能的局面如下 : 

```
End Turn
player(id=0) has cards:
none_W+4 none_W red_S green_3 green_S blue_5 red_5 yellow_1
player(id=1) has cards:
blue_7 green_6 blue_8 yellow_5 blue_9 yellow_2 blue_4 yellow_+2 
player(id=2) has cards:

player 2 wins !
```

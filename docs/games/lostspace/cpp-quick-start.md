# 如何快速写一个C++ AI



## 我应该从哪里开始写代码？

在下发的AI文件夹中，我们需要在文件 `example_new.cpp` 的 `play(AI_Client &client)` 函数中编写我们AI每回合的操作。



## 我可以用哪些接口？

样例AI中，使用了：

#### 信息类：

get_id():获得自己的id(0-3)

get_neighbors(Pos):获得某位置能够前往的相邻位置集合

find_shortest_path(Pos,Pos):生成两个位置间的最短路

get_spawn_pos(int):获得某玩家的出生点位置

get_escape_pos():获得逃生舱的位置

get_keys():获得自己当前拥有的密钥列表

get_others_pos(int):获取一名其他玩家的位置，若不可见则会返回(-1,-1,-1)

get_player_hp():获取自己的生命值

get_others_hp(int):获取其他玩家的生命值

get_tools():获取当前物品栏，返回格式参见文档

judge_if_can_pick(Node):判断某一位置是否有可以拾取的物资点/掉落物

pos2node():通过位置获取对应的节点

get_check_cd():获取探查冷却时间

#### 操作类：

detect(Pos):对指定的相邻位置使用探查

move(Pos):移动到相邻位置

attack(Pos,int):攻击指定位置的指定玩家

use_kit():使用医疗包

place_trap(string):在当前位置放置陷阱

inspect_materials(string):拾取当前位置的掉落物/物资点

get_key():使用当前位置的密钥机

escape(bool):使用逃生舱，bool参数用于标识开始倒数/终止倒数



其余接口见AI SDK文档。



## 我该如何快速地写出一个样例AI？

样例AI预先定义了移动、攻击、逃跑、拾取等多种策略，通过维护一个策略栈实现策略的动态调整。在每个回合开始时，样例AI会根据预先定义的条件弹出栈顶的策略，或向栈中加入新的策略，最终只根据栈顶策略执行行动。可以尝试在样例AI的基础上对选择策略的条件、执行策略的具体操作方式进行调整，观察这些调整对AI性能的影响。



## 我应该如何编译测试本地AI？

对于本地编译，您需要有`make`、`g++`与`python3`，在逻辑包的文件夹下，打开命令行，进行如下操作：

```bash
g++ ./sdk/ai_client.cpp ./sdk/example_new.cpp ./sdk/main.cpp ./sdk/jsoncpp/jsoncpp.cpp  -o ./sdk/ai -std=c++2a
```

即可进行编译；

执行

```bash
python3 judger.py python3+-m+src+./src/mapconf2.map ./sdk/ai ./sdk/ai ./sdk/ai ./sdk/ai replay.json
```

或者直接执行 `./run.sh` 即可测试。

由于sdk中有部分python代码并使用了一些python库，请确保您的python环境中安装了要求的包，详见judger和逻辑的使用说明文档。



## 我应该如何提交AI？

一切按照saiblo上的指示进行即可。实际上，您完全可以全程在saiblo上进行测试、而无需本地环境，只需将调试输出输出到`stderr`中即可（即使用 `fprintf(stderr, ...)` 或 `cerr` ）。

请注意：**不要试图向stdout中输出任何东西（即使用`printf`/`cout`）**，否则将会将您的AI判负。

接下来就可以按照saiblo上的指示进行了。

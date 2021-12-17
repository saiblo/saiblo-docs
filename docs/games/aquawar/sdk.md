# sdk手册

[TOC]

## 选手填写函数

需要选手填写的sdk函数为**Pick(Game)**，**Assert(Game)**和 **Act(Game)**三个函数。

### Pick(Game)

选鱼函数

#### 功能

给定当前局面，从剩余的鱼中挑选4条鱼出战。值得注意的是拟态鱼的格式略有不同，将在下方说明。

这个函数的调用时机是游戏刚开始的时候，或是一轮对战结束，进入下一轮对战并开始选鱼的时候。

**函数原型**

```c++
std::vector<int> AI::Pick(Game game)
{
    // TODO: fill your code
}
```

#### 参数

传入的参数是一个表示当前局面的类**Game**，其定义[点击这里](#game)查看。

#### 返回值

返回的值是一个**std::vector<int\>**。不熟悉**STL**的参赛选手可以[点击这里](#stdvector)查看使用方法。

一个合法的返回值应该包含4个**int**类型变量，其含义为挑选的4条鱼的ID。鱼的编号从1至12分别代表射水鱼，喷火鱼，电鳗，翻车鱼，梭子鱼，蝠鲼，海龟，章鱼，大白鲨，锤头鲨，小丑鱼，拟态鱼。其中12不能作为合法的挑选类型存在，因为拟态鱼需要选择一条其他鱼进行拟态，所以拟态鱼的编号应该设置为`12+X`，其中 `X`的含义为模仿的鱼的编号。所以一个合法的返回值，其形式可能为`(1, 2, 4, 15)`，代表出战的鱼为射水鱼，喷火鱼，翻车鱼和拟态鱼(模仿电鳗)。

当然，使用integer类型表示鱼显得很不直观。为此我们提供了预定义的枚举类型**fish_type**，使用鱼的名字作为返回值。关于该枚举变量，可以[点击这里](#fish_type)查看所有鱼的定义。

### Assert(Game)

断言函数

#### 功能

给定当前局面，选择是否需要**断言**一条敌方的鱼。关于断言是什么，可以查看游戏规则介绍文档。

这个函数的调用时机是一个回合开始时，此时选手需要断言。由于此时对方理应完成了一个回合的操作，所以**Game**中的一些信息会对应更新。

**函数原型**

```c++
std::pair<int, int> AI::Assert(Game game)
{
    // TODO: fill your code
}
```

#### 参数

传入的参数是一个表示当前局面的类**Game**，其定义[点击这里](#game)查看。

#### 返回值

返回的值是一个**std::pair<int, int\>**。不熟悉**STL**的参赛选手可以[点击这里](#stdpair)查看使用方法。

对于断言操作而言，我们只需要获取两个变量：敌方鱼的**位置**和“断言”敌方鱼的**ID**。所以返回值的含义十分清楚：第一个int代表的是敌方鱼的**位置**，其取值范围为`[0, 3]`；第二个int代表的是敌方鱼的**ID**，其取值范围为`[1, 12]`。敌方鱼的ID也可以使用枚举类型**fish_type**来表示。关于该枚举变量，可以[点击这里](#fish_type)查看所有鱼的定义。

当然，如果你不能确定敌方鱼是什么，可以令第一个int值为-1，来表示这一回合放弃断言。

所以一个合法的返回值，其形式可能为**std::pair<0, 3>**，其含义为断言敌方0号鱼是电鳗；还有可能是**std::pair<-1, -1>**，其含义为我方该回合放弃断言。

#### 提示

如果你没有把握自己的AI有一定的推断能力，那么建议AI不要轻易断言——因为断言失败会带来很大的负面效果！所以如果你对自己的AI没有信心，可以复制下面这段代码来跳过断言阶段：

```c++
return std::make_pair(-1, -1);
```

### Act(Game)

行动函数

#### 功能

给定当前局面，选择这一回合需要进行的行动。

这个函数的调用时机是断言阶段结束后，行动回合开始阶段。如果你在断言阶段进行了一次断言，那么**Game**里的内容会随之进行更新(主要是**my_assert**)。

**函数原型**

```c++
Action AI::Act(Game game)
{
    // TODO: fill your code
}
```

#### 参数

传入的参数是一个表示当前局面的类**Game**，其定义[点击这里](#game)查看。

#### 返回值

返回值是一个类**Action**，代表这一回合做出的行动。关于**Action**，可以[点击这里](#action)查看定义和使用方法。

#### Action的简单使用

由于每条鱼的主动技能不一样，所以做出的操作会比较复杂。为了让这件事情变得更简单一些，我们定义了一个**Action**类，里面存有这一回合的操作。下面将介绍怎么使用**Action**类进行行动的操作。

首先，选手需要做的事情是创建一个**Action**类的实例：

```c++
Action action(game);
```

这一行代码不需要改动。这样我们就拥有了一个**Action**类实例。接下来只需要向**action**中添加行动，再返回**action**即可。

其次，选手需要挑选一条该回合能行动的鱼，并通过**set_action_fish()**函数向**action**中添加行动的鱼的信息。举个例子，假设我方2号位的鱼是海龟，我想让海龟这一回合进行行动，那么我需要写以下代码：

```c++
action.set_action_fish(2);
```

参数“2”代表行动的鱼是2号位的鱼。

然后，选手需要指定选中的鱼这一回合是需要进行普通攻击还是使用技能。假设我让海龟使用技能，那么我需要写以下代码：

```c++
action.set_action_type(1);
```

参数“1”代表使用主动技能，“0”代表使用普通攻击。

接下来，由于海龟的主动技能需要选中一个友方随从和一个敌方随从，所以在返回**action**之前还需要再指定两个目标。对于指定目标为友方鱼的情况，我需要使用**set_friend_target()**来选择友方随从。假设我选择友方1号鱼为友方目标，那么我需要写以下代码:

```c++
action.set_friend_target(1);
```

参数“1”代表选中的友方目标是位置为1的友方鱼。

最后我们考虑选择敌方目标。我需要使用**set_enemy_target()**函数来选择敌方随从。假设我选择敌方3号鱼为目标，那么我需要写以下代码：

```c++
action.set_enemy_target(3);
```

参数“3”代表选中的敌方目标是位置为3的敌方鱼。

这时候，我已经设置完了全部的行动参数了！所以我需要提交我的行动，将**action**返回：

```c++
return action;
```

这样子，我就完成了这个函数的任务了。

**注意：以上函数的调用次序最好不要更改！否则可能会导致设置行动失败！**

## 选手可调用函数

### debug_msg(std::string)

该函数的作用是将一个给定字符串输出到一个名为**ai_debug_info.txt**的文件里，可以输出调试信息。

**注意：该函数只能在本地运行的时候使用，不能在saiblo上使用，否则可能导致AI错误！**

**函数原型**

```c++
void debug_msg(std::string str)；
```

**使用示范**

```c++
debug_msg("This is a debug info");
```

等到本地运行结束后，在**judger.py**文件的同级目录下会出现一个名为**ai_debug_info.txt**的文件，里面写有"This is a debug info"。

关于本地运行，可以参考游戏包内的文档**judger使用说明**。

### get_my_remain_fishes()

该函数的作用是将当前剩余可选的鱼以**std::vector<int\>**的形式返回给选手，一般在选鱼阶段使用。

**函数原型**

```c++
std::vector<int> get_my_remain_fishes();
```

**使用示范**

```c++
std::vector<int> remain_fishes = get_my_remain_fishes();
```

### get_enemy_id(int)

该函数的作用是获取对方指定位置的鱼的id(或者称之为种类)。如果这一条鱼已经被断言成功，则返回这条鱼的id；如果这一条鱼还没有被断言成功，则返回-1。

可以直接访问**Game**中的变量来替代本函数：(pos为敌方鱼的位置)

```c++
int enemy_id = game.enemy_id[pos].id;
```

**函数原型**

```c++
int get_enemy_id(int pos);
```

**使用示范**

```c++
int enemy_id = get_enemy_id(pos);
```

### get_enemy_hp(int)

该函数的作用是获取对方指定位置的鱼的hp。

可以直接访问**Game**中的变量来替代本函数：(pos为敌方鱼的位置)

```c++
int enemy_hp = game.enemy_id[pos].hp;
```

**函数原型**

```c++
int get_enemy_hp(int pos);
```

**使用示范**

```c++
int enemy_hp = get_enemy_hp(pos)
```

### get_my_id(int)

该函数的作用是获取我方指定位置的鱼的id。如果这一条鱼已经被断言成功，则返回这条鱼的id的相反数；如果这一条鱼还没有被断言成功，则返回这条鱼的id。

可以直接访问**Game**中的变量来替代本函数：(pos为我方鱼的位置)

```c++
int my_id = game.my_id[pos].id;
```

**函数原型**

```c++
int get_my_id(int pos);
```

**使用示范**

```c++
int my_id = get_my_id(int pos);
```

### get_my_hp(int)

该函数的作用是获取我方指定位置的鱼的hp。

可以直接访问**Game**中的变量来替代本函数：(pos为我方鱼的位置)

```c++
int my_hp = game.my_id[pos].hp;
```

**函数原型**

```c++
int get_my_hp(int pos);
```

**使用示范**

```c++
int my_hp = get_my_hp(pos);
```

### get_my_atk(int)

该函数的作用是获取我方指定位置的鱼的攻击力。

可以直接访问**Game**中的变量来替代本函数：(pos为我方鱼的位置)

```c++
int my_atk = game.my_id[pos].atk;
```

**函数原型**

```c++
int get_my_atk(int);
```

**使用示范**

```c++
int my_atk = get_my_atd(pos);
```

### get_lowest_health_enemy()

该函数的作用是获取敌方血量最低的一条鱼。

返回值为一个**int**，代表敌方血量最少的鱼的位置。如果有多条敌方鱼是敌方血量最低的鱼，则返回位置编号最小的鱼。

**函数原型**

```c++
int get_lowest_health_enemy();
```

**使用示范**

```c++
// lowest_health_enemy是一个0~3的整数，代表敌方鱼的位置。
int lowest_health_enemy = get_lowest_health_enemy();
```

### get_lowest_health_enemies()

该函数的作用是获取敌方血量最低的所有鱼。

返回值为一个**std::vector<int\>**，里面储存了所有敌方血量最少的鱼的位置。

**函数原型**

```c++
std::vector<int> get_lowest_health_enemies();
```

**使用示范**

```c++
// lowest_health_enemies是一个vector，里面存储敌方所有血量最少的鱼。
std::vector<int> lowest_health_enemies = get_lowest_health_enemies();
```

### get_enemy_living_fishes()

该函数的作用是获取所有敌方当前存活的鱼。

返回值为一个**std::vector<int\>**，里面储存了所有敌方活着的鱼的位置。

**函数原型**

```c+
std::vector<int> get_enemy_living_fishes();
```

**使用示范**

```c++
// enemy_living_fishes是一个vector，里面储存敌方所有活着的鱼。
std::vector<int> enemy_living_fishes = get_enemy_living_fishes();
```

### get_my_living_fishes()

该函数的作用是获取所有我方当前存活的鱼。

返回值为一个**std::vector<int\>**，里面储存了所有我方活着的鱼的位置。

**函数原型**

```c++
std::vector<int> get_my_living_fishes();
```

**使用示范**

```c++
// my_living_fishes是一个vector，里面储存我方所有活着的鱼。
std::vector<int> my_living_fishes = get_my_living_fishes();
```

### get_my_living_ally(int)

该函数的作用是获取我方一条鱼的其余存活队友。

返回值为一个**std::vector<int\>**， 里面储存了传入的鱼之外的我方活着的鱼的位置。

与**get_my_living_fishes()**相比，这个函数需要额外指定一条友方鱼，获取其余的友方活着的鱼的位置。

**函数原型**

```c++
std::vector<int> get_my_living_ally(int pos);
```

**使用示范**

```c++
// 假设当前我需要发动位置为3的鱼的主动
int pos = 3;
// 当前这条鱼的主动需要选取一个存活的队友，所以我用my_living_ally来表示其余的存活队友
std::vector<int> my_living_ally = get_my_living_ally(pos);
```

### get_avatar_id()

该函数的作用是获取己方拟态鱼本轮模仿的鱼的编号（如果存在拟态鱼）。

**函数原型**

```c++
int get_avatar_id();
```

**使用示范**

```c++
// avatar_id是我方拟态鱼模仿的鱼的编号
int avatar_id = get_avatar_id();
```

### get_first_mover()

该函数的作用是获取己方这一轮是否是先手，如果是先手会得到 1。

**函数原型**

```c++
bool get_first_mover();
```

**使用示范**

```c++
if (get_first_mover()){
    ...  //  展开我的先手策略！
}
else{
    ...  //  可恶，启用备用方案！
}
```

### get_skill_used(int)

该函数的作用是获取自己的一条鱼的主动技能使用次数。

传入参数是己方鱼的位置，返回值是这条鱼的主动技能使用次数。

**函数原型**

```c++
int get_skill_used(int pos);
```

**使用示范**

```c++
// skill_used_times表示我方第3条鱼使用主动技能的次数
int pos = 3;
int skill_used_times = get_skill_used(pos);
```

### auto_valid_action(int, Action*)

入门神器！新手福音！

如果你不知道行动回合该进行什么操作的话，请使用这个函数来进行自动行动。

该函数的作用是自动为一个位置的鱼选取合法的行动目标，优先高概率使用主动技能，如果不可能合法则使用普通攻击。但是在这之前请先在**Action**中设置你的选鱼，否则会出现错误。

注意：长时间使用主动技能可能会让对手更容易地猜出你的鱼的配置，从而进行断言获得优势。

传入参数是一个**int**（代表鱼的位置）和一个**Action***（代表鱼的行动）。

**函数原型**

```c++
void auto_valid_action(int pos, Action *action);
```

**使用示范**

以一个最简单的行动作示范：

```c++
Action AI::Act(Game game)
{
  	Action action(game);
  	// 获取我这一回合行动的鱼。
  	int my_pos = (get_my_living_fishes())[0];
  	// 设置这一回合行动的鱼。
  	action.set_action_fish(my_pos);
  	// 自动行动
  	auto_valid_action(my_pos, &action);
  	return action;
}
```

## 预定义枚举变量

### fish_type

**fish_type**是为了让鱼的id更加直观而定义的一个枚举变量，其对应关系如下：

| 鱼的名称 | ID   | fish_type  |
| -------- | ---- | ---------- |
| 射水鱼   | 1    | spray      |
| 喷火鱼   | 2    | flame      |
| 电鳗     | 3    | eel        |
| 翻车鱼   | 4    | sunfish    |
| 梭子鱼   | 5    | barracuda  |
| 蝠鲼     | 6    | mobula     |
| 海龟     | 7    | turtle     |
| 章鱼     | 8    | octopus    |
| 大白鲨   | 9    | whiteshark |
| 锤头鲨   | 10   | hammerhead |
| 小丑鱼   | 11   | clownfish  |
| 拟态鱼   | 12   | imitator   |

使用**fish_type**可以替换对应的**ID**，比如下面的代码：

```c++
std::vector<int> chosen_fish;
chosen_fish.push_back(spray);
chosen_fish.push_back(sunfish);
chosen_fish.push_back(barracuda);
chosen_fish.push_back(imitator + turtle);
return chosen_fish;
```

这段代码表示的是选中射水鱼，翻车鱼，蝠鲼和拟态鱼(模仿海龟)出战。

### err_type

**err_type**定义了**Action**类中一些常见的错误操作，同时也是**Action**中**SET**类型函数的返回值。

| 错误类型         | Value | err_type                | 备注                                                        |
| ---------------- | ----- | ----------------------- | ----------------------------------------------------------- |
| 目标选取错误     | 0     | err_target              | 当鱼行动时选取了非法目标(比如选取己方鱼作为攻击对象)        |
| 目标位置错误     | 1     | err_target_out_of_range | 通过位置指定鱼的时候，pos不在`[0, 3]`内                     |
| 目标行动类型错误 | 2     | err_action_type         | set_action_type的时候参数值不为0或1                         |
| 选取的鱼ID错误   | 3     | err_action_fish         | 正常情况下不会出现该错误                                    |
| 未选定行动类型   | 4     | err_action_not_choose   | 未指定行动类型的时候调用set_enemy_target或set_friend_target |
| 未选取鱼         | 5     | err_fish_not_choose     | 未选取鱼的时候调用set_enemy_target或set_friend_target       |
| 成功             | 6     | success                 | 操作成功                                                    |

### passive_type

被动技能的类型，比较粗糙的归类，避免透露过多信息。

| `type`  | 描述     |
| ------- | -------- |
| counter | 膨胀反伤 |
| deflect | 承伤     |
| reduce  | 减伤     |
| heal    | 回血     |
| explode | 爆炸     |

### skill_type

主动行动的类型，比较粗糙的归类，避免透露过多信息。

| `type`       | 描述     |
| ------------ | -------- |
| aoe          | 群体伤害 |
| infight      | 伤害队友 |
| crit         | 暴击     |
| subtle       | 无作为   |
| normalattack | 普通攻击 |

## 预定义类

### Action

**Action**类用于定义鱼在一回合内的操作，是**Act(Game)**函数的返回值。

**Action**类默认实现了一些简单的操作正确性检查。如果出现了非法操作，则会以返回值的形式告诉选手错误类型，方便选手判断和修改。错误类型是**err_type**，[点击此处](#err_type)了解该枚举类型的详细信息。

#### 成员变量

##### int actionType(*private*)

操作类型。0代表普通攻击，1代表使用主动技能。初始值为-1。

##### int actionFish(*private*)

进行操作的鱼的位置(pos)。取值范围为`[0, 3]`。初始值为-1。

##### int enemyTarget(*private*)

选取的敌方目标鱼的位置(pos)。如果只选取一条鱼作为目标，则其值为位置，取值范围为`[0, 3]`；如果选取敌方所有鱼作为目标(aoe)，则其值为`-2`。初始值为-1。

##### int friendTarget(*private*)

选取的友方目标鱼的位置(pos)。取值范围为`[0, 3]`。初始值为-1。

##### Game game(*private*)

局面变量。用于协助判断操作正确性。

#### 成员函数

##### set_action_type(int)

设置操作类型**actionType**。如果参数值为0或1，将其赋值给**acitionType**并返回**success**，否则返回**err_action_type**。

##### set_action_fish(int)

设置行动鱼**actionFish**。如果参数值在区间`[0, 3]`内，将其赋值给**actionFish**并返回**success**，否则返回**err_target_out_of_range**。

##### set_enemy_target(int)

设置敌方目标**enemyTarget**。如果选取的行动为普通攻击，或者选取的行动为发动技能且需要指定敌方鱼为目标，则调用此函数。

如果传入参数在区间`[0, 3]`内，则检查当前鱼和行动类型是否支持当前操作；若否，则根据错误类型返回对应参数。如果选取了鱼且发动主动且鱼的主动是对敌方的aoe，则传入任意参数都将会把**enemyTarget**置为-2。

##### set_friend_target(int)

同上。但是由于没有对友方鱼的群体技能，所以传入参数区间限定在`[0, 3]`。

### ActionInfo

用于描述某一方在一个回合内进行的操作。

#### 成员变量

##### int action_fish(*public*)

行动的鱼的位置(pos)。初始值为-1。

##### int enemy_excepted_injury[](*public*\)

该行动玩家的敌方预期受到的伤害(与实际受到的伤害相对)。数组中数据顺序与**enemy_targets[]**一致。即假设enemy_expected_injury[0] = 50，enemy_targets[0] = 1，则代表行动玩家的敌方1号位置的鱼预期受到50点伤害。

##### int enemy_injury[](*public*\)

该行动玩家的敌方实际受到伤害。数组中数据顺序与**enemy_injury_id[]**一致。

##### int enemy_injury_id[](*public*\)

该行动玩家的敌方实际受到伤害的鱼的位置(pos)。数组中数据顺序与**enemy_injury[]**一致。

##### int enemy_injury_timestamp\[\](*public*)

该次伤害的时间戳，即结算过程中，该次伤害结算的次序（第几个进行结算的，从1开始标号，这个标号从小到大记录了敌我方被动技能、敌我方受到伤害事件的结算顺序）。数组中数据顺序与**enemy_injury[]**和**enemy_injury_id[]**一致。

##### int enemy_passive_id[](*public*\)

该行动玩家的敌方触发的被动的鱼的位置(pos)。数组中数据顺序与**enemy_passive_value[]**和**enemy_types[]**一致。

##### int enemy_passive_timestamp\[\](*public*)

该次被动的时间戳，即结算过程中，该次被动结算的次序（第几个进行结算的，从1开始标号，这个标号从小到大记录了敌我方被动技能、敌我方受到伤害事件的结算顺序）。数组中数据顺序与**enemy_passive_id[]**和**enemy_passive_value[]**，**enemy_types[]**一致。

##### bool enemy_injury_traceable\[\](*public*)

该次伤害是否可追踪，即该次伤害来源是否是由攻击和技能**直接**造成的伤害；如果是，则对应值为true，如果不是则为false。数组中数据顺序与**enemy_injury\[\]**和**enemy_injury_id\[\]**一致。

##### double enemy_passive_value[](*public*\)

如果该行动玩家的敌方触发的被动类型为减少伤害(**reduce**)，则其值为减伤比；

如果该行动玩家的敌方触发的被动类型为治疗(**heal**)，则其值为**实际治疗量**（考虑了血量上限），

否则其值无意义。数组中数据顺序与**enemy_passive_id[]**和**enemy_types[]**一致。

##### int enemy_targets[](*public*\)

该行动玩家选取的敌方鱼的位置(pos)。数组中数据顺序与**enemy_excepted_injury[]**一致。

##### passive_type enemy_types[](*public*\)

该行动玩家的敌方触发的被动类型。被动类型**passive_type**[点击这里](#passive_type)查看定义。数组中数据顺序与**enemy_passive_id[]**和**enemy_passive_value[]**一致。

##### int friend_excepted_injury[](*public*\)

该行动玩家的友方预期受到的伤害(与实际受到的伤害相对)。数组中数据顺序与**friend_targets[]**一致。即假设friend_expected_injury[0] = 50，friend_targets[0] = 1，则代表行动玩家的友方1号位置的鱼预期受到50点伤害。

##### int friend_injury[](*public*\)

该行动玩家的友方实际受到伤害。数组中数据顺序与**friend_injury_id[]**一致。

##### int friend_injury_id[](*public*\)

该行动玩家的友方实际受到伤害的鱼的位置(pos)。数组中数据顺序与**friend_injury[]**一致。

##### int friend_injury_timestamp\[\](*public*)

该次伤害的时间戳，即结算过程中，该次伤害结算的次序（第几个进行结算的，从1开始标号，这个标号从小到大记录了敌我方被动技能、敌我方受到伤害事件的结算顺序）。数组中数据顺序与**friend_injury[]**和**friend_injury_id[]**一致。

##### bool friend_injury_traceable\[\](*public*)

该次伤害是否可追踪，即该次伤害来源是否是由攻击和技能**直接**造成的伤害；如果是，则对应值为true，如果不是则为false。数组中数据顺序与**friend_injury\[\]**和**friend_injury_id\[\]**一致。

##### int friend_passive_id[](*public*\)

该行动玩家的友方触发的被动的鱼的位置(pos)。数组中数据顺序与**friend_passive_value[]**和**friend_types[]**一致。

##### int friend_passive_timestamp\[\](*public*)

该次被动的时间戳，即结算过程中，该次被动结算的次序（第几个进行结算的，从1开始标号，这个标号从小到大记录了敌我方被动技能、敌我方受到伤害事件的结算顺序）。数组中数据顺序与**friend_passive_id[]**和**friend_passive_value[]**，**friend_types[]**一致。

##### double friend_passive_value[](*public*\)

如果该行动玩家的友方触发的被动类型为减少伤害(**reduce**)，则其值为减伤比；

如果该行动玩家的友方触发的被动类型为治疗(**heal**)，则其值为**实际治疗量**（考虑了血量上限），

否则其值无意义。数组中数据顺序与**friend_passive_id[]**和**friend_types[]**一致。

##### int friend_targets[](*public*\)

该行动玩家选取的友方鱼的位置(pos)。数组中数据顺序与**friend_excepted_injury[]**一致。

##### passive_type friend_types[](*public*\)

该行动玩家的友方触发的被动类型。被动类型**passive_type**[点击这里](#passive_type)查看定义。数组中数据顺序与**friend_passive_id[]**和**friend_passive_value[]**一致。

##### bool is_skill(*public*)

该次行动是否使用了技能。true代表使用了技能，false代表使用了普通攻击。初始值为false。

##### int num_enemy_injury(*public*)

该行动玩家的敌方受伤鱼的数量。需要注意的是，该值为数组**enemy_injury[]**，**enemy_injury_id[]**的长度。

##### int num_enemy_targets(*public*)

该行动玩家选取的敌方目标个数。需要注意的是，该值为数组**enemy_targets[]**，**enemy_excepted_injury[]**的长度。初始值为0。

##### int num_enemy_passives(*public*)

该行动玩家的敌方鱼触发被动的个数。需要注意的是，该值为数组**enemy_passive_id[]**，**enemy_types[]**和**enemy_passive_value[]**的长度。初始值为0。

##### int num_friend_injury(*public*)

该行动玩家的友方受伤鱼的数量。需要注意的是，该值为数组**friend_injury[]**，**friend_injury_id[]**的长度。初始值为0。

##### int num_friend_targets(*public*)

该行动玩家选取的友方目标个数。需要注意的是，该值为数组**friend_targets[]**，**friend_excepted_injury[]**的长度。初始值为0。

##### int num_friend_passives(*public*)

该行动玩家的友方鱼触发被动的个数。需要注意的是，该值为数组**friend_passive_id[]**，**friend_types[]**和**friend_passive_value[]**的长度。初始值为0。

##### skill_type type(*public*)

如果行动类型为主动，则**type**表示该玩家发动的主动的类型。关于**skill_type**，[点击这里](#skill_type)查看具体定义。

#### 成员函数

##### bool is_empty()

判定当前Info是否为空，即上一回合玩家是否有操作。

### AssertInfo

用于描述某一方的断言操作和断言结果。

#### 成员变量

##### int assertContent(*public*)

断言鱼的ID。初始值为0。

**注意**：当对手上一回合没有进行断言的时候，该变量值为0。

##### int assertPos(*public*)

被断言鱼的位置(pos)。初始值为0。

**注意**：当对手上一回合没有进行断言的时候，该变量值为0。所以不要用该变量来判断对手上一回合是否进行了断言。

##### bool assertResult(*public*)

断言结果。true代表断言成功，false代表断言失败。初始值为false。

**注意**：当对手上一回合没有进行断言的时候，该变量值为false。

#### 成员函数

##### bool is_empty()

判定当前Info是否为空，即上一回合玩家是否有操作。

### Fish

用于描述一条鱼的信息。

#### 成员变量

##### int id(*public*)

鱼的id。如果是己方鱼，则id为其真实id；如果是敌方鱼，则如果这条鱼已经被断言成功，它的id是其真实id，否则是-1。

##### int hp(*public*)

鱼的血量。

##### int atk(*public*)

鱼的攻击力。如果是己方鱼，则该值为其实际攻击力；如果是敌方鱼，则这个值无意义。

### Game

描述当前游戏局面的类。

#### 成员变量

##### ActionInfo enemy_action(*public*)

描述了敌方的行动及其结果。关于**ActionInfo**，[点击这里](#actioninfo)查看其定义。

##### AssertInfo enemy_assert(*public*)

描述了敌方的断言及其结果。关于**AssertInfo**，[点击这里](#assertinfo)查看其定义。

##### Fish enemy_fish[](*public*\)

描述了敌方的四条鱼的状态。关于**Fish**，[点击这里](#fish)查看其定义。

##### ActionInfo my_action(*public*)

描述了我方上一回合的行动及其结果。关于**ActionInfo**，[点击这里](#actioninfo)查看其定义。

##### AssertInfo my_assert(*public*)

描述了我方上一回合的断言及其结果。关于**AssertInfo**，[点击这里](#assertinfo)查看其定义。

##### Fish my_fish[](*public*\)

描述了我方的四条鱼的现在的状态。关于**Fish**，[点击这里](#fish)查看其定义。

##### int round(*public*)

当前回合数，取值范围为`[1, 3]`。

##### int avatar_id(*public*)

当前回合己方拟态鱼模仿的鱼的id（如果没有的话是-1）。

##### bool first_mover(*public*)

当前回合我方是不是先手（如果是的话是true）。

##### bool round1_win(*public*)

描述第一局结果。如果第一局我方获胜，则该值为true，否则为false。

##### bool round2_win(*public*)

描述第二局结果，同上。

##### bool round3_win(*public*)

描述第三局结果，同上~~虽然完全没有用但为了满足强迫症我还是加上了~~。

##### int last_round_finish_reason(*public*)

描述上一轮结束原因和时刻。取值说明如下：

* 0：第一轮，没有上一轮的值。
* 1：我方断言后上一轮结束。
* 2：我方行动回合结束后上一轮结束。
* 3：敌方断言后上一轮结束。
* 4：敌方行动回合结束后上一轮结束。

##### bool state_limit_exceed(*public*)

上一轮结束原因是否是到达时间上限。

## STL简介

### std::vector

可以将其理解为一个可变长数组。完成AI只需要知道以下操作即可：

* **clear()**：清空vector。
* **push_back()**：在vector的尾部添加一个元素。
* **size()**：获取vector的长度。
* 访问vector的形式与访问数组的形式一样，使用中括号即可。

以下是操作示例：

```c++
std::vector<int> sample;    //定义一个存放类型为int的vector，可以将其类比为int sample[?]
sample.clear();             //清空sample内的元素。
sample.push_back(2);        //将2放在sample的尾部。此时sample内存放的数据为{2}
sample.push_back(3);        //将3放在sample的尾部。此时sample内存放的数据为{2, 3}
sample.size();              //取sample的大小。结果为2.
sample[1];                  //取sample[1]的值。结果为3.
```

### std::pair

可以将其理解为将两个基本数据捆绑在一起。比如**std::pair<int, int\>**就是将两个int类型的数据捆绑在一起。

不理解也没关系，只需要先计算出**Assert(Game)**函数中要求返回的两个数，假定其为**A**和**B**，则加入以下代码即可：

```c++
std::pair<int, int> Assert(Game game)
{
  	int A;
  	int B;
  	// Calculate the value of A and B
  	return std::make_pair(A, B);
}
```




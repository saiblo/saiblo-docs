# 鱼洋陷役快速入门教程

这个文档的目的是帮助选手可以快速上手游戏并提交有效AI。

[TOC]

## 零、游戏包介绍

游戏包内有若干文件夹和两篇文档。两篇文档分别是**必看文档**和本文档，所以这里介绍一下各个文件夹的内容。

* AI_SDK：AI文件夹。这一个文件夹是AI所在的文件夹，后文介绍如何编写AI代码时提到的**Action.cpp**和**Action.py**就在本文件夹下。
* logic：逻辑文件夹，游戏逻辑代码在该文件夹下。
* judger：评测文件夹。本地评测所需要的python文件在该文件夹下。

## 一、鱼洋陷役游戏规则

鱼洋陷役由两个玩家挑选角色、构建队伍、进行回合制对抗的SRPG游戏

本游戏中的角色库中共有12条鱼，所有鱼都有相同的攻击力和血量值，但有着不完全相同的技能组（包括一个主动技能和一个被动技能）。

双方玩家用12条鱼进行三轮比拼，每轮需要选择尚未被选择的4条鱼上场进行对战。三轮中获得两轮及以上胜利的玩家成为最终的胜者。

详细规则请参见[游戏规则](./rules.md)文档。



## 二、编写AI代码简易教程

### C++

**AI_SDK**是给选手提供的AI开发工具包（以下简称**SDK**），其中**Action.cpp**是唯一需要选手修改的文件，如果需要添加新的文件，请对应修改makefile并确保编译成功。

为帮助选手快速上手，我们提供了简易的**Action_sample.cpp**作为样例AI供选手参考

#### 我应该在哪里写

鱼洋陷役对战共有三种操作：**选鱼**，**断言**和**攻击**(**主动**)。这三个操作需要选手自行实现。

选手实现的**选鱼**的功能需要写在**Action.cpp**中的**Pick(Game)**函数里

```c++
std::vector<int> AI::Pick(Game game)
{
    // TODO: fill your code
}
```

选手实现的**断言**功能需要写在**Action.cpp**中的**Assert(Game)**函数里

```c++
std::pair<int, int> AI::Assert(Game game)
{
    // TODO: fill your code
}
```

选手实现的**攻击**(**主动**)功能需要写在**Action.cpp**中的**Act(Game)**函数里

```c++
Action AI::Act(Game game)
{
    // TODO: fill your code
}
```

有基础和有兴趣的选手可以自行查看sdk内的代码并改动，但是请自行确保修改后的正确性。

#### 我应该怎么写

下面将编写一个最简单的AI来展示AI的编写流程。

**【选鱼】**

在**Pick(Game)**函数中，需要做的事情是挑选这一轮需要出战的鱼。

查阅[SDK手册](./sdk.md)，可以调用函数**get_my_remain_fishes()**来知道还有哪些鱼能挑选。

一个**简易**的AI就直接选择返回的前4条鱼就OK了，在**Pick**函数编写代码如下：

```c++
// 获取剩余的鱼
std::vector<int> remain_fishes = get_my_remain_fishes();
// 挑选的鱼的vector
std::vector<int> pick_fish;
pick_fish.clear();
// 挑选前四条鱼
for(int i = 0; i < 4; i ++) {
  	// 特殊处理一下拟态鱼
  	int picked_fish_id = remain_fishes[i];
  	// 固定拟态鱼模仿射水鱼
  	if(picked_fish_id == 12) picked_fish_id = 13;
  	// 将挑选的鱼放进vector里
  	pick_fish.push_back(picked_fish_id);
}
return pick_fish;
```

**【断言】**

在**Assert(Game)**函数中，我需要做的事情是对敌方的一条鱼进行断言。

断言操作是可选的，最**简易**的AI当然是什么都不做，跳过断言阶段。

在**Assert**函数下编写代码如下：

```c++
return std::make_pair<-1, -1>;
```

**【行动】**

在**Act(Game)**函数中，需要做的事情是操作我的鱼进行进攻。

查阅[SDK手册](./sdk.md)，有一个函数叫做**auto_valid_action(int, Action*)**，可以进行简单的自动行动，在**Act**函数下编写代码如下：

```c++
// 创建Action类的实例action
Action action(game);
// 获取我能出战的鱼的位置
int my_pos = (get_my_living_fishes())[0];
// 设置我出战的鱼
action.set_action_fish(my_pos);
// 到我的回合了，自动行动！
auto_valid_action(my_pos, &action);
// 行动！
return action;
```

#### 总结

写出来的完整代码应该是长这样子的：

```c++
#include "Action.cpp"
#include <algorithm>

std::vector<int> AI::Pick(Game game)
{
  	// 获取剩余的鱼
    std::vector<int> my_remain_fishes = get_my_remain_fishes();
    // 挑选的鱼的vector
    std::vector<int> pick_fish;
    pick_fish.clear();
    // 挑选前四条鱼
    for(int i = 0; i < 4; i ++) {
        // 特殊处理一下拟态鱼
        int picked_fish_id = my_remain_fishes[i];
        // 固定拟态鱼模仿射水鱼
        if(picked_fish_id == 12) picked_fish_id = 13;
        // 将挑选的鱼放进vector里
        pick_fish.push_back(picked_fish_id);
    }
    return pick_fish;
}

std::pair<int, int> AI::Assert(Game game)
{
  	return std::make_pair<-1, -1>;
}

Action AI::Act(Game game)
{
  	// 创建Action类的实例action
    Action action(game);
    // 获取我能出战的鱼的位置
    int my_pos = (get_my_living_fishes())[0];
    // 设置我出战的鱼
    action.set_action_fish(my_pos);
    // 到我的回合了，自动行动！
    auto_valid_action(my_pos, &action);
    // 行动！
    return action;
}
```

可以发现这个简易的AI使用的固定的出阵规则，完全没有利用断言功能，行动也没有充分利用场面信息，是一个智商不是很高的AI。如果你想编写出更厉害的AI，请查阅[SDK手册](./sdk.md)获取更多接口信息，也可以直接查看AI_SDK下的源码。

但是至少我们已经成功编写了一个可以跑起来的AI了，在接下来的教程中，将教会大家如何进行本地测试和提交到评测网站进行对局。

### Python

**AI_SDK**是给选手提供的AI开发工具包（以下简称**SDK**），其中**Action.py**是唯一需要选手修改的文件。

为帮助选手快速上手，我们提供了简易的**Action_sample.py**作为样例AI供选手参考

#### 我应该在哪里写

鱼洋陷役对战共有三种操作：**选鱼**，**断言**和**攻击**(**主动**)。这三个操作需要选手自行实现。

选手实现的**选鱼**的功能需要写在**Action.py**中的**Pick(Game)**函数里

```python
def Pick(self: AI, game: Game) -> List[int]:
    # TODO: fill your code
    ...
```

选手实现的**断言**功能需要写在**Action.py**中的**Assert(Game)**函数里

```python
def Assert(self: AI, game: Game) -> Tuple[int, int]:
    # TODO: fill your code
    ...
```

选手实现的**攻击**(**主动**)功能需要写在**Action.py**中的**Act(Game)**函数里

```python
def Act(self: AI, game: Game) -> Action:
    # TODO: fill your code
    ...
```

有基础和有兴趣的选手可以自行查看sdk内的代码并改动，但是请自行确保修改后的正确性。

#### 我应该怎么写

下面将编写一个最简单的AI来展示AI的编写流程。

**【选鱼】**

在**Pick(Game)**函数中，需要做的事情是挑选这一轮需要出战的鱼。

查阅[SDK手册](./sdk.md)，可以调用函数**get_my_remain_fishes()**来知道还有哪些鱼能挑选。

一个**简易**的AI就直接选择返回的前4条鱼就OK了，在**Pick**函数编写代码如下：

```python
# 获取剩余的鱼
remain_fishes: List[int] = self.get_my_remain_fishes()
# 挑选的鱼的list
pick_fish = list(map( # 特殊处理一下拟态鱼
    lambda fish: 13 if fish == 12 else fish, # 固定拟态鱼模仿射水鱼
    remain_fishes))[:4] # 挑选前四条鱼
return pick_fish
```

**【断言】**

在**Assert(Game)**函数中，我需要做的事情是对敌方的一条鱼进行断言。

断言操作是可选的，最**简易**的AI当然是什么都不做，跳过断言阶段。

在**Assert**函数下编写代码如下：

```python
return (-1, -1)
```

**【行动】**

在**Act(Game)**函数中，需要做的事情是操作我的鱼进行进攻。

查阅[SDK手册](./sdk.md)，有一个函数叫做**auto_valid_action(int, Action)**，可以进行简单的自动行动，在**Act**函数下编写代码如下：

```python
# 创建Action类的实例action
action = Action(game)
# 获取我能出战的鱼的位置
my_pos: int = self.get_my_living_fishes()[0]
# 设置我出战的鱼
action.set_action_fish(my_pos)
# 到我的回合了，自动行动！
return self.auto_valid_action(my_pos, action)
```

#### 总结

写出来的完整代码应该是长这样子的：

```python
from typing import List, Tuple

from sdk.ai_client import Action, AIClient, Game


class AI(AIClient):
    def __init__(self) -> None:
        super().__init__()

    def Pick(self, game: Game) -> List[int]:
        # 获取剩余的鱼
        remain_fishes: List[int] = self.get_my_remain_fishes()
        # 挑选的鱼的list
        pick_fish = list(map( # 特殊处理一下拟态鱼
            lambda fish: 13 if fish == 12 else fish, # 固定拟态鱼模仿射水鱼
            remain_fishes))[:4] # 挑选前四条鱼
        return pick_fish

    def Assert(self, game: Game) -> Tuple[int, int]:
        return (-1, -1)

    def Act(self, game: Game) -> Action:
        # 创建Action类的实例action
        action = Action(game)
        # 获取我能出战的鱼的位置
        my_pos: int = self.get_my_living_fishes()[0]
        # 设置我出战的鱼
        action.set_action_fish(my_pos)
        # 到我的回合了，自动行动！
        return self.auto_valid_action(my_pos, action)
```

可以发现这个简易的AI使用的固定的出阵规则，完全没有利用断言功能，行动也没有充分利用场面信息，是一个智商不是很高的AI。如果你想编写出更厉害的AI，请查阅[SDK手册](./sdk.md)获取更多接口信息，也可以直接查看AI_SDK下的源码。

但是至少我们已经成功编写了一个可以跑起来的AI了，在接下来的教程中，将教会大家如何进行本地测试和提交到评测网站进行对局。



## 三、本地编译

我们提供了Makefile文件来帮助大家把AI和逻辑编译成可执行文件。首先先介绍一下需要用到的工具：

**g++**：g++是编译c++文件的编译器。

**make**：make是协助编译的一种工具。

* 对于**MacOS**系统而言，需要安装Xcode的**Command Line Tools**工具。首先在终端里运行`g++ -v`，如果出现版本信息则说明电脑上已经安装了g++；再在终端里运行`make`，如果提示没有目标则说明电脑上已经安装了make。否则，在Xcode的prefrence->components里找到**Command Line Tools**，点击安装即可。
* 对于**Linux**而言，运行指令`sudo apt-get install g++ make -y`即可安装g++和make。
* 对于Windows系统的选手而言，为了能够使用g++指令，需要使用**MinGW**工具进行安装。由于安装的教程长度比较长，所以请查阅游戏包中 Windows下的g++和make安装指导 文档获取安装教程。

**CMake**：CMake 是**只在**编译 Python AI SDK 使用的动态链接库时需要的工具。可以直接到 [CMake 官网](https://cmake.org/)下载安装。

介绍完工具，下面介绍怎么进行编译。

### AI编译

#### C++

##### Linux/MacOS

打开终端(terminal)，进入到AI_SDK目录下（本目录下有一个名叫Makefile的文件，同时也有main.cpp，Action.cpp等文件），输入make，回车即可。在AI_SDK/C++/目录下可以找到生成的名为**main**的AI。

##### Windows

进入到AI_SDK目录下（本目录下有一个名叫Makefile的文件，同时也有main.cpp，Action.cpp等文件），在上方的文件栏里输入**cmd**，按下回车就会弹出一个命令行窗口。在窗口里输入“make”，按下回车即可。在AI_SDK/C++/目录下可以找到生成的名为**main**的AI。

**注意**：对于 Windows 用户，如果提示 make 命令不存在，则可能需要将命令修改为 mingw32-make 后再次尝试。

#### Python

Python 版本的 AI SDK 是通过 pybind11 让 Python 调用 C++ 动态链接库实现的，因此您需要先将相关的 C++ 代码编译。在游戏包的 AI_SDK/Python/sdk/pyd/ 目录下，我们已经提供了预先编译好的适用于 Python 3.8 版本的动态链接库，您可以先尝试使用它们，如果无法 `import` 再自行编译。

##### 依赖库

需要安装 pybind11 2.6.x，例如使用 pip，在终端输入：

```bash
pip install pybind11
```

而后，您需要将游戏包 AI_SDK/Python/sdk/CMakeLists.txt 的第 16 行修改为包含 pybind11Config.cmake 的目录。如果您是使用 pip 安装的话，可以通过命令：

```bash
pip show pybind11
```

查看 pybind11 的安装目录，而后直接将 `<pybind11-install-path>` 改为对应的目录即可。

##### Windows

由于 Windows 版本的 Python 是用 MSVC 编译的，因此也需要使用 MSVC 编译供 Python 调用的 C++ 动态链接库。如果您的 Python 不是 MSVC 编译的，请按照 Linux 版本的教程进行编译。

在目录 AI_SDK/Python/sdk/ 下打开终端：

```bash
cmake . -B build
```

而后输入：

```bash
devenv build/py_ai_sdk.sln /Build release
```

如果提示命令不存在，且您**安装了 Visual Studio**，则您需要将 devenv.com 所在目录添加至 PATH 环境变量。devenv.com 应该在形如：

```bash
C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\
```

的目录下。关于如何将一个目录添加至 PATH 环境变量，请自行查阅有关资料。

编译好的动态链接库应该在 AI_SDK/Python/sdk/build/bin/Release/ 下，为 .pyd 文件。

##### Linux 或 macOS

在目录 AI_SDK/Python/sdk/ 下打开终端，输入：

```bash
cmake . -B build
cd build
make
```

编译好的动态链接库应该在 AI_SDK/Python/sdk/build/bin/ 下，为 .so 文件。

**注意**：如果您的电脑中存在多个版本的 Python，请确保 CMake 找到的 Python 和运行 judger.py 的版本相同。如果 CMake 找到了错误的版本，可以将第 14 行的 “#” 去除，将 `<Python-version>` 替换为您运行 judger.py 的 Python 版本；如果还有问题，可以考虑去除第 13 行的 “#”，将 `<Python-path>` 替换为 Python 的安装目录。

### 逻辑编译

逻辑编译同 C++ AI 的编译，只是目录不一样。

进入到文件包中的logic/目录下，在这个目录里打开终端/cmd，运行make指令即可编译逻辑。编译成功的逻辑在文件包中的logic/bin/目录下，名为**main**。

## 四、本地测试

使用本地测试功能需要先安装python3。没有python3的选手可以参考[菜鸟教程](https://www.runoob.com/python3/python3-install.html)来安装python3。

同时，judger的使用还需要pip3安装一些内容，所以还需要安装pip3。没有pip3的选手可以参考[菜鸟教程](https://www.runoob.com/w3cnote/python-pip-install-usage.html)来安装pip3。

### 环境说明

- Python 版本：建议版本为 3.7.6 及以上。

- 依赖库：judger.py 依赖 websockets 库。以使用 pip 安装为例，在终端输入：

```bash
pip3 install websockets
```

### 使用说明

judger.py 在游戏包的 judger/ 目录下，供选手进行简单的本地测试，提供了简陋的本地人人、人机和机机对战功能。

需要**注意**的是，为了提高评测的安全性和选手的可调试性，以及出于性能方面的考虑，网站评测端进行了从架构到代码各方面的彻底重构，只兼容了 judger.py 的接口和通信协议。由于本地环境和网站评测环境可能存在差异，且 judger.py 和网站评测在输入输出限制和空间限制等方面略有不同，因此通过了 judger.py 的测试并不能代表提交到网站上就绝对没有问题。但是使用 judger.py 进行测试可以帮助您在提交到网站上之前发现绝大多数的错误。

#### 编译游戏逻辑

您需要先将游戏包中 logic/ 目录下的游戏逻辑模块编译为可执行文件，流程和编译 AI 的完全一样，请参考编译部分的教程，这里不再赘述。

#### AI 和 AI 对战

在 judger.py 所在目录下（interactive_judger/）使用命令行参数启动，格式如下：

```bash
python judger.py <游戏逻辑的可执行文件路径> <0号AI启动命令> <1号AI启动命令> <配置信息> <回放文件输出路径>
```

其中：

- 文件路径应为绝对路径或相对于 judger.py 的路径。

- AI 启动指令：

    - 若 AI 为 Python 脚本，则命令为将普通的运行命令中空格替换为“+”号后的命令。例如，原本是```python ../my_ai/main.py```，应输入```python+../my_ai/main.py```。

    - 若 AI 为可执行文件，则启动命令为该可执行文件的路径。

- 配置信息：这是一个暂时废弃的参数，随便输入一个字符串即可，如 `config`.

- 回放文件输出路径：根据您的需要指定即可。请**注意**，回放文件是 JSON 格式，因此最好将文件名指定为 *.json.

一个可行的示例如下：

```bash
python judger.py ../logic/bin/main.exe python+../ai_0/main.py ../ai_1/main.exe config ../replay.json
```

其中，0 号 AI 为从 ../ai_0/main.py 启动的 Python 脚本，1 号 AI 为一个可执行文件，其相对于 judger.py 的路径为 ../ai_1/main.exe.

运行命令后，终端会输出一些含有 `type` 和 `success` 之类的信息，如：

```plaintext
&{"type": 4, "success": 1, "index": -1}
```

这是软件工程单元测试时使用的一些废弃协议，您可以忽略。

在对战结束后，终端会有关于玩家分数的输出，例如：

```json
{   
    "0": 100,
    "1": 0
}
```

有关分数的定义，请参考游戏规则的说明文档。

在对战结束后，您可在先前指定的路径下找到回放文件，可以使用播放器进行对局回放，详见播放器的使用说明文档。此外，如果你在ai中调用了**debug_msg**函数，则judger.py 所在目录下还会有你输出信息的文本记录**ai_debug_info.txt**用于调试。

#### 测试模式

在 judger.py 所在目录下打开终端：

```bash
python judger.py test_mode
```

将进入较简陋的用于测试的测试模式，您可以输入 `help` 来查看可以使用的命令。

1. 启动 AI：

   ```
   0 <玩家ID> <AI启动指令>
   ```

   其中 `玩家ID` 为 0 或 1，即先后手，对应于前一节的 AI 编号；`<AI启动指令>` 与前一节中的格式相同。一个示例：

   ```
   0 0 python+../ai_0/main.py
   0 1 ../ai_1/main.exe
   ```

2. 添加人类玩家：

   ```
   1 <玩家ID> <IP地址> <端口号> <房间号>
   ```

   其中`玩家ID`为 0 或 1，与上一节描述相同。对于本地测试而言，`IP地址`填`127.0.0.1`，`端口号`为一个整数，须确保没有被其他进程占用，`房间号`是废弃的协议，随便输入个数字即可。

   一个样例指令如下：

   ```
   1 1 127.0.0.1 9010 1
   ```

   之后打开您的播放器, 在“在线对局”模式下输入

   ```
   <IP地址>:<端口号>/<房间号>/<玩家名称>/<玩家ID>
   ```

   其中，`IP地址`、`端口号`、`房间号`、`玩家ID`与之前所描述的相同，`玩家名称`随便填入一个字符串即可。

   一个可行的样例是（对应上面的指令）：

   ```
   127.0.0.1:9010/1/Aglove/1
   ```

   在输入token后点击确定，播放器会提示你“连接成功，等待对手中”。

   在双方玩家无论是AI还是人类都已经启动或者连接成功后，进入下一环节。

3. 开始对战：

   ```
   4 <游戏逻辑的可执行文件路径> <配置信息> <回放文件输出路径>
   ```

   其中的命令与前一节中的格式相同。一个示例：

   ```
   4 ../logic/bin/main.exe config ../replay.json
   ```

4. 对战结束：judger.py 的行为与前一节中的相同。

**注意**：测试模式下，命令的格式检查比较严格，参数间若含有多个空格，可能会引发错误。

#### 关于输出调试

judger.py 将 AI 和游戏逻辑的标准输入输出都进行了重定向，因此不建议您使用标准输入输出进行调试。几种可行的输出调试方案如下：

- 将调试信息输出到文件。我们建议您在每次输出后及时进行 flush，以避免当 AI 进程结束后仍有部分消息在缓冲区中而没有写入文件。
- 修改 judger.py. 由于 judger.py 的标准输入输出没有被重定向，因此您可以在其中添加一些语句以输出可供您调试使用的信息。如果您不想仔细研究 judger.py 的代码，可以只关注 `Player` 类的 `write` 方法和 `Judger` 类的 `write` 方法，分别对应着游戏逻辑向 AI 发送的消息和 AI 向游戏逻辑发送的消息。
- 修改游戏逻辑的代码。您可以在游戏逻辑中添加一些语句，将一些变量输出到文件中（**注意**：游戏逻辑的标准输入输出将被重定向）。由于游戏逻辑的代码文件较多，您可以只关注 aqua_sdk.cpp 中的内容。

## 五、播放器

播放器是用来将对局可视化的一个工具。AI对战生成的回放文件可以放进播放器里进行播放，具体内容参考游戏包中**doc**目录下的**附录四 播放器使用说明**。

## 六、代码提交与AI对战

选手测试完AI之后，将需要把代码提交到[saiblo](https://www.saiblo.net)上。关于如何提交代码到saiblo上和如何使用AI进行对战，请参考游戏包中**doc**目录下的**附录五 AI的提交与对战**。

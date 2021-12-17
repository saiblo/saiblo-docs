# 如何快速写一个Python AI



## 我应该从哪里开始写代码？

在下发的AI文件夹中，我们需要在文件 AI_client 的 play 函数中编写我们AI每回合的操作。



## 我可以用哪些接口？

样例AI中，使用了：

self.get_my_pos()

这是最基础的，获取当前玩家位置

self.get_neighbors(pos)

通过这个函数，你可以获得与pos相邻的节点

self.get_my_num()

获得我的编号一般来说刚开始调一次就够了，但是我懒得记（X

self.get_other_pos(int) 

获取别人的位置，注意不能将自己的id输入

self.get_my_hp()

获得我的血量

self.get_other_hp(int)

获得别人的血量（不要说我没警告你不能传什么）

self.view_box("Box","Key")

self.view_box("Materials","Kit")

上面两个分别代表掉落物和物资点的拾取，实际游戏中还有更多玩法等你发现

self.use_tool("Kit")

拿了东西不用怎么行

self.interact("KeyMachine")

从钥匙机获取钥匙，十分有用（除非你是个舔包高手/杀人狂魔）

self.interact("EscapeCapsule",1)

开始逃咯！！！

self.move(pos)

移动，如果不调这个函数也能赢，请联系游戏开发者

其余接口见python AI SDK文档。



## 我该如何快速地写出一个样例AI？

打开样例ai，找到play以及上面的test_move、add、bfs_move，然后复制粘贴就行了（X

## 我应该如何编译测试本地AI？

这是python AI，不用编译的啦。但可以在本地配合judger进行测试，见judger说明文档。



## 我应该如何提交AI？

接下来就可以按照saiblo上的指示进行了。

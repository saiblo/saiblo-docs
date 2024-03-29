随网络学堂附件提供给同学们的还有一个 Compete 项目，与 Strategy 在同级目录。该项目实际上是一个非界面、命令行式的对抗平台，你不需要关心这里面的代码。Compete 可执行程序允许你将两个策略文件进行指定轮数的对抗，并给出结果统计，避免你在图形界面上频繁的点鼠标。

具体使用方法可参见该项目下的 `readme.txt`。

## 基本用法

首先编译自己的 Strategy 项目，得到自己的策略文件。

- Windows 系统：经编译后生成的名为 `Strategy.dll` 的文件即为包含你的策略的策略文件（如果使用 Visual Studio 开发，该文件在 `Release` 或 `Debug` 目录内）。
- Mac 系统：经编译后生成的名为 `libStrategy.dylib` 的文件即为包含你的策略的策略文件（如果使用 Xcode 开发，该文件在 `Products` 下的 `Release` 或 `Debug` 目录内）。
- Linux 系统：经编译后生成的名为 `Strategy.so` 的文件即为包含你的策略的策略文件。

然后调用 Compete 项目下的可执行文件进行对抗测试。

## 批量测试

如果你希望进行本地批量测试，作业附件的 Batch 目录下提供了 Windows、Mac 和 Linux 系统各自的批量编译、对抗脚本，需要将脚本中的待编译文件及编译器路径修改为本地目录，同时指定对抗轮数等参数。具体使用方法见各自系统目录下 `readme.txt` 文件。

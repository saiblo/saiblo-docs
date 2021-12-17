# 比赛SDK和题面下载

## 题面和评测逻辑

### 题面下载

点击[链接](https://cdn.jsdelivr.net/gh/ssast-tech/thuai-egg-releases@d51d1b8a/statements.pdf)查看题面

### 评测逻辑文件下载

`judger.py` 可在上述文件列表下载`judger.zip`后解压得到。请不要改变解压后的文件夹结构。这里以使用Python SDK为例，介绍评测的基本方法。
**注意：解压judger后请安装python包websockets。**
运行命令：
```bash
pip install websockets
```

1. 下载下面文件中和自己系统对应的评测逻辑，并且重命名为 `eggs.exe` 或者 `eggs`
2. 把评测逻辑，`judger.py`都放到含有`main.py`的文件夹
3. 在上述文件夹打开一个命令行窗口，然后采用命令 `python ./judger.py eggs.exe python+main.py python+main.py python+main.py replay.bin` 以在 Windows 下进行评测。这条命令会让3个队伍都使用你的AI进行对战。

不过，我们更推荐采用Saiblo进行在线评测，这样和别的AI的对战也会更简单。

出于安全考虑，评测逻辑只提供构建好的版本，可在 [https://github.com/ssast-tech/thuai-egg-releases](https://github.com/ssast-tech/thuai-egg-releases) 进行下载，也可以直接采用下列链接。下载后即可采用 judger.py 和评测逻辑文件配合，进行测试。

- [Windows x86_64](https://cdn.jsdelivr.net/gh/ssast-tech/thuai-egg-releases@d51d1b8a/eggs-windows-x86_64)
    - 注：下载Windows版本后请自行添加.exe后缀
    - Windows版本存在各种问题，目前暂时不建议使用，建议使用Ubuntu版本
- [Ubuntu 16.04 x86_64](https://cdn.jsdelivr.net/gh/ssast-tech/thuai-egg-releases@d51d1b8a/eggs-ubuntu-16.04-x86_64)
    - 注：Linux版本下载后需要手动授予可执行权限
- [Mac OS 10.15 x86_64](https://cdn.jsdelivr.net/gh/ssast-tech/thuai-egg-releases@d51d1b8a/eggs-macos-10.15-x86_64)
    - 注：Mac版本下载后需要手动授予可执行权限
    - 还可能需要设置允许运行未知来源的可执行文件

## SDK

上述列表中所列的文件即为比赛所用SDK，点击即可下载。

其中，以cpp开头的是C++ SDK，以py开头的是Python SDK，结尾为SDK版本号。

注意：Python SDK 需要采用 `pip` 安装 `json_stream_parser` 包

SDK文档位于压缩包内。

## 回放文件

对于回放文件的解析，可以采用基于 Node.js 的回放文件读取SDK.

使用方法如下：

1. 安装 Node.js
2. 克隆代码仓库：
```bash
git clone https://github.com/ssast-tech/thuai-egg-record-sdk
```
3. `cd thuai-egg-record-sdk/js_record_sdk`
4. 安装依赖并构建运行：
```bash
npm install -g yarn
yarn
yarn build
yarn dist "二进制回放文件的路径"
```
此后，JSON格式的回放文件内容即被输出到终端。

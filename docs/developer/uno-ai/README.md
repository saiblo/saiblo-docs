## Introduction

​	This is a sample AI of uno game.



## AI SDK

​	代码```src\ai_client.h```提供了一份供AI重写的SDK，它可以让AI与裁判程序进行通信。

​	选手在实现代码时，应继承```AiClient```类并重写```player_AI()```函数，在该函数中进行AI每回合的决策。在这个类中，选手可以：

* 调用```get_my_card()```来获取当前的手牌；
* 调用```get_top_card()```来获取牌顶的牌；
* 调用```get_current_color()```来获取当前指定的颜色；
* 调用```send_my_card(int card, std::string color)```来打出自己的手牌，其中card为你要打出的牌的编号，这个编号从0开始，它应对应着```get_top_card()```获取到的序列中的牌。

​    重写```AiClient```类后，选手应在```main()```函数内```new```一个自己所重写的```AiClient```对象，并调用$run()$函数，换句话说，```main()```函数应当是这样的：

```cpp
int main() {
    AiClient* client = new MyClient();
    client->run();
    delete client;
    return 0;
}
```

而无需做其他操作。



## AI通信

​	AI与Judger的每一条通信均需要满足如下格式：

​	信息的头四个字节$c_1\dots c_4$表示了接下来要传输的信息的字节数，为$len=\sum_{k=1}^4 c_k2^{32-k}$；接下来$len$个字节即为真正需要传递的信息。

​	AI接收到的真正信息为一个JSON：

```json
{
    "color": color, // color 为一字符串
    "top": top_card, // top_card 为一字符串
    "card": [card1, card2, ...], // card1, card2, ...为一系列字符串
    "state": state, // state 为一整数
}
```

​	AI真正发送的消息也为一个JSON：

```json
{
    "card": card, // card为一整数
    "color": color, // color为一字符串，可能为空
    "state": state, // state为一整数
    "count": count, // count为一整数，调试用变量
}
```

​	代码```src\ai_client.h``` 提供了一份AI与Judger通信的SDK，它封装了一些通信的函数。

```cpp

/*
	发送信息s之前，先发送信息s的长度。
*/
void sendLen(string s); 

/*
	发送信息s。
	在这个函数中会调用sendLen(s)。
*/
void sendrecv_msg(string s);

/*
	阻塞监听judger发送的消息。
	会先读取头四个字节获得信息长度后，再持续读对应长度的信息。
*/
void listen(string s);

/*
	获取本回合指定的颜色。
*/
string get_current_color();

/*
	获取本回合牌顶的牌。
*/
string get_top_card();

/*
	获取本回合自己的手牌。
*/
string get_my_card();

/*
	将自己的手牌打出去，如果有指定颜色，则也发送该颜色。
*/
void send_my_card(int card, string color="");

/*
	AI需要重写的部分，执行AI的决策
*/
void player_AI();
```


# 游戏逻辑和judger通信示例

游戏逻辑和judger通过stdio进行通信

游戏逻辑可能需要修改的部分如下：

```C++
/*
    Copyright [2019] <OneMorePigeon>
*/
#include <iostream>
#include <string>
#include "./deck.h"
#include "./player.h"
#include "./card.h"
#include "./game.h"

std::string op[1000];
void start_turn();

void sendLen(string s){
	int len = s.length();
	unsigned char lenb[4];
	lenb[0] = (unsigned char)(len);
	lenb[1] = (unsigned char)(len >> 8);
	lenb[2] = (unsigned char)(len >> 16);
	lenb[3] = (unsigned char)(len >> 24);
	for(int i = 0; i < 4; i++)printf("%c", lenb[3 - i]);
}

void sendMsg(string s){
	sendLen(s);
	cout << s;
	cout.flush();
}

// 输入形式形如 ./main num , 其中num为一个正整数
int main(int argc, char* argv[]) { 
	/*
		控制器启动的命令行参数可能会更多,例如每个玩家的天梯分数等等
		需要和控制器沟通
	*/
    int n = atoi(argv[1]);
    game g(n);  // 开启新局
  	/*std::cout << "Start Game!" << std::endl;    
    for (int i = 0; i <n; i++)
        std::cout << g.players[i];
    std::cout.flush();*/
    /* 
        回合制判定逻辑
        输入n行, 第i行表示i选手这回合选择出的牌。
        每一行的形式如 'rank [options]'
        rank 为选手选择要打的牌编号
        options为特殊附加(变色卡的目标颜色), 只有在出了颜色为none的卡牌才有效
        若未输入options, 则视为随机选颜色。
        注意, 只有在是该回合可出牌的选手才会真正进行回合卡牌判定
        若超时、空行视为本回合没行动。
        若没行动、编号超出牌组范围、输入不合法、没牌可出, 则抽取一张罚牌
    */
    while (!g.end_game)
    {
        /*std::cout << "Current Card:" << std::endl << g.cur_card << std::endl
                  << "Current Turn:" << std::endl << g.cur_turn << std::endl
                  << "Start Turn" << std::endl;
        std::cout.flush();*/
    	/*
    		此处应为发送消息给judger
    		通信格式: json
    		通信内容: 回合数
    				 需要发送消息的选手id列表
					 按照上面的列表依次需要发送的数据包(uno应该是最后一张打出的牌和选手手里的牌)
					 本回合需要行动的选手id列表(就是judger需要发送操作给逻辑的选手id列表)
    	*/
        /*for (int i = 0; i < n; i++)
            std::getline(std::cin,op[i]);
        */
        /*
        	此处应为接收judger的消息
        	因为一个回合可能会有多个选手做出相应,judger每接收一个选手的操作就会把消息转发给逻辑
        	逻辑可能会接收多次消息
      		逻辑需要对选手发过来的数据包做格式限制以便于解析
        	通信格式: json
        	通信内容: 回合数
        	         选手id
        	         数据包
        */

        g.start_turn(op);
        /*std::cout << "End Turn" << std::endl;
        for (int i = 0; i <n; i++)
            std::cout << g.players[i];
        std::cout.flush();*/
    }
    //std::cout << "player " << g.winner << " wins !" << std::endl;
    /*
    	此处应为返回局面结束信息
    	通信格式: json
    	通信内容: 游戏结束标志符
    			 选手id列表
    	         按照上面的列表依次给出选手的天梯分数增量
    */
    
    return 0;
}
```

为了解决缓冲区信息粘包的问题，需要在json数据前用四个byte的长度标明数据包的大小，C++的写法可以参考如下：

```C++
void sendLen(string s){
	/*
		发送消息前需要先发送长度
		调用sendLen函数
	*/
	int len = s.length();
	unsigned char lenb[4];
	lenb[0] = (unsigned char)(len);
	lenb[1] = (unsigned char)(len >> 8);
	lenb[2] = (unsigned char)(len >> 16);
	lenb[3] = (unsigned char)(len >> 24);
	for(int i = 0; i < 4; i++)printf("%c", lenb[3 - i]);
}

void sendMsg(string s){
	/*
		发送消息
	*/
	sendLen(s);
	cout << s;
	cout.flush();
}
```

python的写法可以参考如下:

```python
def convert_byte(self, str):
	Len = len(str)
	message = Len.to_bytes(4, byteorder='big', signed=True)
  message += bytes(str, encoding="utf8")
  return message
```

同样的，在接收数据的时候，也需要先读入数据包的长度，然后根据长度读入数据包。C++的参考写法如下：

```C++
void listen(){
	/*
		监听直到收到judger的消息
	*/
	char *msg = new char[1024];//足够长的大小，或者可以根据数据包的大小动态开辟
	char lenr[4];
	while(true){
		scanf("%4c", lenr);
		int len = (unsigned int)((((unsigned int)lenr[3]) & 255) | ((((unsigned int)lenr[2]) & 255) << 8) | ((((unsigned int)lenr[1]) & 255) << 16) | ((((unsigned int)lenr[0]) & 255) << 24));
		fgets(msg, len + 1, stdin);
	}return;
}
```

python的参考写法如下：

```python
read_buffer = self.subpro.stdout.buffer
while True:
	Len = int.from_bytes(read_buffer.read(4), byteorder='big', signed=True)
	data = read_buffer.read(Len)
```

此外，C++压成json字符串不建议手写，推荐使用现有的开源库如jsoncpp等
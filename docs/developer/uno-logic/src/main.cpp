/*
    Copyright [2019] <OneMorePigeon>
    Created by choumartin1234
*/

#include "uno_sdk.cpp"   // NOLINT


// 输入形式形如 ./main num , 其中num为一个正整数
int main(int argc, char* argv[]) {
    /*
        控制器启动的命令行参数可能会更多,例如每个玩家的天梯分数等等
        需要和控制器沟通
    */
    int n;
    if (argc >= 2)
        n = atoi(argv[1]);
    else
        n = 5;
    UnoSDK g(n);
    g.start();
    g.run();

    return 0;
}

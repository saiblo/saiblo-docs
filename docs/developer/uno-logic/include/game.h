/*
    Copyright [2019] <OneMorePigeon>
    This is the definition of "games" class for game Uno
*/

#pragma once
#include <vector>
#include <string>
#include "deck.h"
#include "player.h"

class game {
 public:
    explicit game(int _num);
    game();
    bool end_game = false;
    bool reversed = false;  // 打牌顺序, false为++turn, true则--turn
    std::vector<player> players;  // 游戏中的玩家序列
    int player_num;  // 玩家总数
    int cur_turn;  // 当前玩家的编号
    int winner = -1;  // 获胜玩家的编号
    int state = 1;  // 当前回合
    int plus_card = 0;  // 叠加的罚牌数量
    card cur_card;  // 游戏当前最上方的卡牌, 决定规则
    void give_card(int i, int n = 1);  // give num cards to player no. i
    card draw_card();  // 从卡牌上方抽出一张卡
    void start_turn(std::string op[]);  // 开始一个新回合
    bool judge_valid(std::string op[]);  // 为播放器判断操作是否合法
    bool check_deck();  // 牌不足的时后从废牌区补牌
    void judge_rule(std::string opt);  // 判断玩家打出该牌造成的影响
    void end_turn(int n = 1);  // 结束n个回合, cur_turn随reverse变化
 private:
    deck game_deck;  // 剩余牌堆
    deck used_deck;  // 弃牌堆
};

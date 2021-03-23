/*
    Copyright [2019] <OneMorePigeon>
    This is the implementation of "game" class for game Uno
*/
#include "../include/game.h"
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <random>
#include <cstdio>

// 随机生成引擎
std::default_random_engine e;

game::game() {
}

game::game(int _num):player_num(_num) {  // start a new game with _num players
    e.seed((unsigned)time(nullptr));  // random seed for shuffle
    // initial deck
    game_deck.clear();
    for (int col = 1; col <= 4; col++)
        for (int num = 0; num <= 12; num++) {
            int k = (num == 0? 1 : 2);  // 0号牌只有1张, 其余2张
            while (k--) {
                card temp(COLOR(col), num);
                game_deck.add(temp);
            }
        }
    for (int i = 1; i <= 4; i++) {
        card wild(COLOR(0), 13);  // 无色牌, 共有4张
        card wild_plus_four(COLOR(0), 14);  // 无色+4卡牌, 共有4张
        game_deck.add(wild);
        game_deck.add(wild_plus_four);
    }
    game_deck.shuffle();
    used_deck.clear();
    // distribute card;
    for (int i = 0; i < _num; i++) {  // 新增选手, 打印信息
        // std::cout << "add players\n";
        player temp(i);
        players.push_back(temp);
        // std::cout << "player " << i << " added\n";
        give_card(i, 7);
    }
    cur_turn = e() % _num;
    cur_card = draw_card();
}

void game::give_card(int i, int n) {  // 发n张牌给第i个选手
    for (int j = 1; j <= n; j++) {
        auto temp = draw_card();
        if (temp.get_num() < 0)
            break;
        players[i].receive_card(temp);
    }
}

bool game::check_deck() {
    if (game_deck.empty() && used_deck.empty())
        return true;
    if (game_deck.empty()) {
        game_deck.move_to_deck(used_deck);
        used_deck.clear();
        game_deck.shuffle();
    }
    return false;
}

card game::draw_card() {  // 从卡牌上方抽出一张卡
    if (check_deck()) {
        end_game = true;
        return card();
    }
    auto _card = game_deck.back();
    game_deck.pop();
    return _card;
}

/* 
    回合制判定逻辑
    op[i] 代表第i位选手的操作,形如 'rank [options]'
    注意, 只有在是该回合可出牌的选手才会真正进行回合卡牌判定
    若没行动、输入不合法、找不到指定牌、没牌可出, 则抽取一张罚牌
    输出：当前牌顶的牌, 以及下一位选手的编号。
*/

bool game::judge_valid(std::string op[]) {
    for (int i = 0; i < player_num; i++)
        if (i == cur_turn) {
            if (op[i] == "") {
                // std::cout << "No move!";
                return false;
            }
            std::string str = "", opt = "";
            int j = 0;
            while (j < op[i].length() && op[i][j] != ' ') {
                str += op[i][j];
                j++;
            }
            int num;
            try {
                num = stoi(str);
            }
            catch(std::invalid_argument&) {
                // std::cout << "Input Error!";  // 输入非数值
                return false;
            }
            if (j < op[i].length())
                opt = std::string(op[i], j+1, op[i].length());
            if (num >= players[i].size() || num < 0) {  // 输入秩不合法
                return false;
            }
            auto c = players[i].my_card.get_list()[num];
            if (c.get_color() == 0 ||                       // 打出无色牌
                c.get_color() == cur_card.get_color() ||    // 打出同色牌
                c.get_num() == cur_card.get_num() ||        // 打出同數字牌
                cur_card.get_color() == none) {             // 前一張牌是無色牌
                if (players[i].size() == 1) {
                    if (c.get_num() >= 0 && c.get_num() <= 9) {
                        return true;
                    } else {
                        // the last card must be normal
                        return false;
                    }
                } else {
                    return true;
                }
            } else {
                // std::cout << "Disobey the rule!";
                return false;
            }
        }
}

void game::start_turn(std::string op[]) {
    for (int i = 0; i < player_num; i++)
        if (i == cur_turn) {
            if (op[i] == "") {
                // std::cout << "No move!";
                give_card(i,plus_card? plus_card:1);
                plus_card = 0;
                end_turn();
                return;
            }
            std::string str = "", opt = "";
            int j = 0;
            while (j < op[i].length() && op[i][j] != ' ') {
                str += op[i][j];
                j++;
            }
            int num;
            try {
                num = stoi(str);
            }
            catch(std::invalid_argument&) {
                // std::cout << "Input Error!";  // 输入非数值
                give_card(i,plus_card? plus_card:1);
                plus_card = 0;
                end_turn();
                return;
            }
            if (j < op[i].length())
                opt = std::string(op[i], j+1, op[i].length());
            if (num >= players[i].size() || num < 0) {  // 输入秩不合法
                // std::cout << "Index out of range!";
                give_card(i,plus_card? plus_card:1);
                plus_card = 0;
                end_turn();
                return;
            }
            auto c = players[i].my_card.get_list()[num];
            if (plus_card) {  //在有加牌叠加的情况下, 只能打加牌噢
                if (players[i].size() == 1 ||  // 只剩一张牌, 必不能打加牌
                    c.get_num()!=12 ||   // 打的不是+2卡
                    c.get_num()!= 14) {  // 打的不是+4卡
                    give_card(i,plus_card);
                    plus_card = 0;
                    end_turn();
                    return;
                }
            }
            if (c.get_color() == 0 ||                       // 打出无色牌
                c.get_color() == cur_card.get_color() ||    // 打出同色牌
                c.get_num() == cur_card.get_num() ||        // 打出同數字牌
                cur_card.get_color() == none) {             // 前一張牌是無色牌
                if (players[i].size() == 1) {
                    if (c.get_num() >= 0 && c.get_num() <= 9) {
                        players[i].remove_card(num);
                        used_deck.add(c);
                        winner = i;
                        end_game = true;
                        return;
                    } else {
                        // the last card must be normal
                        give_card(i);
                        end_turn();
                        return;
                    }
                } else {
                    players[i].remove_card(num);
                    used_deck.add(c);
                    cur_card = c;
                    judge_rule(opt);
                    return;
                }
            } else {
                // std::cout << "Disobey the rule!";
                give_card(i);
                end_turn();
                return;
            }
        }
}

void game::end_turn(int n) {
    for (int i = 0; i < n; i++) {
        (reversed) ? --cur_turn : ++cur_turn;
        cur_turn %= player_num;
        if (cur_turn < 0)
            cur_turn += player_num;
    }
}

void game::judge_rule(std::string opt) {

    if (cur_card.get_color() == none)  {
        if (opt == "red")
            cur_card.set_color(COLOR(1));
        else if (opt == "green")
            cur_card.set_color(COLOR(2));
        else if (opt == "blue")
            cur_card.set_color(COLOR(3));
        else if (opt == "yellow")
            cur_card.set_color(COLOR(4));
        else
            cur_card.set_color(COLOR(e()%4+1));
    }
    switch (cur_card.get_num()) {
        case 10:
            reversed = !reversed;
            end_turn();
            break;
        case 11:
            end_turn(2);
            break;
        case 12:  //加牌可叠加
            end_turn();
            plus_card += 2;
            break;
        case 14:
            end_turn();
            plus_card += 4;
            break;
        default:
            end_turn();
            break;
    }
}

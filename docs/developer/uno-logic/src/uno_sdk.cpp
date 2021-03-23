/*
    Copyright [2019] <OneMorePigeon>
    Created by choumartin1234
*/

#include <fstream>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <utility>
#include <algorithm>
#include "../include/deck.h"
#include "../include/player.h"
#include "../include/card.h"
#include "../include/game.h"
#include "../include/jsoncpp/json/json.h"

inline void write_log(std::string msg) {
    if (msg != "log") // if you want to use this function, delete these two lines.
        return;
    std::ofstream log("game_log.txt", std::ios::app);
    log << "uno_sdk " << msg << std::endl;
    log.close();
}

static std::string convert_to_byte(std::string msg) {
    int len = msg.length();
    char lenb[4];
    for (int i = 0; i < 4; i ++)
        lenb[3 - i] = (unsigned char) (len >> (8 * i));
    std::string res(lenb);
    res += msg;
    return res;
}

class UnoSDK {
 public:
    explicit UnoSDK(int _n = 5) {
        g = game(_n);
        n = _n;
        Msg = "";
        f = fopen("data1.out", "w+");
        // fout.open("replay.json");
        writer = std::unique_ptr<Json::StreamWriter>
                            (w_builder.newStreamWriter());
    }

    void start() {
        receive_player_list();
        write_limit();
    }

    void sendLen(size_t s) {
        /*
            发送消息前需要先发送长度
            调用sendLen函数
        */
        int len = s;  // 原字串 + 數據頭
        unsigned char lenb[4];
        lenb[0] = (unsigned char)(len);
        lenb[1] = (unsigned char)(len >> 8);
        lenb[2] = (unsigned char)(len >> 16);
        lenb[3] = (unsigned char)(len >> 24);
        for (int i = 0; i < 4; i++)
            printf("%c", lenb[3 - i]);  // 發給Judger
    }

    void sendMsg(std::string s) {
        /*
            发送消息
        */
        // 在包前加入數據頭
        sendLen(s.length());
        sendLen((size_t) -1);
        std::cout << s;
        std::cout.flush();
    }

    std::string listen() {
        /*
            监听直到收到judger的消息
        */
        char lenr[4];
        std::string msg;
        while (true) {
            scanf("%4c", lenr);
            int len = (unsigned int)((((unsigned int)lenr[3]) & 255) |
                                    ((((unsigned int)lenr[2]) & 255) << 8) |
                                    ((((unsigned int)lenr[1]) & 255) << 16) |
                                    ((((unsigned int)lenr[0]) & 255) << 24));
            for (int i = 0; i < len; i ++)
                msg += getchar();
            break;
        }
        // return std::string(msg);
        // fprintf(f, "receive from player: %s\n", msg.c_str());
        return msg;
    }

    void receive_player_list() {
     /*
        通信格式: json
        通信内容: 启动的AI状态 
                 0:启动失败 1:启动成功 2:播放器
        通信样例: 
                {
                    "player_list":[1, 1, 1, 0]
                    "player_num": int
                }
    */
        std::string msg = listen();
        Json::Value root;
        if (!reader.parse(msg, root)) {
            fprintf(f, "fail to parse root\n");
            fclose(f);
            exit(0);
        }
        auto& ls = root["player_list"];
        fout.open(root["replay"].asString());
        this->n = root["player_num"].asInt();
        this->g = game(this->n);
        int n = ls.size();
        for (int i = 0; i < n; i++) {
            if (ls[i] == 2)  // 如果是播放器
                g.players[i].type = 2;
        }
    }

    void write_limit() {
    /*
        通信格式: json
        通信内容: 回合限制
        通信样例: 
                {
                    "state": 0
                    "time": 3, 
                    "length": 1024
                },

    */  
        Json::Value root;
        root["state"] = 0;
        root["time"] = 5;
        root["length"] = 1024;
        writer->write(root, &os);
        Msg = os.str();
        os.str("");  // 清空串流
        sendMsg(Msg);
        Msg = "";
    }

    void save_json() {  // 保存當前局面信息至錄播文件
    /*
        文件格式: json
        文件樣例:  
                {
                    'current player': 1,
                    'top card': 'red_0',
                    'direction': 0,
                    '1': ['red_1', 'blue_2'],
                    '2': ['red_2'],
                 },
    */
        Json::Value state;
        state["current player"] = g.cur_turn;
        state["top card"] = g.cur_card.to_str();
        auto color = g.cur_card.get_color();
        std::string col = "";
        switch (color) {
            case red: col = "red"; break;
            case green: col = "green"; break;
            case blue: col = "blue"; break;
            case yellow: col = "yellow"; break;
        }
        state["color"] = col;
        for (int i = 0; i < n; i++) {
            for (auto& card : g.players[i].my_card.get_list()) {
                state[std::to_string(i)].append(card.to_str());
            }
        }
        replay.append(state);
    }
    void initialize() {
        std::vector<int> is_online_player;
        for (int i = 0; i < n; i ++)
            if (g.players[i].type == 2)
                is_online_player.push_back(i);
        if (is_online_player.size() == 0)
            return;
        Json::Value root;
        root["state"] = g.state;
        root["listen"].append(g.cur_turn);
        for (auto& i : is_online_player) {
            Json::Value player;
            root["player"].append(i);
            for (int j = 0; j < n; j++)
                player["player"].append(std::to_string(j));
            player["position"] = std::to_string(i);
            player["time_limit"] = 15;
            root["content"].append(player.toStyledString());
        }
        writer->write(root, &os);
        Msg = os.str();
        os.str("");  // 清空串流
        fprintf(f, "MSG @ 218: %s\n", Msg.c_str());
        sendMsg(Msg);
        Msg = "";
    }
    /*
        传给播放器初始参数
        编号 i 的播放器传出错误信息, 回传提示
        通信格式: json
        通信内容: 播放器的初始参数
        通信样例:
                {
                    "state": 233
                    "player": [a,b,c...]   // 是播放器的列表
                    "content":      
                    "
                    {
                        'player': ['0','1','2'],
                        'position': 'a',
                        'time limit': time_limit
                    }
                    ",
                    "listen": i
                }              
    */
    void send_error_message(int i) {
    /*
        编号 i 的播放器传出错误信息, 回传提示
        通信格式: json
        通信内容: invalid消息
        通信样例:
                {
                    "state": 233
                    "player": i
                    "content":      
                    "
                        {
                            'invalid': '',
                        } 
                    ",
                    "listen": i
                }       

    */     
        Json::Value root;
        root["state"] = g.state;
        root["listen"].append(g.cur_turn);
        root["player"].append(i);
        Json::Value player;
        player["invalid"] = "";
        root["content"].append(player.toStyledString());
        writer->write(root, &os);
        Msg = os.str();
        os.str("");  // 清空串流
        fprintf(f, "MSG @ 218: %s\n", Msg.c_str());
        sendMsg(Msg);
        Msg = "";
    }

    void write_json() {
    /*
        通信格式: json
        通信内容: 回合数
                 需要发送消息的选手id列表
                 按照上面的列表依次需要发送的数据包(uno应该是最后一张打出的牌和选手手里的牌)
                 本回合需要行动的选手id列表(就是judger需要发送操作给逻辑的选手id列表)
        通信样例:
                {
                    "state": 233
                    "player": 0, 1,2  // 0 to n-1
                    "content":
                    "
                        {   // 传给AI的
                            "top": "blue_0",
                            "color": "blue",
                            "card": ["blue_1", "red_0"],
                        }
                    ",
                    "
                        {   // 传给AI的
                            "top": "blue_0",
                            "color": "blue",
                            "card": ["none+4", "none_W"],
                        }

                    ",
                    "
                        {    // 传给在线播放器的 , 假设是2号
                            'current player': 0,
                            'top card': 'red_0',
                            'color': 'red',
                            'direction': 0,
                            '2': ['red_1', 'red_+2'],
                            '0': 3,
                            '1': 3
                        }
                    ",
                    "listen": 1
                }

    */  
        test_cnt++;
        Json::Value root;
        root["state"] = g.state;
        root["listen"].append(g.cur_turn);
        for (int i = 0; i < n; i++) {
            root["player"].append(i);
            Json::Value player;
            if (g.players[i].type == 1) {
                player["num"] = g.players[i].size();
                player["state"] = g.state;
                player["top"] = g.cur_card.to_str();
                if (g.cur_card.get_color() == COLOR(0))
                    player["color"] = "none";
                else if (g.cur_card.get_color() == COLOR(1))
                    player["color"] = "red";
                else if (g.cur_card.get_color() == COLOR(2))
                    player["color"] = "green";
                else if (g.cur_card.get_color() == COLOR(3))
                    player["color"] = "blue";
                else if (g.cur_card.get_color() == COLOR(4))
                    player["color"] = "yellow";
                for (auto& card : g.players[i].my_card.get_list())
                    player["card"].append(card.to_str());
            } else if (g.players[i].type == 2) {
                player["current player"] = g.cur_turn;
                player["top card"] = g.cur_card.to_str();
                player["state"] = g.state;
                if (g.cur_card.get_color() == COLOR(0))
                    player["color"] = "none";
                else if (g.cur_card.get_color() == COLOR(1))
                    player["color"] = "red";
                else if (g.cur_card.get_color() == COLOR(2))
                    player["color"] = "green";
                else if (g.cur_card.get_color() == COLOR(3))
                    player["color"] = "blue";
                else if (g.cur_card.get_color() == COLOR(4))
                    player["color"] = "yellow";
                player["direction"] = g.reversed;
                for (auto& card : g.players[i].my_card.get_list())
                    player[std::to_string(i)].append(card.to_str());
                for (int j = 0; j < n; j ++)
                    if ( j != i )
                        player[std::to_string(j)].append(g.players[j].size());
            }
            root["content"].append(player.toStyledString());
        }
        writer->write(root, &os);
        Msg = os.str();
        os.str("");  // 清空串流
        fprintf(f, "MSG @ 218: %s\n", Msg.c_str());
        sendMsg(Msg);
        Msg = "";
    }

    void end() {  // NOLINT
    /*
    1. 傳給 judger 結束信息
        通信格式: json
        通信内容: 游戏结束标志符
                 选手id列表
                 按照上面的列表依次给出选手的天梯分数增
        通信样例: 
                {
                    "state": 0,
                    "end_info":
                    {
                        "0": 100,
                        "1": 0,
                        "2": 200, ...
                    }
                }
    2. 錄播文件保存結局
            {
                'winner': '2',
                'top card': 'red_2',
                '1': 0,
                '2': 100,
            }
    */
        Json::Value end_replay;
        end_replay["top card"] = g.cur_card.to_str();
        Json::Value root;
        root["state"] = -1;
        Json::Value end_info;
        // 为选手排名
        std::vector<std::pair<int, int>> rank;
        for (int i = 0; i < n; i++)
            rank.push_back(std::make_pair(g.players[i].size(), i));
        std::sort(rank.begin(), rank.end());  // 依照牌数由小排到大
        end_replay["winner"] = std::to_string(rank[0].second);  // 牌最少胜
        int score[5];
        for (int i = 0; i < n; i++)
            score[rank[i].second] = 100 * (n-i);  // 分数递减
        for (int i = 1; i < n; i++)
            if (rank[i].first == rank[i-1].first)  // 牌数一样 , 分数和前者相同
                score[rank[i].second] = score[rank[i-1].second];
        for (int i = 0; i < n; i++) {
            end_info[std::to_string(i)] = score[i];
            end_replay[std::to_string(i)] =  score[i];
        }
        replay.append(end_replay);
        fout << replay.toStyledString() << "\n\n\n";
        fout.close();
        root["end_info"] = end_info.toStyledString();  // 备注: 这边传字符串型
        writer->write(root, &os);
        Msg = os.str();
        os.str("");  // 清空串流
        sendMsg(Msg);
        Msg = "";
    }

    void read_json() {  // NOLINT
    /*
        此处应为接收judger的消息
        因为一个回合可能会有多个选手做出相应,judger每接收一个选手的操作就会把消息转发给逻辑
        逻辑可能会接收多次消息
        逻辑需要对选手发过来的数据包做格式限制以便于解析,
        另外, 需处理异常
        另外, 如果是播放器的回合, 除非超时, 不然不应该
        注意 : 统一给judger转发字符串类型
        通信格式: json
        通信内容: 回合数
                 选手id
                 数据包
        通信样例: 
                {
                    "player": "i",
                    "content": 
                    "{
                        {
                            "state": "233",
                            "card": "1"  // the rank of the card
                            "color": "yellow",
                        }
                    }"
                }
        异常样例:
                {
                    "player":-1, 
                    "content":
                    "{
                        {
                            "player":4, 
                            "error": 0,   // 0 -> collapse ; 1, timeout
                        }
                    }" 
    */  
        while (true) {
            std::string msg = listen();
            write_log("receive msg: " + msg);
            Json::Value root;
            if (!reader.parse(msg, root)) {
                fprintf(f, "fail to parse root\n");
                continue;
                // fclose(f);
                // exit(0);
            }
            fprintf(f, "msg from judger: %s\n", msg.c_str());
            Json::Value content;
            std::string s_content = root["content"].asString();  // 字符串类型
            write_log("receive content: " + s_content);
            if (!reader.parse(s_content, content)) {
                //fprintf(f, "Fail to receive json:!\n %s\n",
                //        root["content"].asString().c_str());
                write_log("failed to parse json.");
                continue;
                // fclose(f);
                // exit(0);
            }
            /*
            稍微修改了一下，仅供第一个周期使用，这样写无法阻止选手恶意修改SDK炸逻辑
            */
            int player_id;
            try {
                player_id = stoi(root["player"].asString());
            }
            catch(std::invalid_argument&) {
                continue;
            }
            if (player_id == -1) {  // 处理异常, 就视为挂机了 , 啥都没干。
                int error_id = stoi(content["player"].asString());
                int error_state = stoi(content["error"].asString());
                if (error_state == 0) {   // 之后依照情况判断
                    if (error_id == g.cur_turn) {  // 该出牌的玩家崩掉了
                        op[error_id] = "";
                        break;
                    } else {
                        op[error_id] = "";
                        continue;
                    }
                } else if (error_state == 1) {  // 逻辑超时了, 判断是否当前回合
                    if (error_id == g.state)
                        break;
                }
            }
            if (content["state"] != g.state) {  // 回合不匹配
                // std::cerr << "state didn't match !";
                // 注意 : 在进程被调用的时候stderr的消息会被输入到stdout里
                continue;
            }
            write_log("player_id " + root["player"].asString() + " current id " + std::to_string(g.cur_turn));
            if  (player_id == g.cur_turn) {
                op[player_id] = content["card"].asString() + " " +
                                content["color"].asString();
                if (g.players[g.cur_turn].type != 2) {  // 并非播放器, AI视为有效操作
                    break;
                } else if (g.players[g.cur_turn].type == 2)  {  // 为播放器
                    if (content["card"] == -1){  // 播放器想要跳过回合抽牌, 
                        break;  // 就让他抽吧！
                    }
                    if (g.judge_valid(op)) {
                        write_log("player operatiog: " + op[player_id]);
                        break;
                    }
                    else{
                        send_error_message(g.cur_turn);
                        op[player_id] = "";  //置空
                    }
                }
                continue;
            }
            /*for (int i = 0; i < n; i++) {
                if (i == g.cur_turn) {
                    op[i] = content["card"].asString() + " " + content["color"].asString();
                }
            }*/
        }
    }

    void save_player() {  // 保存選手列表至錄播文件
    /*
        格式 : list (json數組)
        範例 : { 
                'player': [1, 2]
                }
    */
        Json::Value p;
        for (int i = 0; i < n; i++)
            p["player"].append(std::to_string(i));
        replay.append(p);
    }

    void run() {
        save_player();
        initialize();  // 发送初始化参数给播放器
        while (!g.end_game) {
            save_json();  // 保存回合至錄播文件
            write_json();  // 发送消息给judger
            read_json();  // 接收judger的讯息
            /* 
                回合制判定逻辑 -- 游戏逻辑
                输入n行, 第i行表示i选手这回合选择出的牌。
                每一行的形式如 'rank [options]'
                rank 为选手选择要打的牌编号
                options为特殊附加(变色卡的目标颜色), 只有在出了颜色为none的卡牌才有效
                若未输入options, 则视为随机选颜色。
                注意, 只有在是该回合可出牌的选手才会真正进行回合卡牌判定
                若超时、空行视为本回合没行动。
                若没行动、编号超出牌组范围、输入不合法、没牌可出, 则抽取一张罚牌 (对于AI来说)
            */
            g.start_turn(op);
            /*std::cout << "End Turn" << std::endl;
            for (int i = 0; i <n; i++)
                std::cout << g.players[i];
            std::cout.flush();*/
            g.state++;  // 回合结束, 回合数增加
        }
        // std::cout << "player " << g.winner << " wins !" << std::endl;
        end();  // 返回局面结束信息
    }

 private:
    Json::Value root;
    int cnt;
    int test_cnt;
    game g;
    int n;  // 游戏选手数
    std::string op[1000];  //  每回合选手的操作符
    std::string Msg;
    Json::StreamWriterBuilder w_builder;
    std::unique_ptr<Json::StreamWriter> writer;
    Json::Reader reader;
    std::ostringstream os;
    Json::Value replay;  // 紀錄回放文件
    FILE* f;
    std::ofstream fout;
};

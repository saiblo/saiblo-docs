//
// Copyright (c) 2019 OneMorePigeon. All rights reserved.
// Created by Andi.
//

#pragma once

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#ifdef linux
#include <jsoncpp/json/json.h>
#else
#include "jsoncpp/json/json.h"
#endif

using namespace std; // NOLINT

class AiClient {
 public:
    AiClient() {
        recv_msg = "";
        send_msg = "";
        cnt = 0;
    }

    void clear_msg() {
        recv_msg = "";
        send_msg = "";
        root.clear();
    }

    void sendLen(string s) {
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
        for (int i = 0; i < 4; i++)
            printf("%c", lenb[3 - i]);
    }

    void sendrecv_msg(string s) {
        /*
            发送消息
        */
        sendLen(s);
        cout << s;
        cout.flush();
    }

    void listen() {
        /*
         * ...
         */
        recv_msg = "";
        while (true) {
            recv_msg += getchar();
            if (reader.parse(recv_msg, root))
                break;
        }
        if (!reader.parse(recv_msg, root)) {
            // fprintf(f, "Not a valid JSON file!\n");
            return;
        }
    }

    string get_current_color() {
        return root["color"].asString();
    }

    string get_top_card() {
        return root["top"].asString();
    }

    vector<string> get_my_card() {
        vector<string> my_card;
        for (int i = 0; i < root["card"].size(); i ++)
            my_card.push_back(root["card"][i].asString());
        return my_card;
    }

    void send_my_card(int card, string color = "") {
        Json::Value operation;
        operation.clear();
        operation["card"] = card;
        operation["color"] = color;
        operation["state"] = root["state"];
        operation["count"] = cnt;
        Json::FastWriter writer;
        send_msg = writer.write(operation);
    }

    virtual void player_AI() {
        /*
            用户编写AI的函数

            调用get_top_card()可得到现在牌顶的牌
            调用get_my_card()可得到我当前手持的牌
            调用send_my_card()可将我当前回合的操作发送出去
        */
        // cerr << "Top card >>> " << get_top_card() << endl;
        // cerr << "My card >>> " << get_my_card() << endl;
        // send_my_card("none+4", "red");
    }

    void run() {
        srand((unsigned)time(NULL));
        string str;
        /*
            回合制AI循环
        */
        while (true) {
            clear_msg();
            listen();
            player_AI();
            sendrecv_msg(send_msg);
        }
    }

 private:
    string recv_msg;
    string send_msg;
    Json::Reader reader;
    Json::Value root;
    int cnt;
};

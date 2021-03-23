//
// Copyright (c) 2019 OneMorePigeon. All rights reserved.
// Created by hogura.
//

#include <string>
#include <vector>
#include <cstdio>

#include "Player.h"
#include "ai_client.h" // NOLINT
#include "wrong_ai_1.h" // NOLINT

class MyClient: public AiClient {
 public:
    void player_AI() override {
        std::string top_card = this->get_top_card();
        std::vector<std::string> my_cards = this->get_my_card();
        player.clear();
        for (auto card : my_cards)
            player.add_card(card);
        auto tp_card = Card(top_card);
        if (tp_card.type() == Card::COL_CHANGE || tp_card.type() == Card::ADD4)
            tp_card = Card(Card::type2str[tp_card.type()], get_current_color());
        int id;
        Card card = player.get_card(tp_card, id);
        if (card.type() == Card::ADD4 || card.type() == Card::COL_CHANGE)
            this->send_my_card(id, this->get_current_color());
        else if (id != -1)
            this->send_my_card(id);
        else
            this->send_my_card(-1);
        FILE* f = fopen("data_player.out", "a+");
        fprintf(f, "send card %s %s\n",
                    Card::color2str[card.color()].c_str(),
                    Card::type2str[card.type()].c_str());
        fclose(f);
    }

 private:
    Player player;
};

int main() {
#ifdef DEBUG
    puts("debugging");
    return 0;
#endif
    AiClient *client = new MyClient();
    // AiClient *client = new WrongClient();
    client->run();
    delete client;
    return 0;
}

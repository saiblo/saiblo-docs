//
// Copyright (c) 2019 OneMorePigeon. All rights reserved.
// Created by hogura.
//

#include "Player.h"

#include <ctime>
#include <vector>
#include <cstdlib>

static void write_log(std::string msg) {
    FILE* f = fopen("ai.out", "a+");
    fprintf(f, "%s\n", msg.c_str());
    fclose(f);
}

void Player::clear() {
    cards.clear();
}

void Player::add_card(const std::string &card) {
    cards.emplace_back(Card(card));
}

std::string Player::send_card(const std::string &last_card) {
    Card lst(last_card);
    int id;
    Card cur = get_card(lst, id);
    return cur.translate();
}

Card Player::get_card(const Card &last, int &id) { // NOLINT
    std::vector<Card> candidate;
    for (Card card : cards) {
        if (card.color() == last.color() || card.type() == last.type()) {
            candidate.push_back(card);
        } else {
            switch (card.type()) {
            case Card::ADD4: case Card::COL_CHANGE:
                    candidate.push_back(card);
            default:
                continue;
            }
        }
    }
    id = -1;
    FILE* f = fopen("ai.out", "a+");
    // fprintf(f, "%s\n", msg.c_str());
    fprintf(f, "I have cards:\n");
    for (Card card : cards) {
        fprintf(f, "%s_%s\n", Card::color2str[card.color()].c_str(),
                              Card::type2str[card.type()].c_str());
    }
    fclose(f);
    if (candidate.empty()) {
        return Card();
    } else {
        id = rand() % candidate.size();  // NOLINT
        write_log("sending card "+ Card::color2str[candidate[id].color()]
                    + "_" + Card::type2str[candidate[id].type()]
                    + "with card id " + std::to_string(id));
        auto res = candidate[id];
        for (int i = 0; i < cards.size(); i ++)
            if ( cards[i].type() == res.type() &&
                cards[i].color() == res.color()) {
                id = i;
                break;
            }
        return res;
    }
}

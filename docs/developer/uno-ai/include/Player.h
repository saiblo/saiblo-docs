//
// Copyright (c) 2019 OneMorePigeon. All rights reserved.
// Created by hogura.
//

#pragma once

#include <string>
#include <vector>

#include "Card.h"

class Player {
 public:
    Player() = default;
    ~Player() = default;

    void clear();

    void add_card(const std::string &card);
    std::string send_card(const std::string &last_card);

    virtual Card get_card(const Card &last_card, int &id); // NOLINT

 private:
    std::vector<Card> cards;
};

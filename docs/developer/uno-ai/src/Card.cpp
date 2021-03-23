//
// Copyright (c) 2019 OneMorePigeon. All rights reserved.
// Created by hogura.
//

#include "Card.h"

#include <ctime>
#include <cassert>
#include <iostream>

static void write_log(std::string msg) {
    FILE* f = fopen("ai.out", "a+");
    fprintf(f, "%s\n", msg.c_str());
    fclose(f);
}

std::map<std::string, Card::Type> Card::str2type = {
        {"0", ZERO},
        {"1", ONE},
        {"2", TWO},
        {"3", THREE},
        {"4", FOUR},
        {"5", FIVE},
        {"6", SIX},
        {"7", SEVEN},
        {"8", EIGHT},
        {"9", NINE},
        {"R", TURN},
        {"W", COL_CHANGE},
        {"+2", ADD2},
        {"+4", ADD4},
        {"S", BAN}
};
std::map<Card::Type, std::string> Card::type2str = {
        {ZERO, "0"},
        {ONE, "1"},
        {TWO, "2"},
        {THREE, "3"},
        {FOUR, "4"},
        {FIVE, "5"},
        {SIX, "6"},
        {SEVEN, "7"},
        {EIGHT, "8"},
        {NINE, "9"},
        {TURN, "R"},
        {COL_CHANGE, "W"},
        {ADD2, "+2"},
        {ADD4, "+4"},
        {BAN, "S"}
};
std::map<std::string, Card::Color> Card::str2color = {
        {"red", RED},
        {"green", GREEN},
        {"blue", BLUE},
        {"yellow", YELLOW}
};
std::map<Card::Color, std::string> Card::color2str = {
        {RED,   "red"},
        {GREEN, "green"},
        {BLUE, "blue"},
        {YELLOW, "yellow"}
};

Card::Card() {
    m_type = NONE;
    m_color = RED;
}

Card::Card(std::string card) {
    if (card.find('_') != std::string::npos) {
        int p = card.find('_');
        std::string color = card.substr(0, p);
        std::string type = card.substr(p + 1,
                card.length() - p);
        // std::cerr << color << " " << type << std::endl;
        if (type == "+4") {
            Color col = Color(rand() % 4);  // NOLINT
            *this = Card(ADD4, col);
        } else if (type == "+2") {
            *this = Card(ADD2, str2color[color]);
        } else {
            if (type == "W")
                color = "red";
            *this = Card(type, color);
        }
    } else {
        std::cout << "Received card" << card << std::endl;
        assert(false);
    }
    // write_log(" have card: " + card);
}

Card::Card(Card::Type type, Card::Color color) {
    m_type = type;
    m_color = color;
}

Card::Card(std::string type, std::string color) {
    m_type = str2type[type];
    m_color = str2color[color];
}

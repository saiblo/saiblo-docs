//
// Copyright (c) 2019 OneMorePigeon. All rights reserved.
// Created by hogura.
//

#pragma once

#include <string>
#include <map>

class Card {
 public:
    enum Type {
        ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
        TURN, COL_CHANGE, ADD2, ADD4, BAN,
        NONE,
    };

    enum Color {
        RED, BLUE, YELLOW, GREEN,
    };

    Card();
    explicit Card(std::string card);
    explicit Card(Type type, Color color);
    explicit Card(std::string type, std::string color);
    ~Card() = default;

    static std::map<std::string, Type> str2type;
    static std::map<Type, std::string> type2str;
    static std::map<std::string, Color> str2color;
    static std::map<Color, std::string> color2str;

    inline Type type() const {
        return m_type;
    }

    inline Color color() const {
        return m_color;
    }

    inline std::string translate() const {
        return color2str.at(m_color) + "_" + type2str.at(m_type);
    }

 private:
    Type m_type;
    Color m_color;
};

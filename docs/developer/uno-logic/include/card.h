/*
    Copyright [2019] <OneMorePigeon>
    This is the definition of "card" class for game Uno
*/
#pragma once
#include <iostream>
#include <string>
enum COLOR {
    none,
    red,
    green,
    blue,
    yellow
};

class card {
friend std::ostream& operator<<(std::ostream& out, const card& _card);
 public:
    explicit card(COLOR _color = none, int _num = -1);
    COLOR get_color();
    int get_num();
    void set_color(COLOR col);
    bool operator==(card other);
    std::string to_str() const;
 private:
    COLOR color;
    int num;  // 0~9, 10: reverse, 11: skip, 12: +2, 13: Wild, 14: Wild+4
};


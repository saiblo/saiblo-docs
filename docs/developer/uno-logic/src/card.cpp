/*
    Copyright [2019] <OneMorePigeon>
    This is the implementation of "card" class for game Uno
*/
#include "../include/card.h"
card::card(COLOR _color, int _num): color(_color), num(_num) {}
COLOR card::get_color() {
    return color;
}
int card::get_num() {
    return num;
}
void card::set_color(COLOR col) {
    color = col;
}
bool card::operator==(card other) {
    return this->color == other.get_color() && this->num == other.get_num();
}

std::ostream& operator<<(std::ostream& out, const card& _card) {
    switch (_card.color) {
        case 0:
            out << "none";
            break;
        case 1:
            out << "red";
            break;
        case 2:
            out << "green";
            break;
        case 3:
            out << "blue";
            break;
        case 4:
            out << "yellow";
            break;
    }
    switch (_card.num) {
        case 10:
            out<< "_R";  // reverse
            break;
        case 11:
            out<< "_S";  // Skip
            break;
        case 12:
            out<< "_+2";
            break;
        case 13:
            out<< "_W";  // Wild
            break;
        case 14:
            out<< "_+4";
            break;
        default:
            out << "_" << _card.num;
            break;
    }
    return out;
}

std::string card::to_str() const {
    std::string col;
    std::string n;
    switch (color) {
        case 0:
            col = "none";
            break;
        case 1:
            col = "red";
            break;
        case 2:
            col = "green";
            break;
        case 3:
            col = "blue";
            break;
        case 4:
            col = "yellow";
            break;
    }
    switch (num) {
        case 10:
            n = "_R";  // reverse
            break;
        case 11:
            n = "_S";  // Skip
            break;
        case 12:
            n = "_+2";
            break;
        case 13:
            n = "_W";  // Wild
            break;
        case 14:
            n = "_W+4";
            break;
        default:
            n = "_" + std::to_string(num);
            break;
    }
    return col+n;
}

/*
    Copyright [2019] <OneMorePigeon>
    Created by choumartin1234
    
    This is the implementation of "deck" class for game Uno
*/
#include <algorithm>
#include "../include/deck.h"
deck::deck() {}
deck::deck(const deck& _copy) {
    for (auto card : _copy.get_list())
        cards.push_back(card);
}
void deck::move_to_deck(const deck& _copy) {
    for (auto card : _copy.get_list())
        cards.push_back(card);
}
const std::vector<card>& deck::get_list() const {
    return cards;
}
int deck::get_size() {
    return cards.size();
}
bool deck::empty() {
    return get_size() == 0;
}
void deck::clear() {
    cards.clear();
}
void deck::add(const card& _card) {
    cards.push_back(_card);
}
void deck::pop() {
    cards.pop_back();
}
card& deck::back() {
    return cards.back();
}
void deck::shuffle() {
    std::random_shuffle(cards.begin(), cards.end());
}
bool deck::remove(const card& _card) {
    for (auto it = cards.begin(); it!= cards.end(); it++)
        if ((*it) == _card) {
            cards.erase(it);
            break;
            return true;
        }
    return false;
}
bool deck::remove(int i) {
    if (i < 0 || i >= cards.size())
        return false;
    cards.erase(cards.begin()+i);
    return true;
}
std::ostream& operator<<(std::ostream& out, const deck& _deck) {
    for (auto card : _deck.cards)
        out << card << " ";
    out << std::endl;
    return out;
}

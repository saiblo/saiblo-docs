/*
    Copyright [2019] <OneMorePigeon>
    This is the definition of "deck" class for game Uno
*/
#pragma once
#include <vector>
#include "card.h"

class deck {
friend std::ostream& operator<<(std::ostream& out, const deck& _deck);
 public:
    deck();
    deck(const deck& _copy);
    void move_to_deck(const deck& _copy);  // move deck _copy into current deck
    const std::vector<card>& get_list() const;
    int get_size();
    bool empty();
    void clear();
    void add(const card& _card);
    void pop();
    card& back();
    bool remove(const card& _card);
    bool remove(int i);
    void shuffle();

 private:
    std::vector<card> cards;  // current list of the card
};

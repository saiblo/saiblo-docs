/*
    Copyright [2019] <OneMorePigeon>
    This is the definition of "player" class for game Uno
*/
#pragma once
#include "deck.h"

class player {
friend std::ostream& operator<<(std::ostream& out, const player& _player);
 public:
    explicit player(int _id);
    player(int _id, const deck& _deck);
    deck my_card;
    int type = 1;  // 1 => AI,  2 => 播放器
    void receive_card(card _card);
    bool remove_card(card _card);
    bool remove_card(int i);
    int get_id();
    int size();

 private:
    int id;
};

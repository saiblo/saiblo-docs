/*
    Copyright [2019] <OneMorePigeon>
    This is the definition of "player" class for game Uno
*/
#include "../include/player.h"
player::player(int _id, const deck& _deck): id(_id), my_card(_deck) {}
player::player(int _id): id(_id) {}
void player::receive_card(card _card) {
    my_card.add(_card);
}
bool player::remove_card(card _card) {
    return my_card.remove(_card);
}
bool player::remove_card(int i) {
    return my_card.remove(i);
}
int player::get_id() {
    return id;
}
std::ostream& operator<<(std::ostream& out, const player& _player) {
    out << "player(id=" << _player.id << ") has cards:" << std::endl;
    out << _player.my_card;
    return out;
}
int player::size() {
    return my_card.get_size();
}

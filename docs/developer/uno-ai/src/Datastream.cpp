//
// Copyright (c) 2019 OneMorePigeon. All rights reserved.
// Created by hogura.
//

#include "Datastream.h"

Datastream::Datastream(std::istream &in_, std::ostream &out_):
    in(in_), out(out_) { }

bool Datastream::eoln() {
    return ssin.eof();
}

bool Datastream::eof() {
    return in.eof();
}

void Datastream::next_line() {
    ssin.clear();
    std::string str;
    std::getline(in, str);
    ssin << str;
}

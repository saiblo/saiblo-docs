//
// Copyright (c) 2019 OneMorePigeon. All rights reserved.
// Created by hogura.
//

#pragma once

#include <string>
#include <cstdio>
#include <sstream>
#include <iostream>

class Datastream {
 public:
    explicit Datastream(std::istream& in, std::ostream& out);
    ~Datastream() = default;

    bool eoln();
    bool eof();

    void next_line();

    template<class T> Datastream&operator>>(T &value) {
        ssin >> value;
        return *this;
    }

    template<class T> Datastream&operator<<(const T &value) {
        out << value;
        return *this;
    }

 protected:
    std::istream &in;
    std::ostream &out;
    std::stringstream ssin;
};

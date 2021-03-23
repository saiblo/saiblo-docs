//
// Copyright (c) 2019 OneMorePigeon. All rights reserved.
// Created by hogura.
//

#pragma once

#include <ctime>
#include <random>
#include <string>
#include <cassert>
#include "ai_client.h"

class WrongClient: public AiClient {
 public:
    WrongClient() {
        rand.seed(time(NULL));
        u = std::uniform_int_distribution<int>(1, 5);
    }

    void player_AI() override {
        int t = rand()%5+1;  // NOLINT
        switch (t) {
            case 1:
                // normal operation, but might be invalid output.
                send_my_card(0, "");
                break;
            case 2:
                // runtime error
                assert(0);
                break;
            case 3:
                // infinite output
                while (1)
                    putchar('0');
                break;
            case 4:
                // infinite input
                while (u(rand) != 0)
                    getchar();
                break;
            case 5:
                // exit directly
                exit(0);
        }
    }

 private:
    std::default_random_engine rand;
    std::uniform_int_distribution<int> u;
};

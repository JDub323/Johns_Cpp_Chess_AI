//
// Created by jwhal on 6/10/2024.
//

#ifndef JOHNS_CPP_CHESS_AI_UCIHANDLER_H
#define JOHNS_CPP_CHESS_AI_UCIHANDLER_H

#include <string>


class UCIHandler {
private:

public:
    static void receiveCommand(const std::string& message);

    static void giveCommand(const std::string &message);
};


#endif //JOHNS_CPP_CHESS_AI_UCIHANDLER_H

//
// Created by jwhal on 6/10/2024.
//

#include "UCIHandler.h"
#include "Utilities.h"
#include <string>
#include <iostream>

void UCIHandler::receiveCommand(const std::string& message) {
    std::string firstWord = Utilities::split((string &) message, ' ')[0];
    if (firstWord == "uci") {
        giveCommand("uciok");
    }
    else if (firstWord == "isready") {
        giveCommand("readyok");
    }
    else if (firstWord == "position") {
        //TODO later
    }
    else if (firstWord == "go") {
        //TODO later
    }
    else if (firstWord == "stop") {
        //TODO later
    }
    else if (firstWord == "printfen") {
        //TODO later
    }
    else if (firstWord == "ucinewgame" || firstWord == "quit") {}
    else {
        giveCommand("Unknown statement: "+message);
    }
}
void UCIHandler::giveCommand(const string& message) {
    cout << message << endl;
}
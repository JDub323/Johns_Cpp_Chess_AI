//
// Created by jwhal on 11/21/2025.
//
#include <iostream>
#include <queue>

// #include "ChessConstants.h"
//#include <bits/ranges_algo.h>
#include <chrono>
#include <utility>
#include <algorithm>
#include <unistd.h>
#include <unordered_set>
#include <oneapi/tbb/detail/_task.h>

#include "Movement.h"
#include "Pieces.h"
#include "Utilities.h"

using namespace Pieces;
using namespace std;

void getPieceType(char& piece) {
    while (true) {
        cout << "Give a piece:\n";
        cout << "  p for pawn\n";
        cout << "  n for knight !!not k!!\n";
        cout << "  b for bishop\n";
        cout << "  r for rook\n";
        cout << "  q for queen\n";
        cout << "  k for king\n";
        cin >> piece;

        piece = static_cast<char>(Utilities::getPiece(piece)); // convert the letter to a number
        if (piece == -1) {
            cout << "Invalid piece. Please try again\n";
            continue;
        }
        piece /= 8; // take off the "black" flag if the user put a capital letter. for all these functions,
        // friendly will be white, and enemy will be black

        break;
    }
}

void getSquare(char& square) {
    cout << "I enforce the standard chess representation of the board on the user so bitboards are more natural when grading my code.\n"
            "This means squares are counted little-endian, column-first. a1->0, b1->1, ..., h8->63\n" <<
                "so if you wanted c8, row = 2, col=7, and square_idx = row * 8 + col = 23\n\n" <<
                "this is how almost all good chess engines are written (although some are big endian)\n"
                "for more info on chess board endianness, see: https://www.chessprogramming.org/Endianness\n";
    while (true) {
        cout << "Give a square\n";
        cin >> square;
        if (square < 64 && square >= 0) break;

        cout << "Invalid square. It should be a number. Please try again.";
    }
}

void getPiecesToPlace(ull &friendlyBB, ull &enemyBB) {
    cout << "I enforce the standard chess representation of the board on the user so bitboards are more natural when grading my code.\n"
            "This means squares are counted little-endian, column-first. a1->0, b1->1, ..., h8->63\n" <<
                "so if you wanted c8, row = 2, col=7, and square_idx = row * 8 + col = 23\n\n" <<
                "this is how almost all good chess engines are written (although some are big endian)\n"
                "for more info on chess board endianness, see: https://www.chessprogramming.org/Endianness\n\n";
    cout << "Note that I ignore checks, since taking them into account would be outside the scope of this project.\n";
    cout << "Therefore I treat every piece the same, except friendly vs enemy of course\n\n";


    char piece;
    int square;
    friendlyBB = 0l;
    enemyBB = 0l;

    while (true) {
        cout << "Give a piece (f/e) and square, or q to quit\n";

        cin >> piece;
        if (!cin) {                      // If reading piece failed
            cin.clear();                 // clear failbit
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (piece == 'q') break;
        if (piece != 'f' && piece != 'e') {
            cout << "invalid friend status :(\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin >> square;
        if (!cin) {                      // If reading square failed
            cout << "invalid square input\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (square < 0 || square > 63) {
            cout << "invalid square. please try again\n";
            continue;
        }

        if (piece == 'f') friendlyBB |= Utilities::toBitboard(square);
        else if (piece == 'e') enemyBB |= Utilities::toBitboard(square);
    }

    if ((friendlyBB & enemyBB) != 0) {
        cout << "there are some friendly and enemy pieces on the same square(s).\nMaking all these collisions friendly-occupied squares";
        enemyBB ^= friendlyBB & enemyBB;
    }
}

void printQuadrupleBB(const ull piece, const char pieceType, const ull friendlyBB, const ull enemyBB, const ull attackingBB) {
    for (int row = 7; row >=0; row--) {
        for (int col = 0; col<8; col++) {
            const ull squareBB = Utilities::toBitboard(row*8+col);
            if (piece & squareBB) {
            std::cout << pieceType;
            }
            else if (attackingBB & enemyBB & squareBB) {
                std::cout << 'X';
            }
            else if (attackingBB & squareBB) {
                std::cout << 'O';
            }
            else if (friendlyBB & squareBB) {
                std::cout << 'f';
            }
            else if (enemyBB & squareBB) {
                std::cout << 'e';
            }
            else {
                std::cout << ' ';
            }
            if (col != 7) {
                std::cout << '|';
            }
        }
        std::cout << std::endl;
    }
}

// breadth-first search for the number of moves it would take to get from one square to another
int BFS(const char piece, ull fromSquareBB, ull toSquareBB, const ull enemyPieces, ull friendlyPieces) {
    typedef struct {
        ull fromSquare;
        int depth;
    } pos;
    auto myq = std::queue<pos>();
    const auto visited = static_cast<bool *>(calloc(64, sizeof(bool))); // Track visited squares, only 64 options so use bool array
    const int fromSquare = __builtin_ctzll(fromSquareBB);
    ull moves = Movement::getPieceMovement(piece, fromSquare, friendlyPieces, enemyPieces);

    // add all the moves to the queue
    while (moves) {
        const ull move = moves & -moves;
        moves ^= move;

        pos thisPos;
        thisPos.fromSquare = move; // where you go. add where you came from below--v
        thisPos.depth = 1;

        myq.push(thisPos);

    }

    while (!myq.empty()) {
        pos p = myq.front(); // get reference to first element
        myq.pop(); // doesn't actually return a reference, so I call it after

        visited[__builtin_ctzll(p.fromSquare)] = true; // add the square to the visited table

        ull posMoves = Movement::getPieceMovement(piece, __builtin_ctzll(p.fromSquare), friendlyPieces, enemyPieces);

        if ((posMoves & toSquareBB) != 0) {
            free(visited); // idk why I decided to put this on the heap lol this should have been on the stack.
            return p.depth + 1; // a solution has been found
        }

        // otherwise, add all moves from this square to the queue
        while (posMoves) {
            const ull move = posMoves & -posMoves;
            posMoves ^= move;
            if (visited[__builtin_ctzll(move)]) continue; // if it has been visited, continue. don't add to queue

            pos thisPos;
            thisPos.fromSquare = move;
            thisPos.depth = p.depth + 1;

            myq.push(thisPos);
        }
    }

    free(visited);

    return -1;
}



// depth-first search for the number of moves it would take to get from one square to another
// uses recursion:
// base case: if I can get to the toSquare from this node, return 1.
// if not better than the best depth, return -1 (prune this node)
// if the best depth is already = 1, return -1 (no way to get a better depth, prune all nodes)
// for all moves at this node:
// make the move, dfs from the new position, unmake the move. (disallow moving back to current node)
// if the move set a new record, save that as the new best depth
// return the best depth + 1
int DFS(const char piece, const ull fromSquareBB, const ull toSquareBB, const ull friendlyPieces, const ull enemyPieces,
    int bestDepth, chrono::steady_clock::time_point startTime) {
    // since I sometimes get in an infinite loop:
    // Check timeout periodically
    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
    if (constexpr int TIMEOUT_MS = 10000; elapsed > TIMEOUT_MS) return -1;


    uint64_t moves = Movement::getPieceMovement(piece, __builtin_ctzll(fromSquareBB), friendlyPieces, enemyPieces);

    if (moves == 0) { // no moves remain, no solution found. Return -1. only possible since I disallow moving back to a square previously occupied
        return -1;
    }
    if ((moves & toSquareBB) != 0) return 1; // solution found
    if (bestDepth == 1) return -1; // no way to get a better depth (redundant check)

    // otherwise, BFS
    while (moves) {
        const ull newSquareBB = moves & -moves; // isolates the LSB
        moves ^= newSquareBB; // pop the LSB

        const int score = DFS(piece, newSquareBB, toSquareBB, friendlyPieces | fromSquareBB, enemyPieces, bestDepth - 1, startTime);

        if (score != -1 && score < bestDepth) {// if there is a solution, and it is strictly less than the best depth
            bestDepth = score;
        }
        if (bestDepth == 1) { // cannot do better, break
            return bestDepth + 1;
        }
    }

    return bestDepth + 1;
}

// base DFS function to be called for timing
int BaseDFS(const char piece, const ull fromSquareBB, const ull toSquareBB, const ull enemyPieces, const ull friendlyPieces) {
    static auto startTime = std::chrono::steady_clock::now();

    // 64 squares on the board, so no way to traverse more than 64 squares. worst case scenario.
    const int ret = DFS(piece, fromSquareBB, toSquareBB, friendlyPieces, enemyPieces, 64, startTime);

    if (ret >= 64) return -1; // no solution found
    return ret; // otherwise return the solution
}

// super parallel search, can only be done for non-sliding pieces.
int SPS(char piece, ull fromSquare, ull toSquare, ull enemyPieces, ull friendlyPieces) {
    if (piece == ROOK || piece == BISHOP || piece == QUEEN) {
        //printf("Cannot super search with piece %d\n", piece);
        return -1;
    }

    ull nextBB = fromSquare;
    int moves = 0;

    if (piece == PAWN) {
        ull prevBB = 0l;
        while (nextBB != prevBB) {
            prevBB = nextBB;
            nextBB = Movement::createWhitePawnMovesBB(nextBB, friendlyPieces, enemyPieces);
            moves++;
            if ((nextBB & toSquare) != 0) return moves;
        }
    }
    if (piece == KNIGHT) {
        ull prevBB = 0l;
        while (nextBB != prevBB) {
            prevBB = nextBB;
            //Utilities::printBitboard(prevBB);
            //cout << "move " << moves << '\n';
            nextBB = (Movement::createKnightAttacksBB(nextBB) | prevBB) & ~friendlyPieces; //  OR w/ prevBB to preserve the "from-squares"
            moves++;
            if ((nextBB & toSquare) != 0) return moves;
        }
        //cout << "\n";
    }
    if (piece == KING) {
        ull prevBB = 0l;
        while (prevBB != nextBB) {
            prevBB = nextBB;
            nextBB = Movement::createKingAttacksBB(nextBB) & ~friendlyPieces; // assumes enemies cannot attack king
            moves++;
            if ((nextBB & toSquare) != 0) return moves;
        }
    }

    return -1; // no way to get to the square searching for (by the exit condition of the while loop
}

// Returns execution time in microseconds. generated by chat, but I modified
template <typename Func, typename A1, typename A2, typename... ExtraArgs>
long long time_function(
        Func f,
        A1* randBlockerBBs,
        A2* outputs,
        const int count,
        ExtraArgs&&... extra)
{
    using namespace std::chrono;

    const auto start = high_resolution_clock::now();

    for (int i = 0; i < count; i++) {
        outputs[i] = f(std::forward<ExtraArgs>(extra)..., randBlockerBBs[i]);
    }

    const auto end = high_resolution_clock::now();

    return duration_cast<microseconds>(end - start).count();
}

void testTime(char piece, char fromSquare, char toSquare, int blockerCount) {
    // get bitboards
    ull fromSquareBB = Utilities::toBitboard(fromSquare);
    ull toSquareBB = Utilities::toBitboard(toSquare);

    // make arrays of friendly pieces:
    constexpr int ARR_SIZE = 200;
    const auto blockerVec = static_cast<ull *>(calloc(ARR_SIZE, sizeof(ull)));
    if (blockerVec == nullptr) return;

    for (int i = 0; i < ARR_SIZE; i++) {
        for (int j = 0; j < blockerCount; j++) {
            blockerVec[i] |= Utilities::toBitboard(rand() & 63); // bitwise and-ing with 63 is same as taking mod 64
        }
        blockerVec[i] &= ~toSquareBB; // potentially undefined behaviour with blockers on the toSquare
    }

    // run each test
    int resultDFS[ARR_SIZE];
    int resultBFS[ARR_SIZE];
    int resultSPS[ARR_SIZE];

    // the 0l is the enemy pieces on the board. I could ask the user for enemy pieces too, but since it doesn't affect
    // much, I am not interested in doing so.
    const long long t1 = time_function(BFS, blockerVec, resultBFS, ARR_SIZE, piece, fromSquareBB, toSquareBB, 0l);
    const long long t2 = time_function(BaseDFS, blockerVec, resultDFS, ARR_SIZE, piece, fromSquareBB, toSquareBB, 0l);
    const long long t3 = time_function(SPS, blockerVec, resultSPS, ARR_SIZE, piece, fromSquareBB, toSquareBB, 0l);

    for (int i = 0; i < ARR_SIZE; i++) {
        printf("BFS- %d\n", resultBFS[i]);
        printf("DFS- %d\n", resultDFS[i]);
        printf("SPS- %d\n", resultSPS[i]);
        printf("Blockers:\n");
        Utilities::printBitboard(blockerVec[i]);
        printf("\n");
    }


    printf("BFS time- %lld\n", t1);
    printf("DFS time- %lld\n", t2);
    printf("SPS time- %lld\n", t3);


    free(blockerVec);
}

void promptUser() {
    int cmd;

    while (true) {
        cout << "This program can do a few things:\n";
        cout << "  1: the possible attacking squares for a piece, given the current pieces on the board\n" <<
                "  2: the number of moves it would take to get from square i to square j for some piece, with friendly or enemy pieces on the board\n" <<
                "  3: the time it takes to calculate how long it takes to get from square i to square j, with some random set of \n" <<
                "     friendly pieces on the board. There are two different ways to calculate this, or three for non-sliding pieces.\n" <<
                "     this function will time each, to see whether depth first or breadth-first search is faster\n"  <<
                "     on average. User can control the number of friendly pieces on the board.\n";

        cout << "\n" << "Press the number for the command you would like to do:\n";
        cin >> cmd;

        if (cmd >= 1 && cmd <= 3) break;

        //else
        cout << "Invalid argument. Please try again\n\n";
    }

    if (cmd == 1) {
        char piece;
        char square;
        ull friendlyBB;
        ull enemyBB;
        cout << "\nFinding the attacking squares for a piece.\n";
        getPieceType(piece);
        cout << "\n\n";
        cout << "Give a square to put this piece on:\n";
        getSquare(square);
        cout << "\n\n";
        getPiecesToPlace(friendlyBB, enemyBB);

        const ull attacks = Movement::getPieceMovement(piece, square, friendlyBB, enemyBB);
        printQuadrupleBB(1l << square, piece, friendlyBB, enemyBB, attacks);
    }
    else if (cmd == 2) {
        char piece;
        char fromSquare;
        char toSquare;
        ull friendlyBB;
        ull enemyBB;
        cout << "\nFinding the attacking squares for a piece.\n";
        getPieceType(piece);
        cout << "\n\n";
        cout << "Give a square to put this piece on:\n";
        getSquare(fromSquare);
        cout << "\n\n";
        cout << "\nNow give the square you want the piece to go to:\n";
        getSquare(toSquare);
        cout << "\n\n";
        getPiecesToPlace(friendlyBB, enemyBB);

        int moveCount = BFS(piece, Utilities::toBitboard(fromSquare), 1l << Utilities::toBitboard(toSquare), friendlyBB, enemyBB);

        cout << "\nMoves for " << Utilities::getPiece(piece) << " to get from " << Utilities::getSquare(fromSquare) <<
            " to " << Utilities::getSquare(toSquare) << ": " << moveCount;
        if (moveCount == -1) {
            cout << "This means " << Utilities::getPiece(piece) << " cannot reach " << Utilities::getSquare(toSquare) << "\nn";
        }
    }
    else if (cmd == 3) {
        // User gives
        // fromSquare, toSquare, piece, number of blockers
        // then I calculate how many moves it takes to get to that square, with randomly placed blockers, some set large
        // number of times. Print out the speed results with super parallel search, breadth-first, and depth-first,
        // and assert that they all always get the same numbers with the same set of blockers.
        char piece;
        char fromSquare;
        char toSquare;
        int blockerCount;
        cout << "\nFinding the attacking squares for a piece.\n";
        getPieceType(piece);
        cout << "\n\n";
        cout << "Give a square to put this piece on:\n";
        getSquare(fromSquare);
        cout << "\n\n";
        cout << "\nNow give the square you want the piece to go to:\n";
        getSquare(toSquare);
        cout << "\nFinally, give the number of friendly pieces (blockers) you would like on the board:\n";
        cin >> blockerCount;
        ranges::clamp(blockerCount, 0, 63); // make sure the blockers are a kinda valid number

        testTime(piece, fromSquare, toSquare, blockerCount);
    }
    else {
        cout << "Invalid command. Please try again.\n\n";
    }
}

void runTests(const int waitTime) {
    cout << "first I will show all the ways a piece can move on each square, printing out the bitboard, and then\n"
    << "I will test the time it takes for each piece to get from some arbitrary square to another, using different\n"
    << "searching algorithms. Read the README before running this, as there are some good reasons DFS and SPS may fail.\n\n";
    sleep(waitTime);
    cout << "running movement tests with the following friendly and enemy bitboards:\n";
    ull friendlyBB = 0;
    ull enemyBB = 0;

    for (int j = 0; j < 11; j++) { // say there are 11 friendly pieces. arbitrary.
        friendlyBB |= Utilities::toBitboard(rand() & 63); // bitwise and-ing with 63 is same as taking mod 64
    }
    for (int j = 0; j < 13; j++) { // say there are 13 enemy pieces. arbitrary.
        enemyBB |= Utilities::toBitboard(rand() & 63); // bitwise and-ing with 63 is same as taking mod 64
    }
    cout << "friendlyBB = \n";
    Utilities::printBitboard(friendlyBB);
    cout << "enemyBB = \n";
    Utilities::printBitboard(enemyBB);
    sleep(waitTime);

    for (unsigned short piece = PAWN; piece <= KING; piece++) {
        for (int square = 0; square < 64; square++) {
            const char pieceType = Utilities::getPiece(piece);
            if ((square & (enemyBB | friendlyBB)) != 0) {
                cout << "skipping square " << square << " since it is occupied\n";
                continue;
            }
            const ull attacks = Movement::getPieceMovement(piece, square, friendlyBB, enemyBB);
            printQuadrupleBB(1l << square, pieceType, friendlyBB, enemyBB, attacks);
            cout << "^^Square " << square << "^^\n\n";
        }
        sleep(waitTime);
    }
    sleep(waitTime);

    cout << "running timing tests:\n";
    testTime(PAWN, 8, 32, 5);
    cout << "Pawn Tests^\n";
    sleep(waitTime);

    testTime(KNIGHT, 5, 40, 10);
    cout << "Knight Tests^\n";
    sleep(waitTime);

    testTime(BISHOP, 1, 62, 10); // technically, there can be less than 10 blockers
    cout << "Bishop Tests^\n";
    sleep(waitTime);

    testTime(ROOK, 1, 62, 10); // technically, there can be less than n blockers
    cout << "Rook Tests^\n";
    sleep(waitTime);

    testTime(QUEEN, 1, 62, 20); // technically, there can be less than n blockers
    cout << "Queen Tests^\n";
    sleep(waitTime);
}

int main(int argc, char** argv) {
    // for this project, I will make a cli-calculator which will calculate one of:
    // 1: the possible attacking squares for a piece, given the current pieces on the board
    // 2: the number of moves it would take to get from square i to square j for some piece, with friendly or enemy pieces on the board
    // 3: the time it takes to calculate how long it takes to get from square i to square j, with some random set of blockers
    //    optionally with friendly or enemy pieces to the board, with two different ways to calculate it, and then I will
    //    calculate the time it takes to calculate either, to see whether depth first or breadth-first search is faster
    //    in this case, on average. User can control the number of blockers.
    Movement::initializePieceMovement();
    // srand(42);  // set seed so I am consistent in between attempts. ok this actually isn't working??

    if (argc != 2) {
        cout << "need one arg: 0 for interactive demo of searching, 1 for unittests on piece movement and searches";
        exit(1);
    }
    if (argv[1][0] - '0' == 0) promptUser();
    else {
        runTests(1);
    }

    return 0;
}

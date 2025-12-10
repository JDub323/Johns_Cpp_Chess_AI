
The two data structures concepts used:
- Hash table
- Tree traversal

The complexity comes from the underlying bit manipulations of bitboards for the program. I will start by explaining what a bitboard is, then go into the hash table "fancy magic bitboards" algorithm, then talk about the functions I made, and then finally go into what the output of `make tests` and `make demo` does.

(Include links to Chess Programming Wiki below.)  
For more information on bitboards, see: https://www.chessprogramming.org/Bitboards  
For more information on fancy magic bitboards, see: https://www.chessprogramming.org/Magic_Bitboards

---

# BITBOARDS

A bitboard is a mapping of the squares of a chess board to the indices of bits of an unsigned 64 bit integer. This is nice because there is a one-to-one correspondence, and then bit manipulations can be used `<<, >>, ^, &, |` directly on the integer to "move" the pieces around the board. Since there are 6 piece types of each color, the standard representation is 12 bitboards to represent the entire board.

---

# FANCY MAGIC BITBOARDS

Normally, piece movement can quickly be calculated by bit manipulations on the occupancy bitboard of a certain piece. For example, in the (little endian) mapping I have, the square a pawn can push to can be calculated by:

```c
uint_64 pawnBB = ...;
uint_64 pushSquares = pawnBB << 8;
```

for a white pawn. The mapping is as follows:  
a1 = 0 (the LSB), a2 = 1, a3 = 2, a4 = 3, ..., b1 = 8, b2 = 9, ... h8 = 63. In practice, to populate the bit which contains that square, you can do:

```c
1l << bit_index;
```

This is great for pieces like knights, since you can just do this to find all the attacking squares:

```c
uint_64 squareBB = 1l << bit_index;
temp = ((squareBB<<6|squareBB>>10)&NOT_GH_FILE)|((squareBB<<10|squareBB>>6)&NOT_AB_FILE);
temp |= ((squareBB<<17|squareBB>>15)&NOT_A_FILE)|((squareBB<<15|squareBB>>17)&NOT_H_FILE);
temp &= ~friendlyPieceBB; // make sure the knight doesn't jump on same-colored pieces
```

Which, although less readable, is definitely a lot faster on 64-bit machines, where the bits can be manipulated directly.

But there is an issue with the sliding pieces: you can't jump over any pieces that block you. You could do bit manipulations like the following:

```c
temp = squareBB << 8 | squareBB << 16 | squareBB << 24 | squareBB << 32 ... for each direction
temp &= emptySquares; // make sure the rook doesn't pass through pieces (doesn't work)
```

But it is impossible in this case to know how far you need to go before stopping due to a blocker, since although the bitmask will disallow the rook from capturing the piece it sees, it will still be able to pass right through it!

It seems like the only solution is to loop over each square, but that is painfully slow and inelegant. Thus, Pradu Kannan proposed FANCY MAGIC BITBOARDS, and it became standard.

**The idea:**  
Create a database for the sliding pieces. Consider the square b2 for rooks. Make an array for that square which, after some magic, will let a bitboard of potential blockers give an index for the correct attacking bitboard (a `uint_64`). How do you get a bitboard of potential blockers? You get all the squares the rook could attack if the board was empty, and remove the attacking squares which don't have any attacking squares behind them. That is, if the rook can't see anything behind that square (an edge), then there can't be a blocker there. You bitwise `&` that bitboard with the occupancy bitboard (a bitboard with a `1` where there is any piece, a `0` otherwise), so that you get a bitboard of all the pieces that could block the rook on b2. That is, you get A NUMBER.

The hashing algorithm is as follows: you multiply that bitboard, which IS A NUMBER, by another "magic number" with the property that "it makes a perfect hash" and then right-shift by the most you can to make the table still give the right outputs (this will vary based on the square and piece). The magic number is found by trial and error. You just randomly generate them until one works. I saved the ones I generated so I don't have to re-generate them every time, and some of the magics (the ones around the rook corners) are too hard to find just with trial and error, and very important to find, due to the size of these lookup tables. Chessprogrammingwiki says: Recent table sizes were about 38 KiB for the bishop attacks, but still about 800 KiB for rook attacks. This is mostly due to expensive corner square lookup tables, and two of the four corners have very optimal, halved magics with many constructive collisions. But that is not too important.

Do the same process for each square on the board, for rooks and for bishops. For queens, you can just `|` the outputs for rooks and for bishops themselves.

---

# MY TREE TRAVERSAL FUNCTIONS

I let each square represent (at a high level — I still am using bitboards for the most part) a node, and each edge represent the ability of a certain piece to move from square `i` to square `j`. Thus, there will be a different graph for each piece.

I want to find out how many moves it takes (edges traversed) for some piece to get from square `i` to square `j`. I give three ways to find this, and yes, none of them are Dijkstra. I wrote this before we learned about Dijkstra.

### BFS
Use a queue to search each node at a certain depth. Disallow moving back to searched nodes. Standard stuff.

### DFS
Use recursion to search until there are no more moves you can make (disallowing moving to a previously occupied square).

Pretty standard stuff. This isn't:

### SPS
What I call "Super Parallel Search." I move to every node I see at the same time after each iteration, increasing the depth by 1, until I find the square I am looking for (or I reach all nodes). This isn't iterative deepening; this takes advantage of bit manipulations on a bitboard to calculate the moves all knights can make at the same time. Remember the knight attacking bitboard from earlier? There isn't a requirement that there is only one knight.

The con with this search is that it doesn't work for the sliding pieces (since their moves are calculated differently), and you have no way of knowing which set of moves actually got you to the square you want to go to.

---

# INTERPRETING MY OUTPUTS

### timing test
Not a command. This will run the searches 200 times, randomly generate blockers for each time, and print out the number of moves each search returned, and how long each search took (for the sum of time).

### make demo
An interactive demo to see attacking squares for each piece. You can really see anything you want to check with it; I gave a lot of options for friendly and enemy pieces.

There are three options:

```
// 1: the possible attacking squares for a piece, given the current pieces on the board
// 2: the number of moves it would take to get from square i to square j for some piece, with friendly or enemy pieces on the board
// 3: the time it takes to calculate how long it takes to get from square i to square j, with some random set of blockers
//    optionally with friendly or enemy pieces to the board, with two different ways to calculate it, and then I will
//    calculate the time it takes to calculate either, to see whether depth first or breadth-first search is faster
//    in this case, on average. User can control the number of blockers.
```

### make tests
For your convenience, only a bit of the output is printed each time, and then it is paused from printing so one can skim through it and make sure it is all correct.

- Creates a friendly piece and enemy piece bitboard.
- Calculates all the moves a piece can make on each unoccupied square of the board.
- Prints the board, with `O` being squares you can move to, and `X` being pieces you can capture.
- Outputs all the timing tests for each piece. Movement and number of blockers is arbitrary and constant.

---

# IMPORTANT NOTE ON TIMING TESTS

You may notice that DFS sometimes returns `-1` (especially with many blockers), and for the sliding pieces, SPS always returns `-1`. The former is because there was an infinite loop bug which squashing was outside the scope of this project (it was really annoying, just trust me), so I put a timer on the DFS function; the latter is because SPS is only possible with pieces whose moves can be calculated with easy bit manipulations.  
BFS works for all setups, AFAIK.

---

I spent too long writing this for what it does :)

All words of this file are my own, but I pasted this into ChatGPT to format things nicely in md and to fix capitalization and punctuation only.

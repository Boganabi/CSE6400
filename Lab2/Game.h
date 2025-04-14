
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Move{
    string move;
    string moveType;
    int evaluation;
    char player;

    // for internal use to manage board as vector
    int row;
    int col;
};

class Game{
    public:

    Game();
    Game(vector<vector<char>> board);

    vector<vector<char>> Make_move(Move move/*, bool& success*/);

    Move Undo_move();

    void setPosition(vector<vector<char>> newPos);

    vector<Move> getMoves();

    bool game_over = false;

    private:
    vector<vector<char>> board;
    
    vector<Move> history;

    // converts column to excel-styled character
    // i.e. 0 = A, 1 = B, ... 25 = Z, 26 = AA, 27 = AB, etc
    Move CoordToMove(int col, int row);

    // does the inverse of the above
    void MoveToCoord(int& col, int& row, Move move);

    bool check_game_over();

    bool is_raid_legal(Move move);

    // helper function for checking legality of move
    bool check_near(Move move, char player);

    // a slower version of check_near since it gets all squares near it
    vector<Move> get_raided_squares(Move move);

    void print_vec(vector<Move> vec);
};
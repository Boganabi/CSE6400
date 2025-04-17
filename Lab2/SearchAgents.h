
#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include "Game.h"

using namespace std;

class SearchAgents{
    public:

    // constructors
    SearchAgents();
    SearchAgents(vector<vector<char>> board);

    void FindBestMove(string& move, string& moveType, vector<vector<char>>& boardState, vector<vector<int>> board, int depth, string mode, char player);

    private:

    // handles both minimax and alpha/beta since search strategies are similar
    Move Search(vector<vector<int>> board, vector<vector<char>> state, /*Game& gameCopy,*/ int depth, char player, bool isMaximizing, bool isABSearch, int alpha, int beta);

    Move maxim(vector<vector<int>> board, vector<vector<char>> state, int depth, char player, bool isABSearch, int alpha, int beta);
    Move minim(vector<vector<int>> board, vector<vector<char>> state, int depth, char player, bool isABSearch, int alpha, int beta);

    int evaluate(vector<vector<int>> board, vector<vector<char>> state, char player);

    // to keep track of the game
    Game game;

};
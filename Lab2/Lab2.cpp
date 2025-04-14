
// reads in file input and finds optimal move, then outputs to text file
#include <iostream>
#include <string>
#include <vector>
#include "Lab2FileManip.h"
#include "SearchAgents.h"

using namespace std;

int main(){

    string mode; // either minimax or alpha/beta
    char player; // either "x" or "o"
    int depth; // how far we search
    // we read in cell values and board state later since we need to know n
    vector<vector<int>> board_values;
    vector<vector<char>> board_state;

    Lab2FileManip::read_file(mode, player, depth, board_values, board_state);

    // find the best move with minimax stuff
    SearchAgents searcher = SearchAgents(board_state);
    string outMove;
    string outType;
    searcher.FindBestMove(outMove, outType, board_state, board_values, depth, mode, player);

    // write output
    Lab2FileManip::write_output(outMove, outType, board_state);
    
    return 0;
}
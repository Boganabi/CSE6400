
#include "SearchAgents.h"

SearchAgents::SearchAgents(){
    game = Game();
}

SearchAgents::SearchAgents(vector<vector<char>> board){
    game = Game(board);
}

void SearchAgents::FindBestMove(string& move, string& moveType, vector<vector<char>>& boardState, vector<vector<int>> board, int depth, string mode, char player){

    Move bestmove = Search(board, boardState, depth, player, true, mode == "ALPHABETA", INT_MIN, INT_MAX);
    move = bestmove.move;
    moveType = bestmove.moveType;
    boardState = game.Make_move(bestmove);
    // output is now passed to be written in text file
}

Move SearchAgents::Search(vector<vector<int>> board, vector<vector<char>> state, int depth, char player, bool isMaximizing, bool isABSearch, int alpha, int beta){
    Move tempmove;
    tempmove.player = player;
    // terminal test
    if(depth <= 0 || game.game_over){
        player = isMaximizing ? player : game.getOtherPlayer(player); 
        int eval = evaluate(board, state, player);
        tempmove.evaluation = eval;
        return tempmove;
    }
    vector<Move> moves = game.getMoves(player);
    int score = isMaximizing ? INT_MIN : INT_MAX;
    for(Move m : moves){
        m.player = player;
        state = game.Make_move(m);
        char newplayer = game.getOtherPlayer(player); // player == 'X' ? 'O' : 'X';
        Move result = Search(board, state, depth - 1, newplayer, !isMaximizing, isABSearch, alpha, beta);
        m.evaluation = result.evaluation;
        if((isMaximizing && result.evaluation > score) || (!isMaximizing && result.evaluation < score)){ // checks if we need to update score
            score = result.evaluation;
            tempmove = m;
            tempmove.evaluation = result.evaluation;
        }
        if(isABSearch){ // if we are using alpha/beta pruning
            if ((isMaximizing && score >= beta) || (!isMaximizing && score <= alpha)){ // checks if we need to prune
                game.Undo_move();
                return tempmove; // prune
            }
            if(isMaximizing){
                alpha = max(alpha, score);
            }
            else{
                beta = min(beta, score);
            }
        }
        game.Undo_move();
    }
    return tempmove;
}

int SearchAgents::evaluate(vector<vector<int>> board, vector<vector<char>> state, char player){
    // cout << "evaluating board: " << endl;
    int eval = 0;
    for(int i = 0; i < state.size(); i++){
        for(int j = 0; j < state[i].size(); j++){
            if(state[i][j] == '.'){
                continue;
            }
            if(state[i][j] == player){
                eval += board[i][j];
            }
            else{
                eval -= board[i][j];
            }
        }
        // cout << endl;
    }
    return eval;
}

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
    // bool success = false;
    boardState = game.Make_move(bestmove/*, success*/);
    // if(!success){
    //     cout << "Best move was illegal: " << move << endl;
    // }
    cout << "Evaluation: " << bestmove.evaluation << endl;
}

Move SearchAgents::Search(vector<vector<int>> board, vector<vector<char>> state, int depth, char player, bool isMaximizing, bool isABSearch, int alpha, int beta){
    Move tempmove;
    tempmove.player = player;
    if(depth <= 0 || game.game_over){
        int eval = evaluate(board, state, player);
        tempmove.evaluation = eval;
        return tempmove;
    }
    vector<Move> moves = game.getMoves();
    int score = isMaximizing ? INT_MIN : INT_MAX;
    for(Move m : moves){
        m.player = player;
        state = game.Make_move(m);
        char newplayer = player == 'X' ? 'O' : 'X';
        tempmove = Search(board, state, depth - 1, newplayer, !isMaximizing, isABSearch, alpha, beta);
        if((isMaximizing && tempmove.evaluation > score) || (!isMaximizing && tempmove.evaluation < score)){ // checks if we need to update score
            score = tempmove.evaluation;
            tempmove.move = m.move;
            tempmove.moveType = m.moveType;
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
    }
    return eval;
}
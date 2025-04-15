
#include "SearchAgents.h"

SearchAgents::SearchAgents(){
    game = Game();
}

SearchAgents::SearchAgents(vector<vector<char>> board){
    game = Game(board);
}

void SearchAgents::FindBestMove(string& move, string& moveType, vector<vector<char>>& boardState, vector<vector<int>> board, int depth, string mode, char player){

    Move bestmove = Search(board, boardState, game, depth, player, true, mode == "ALPHABETA", INT_MIN, INT_MAX);
    cout << "got best move" << endl;
    move = bestmove.move;
    moveType = bestmove.moveType;
    // bool success = false;
    boardState = game.Make_move(bestmove/*, success*/);
    // if(!success){
    //     cout << "Best move was illegal: " << move << endl;
    // }
    cout << "Evaluation: " << bestmove.evaluation << endl;
}

Move SearchAgents::Search(vector<vector<int>> board, vector<vector<char>> state, Game gameCopy, int depth, char player, bool isMaximizing, bool isABSearch, int alpha, int beta){
    cout << "searching depth " << depth << endl;
    Move tempmove;
    tempmove.move = "blank";
    tempmove.moveType = "blank";
    tempmove.row = -1;
    tempmove.col = -1;
    tempmove.evaluation = 0;
    tempmove.player = player;
    if(depth <= 0 || gameCopy.game_over){
        int eval = evaluate(board, state, player);
        tempmove.evaluation = eval;
        cout << "terminal" << endl;
        tempmove.print();
        return tempmove;
    }
    vector<Move> moves = gameCopy.getMoves(player);
    int score = isMaximizing ? INT_MIN : INT_MAX;
    for(Move m : moves){
        m.player = player;
        cout << "making move" << endl;
        state = gameCopy.Make_move(m);
        char newplayer = gameCopy.getOtherPlayer(player); // player == 'X' ? 'O' : 'X';
        Move result = Search(board, state, gameCopy, depth - 1, newplayer, !isMaximizing, isABSearch, alpha, beta);
        cout << "left lower depth" << endl;
        // tempmove = Search(board, state, gameCopy, depth - 1, newplayer, !isMaximizing, isABSearch, alpha, beta);
        if((isMaximizing && result.evaluation > score) || (!isMaximizing && result.evaluation < score)){ // checks if we need to update score
        // if((isMaximizing && tempmove.evaluation > score) || (!isMaximizing && tempmove.evaluation < score)){ // checks if we need to update score
            // score = tempmove.evaluation;
            score = result.evaluation;
            // tempmove.move = m.move;
            // tempmove.moveType = m.moveType;
            tempmove = m;
            tempmove.evaluation = result.evaluation;
        }    
        if(isABSearch){ // if we are using alpha/beta pruning
            if ((isMaximizing && score >= beta) || (!isMaximizing && score <= alpha)){ // checks if we need to prune
                gameCopy.Undo_move();
                return tempmove; // prune
            }
            if(isMaximizing){
                alpha = max(alpha, score);
            }
            else{
                beta = min(beta, score);
            }
        }
        cout << "undoing move" << endl;
        gameCopy.Undo_move();
    }
    cout << "exiting layer: " << depth << endl;
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
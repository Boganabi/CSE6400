
#include "SearchAgents.h"

SearchAgents::SearchAgents(){
    game = Game();
}

SearchAgents::SearchAgents(vector<vector<char>> board){
    game = Game(board);
}

void SearchAgents::FindBestMove(string& move, string& moveType, vector<vector<char>>& boardState, vector<vector<int>> board, int depth, string mode, char player){

    cout << "searching depth " << depth << endl;
    Move bestmove = Search(board, boardState, /*game,*/ depth, player, true, mode == "ALPHABETA", INT_MIN, INT_MAX);
    // Move bestmove = maxim(board, boardState, depth, player, mode == "ALPHABETA", INT_MIN, INT_MAX);
    move = bestmove.move;
    moveType = bestmove.moveType;
    cout << "got best move: " << move << " (" << moveType << ")" << endl;
    // bool success = false;
    boardState = game.Make_move(bestmove/*, success*/);
    // if(!success){
    //     cout << "Best move was illegal: " << move << endl;
    // }
    cout << "Evaluation: " << bestmove.evaluation << endl;
}

Move SearchAgents::maxim(vector<vector<int>> board, vector<vector<char>> state, int depth, char player, bool isABSearch, int alpha, int beta){
    // terminal test
    if(depth < 0 || game.game_over){
        Move tempmove;
        // player = isMaximizing ? player : game.getOtherPlayer(player);
        int eval = evaluate(board, state, player);
        tempmove.evaluation = eval;
        return tempmove; // since only the evaluation is necessary on this layer
    }
    int score = INT_MIN;
    Move bestMove;
    vector<Move> moves = game.getMoves(player);
    for(Move m : moves){
        if(depth == 2){
            cout << "searching tree with move " << m.move << endl;
        }
        m.player = player;
        state = game.Make_move(m);
        char otherPlayer = game.getOtherPlayer(player);
        Move result = minim(board, state, depth - 1, otherPlayer, isABSearch, alpha, beta);
        if(result.evaluation > score){
            score = result.evaluation;
            bestMove = m;
            bestMove.evaluation = score;
        }
        if(depth == 2){
            cout << "move " << m.move << " evaluated at " << result.evaluation << endl;
            game.print_board();
        }
        if(isABSearch){
            if(score >= beta){
                game.Undo_move();
                return bestMove;
            }
            alpha = max(alpha, score);
        }
        game.Undo_move();
    }
    // cout << "best move from depth " << depth << " " << bestMove.move << endl;
    return bestMove;
}

Move SearchAgents::minim(vector<vector<int>> board, vector<vector<char>> state, int depth, char player, bool isABSearch, int alpha, int beta){
    // terminal test
    if(depth < 0 || game.game_over){
        Move tempmove;
        // player = isMaximizing ? player : game.getOtherPlayer(player);
        int eval = evaluate(board, state, player);
        tempmove.evaluation = eval;
        return tempmove;
    }
    int score = INT_MAX;
    Move bestMove;
    vector<Move> moves = game.getMoves(player);
    for(Move m : moves){
        m.player = player;
        state = game.Make_move(m);
        char otherPlayer = game.getOtherPlayer(player);
        Move result = maxim(board, state, depth - 1, otherPlayer, isABSearch, alpha, beta);
        if(result.evaluation < score){
            score = result.evaluation;
            bestMove = m;
            bestMove.evaluation = score;
        }
        if(isABSearch){
            if(score <= alpha){
                game.Undo_move();
                return bestMove;
            }
            beta = min(beta, score);
        }
        game.Undo_move();
    }
    return bestMove;
}

Move SearchAgents::Search(vector<vector<int>> board, vector<vector<char>> state, /*Game& gameCopy,*/ int depth, char player, bool isMaximizing, bool isABSearch, int alpha, int beta){
    Move tempmove;
    tempmove.player = player;
    // terminal test
    if(depth < 0 || game.game_over){
        // player = isMaximizing ? player : game.getOtherPlayer(player);
        int eval = evaluate(board, state, player);
        tempmove.evaluation = eval;
        return tempmove;
    }
    vector<Move> moves = game.getMoves(player);
    // game.print_vec(moves);
    string indent = "";
    int debugindent = depth;
    while(debugindent <= 2){
        indent += " ";
        debugindent++;
    }
    vector<Move> evaledMoves;
    // cout << indent << "moves at depth " << depth << ": ";
    // for(Move mtest : moves){
    //     cout << mtest.move << " ";
    // }
    // cout << endl;
    // if(depth == 0){
    //     for(Move debugmove : moves){
    //         // cout << indent;
    //         cout << debugmove.move << " ";
    //     }
    //     cout << endl;
    // }
    int score = isMaximizing ? INT_MIN : INT_MAX;
    for(Move m : moves){
        // vector<vector<char>> prevstate = state;
        if(depth == 2){
            cout << "searching tree with move " << m.move << endl;
        }
        m.player = player;
        state = game.Make_move(m);
        // game.Make_move(m);
        // game.setPosition(state);
        char newplayer = game.getOtherPlayer(player); // player == 'X' ? 'O' : 'X';
        Move result = Search(board, state, /*gameCopy,*/ depth - 1, newplayer, !isMaximizing, isABSearch, alpha, beta);
        m.evaluation = result.evaluation;
        if((isMaximizing && result.evaluation > score) || (!isMaximizing && result.evaluation < score)){ // checks if we need to update score
            // if(depth == 1) cout << indent << "accepted" << endl;
            // cout << "for player " << player << " depth " << depth << " setting best move to ";
            // m.print();
            score = result.evaluation;
            tempmove = m;
            tempmove.evaluation = result.evaluation;
            // tempmove.print();
        }
        // else{
        //     if(depth == 1) cout << indent << "rejected" << endl;
        // }
        // if(depth == 1){
        //     cout << indent << "evaluated move: ";
        //     result.print();
        //     evaledMoves.push_back(m);
        // }
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
        if(depth == 2){
            cout << "move " << m.move << " evaluated at " << result.evaluation << endl;
            game.print_board();
        }
        game.Undo_move();
    }
    // cout << indent << "returning best move: ";
    // tempmove.print();
    if(evaledMoves.size() > 0){
        cout << "moves evaluated: ";
        for(Move e: evaledMoves){
            cout << e.move << " (" << e.evaluation << ") " ;
        }
        cout << endl;
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
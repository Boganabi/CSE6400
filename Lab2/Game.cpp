
#include "Game.h"

Game::Game(){
    board = vector<vector<char>>();
}

Game::Game(vector<vector<char>> board){
    setPosition(board);
}

void Game::setPosition(vector<vector<char>> newPos){
    board = newPos;
}

bool Game::check_legal(Move move){
    if(board[move.row][move.col] == '.'){
        if(move.row >= 0 && move.row < board.size() && move.col >= 0 && move.col < board[move.row].size()){
            return true;
        }
    }
    
    return false;
}

vector<vector<char>> Game::Make_move(Move move/*, bool& success*/){
    // int r, c = -1;
    // MoveToCoord(c, r, move);
    // if(c != -1 && r != -1){
    //     board[r][c] = move.player; // if column is not represented by letter then swap these
    //     success = true;
    // }
    // cout << "before move:" << endl;
    // print_board();
    if(move.row >= 0 && move.row < board.size() && move.col >= 0 && move.col < board[move.row].size() && board[move.row][move.col] == '.'){
        board[move.row][move.col] = move.player; // if column is not represented by letter then swap these
        // do special action on raid
        if(move.moveType == "Raid"){
            vector<Move> raided = get_raided_squares(move);
            for(Move m : raided){
                board[m.row][m.col] = move.player;
                // m.raidedSquares.push_back({m.row, m.col});
                move.raidedSquares.push_back(pair<int, int>(m.row, m.col));
            }
            // cout << "Raided squares stored in move: ";
            // for (auto& p : move.raidedSquares) {
            //     cout << "(" << p.first << ", " << p.second << ") ";
            // }
            // cout << endl;
        }
        history.push_back(move);
        game_over = check_game_over();
    }
    else{
        cout << "skipping move with row : " << move.row << " col: " << move.col << endl;
    }

    // cout << "after move " << move.move << " (" << move.moveType << ")" << endl;
    // print_board();
    return board;
}

Move Game::Undo_move(){
    // cout << "before undo:" << endl;
    // print_board();
    if(history.size() > 0){
        Move last_move = history.back();
        history.pop_back();
        board[last_move.row][last_move.col] = '.'; // resets space we moved to
        // undo special action on raid
        if(last_move.moveType == "Raid"){
            // for(Move m : get_raided_squares(last_move)){
            //     board[m.row][m.col] = '.';
            // }
            for(pair<int, int> coord : last_move.raidedSquares){
                board[coord.first][coord.second] = getOtherPlayer(last_move.player);
            }
        }
        // cout << "after undo " << last_move.move << " (" << last_move.moveType << ")" << endl;
        // print_board();
        return last_move;
    }
    return Move();
}

vector<Move> Game::getMoves(char player){
    vector<Move> moves;
    vector<Move> raidMoves; // separate vector so that raid moves can be added to end
    // search backward so that moves are added in right order
    // for(int i = 0; i < board.size(); i++){
    for(int i = board.size() - 1; i >= 0; i--){
        // for(int j = 0; j < board[i].size(); j++){
        for(int j = board[i].size() - 1; j >= 0; j--){
            // cout << "board at " << i << ", " << j << " has " << board[i][j] << endl;
            if(board[i][j] == '.'){ 
                Move m = CoordToMove(j, i); // flipped bc columns were being calculated wrong before
                // cout << "adding move: " << m.move << endl;
                m.player = player;
                m.moveType = "Stake";
                // m.evaluation = 0;
                moves.insert(moves.begin(), m); // so that stakes are evaluated first
                if(is_raid_legal(m)){
                    Move raid = m;
                    raid.moveType = "Raid";

                    // hash to debug
                    // size_t hash = 0;
                    // for (auto& p : raid.raidedSquares) {
                    //     hash ^= std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
                    // }
                    // cout << "Hash of raid.raidedSquares: " << hash << endl;
                    raidMoves.push_back(raid);
                }
            }
        }
    }
    moves.insert(moves.end(), raidMoves.begin(), raidMoves.end());
    // cout << "board state:" << endl;
    // print_board();
    return moves;
}

Move Game::CoordToMove(int col, int row){
    string newCol;
    int copyCol = col;
    while(copyCol >= 0){
        char newChar = static_cast<char>((copyCol % 26) + 65); // does the funky little translation
        newCol.push_back(newChar);
        if(copyCol >= 26){
            copyCol -= 26;
        }
        else{
            break;
        }
    }

    string newRow = to_string(row + 1);

    Move madeMove;//(row, col);
    madeMove.move = newCol + newRow;
    madeMove.row = row;
    madeMove.col = col;

    if(row < 0 || row >= board.size() || col < 0 || col >= board[row].size()){
        cout << "invalid pair attempted to add: " << row << " " << col << endl;
    }

    return madeMove;
}

// void Game::MoveToCoord(int& col, int& row, Move move){
//     string moveToConvert = move.move;
//     for(char c : moveToConvert){
//         if(c > 64){
//             // an alphabetical symbol representing column
//             col += c % 65;
//         }
//         else{
//             // a number representing row
//             row += c - 48;
//         }
//     }
//     cout << "column: " << col << " row: " << row << endl;
// }

bool Game::check_game_over(){
    for(vector<char> v : board){
        for(char c : v){
            if(c == '.'){
                return false;
            }
        }
    }
    return true;
}

bool Game::is_raid_legal(Move move){
    // to raid, there must be an adjacent tile with player and adjacent tile with opponent
    // the square must also be unoccupied, but since that is already checked we can skip that check
    if(check_near(move, 'X')){ // optimized version
        return check_near(move, 'O');
    }
    return false;
}

bool Game::check_near(Move move, char player){
    if(move.col - 1 >= 0 && board[move.row][move.col - 1] == player){ // check up
        return true;
    }
    if(move.row - 1 >= 0 && board[move.row - 1][move.col] == player){ // check left
        return true;
    }
    if(move.col + 1 < board[move.row].size() && board[move.row][move.col + 1] == player){ // check down
        return true;
    }
    if(move.row + 1 < board.size() && board[move.row + 1][move.col] == player){ // check right
        return true;
    }
    return false;
}

vector<Move> Game::get_raided_squares(Move move){
    vector<Move> squares;
    char otherPlayer = getOtherPlayer(move.player); //move.player == 'X' ? 'O' : 'X';
    if(move.col - 1 >= 0 && board[move.row][move.col - 1] == otherPlayer){ // check up
        Move tempmove;
        tempmove.col = move.col - 1;
        tempmove.row = move.row;
        squares.push_back(tempmove);
    }
    if(move.row - 1 >= 0 && board[move.row - 1][move.col] == otherPlayer){ // check left
        Move tempmove;
        tempmove.col = move.col;
        tempmove.row = move.row - 1;
        squares.push_back(tempmove);
    }
    if(move.col + 1 < board[move.row].size() && board[move.row][move.col + 1] == otherPlayer){ // check down
        Move tempmove;
        tempmove.col = move.col + 1;
        tempmove.row = move.row;
        squares.push_back(tempmove);
    }
    if(move.row + 1 < board.size() && board[move.row + 1][move.col] == otherPlayer){ // check right
        Move tempmove;
        tempmove.col = move.col;
        tempmove.row = move.row + 1;
        squares.push_back(tempmove);
    }
    return squares;
}

void Game::print_vec(const vector<Move>& vec){
    for(Move item : vec){
    // for(int i = 0; i < vec.size(); i++){
        // cout << "curr i " << i << endl;
        // cout << "max size " << vec.size() << endl;
        // Move item = vec[i];
        cout << item.row << " " << item.col << " " << endl;
        cout << "move string: " << item.move << endl;
        cout << "moveType: " << item.moveType << endl;
        cout << "evaluation: " << item.evaluation << endl;
        cout << "player: " << item.player << endl;
        // cout << "raidedSquares size: " << item.raidedSquares.size() << endl;
        // for (auto& p : item.raidedSquares) {
        //     cout << "(" << p.first << "," << p.second << ") ";
        // }
        cout << endl;
    }
    cout << endl;
}

void Game::print_board(){
    for(vector<char> row : board){
        for(char c : row){
            cout << c;
        }
        cout << endl;
    }
}
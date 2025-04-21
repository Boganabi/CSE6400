
#include "Lab2FileManip.h"

void Lab2FileManip::read_file(string& mode, char& player, int& depth, vector<vector<int>>& board_values, vector<vector<char>>& board_state){
    ifstream inputFile ("input.txt");
    if(inputFile.is_open()){
        int n; // the size of the board
        string tempnum;
        getline(inputFile, tempnum);
        safe_cast_to_int(tempnum, n);

        getline(inputFile, mode);
        getline(inputFile, tempnum);
        player = tempnum[0];

        getline(inputFile, tempnum);
        safe_cast_to_int(tempnum, depth);

        if(n){
            
            // read in the board values
            string templine;
            for(int i = 0; i < n; i++){
                board_values.push_back(vector<int>()); // so that i is accessible
                getline(inputFile, templine);
                stringstream ss(templine);
                string tempnum;
                while(ss >> tempnum){
                    int square_value;
                    safe_cast_to_int(tempnum, square_value);
                    board_values[i].push_back(square_value);
                }
            }
            
            // read in board state
            string line;
            for(int i = 0; i < n; i++){
                getline(inputFile, line);
                board_state.push_back(vector<char>()); // so that i is accessible
                for(char c : line){
                    if(c != '\r'){ // the carriage return likes to mess up lots of things
                        board_state[i].push_back(c);
                    }
                }
            }
        }
        else{
            cout << "Invalid input for N detected." << endl;
        }
        inputFile.close();
    }
    else{
        cout << "There was an error opening input.txt" << endl;
    }
}

void Lab2FileManip::safe_cast_to_int(string start, int& result){
    int target;

    // convert given string to int, and exit if failed
    try{
        target = stoi(start);
        result = target;
    }
    catch(const std::invalid_argument& e){
        std::cerr << "The integer input passed contains non-numerical characters. " << e.what() << endl;
    }
    catch(const std::out_of_range& e){
        cerr << "The integer input passed is too large for int type. " << e.what() << endl;
    }

    
}

void Lab2FileManip::write_output(string move, string type, vector<vector<char>> state){
    ofstream outfile ("output.txt");
    if(outfile.is_open()){
        outfile << move << " " << type << "\n";
        for(vector<char> vc : state){
            for(char c : vc){
                outfile << c;
            }
            outfile << "\n";
        }
        outfile.close();
    }
    else{
        cout << "Unable to write to output.txt" << endl;
    }
}

template<typename T>
void Lab2FileManip::debug_print_board(vector<vector<T>> board){
    for(vector<T> line : board){
        for(T s : line){
            cout << s << " ";
        }
        cout << endl;
    }
}

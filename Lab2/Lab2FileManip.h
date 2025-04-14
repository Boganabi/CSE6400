
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std; 

class Lab2FileManip{
    private:

    template<typename T>
    static void debug_print_board(vector<vector<T>> board);

    public:
    static void safe_cast_to_int(string start, int& result);

    static void read_file(string& mode, char& player, int& depth, vector<vector<int>>& board_values, vector<vector<char>>& board_state);

    static void write_output(string move, string type, vector<vector<char>> state);

};

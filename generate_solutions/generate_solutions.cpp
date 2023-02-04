
#include <iostream>
#include <fstream>
#include <string>
#include "../gameofsudoku.h"

using namespace std;

int main(int argc, char**argv)
{
    GameOfSudoku game;

    ifstream input(argv[1]);
    ofstream output(argv[2]);

    int count = 0;

    string sudoku_def;
    while (input >> sudoku_def) {
        game.readShort(sudoku_def);
        cout << "solving " << count++ << flush;
        std::vector<GameOfSudoku::GridData> solution;
        solution.reserve(1);
        game.solve(solution);
        if (solution.size() != 1)
        {
            throw std::runtime_error("sudoku does not have exactly one solution");
        }
        cout << " solved\n";

        output << game.printShort(solution[0]) << "\n";
    }


    return 0;
}

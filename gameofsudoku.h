#ifndef GAMEOFSUDOKU_H
#define GAMEOFSUDOKU_H

#include <QString>

#include <array>
#include <functional>


class GameOfSudoku
{
public:
    using GridData = std::array<unsigned short, 9*9>;

    GameOfSudoku();
    void generateBoard();
    void solve(std::function<void(int, int, GridData::value_type)> tryCallback
               = [](int, int, GridData::value_type){});
    void print();
    void read(const QString & board);

    static void print(const GridData & grid);

    GridData::value_type at(int row, int col) const;
    GridData::value_type &at(int row, int col);

    bool rowHasValue(int row, GridData::value_type value) const;
    bool colHasValue(int col, GridData::value_type value) const;
    bool blockHasValue(int row, int col, GridData::value_type value) const;

private:
    void makeFullRandom();

    static bool isFull(const GridData & grid);
    static bool rowHasValue(int row, GridData::value_type value, const GridData & grid);
    static bool colHasValue(int col, GridData::value_type value, const GridData & grid);
    static bool blockHasValue(int row, int col, GridData::value_type value, const GridData & grid);

    static bool solve(GridData & grid, std::function<void(int, int, GridData::value_type)> tryCallback
                      = [](int, int, GridData::value_type){});

private:
    GridData m_grid;
};

#endif // GAMEOFSUDOKU_H

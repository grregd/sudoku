#ifndef GAMEOFSUDOKU_H
#define GAMEOFSUDOKU_H

#include <QString>

#include <array>
#include <functional>


class GameOfSudoku
{
public:
    using GridData = std::array<unsigned short, 9*9>;
    using TryCallbackType = std::function<void(GridData::size_type, GridData::size_type, GridData::value_type)>;

    GameOfSudoku();
    void generateBoard();
    void solve(TryCallbackType tryCallback
               = [](int, int, GridData::value_type){});
    void print();
    void read(const QString & board);

    static void print(const GridData & grid);

    GridData::value_type at(GridData::size_type row, GridData::size_type col) const;
    GridData::value_type &at(GridData::size_type row, GridData::size_type col);

    bool rowHasValue(GridData::size_type row, GridData::value_type value) const;
    bool colHasValue(GridData::size_type col, GridData::value_type value) const;
    bool blockHasValue(GridData::size_type row, GridData::size_type col, GridData::value_type value) const;

private:
    void makeFullRandom();

    static bool isFull(const GridData & grid);
    static bool rowHasValue(GridData::size_type row, GridData::value_type value, const GridData & grid);
    static bool colHasValue(GridData::size_type col, GridData::value_type value, const GridData & grid);
    static bool blockHasValue(GridData::size_type row, GridData::size_type col, GridData::value_type value, const GridData & grid);

    static bool solve(GridData & grid, TryCallbackType tryCallback
                      = [](int, int, GridData::value_type){});

private:
    GridData m_grid;
};

#endif // GAMEOFSUDOKU_H

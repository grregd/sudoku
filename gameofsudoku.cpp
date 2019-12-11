#include "gameofsudoku.h"

#include <QStringList>

#include <QDebug>

GameOfSudoku::GameOfSudoku()
{
    m_grid.fill(0);
    m_grid[0] = 9;
}

void GameOfSudoku::generateBoard()
{

}

void GameOfSudoku::solve(std::function<void (int, int, GridData::value_type)> tryCallback)
{
    solve(m_grid, tryCallback);
}

void GameOfSudoku::print()
{
    print(m_grid);
}

void GameOfSudoku::read(const QString &board)
{
    QStringList values = board.split(" ", QString::SkipEmptyParts);
    int cellNum = 0;
    while (!values.empty())
    {
        m_grid[cellNum] = values.front().toInt();
        values.pop_front();
        ++cellNum;
    }
}

void GameOfSudoku::print(const GameOfSudoku::GridData &grid)
{
    for (auto r = 0; r < 9; ++r)
    {
        QString rowStr;
        for (auto c = 0; c < 9; ++c)
        {
            if (grid[r*9 + c] != 0)
                rowStr += QString(" %1 ").arg((int)grid[r*9 + c]);
            else
                rowStr += " _ ";
        }
        qDebug() << rowStr;
    }
}

GameOfSudoku::GridData::value_type GameOfSudoku::at(int row, int col) const
{
    return m_grid[row*9+col];
}

bool GameOfSudoku::rowHasValue(int row, GridData::value_type value) const
{
    return GameOfSudoku::rowHasValue(row, value, m_grid);
}

bool GameOfSudoku::colHasValue(int col, GridData::value_type value) const
{
    return GameOfSudoku::colHasValue(col, value, m_grid);
}

bool GameOfSudoku::blockHasValue(int row, int col, GridData::value_type value) const
{
    return GameOfSudoku::blockHasValue(row, col, value, m_grid);
}

void GameOfSudoku::makeFullRandom()
{

}

bool GameOfSudoku::isFull(const GridData &grid)
{
    return std::end(grid) == std::find(std::begin(grid), std::end(grid), 0);
}

bool GameOfSudoku::rowHasValue(int row, GridData::value_type value, const GridData &grid)
{
    for (int col = 0; col < 9; ++col)
    {
        if (grid[row*9+col] == value)
        {
            return true;
        }
    }

    return false;
}

bool GameOfSudoku::colHasValue(int col, GridData::value_type value, const GridData &grid)
{
    for (int row = 0; row < 9; ++row)
    {
        if (grid[row*9+col] == value)
        {
            return true;
        }
    }
    return false;
}

bool GameOfSudoku::blockHasValue(int row, int col, GridData::value_type value, const GridData &grid)
{
    auto rowOffs = row/3*3;
    auto colOffs = col/3*3;

//    qDebug() << __FUNCTION__ << row << col << rowOffs << colOffs;

    for (auto r = 0; r < 3; ++r)
    {
        for (auto c = 0; c < 3; ++c)
        {
//            qDebug() << (rowOffs+r) << colOffs+c;
            if (grid[9*(rowOffs+r)+colOffs+c] == value)
            {
                return true;
            }
        }
    }
    return false;
}

static std::list<GameOfSudoku::GridData> solutions;
bool GameOfSudoku::solve(GridData &grid, std::function<void(int, int, GridData::value_type)> tryCallback)
{
    int cellNum = 0;
    for (; cellNum < 9*9; ++cellNum)
    {
        auto row = cellNum/9;
        auto col = cellNum%9;

        if (grid[cellNum] == 0)
        {
            for (GridData::value_type value = 1; value <= 9; ++value)
            {
                if (!rowHasValue(row, value, grid) &&
                    !colHasValue(col, value, grid) &&
                    !blockHasValue(row, col, value, grid))
                {
                    tryCallback(row, col, value);
                    grid[cellNum] = value;
                    if (isFull(grid))
                    {
                        solutions.push_back(grid);
                        qDebug() << "found soluiton number" << solutions.size();
                        return true;
//                        break;
                    }
                    else
                    {
                        if (solve(grid, tryCallback))
                        {
                            return true;
                        }
                    }
                }
            }

            break;
        }
    }

    tryCallback(cellNum/9, cellNum%9, 0);
    grid[cellNum] = 0;

    return false;
}


#include "gameofsudoku.h"

#include <random>

#include <QStringList>

#include <QDebug>

GameOfSudoku::GameOfSudoku()
{
    m_grid.fill(0);
}

void GameOfSudoku::generateBoard()
{
    m_grid.fill(0);
    solve();

    std::vector<GridData::size_type> toRemove(81);
    std::iota(toRemove.begin(), toRemove.end(), 1);
    std::random_device rd;
    std::mt19937 g(rd());
    g.seed(time(nullptr));
    std::shuffle(toRemove.begin(), toRemove.end(), g);

    auto toLeave = 34;
    toRemove.erase(toRemove.end()-toLeave, toRemove.end());
    for (auto index: toRemove)
        m_grid[index] = 0;
}

void GameOfSudoku::solve(TryCallbackType tryCallback)
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
    GridData::size_type cellNum = 0;
    while (!values.empty())
    {
        m_grid[cellNum] = static_cast<GridData::value_type>(values.front().toInt());
        values.pop_front();
        ++cellNum;
    }
}

void GameOfSudoku::print(const GameOfSudoku::GridData &grid)
{
    for (GridData::size_type r = 0; r < 9; ++r)
    {
        QString rowStr;
        for (GridData::size_type c = 0; c < 9; ++c)
        {
            if (grid[r*9 + c] != 0)
                rowStr += QString(" %1 ").arg(grid[r*9 + c]);
            else
                rowStr += " _ ";
        }
        qDebug() << rowStr;
    }
}

GameOfSudoku::GridData::value_type GameOfSudoku::at(GridData::size_type row, GridData::size_type col) const
{
    return m_grid[row*9+col];
}

GameOfSudoku::GridData::value_type &GameOfSudoku::at(GridData::size_type row, GridData::size_type col)
{
    return m_grid[row*9+col];
}

bool GameOfSudoku::rowHasValue(GridData::size_type row, GridData::value_type value) const
{
    return GameOfSudoku::rowHasValue(row, value, m_grid);
}

bool GameOfSudoku::colHasValue(GridData::size_type col, GridData::value_type value) const
{
    return GameOfSudoku::colHasValue(col, value, m_grid);
}

bool GameOfSudoku::blockHasValue(GridData::size_type row, GridData::size_type col, GridData::value_type value) const
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

bool GameOfSudoku::rowHasValue(GridData::size_type row, GridData::value_type value, const GridData &grid)
{
    for (GridData::size_type col = 0; col < 9; ++col)
    {
        if (grid[row*9+col] == value)
        {
            return true;
        }
    }

    return false;
}

bool GameOfSudoku::colHasValue(GridData::size_type col, GridData::value_type value, const GridData &grid)
{
    for (GridData::size_type row = 0; row < 9; ++row)
    {
        if (grid[row*9+col] == value)
        {
            return true;
        }
    }
    return false;
}

bool GameOfSudoku::blockHasValue(GridData::size_type row, GridData::size_type col, GridData::value_type value, const GridData &grid)
{
    auto rowOffs = row/3*3;
    auto colOffs = col/3*3;

    for (GridData::size_type r = 0; r < 3; ++r)
    {
        for (GridData::size_type c = 0; c < 3; ++c)
        {
            if (grid[9*(rowOffs+r)+colOffs+c] == value)
            {
                return true;
            }
        }
    }
    return false;
}

//static std::list<GameOfSudoku::GridData> solutions;
bool GameOfSudoku::solve(GridData &grid, TryCallbackType tryCallback)
{
    GridData::size_type cellNum = 0;
    for (; cellNum < 9*9; ++cellNum)
    {
        auto row = cellNum/9;
        auto col = cellNum%9;

        if (grid[cellNum] == 0)
        {
            std::vector<GridData::value_type> values(9);
            std::iota(values.begin(), values.end(), 1);
            std::random_device rd;
            std::mt19937 g(rd());
            g.seed(time(nullptr));
            std::shuffle(values.begin(), values.end(), g);
            for (auto value: values)
            {
                if (!rowHasValue(row, value, grid) &&
                    !colHasValue(col, value, grid) &&
                    !blockHasValue(row, col, value, grid))
                {
                    tryCallback(row, col, value);
                    grid[cellNum] = value;
                    if (isFull(grid))
                    {
//                        solutions.push_back(grid);
//                        qDebug() << "found soluiton number" << solutions.size();
                        qDebug() << "found soluiton";
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


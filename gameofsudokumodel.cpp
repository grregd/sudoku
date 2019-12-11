#include "gameofsudokumodel.h"

#include <thread>

#include <QDebug>


GameOfSudokuModel::GameOfSudokuModel()
{
//    m_game.read(
//                " _ _ 2 3 _ _ _ 1 5 "
//                " _ _ _ _ 6 _ 9 _ _ "
//                " 1 _ _ 4 _ 9 _ _ _ "
//                " _ _ _ _ _ _ _ _ _ "
//                " 6 4 9 _ _ _ _ _ _ "
//                " _ _ _ _ 8 _ _ 7 2 "
//                " _ _ _ _ _ _ 1 _ _ "
//                " _ _ _ _ _ _ 3 9 6 "
//                " 7 5 3 _ _ _ _ _ _ "
//                );
    m_game.read(
                " 9 _ 4 6 _ 5 1 _ _ "
                " 2 _ _ _ _ _ _ _ _ "
                " 8 _ _ _ 1 _ _ _ _ "
                " _ _ _ 9 _ _ _ _ _ "
                " 6 2 7 _ _ _ _ 4 _ "
                " _ _ _ _ _ 3 _ _ 6 "
                " _ 1 _ _ 6 _ 4 2 _ "
                " 4 _ _ _ _ 7 _ 8 _ "
                " 7 _ _ 3 _ _ _ 1 _ "
                );
    m_gameOrigin = m_game;
}

int GameOfSudokuModel::rowCount(const QModelIndex &/*parent*/) const
{
    return 9;
}

int GameOfSudokuModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 9;
}

QVariant GameOfSudokuModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << __FUNCTION__ << index << role;
//    if (!index.isValid() || role != CellRole)
//        return QVariant();

    auto currentValue = m_game.at(index.row(), index.column());
    auto selectedValue = m_game.at(m_selectedCell%9, m_selectedCell/9);
    auto originalValue = m_gameOrigin.at(index.row(), index.column());

    if (role == TextColorRole)
    {
        if (currentValue == originalValue)
        {
            return m_textOriginColor;
        }
        else
        {
            return m_textColor;
        }
    }

    if (role == FillColorRole)
    {
        if (index.row() == m_selectedCell%9 ||
            index.column() == m_selectedCell/9)
        {
            return m_fillSelectedColor;
        }
        else if ((selectedValue != 0) &&
                 (currentValue != 0 ||
                 m_game.colHasValue(index.column(), selectedValue) ||
                 m_game.rowHasValue(index.row(), selectedValue) ||
                 m_game.blockHasValue(index.row(), index.column(), selectedValue)))
        {
            return m_fillSameValueColor;
        }
        else
        {
            return m_fillColor;
        }
    }

    return currentValue;
}

bool GameOfSudokuModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

Qt::ItemFlags GameOfSudokuModel::flags(const QModelIndex &index) const
{
    return Qt::NoItemFlags;
}

void GameOfSudokuModel::newBoard()
{

}

//std::thread t;

void GameOfSudokuModel::solve(/*std::function<void (int, int, GameOfSudoku::GridData::value_type)> tryCallback*/)
{
//    t = std::thread([this]()
    {
        m_game.solve(
//            [this](int row, int col, GameOfSudoku::GridData::value_type value)
//            {
//                static int counter = 0;
//                if (++counter%50 == 0)
//                {
//                    emit dataChanged(index(0, 0), index(9-1, 9-1));
//                    std::this_thread::sleep_for(std::chrono::microseconds(1000));
//                }
//            }
        );
    }
//    );
    emit dataChanged(index(0, 0), index(9-1, 9-1));
}

void GameOfSudokuModel::selectCell(const QVariant & value)
{
    qDebug() << __FUNCTION__ << value;
//    auto save = m_selectedCell;
    m_selectedCell = value.toInt();
    emit dataChanged(index(0, 0), index(9-1, 9-1));
}

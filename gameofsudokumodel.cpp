#include "gameofsudokumodel.h"

#include <thread>

#include <QFile>
#include <QDebug>

class IndexAdapter
{
public:
    IndexAdapter(const QModelIndex& taget)
        : m_target(taget)
    {
    }

    GameOfSudoku::GridData::size_type row() const
    {
        return static_cast<GameOfSudoku::GridData::size_type>(m_target.row());
    }

    GameOfSudoku::GridData::size_type column() const
    {
        return static_cast<GameOfSudoku::GridData::size_type>(m_target.column());
    }

private:
    QModelIndex m_target;
};

void GameOfSudokuModel::onhelpersVisibleChanged()
{
    emit dataChanged(index(0, 0), index(9-1, 9-1));
}

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
//    m_game.read(
//                " 9 _ 4 6 _ 5 1 _ _ "
//                " 2 _ _ _ _ _ _ _ _ "
//                " 8 _ _ _ 1 _ _ _ _ "
//                " _ _ _ 9 _ _ _ _ _ "
//                " 6 2 7 _ _ _ _ 4 _ "
//                " _ _ _ _ _ 3 _ _ 6 "
//                " _ 1 _ _ 6 _ 4 2 _ "
//                " 4 _ _ _ _ 7 _ 8 _ "
//                " 7 _ _ 3 _ _ _ 1 _ "
//                );
    newBoard();
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
    IndexAdapter indexAdapt(index);
    auto currentValue = m_game.at(indexAdapt.row(), indexAdapt.column());
    auto selectedValue = m_game.at(static_cast<GameOfSudoku::GridData::size_type>(m_selectedCell%9),
                                   static_cast<GameOfSudoku::GridData::size_type>(m_selectedCell/9));
    auto originalValue = m_gameOrigin.at(indexAdapt.row(), indexAdapt.column());
    auto correctValue = m_gameSolution.at(indexAdapt.row(), indexAdapt.column());

    if (role == TextColorRole)
    {
        if (currentValue == originalValue)
        {
            return m_textOriginColor;
        }
        else if (currentValue != correctValue)
        {
            return QColor(Qt::red);
        }
        else
        {
            return m_textColor;
        }
    }

    if (role == FillColorRole)
    {
        if (!m_helpersVisible &&
            index.row() == m_selectedCell%9 &&
            index.column() == m_selectedCell/9)
        {
            return m_fillSelectedColor;
        }
        else if (m_helpersVisible &&
                 (index.row() == m_selectedCell%9 ||
                 index.column() == m_selectedCell/9))
        {
            return m_fillSelectedColor;
        }
        else if (m_helpersVisible &&
                 (selectedValue != 0) &&
                 (currentValue != 0 ||
                 m_game.colHasValue(indexAdapt.column(), selectedValue) ||
                 m_game.rowHasValue(indexAdapt.row(), selectedValue) ||
                 m_game.blockHasValue(indexAdapt.row(), indexAdapt.column(), selectedValue)))
        {
            return m_fillSameValueColor;
        }
        else
        {
            return m_fillColor;
        }
    }

    if (role == SameValeRole)
    {
        return currentValue == selectedValue;
    }

    return currentValue;
}

bool GameOfSudokuModel::setData(const QModelIndex &/*index*/, const QVariant &/*value*/, int /*role*/)
{
    return false;
}

Qt::ItemFlags GameOfSudokuModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::NoItemFlags;
}

void GameOfSudokuModel::newBoard()
{
    srand(time(NULL));
    auto offset = 82*(rand()%999);
    {
        QFile file(":/1000sudoku_plain.txt");
        if (file.open(QFile::ReadOnly)) {
            QTextStream sudokus(&file);
            sudokus.seek(offset);
            QString s = sudokus.readLine();
            m_gameOrigin.readShort(s.toStdString());
        }
    }

    {
        QFile file(":/1000sudoku_solutions.txt");
        if (file.open(QFile::ReadOnly)) {
            QTextStream sudokus(&file);
            sudokus.seek(offset);
            QString s = sudokus.readLine();
            m_gameSolution.readShort(s.toStdString());
        }
    }
    m_game = m_gameOrigin;

//    m_game.generateBoard(20);

//    m_gameOrigin = m_game;
//    m_gameSolution = m_game;

//    std::vector<GameOfSudoku::GridData> solutions;
//    solutions.reserve(1);
//    m_gameSolution.solve(solutions);
//    if (solutions.size() != 1)
//    {
//        throw std::runtime_error("something went horribly wrong");
//    }
//    m_gameSolution = GameOfSudoku(solutions.front());

    emit dataChanged(index(0, 0), index(9-1, 9-1));
    emit numberOfRevealedChanged();
}

//std::thread t;

void GameOfSudokuModel::solve(/*std::function<void (int, int, GameOfSudoku::GridData::value_type)> tryCallback*/)
{
//    t = std::thread([this]()
    {
        std::vector<GameOfSudoku::GridData> solutions;
        m_game.solve(solutions
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
    emit numberOfRevealedChanged();
}

void GameOfSudokuModel::selectCell(const QVariant & indexValue)
{
    m_selectedCell = indexValue.toInt();
    emit dataChanged(index(0, 0), index(9-1, 9-1));
}

bool GameOfSudokuModel::showHint()
{
    auto row = static_cast<GameOfSudoku::GridData::size_type>(m_selectedCell%9);
    auto col = static_cast<GameOfSudoku::GridData::size_type>(m_selectedCell/9);

    if (m_game.at(row, col) != 0)
    {
        return false;
    }

    m_game.at(row, col) = m_gameSolution.at(row, col);

    emit dataChanged(index(row, col), index(row, col));
    emit numberOfRevealedChanged();

    return true;
}

void GameOfSudokuModel::insert(const QVariant &nativeText)
{
    auto row = static_cast<GameOfSudoku::GridData::size_type>(m_selectedCell%9);
    auto col = static_cast<GameOfSudoku::GridData::size_type>(m_selectedCell/9);

    if (m_gameOrigin.at(row, col) != 0)
    {
        return ;
    }

    m_game.at(row, col) = static_cast<GameOfSudoku::GridData::value_type>(nativeText.toInt());

    emit dataChanged(index(0, 0), index(9-1, 9-1));
    emit numberOfRevealedChanged();

    if (nativeText.toInt() != 0 &&
        nativeText.toInt() != m_gameSolution.at(row, col))
    {
        emit wrongTry();
    }

    if (m_game == m_gameSolution)
    {
        emit gameSolved();
    }
}

void GameOfSudokuModel::insertIfEmpty(const QVariant &nativeText)
{
    auto row = static_cast<GameOfSudoku::GridData::size_type>(m_selectedCell%9);
    auto col = static_cast<GameOfSudoku::GridData::size_type>(m_selectedCell/9);

    if ((nativeText.toInt() != 0) &&
        (m_game.at(row, col) != 0) &&
        (m_game.at(row, col) == m_gameSolution.at(row, col)))
    {
        return ;
    }

    insert(nativeText);
}

bool GameOfSudokuModel::hasAllNumbers(int number)
{
    return m_game.boardHasValue(
                static_cast<GameOfSudoku::GridData::value_type>(number));
}

unsigned GameOfSudokuModel::getNumberOfRevealed()
{
    using T = GameOfSudoku::GridData::size_type;

    unsigned result = 0u;
    for (T r = 0; r < 9; ++r)
        for (T c = 0; c < 9; ++c)
            if (m_game.at(r, c) == m_gameSolution.at(r, c))
                ++result;

    return result;
}

void GameOfSudokuModel::moveLeft()
{
    m_selectedCell = (m_selectedCell+81-9)%81;
    emit dataChanged(index(0, 0), index(9-1, 9-1));
}

void GameOfSudokuModel::moveRight()
{
    m_selectedCell = (m_selectedCell+81+9)%81;
    emit dataChanged(index(0, 0), index(9-1, 9-1));
}

void GameOfSudokuModel::moveUp()
{
    m_selectedCell+=81;
    m_selectedCell -= 1;
    if (m_selectedCell % 9 == 8)
        m_selectedCell += 9;
    m_selectedCell %= 81;

    emit dataChanged(index(0, 0), index(9-1, 9-1));
}

void GameOfSudokuModel::moveDown()
{
    m_selectedCell+=81;
    m_selectedCell += 1;
    if (m_selectedCell % 9 == 0)
        m_selectedCell -= 9;
    m_selectedCell %= 81;

    emit dataChanged(index(0, 0), index(9-1, 9-1));
}



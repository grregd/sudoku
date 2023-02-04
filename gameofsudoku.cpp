#include "gameofsudoku.h"

#include <algorithm>
#include <ctime>
#include <unordered_set>
#include <list>
#include <numeric>
#include <random>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>


#include <QDebug>

std::vector<GameOfSudoku::GridData::size_type> randomCells()
{
    std::vector<GameOfSudoku::GridData::size_type> result(81);
    std::iota(result.begin(), result.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    bool seeded [[maybe_unused]] = ((void)g.seed(time(nullptr)), true);
    std::shuffle(result.begin(), result.end(), g);

    return result;
}


class BoardGenerator {
public:
  using GridData = GameOfSudoku::GridData;

  const GridData &board() const { return m_grid; }

  bool generate(int cluesCount);

private:
  GridData::size_type selectRandomOccupied(const GridData &grid) const;
  bool tryGenerateTest(int toRemove);
  bool tryGenerate101computing(int toRemove);
  bool tryGenerate(int toRemove);
  bool tryGenerate0(int cluesCount);

private:
  GridData m_grid;
};

static std::vector<GameOfSudoku::GridData::size_type> HASH(81);
static std::list<GameOfSudoku::GridData::size_type> removed;

bool BoardGenerator::generate(int cluesCount) {
  removed.clear();
  HASH = randomCells();

  m_grid.fill(0);

  std::vector<GridData> solutions;
  GameOfSudoku::solve(m_grid, solutions);

  return tryGenerate(81 - cluesCount);
}

BoardGenerator::GridData::size_type BoardGenerator::selectRandomOccupied(
    const BoardGenerator::GridData &grid) const {
  std::random_device rd;
  std::mt19937 g(rd());
  g.seed(time(nullptr));

  auto result = g();
  while (grid[result % 81] == 0)
    result = g();

  return result;
}

template <class _Tnumber, class _Titerator>
bool next_combination(_Titerator const &_First, _Titerator const &_Last,
                      _Tnumber const &_Max //!< Upper bound. Not reachable
) {
  _Titerator _Current = _First;
  if (_Current == _Last) {
    return false;
  }
  *_Current += 1;
  if (*_Current < _Max) {
    return true;
  }
  _Titerator _Next = _Current + 1;
  if (_Next == _Last) {
    return false;
  }
  if (false == next_combination(_Next, _Last, _Max - 1)) {
    return false;
  }
  *_Current = *_Next + 1;
  return *_Current < _Max;
}

bool BoardGenerator::tryGenerateTest(int toRemove) {
  std::vector<int> sequence(toRemove);
  std::iota(sequence.begin(), sequence.end(), 0);
  std::reverse(sequence.begin(), sequence.end());
  do {
    decltype(m_grid) tmp{m_grid};

    for (auto number : sequence) {
      tmp[HASH[number]] = 0;
    }

    //        GameOfSudoku::print(tmp);

    std::vector<GridData> solutions;
    solutions.reserve(1);
    try {
      GameOfSudoku::solve(tmp, solutions);
      if (solutions.size() == 1)
        return true;
    } catch (GameOfSudoku::MaxNumberOfSolutionExceeded &e) {
        qDebug() << "GameOfSudoku::MaxNumberOfSolutionExceeded: " << e.what();
    }
  } while (next_combination(sequence.begin(), sequence.end(), 81));

  //    qDebug() << "XXXXXXXXXXX";
  throw 1;

  return false;
}

bool BoardGenerator::tryGenerate101computing(int toRemove) {

  GridData originGrid = m_grid;

  auto cellsNotTried = randomCells();
  std::unordered_set<GridData::size_type> cellsTried;

  auto stopTime = time(nullptr) + 20;
  int attempts = 500000;
  bool nooutofattempts = true;
  bool notimeout = true;

  while ((nooutofattempts = (attempts > 0)) && toRemove > 0 &&
         (notimeout = (time(nullptr) < stopTime)) /*&&
         (cellsTried.size() < 81)*/)
  {
    if (cellsTried.size() == 81) {
      auto cellNumRevert = rand()%81;
      while (m_grid[cellNumRevert] == 0)
          cellNumRevert = rand() % 81;
      if (cellNumRevert == 0)
          qDebug() << "0";
      qDebug() << "reverting random cell: " << cellNumRevert;
      cellsTried.erase(cellNumRevert);
      cellsNotTried.push_back(cellNumRevert);
      if (m_grid[cellNumRevert] == 0)
          ++toRemove;
      m_grid[cellNumRevert] = originGrid[cellNumRevert];
    }

    qDebug() << "number of cells to remove: " << toRemove;
    auto cellNum = cellsNotTried.back();
    cellsNotTried.pop_back();
//    while (m_grid[cellNum = rand() % 81] == 0 || cellsTried.find(cellNum) != cellsTried.end())
//        qDebug() << "trying to find cell";
//      ;
    cellsTried.insert(cellNum);
    qDebug() << "removing cell: " << cellNum << ". cellsTried.size(): " << cellsTried.size();

    GridData::value_type backup = m_grid[cellNum];
    m_grid[cellNum] = 0;

    bool solved = false;
    try {
      auto tmp{m_grid};
      std::vector<GameOfSudoku::GridData> solutions;
      solutions.reserve(1);
      GameOfSudoku::solve(tmp, solutions);
      if (solutions.size() == 1) {
        solved = true;
      }
    } catch (GameOfSudoku::MaxNumberOfSolutionExceeded &e) {
      qDebug() << "multiple solutions";
      solved = false;
    }

    if (solved) {
      --toRemove;
      if (toRemove == 0)
        break;
    } else {
      m_grid[cellNum] = backup;
      --attempts;
    }
  }

  if (!notimeout) qDebug() << "timeout";
  if (!nooutofattempts) qDebug() << "out of attempts";
  if (cellsTried.size() == 81) qDebug() << "all cells checked";

  //    qDebug() << __FUNCTION__ << "attempts left: " << attempts << "to remove
  //    left: " << toRemove;

  return toRemove == 0;
}

bool BoardGenerator::tryGenerate(int toRemove) {
  //    return tryGenerateTest(toRemove);
  return tryGenerate101computing(toRemove);

  if (toRemove == 0)
    return true;

  decltype(HASH)::size_type cellNum = 0;
  GridData::value_type saveCellValue = 0;
  for (; toRemove > 0 && cellNum < 9 * 9; ++cellNum) {
    if (m_grid[HASH[cellNum]] == 0)
      continue;

    auto cellIndex = HASH[cellNum];

    removed.push_back(cellNum);

    //        qDebug() << removed;

    saveCellValue = m_grid[cellIndex];
    m_grid[cellIndex] = 0;
    --toRemove;

    try {
      //            qDebug() << "--------------------------------------";
      //            qDebug() << __FUNCTION__ << "toRemove: " << toRemove;
      //            GameOfSudoku::print(m_grid);

      auto tmp = m_grid;
      std::vector<GridData> solutions;
      solutions.reserve(1);
      GameOfSudoku::solve(tmp, solutions);
      if (solutions.size() == 1) {
        //                return tryGenerate(toRemove-1);
        //                break;
      } else {
        if (tryGenerate(toRemove - 1)) {
          return true;
        }
      }
      //            break;
    } catch (GameOfSudoku::MaxNumberOfSolutionExceeded &e) {
      qDebug() << "multiple solutions";
      break;
    }
  }

  removed.pop_back();
  m_grid[HASH[cellNum]] = saveCellValue;
  ++toRemove;

  return false;
}

bool BoardGenerator::tryGenerate0(int cluesCount [[maybe_unused]]) {
  std::vector<GridData::size_type> occupiedCells;
  for (auto i = m_grid.begin(); i < m_grid.end(); ++i) {
    if (*i) {
      occupiedCells.push_back(static_cast<GridData::size_type>(
          std::abs(std::distance(m_grid.begin(), i))));
    }
  }

  std::random_device rd;
  std::mt19937 g(rd());
  g.seed(time(nullptr));
  std::shuffle(occupiedCells.begin(), occupiedCells.end(), g);

  GridData::size_type saveCellIndex = 0;
  GridData::value_type saveCellValue = 0;
  while (!occupiedCells.empty()) {
    saveCellIndex = occupiedCells.back();
    saveCellValue = m_grid[occupiedCells.back()];
    m_grid[occupiedCells.back()] = 0;

    std::vector<GridData> solutions;
    try {
      GameOfSudoku::solve(m_grid, solutions);
    } catch (GameOfSudoku::MaxNumberOfSolutionExceeded &e) {
      qDebug() << "GameOfSudoku::MaxNumberOfSolutionExceeded: " << e.what();
      return false;
    }

    return true;
  }

  m_grid[saveCellIndex] = saveCellValue;

  return false;
}

GameOfSudoku::GameOfSudoku() { m_grid.fill(0); }

GameOfSudoku::GameOfSudoku(const GameOfSudoku::GridData &initialGrid)
    : m_grid(initialGrid) {}

bool GameOfSudoku::generateBoard(int numberOfClues) {
//    generateManyBoards(numberOfClues);
  BoardGenerator generator;
  if (!generator.generate(numberOfClues)) {
    m_grid = generator.board();
    return false;
  }
  //    generator.generate(81);
  //    generator.generate(80);
  //    generator.generate(79);

  m_grid = generator.board();

  return true;

  m_grid.fill(0);

  std::vector<GameOfSudoku::GridData> solutions;
  solve(solutions);

  auto toRemove = randomCells();

  auto toLeave = numberOfClues;
  toRemove.erase(toRemove.end() - toLeave, toRemove.end());
  for (auto index : toRemove)
      m_grid[index] = 0;
}

void GameOfSudoku::generateManyBoards(int numberOfClues)
{
    std::ofstream output("sudoku_data.txt", std::ios_base::app);
    BoardGenerator generator;
    while (true) {
        generator.generate(numberOfClues);
        m_grid = generator.board();

        std::string line = std::to_string(81 - std::count(m_grid.begin(), m_grid.end(), 0)) + " ";
        const auto chalange = printFlat(m_grid);

        std::vector<GameOfSudoku::GridData> _;
        solve(m_grid, _);
        line += printFlat(m_grid).c_str();

        line += std::string(" -> ") + chalange;

        output << line.c_str() << std::endl;
    }
}

void GameOfSudoku::solve(std::vector<GameOfSudoku::GridData> &solutions,
                         TryCallbackType tryCallback) {
  solve(m_grid, solutions, tryCallback);
}

std::string GameOfSudoku::print() { return print(m_grid); }

void GameOfSudoku::readShort(const std::string &board)
{
    std::transform(board.begin(), board.end(),
                   std::begin(m_grid),
                   [](const auto & c) { return c - '0'; } );
}

void GameOfSudoku::read(const std::string &board) {
  std::istringstream iss(board);

  for (GridData::size_type cellNum = 0; iss; ++cellNum) {
    std::string word;
    iss >> word;
    m_grid[cellNum] = static_cast<GridData::value_type>(std::stoi(word));
  }
  //    QStringList values = board.split(" ", QString::SkipEmptyParts);
  //    GridData::size_type cellNum = 0;
  //    while (!values.empty())
  //    {
  //        m_grid[cellNum] =
  //        static_cast<GridData::value_type>(values.front().toInt());
  //        values.pop_front();
  //        ++cellNum;
  //    }
}

std::string GameOfSudoku::print(const GameOfSudoku::GridData &grid, bool pretty)
{
    std::string result;
    for (GridData::size_type r = 0; r < 9; ++r) {
        std::string rowStr;

        if (pretty && (r == 3 || r == 6))
            rowStr += "----------+-----------+----------\n";

        for (GridData::size_type c = 0; c < 9; ++c) {
            if (pretty && (c == 3 || c == 6))
                rowStr += " | ";
            if (grid[r * 9 + c] != 0)
                rowStr += ' ' + std::to_string(grid[r * 9 + c]) + ' ';
            else
                rowStr += " _ ";
        }

        result += '\n' + rowStr;
    }

    return result;
}

std::string GameOfSudoku::printFlat(const GameOfSudoku::GridData &grid)
{
    return std::accumulate(grid.begin(), grid.end(), std::string(),
            [](auto init, const auto value){
                return std::move(init) + " " + std::to_string(value);
    });
}

GameOfSudoku::GridData::value_type
GameOfSudoku::at(GridData::size_type row, GridData::size_type col) const {
  return m_grid[row * 9 + col];
}

GameOfSudoku::GridData::value_type &GameOfSudoku::at(GridData::size_type row,
                                                     GridData::size_type col) {
  return m_grid[row * 9 + col];
}

bool GameOfSudoku::rowHasValue(GridData::size_type row,
                               GridData::value_type value) const {
  return GameOfSudoku::rowHasValue(row, value, m_grid);
}

bool GameOfSudoku::colHasValue(GridData::size_type col,
                               GridData::value_type value) const {
  return GameOfSudoku::colHasValue(col, value, m_grid);
}

bool GameOfSudoku::blockHasValue(GridData::size_type row,
                                 GridData::size_type col,
                                 GridData::value_type value) const {
  return GameOfSudoku::blockHasValue(row, col, value, m_grid);
}

bool GameOfSudoku::boardHasValue(GridData::value_type value) const {
  return std::count(m_grid.begin(), m_grid.end(), value) == 9;
}

void GameOfSudoku::makeFullRandom() {}

bool GameOfSudoku::isFull(const GridData &grid) {
  return std::end(grid) == std::find(std::begin(grid), std::end(grid), 0);
}

bool GameOfSudoku::rowHasValue(GridData::size_type row,
                               GridData::value_type value,
                               const GridData &grid) {
  for (GridData::size_type col = 0; col < 9; ++col) {
    if (grid[row * 9 + col] == value) {
      return true;
    }
  }

  return false;
}

bool GameOfSudoku::colHasValue(GridData::size_type col,
                               GridData::value_type value,
                               const GridData &grid) {
  for (GridData::size_type row = 0; row < 9; ++row) {
    if (grid[row * 9 + col] == value) {
      return true;
    }
  }
  return false;
}

bool GameOfSudoku::blockHasValue(GridData::size_type row,
                                 GridData::size_type col,
                                 GridData::value_type value,
                                 const GridData &grid) {
  auto rowOffs = row / 3 * 3;
  auto colOffs = col / 3 * 3;

  for (GridData::size_type r = 0; r < 3; ++r) {
    for (GridData::size_type c = 0; c < 3; ++c) {
      if (grid[9 * (rowOffs + r) + colOffs + c] == value) {
        return true;
      }
    }
  }
  return false;
}

;
bool GameOfSudoku::solve(GridData &grid, std::vector<GridData> &solutions,
                         TryCallbackType tryCallback) {
  GridData::size_type cellNum = 0;
  for (; cellNum < 9 * 9; ++cellNum) {
    auto row = cellNum / 9;
    auto col = cellNum % 9;

    if (grid[cellNum] == 0) {
      std::vector<GridData::value_type> values(9);
      std::iota(values.begin(), values.end(), 1);
      std::random_device rd;
      std::mt19937 g(rd());
      g.seed(time(nullptr));
      std::shuffle(values.begin(), values.end(), g);
      for (auto value : values) {
        if (!rowHasValue(row, value, grid) && !colHasValue(col, value, grid) &&
            !blockHasValue(row, col, value, grid)) {
          tryCallback(row, col, value);
          grid[cellNum] = value;
          if (isFull(grid)) {
            //                        solutions.push_back(grid);
            //                        qDebug() << "found soluiton number" <<
            //                        solutions.size(); qDebug() << "found
            //                        soluiton";
            if (solutions.capacity() > 0) {
              if (solutions.size() == solutions.capacity()) {
                throw MaxNumberOfSolutionExceeded(
                    "MaxNumberOfSolutionExceeded");
              }

              solutions.emplace_back(grid);

              break;
            }

            return true;
          } else {
            if (solve(grid, solutions, tryCallback)) {
              return true;
            }
          }
        }
      }

      break;
    }
  }

  tryCallback(cellNum / 9, cellNum % 9, 0);
  grid[cellNum] = 0;

  return false;
}

bool GameOfSudoku::operator==(const GameOfSudoku &other) {
  return m_grid == other.m_grid;
}

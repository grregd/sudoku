#ifndef GAMEOFSUDOKU_H
#define GAMEOFSUDOKU_H

//#include <QString>

#include <array>
#include <functional>
#include <string>
#include <vector>

class GameOfSudoku {
public:
  class MaxNumberOfSolutionExceeded : public std::runtime_error {
  public:
    MaxNumberOfSolutionExceeded(const std::string &v) : std::runtime_error(v) {}
  };

  using GridData = std::array<unsigned short, 9 * 9>;
  using TryCallbackType = std::function<void(
      GridData::size_type, GridData::size_type, GridData::value_type)>;

  GameOfSudoku();
  GameOfSudoku(const GameOfSudoku::GridData &initialGrid);

  bool generateBoard(int numberOfClues);
  void generateManyBoards(int numberOfClues);
  void solve(
      std::vector<GameOfSudoku::GridData> &solutions,
      TryCallbackType tryCallback = [](int, int, GridData::value_type) {});
  std::string print();
  void readShort(const std::string &board);
  void read(const std::string &board);

  static std::string print(const GridData &grid, bool pretty = false);
  static std::string printShort(const GameOfSudoku::GridData &grid);
  static std::string printFlat(const GridData &grid);

  GridData::value_type at(GridData::size_type row,
                          GridData::size_type col) const;
  GridData::value_type &at(GridData::size_type row, GridData::size_type col);

  bool rowHasValue(GridData::size_type row, GridData::value_type value) const;
  bool colHasValue(GridData::size_type col, GridData::value_type value) const;
  bool blockHasValue(GridData::size_type row, GridData::size_type col,
                     GridData::value_type value) const;
  bool boardHasValue(GridData::value_type value) const;

  static bool isFull(const GridData &grid);
  static bool rowHasValue(GridData::size_type row, GridData::value_type value,
                          const GridData &grid);
  static bool colHasValue(GridData::size_type col, GridData::value_type value,
                          const GridData &grid);
  static bool blockHasValue(GridData::size_type row, GridData::size_type col,
                            GridData::value_type value, const GridData &grid);

  static bool solve(
      GridData &grid, std::vector<GameOfSudoku::GridData> &solutions,
      TryCallbackType tryCallback = [](int, int, GridData::value_type) {});

  bool operator==(const GameOfSudoku &other);

private:
  void makeFullRandom();

private:
  GridData m_grid;
};

#endif // GAMEOFSUDOKU_H

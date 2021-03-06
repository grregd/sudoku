#ifndef GAMEOFSUDOKUMODEL_H
#define GAMEOFSUDOKUMODEL_H

#include <QAbstractTableModel>
#include <QColor>

#include "gameofsudoku.h"



class GameOfSudokuModel: public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(QColor textColor MEMBER m_textColor)
    Q_PROPERTY(QColor textOriginColor MEMBER m_textOriginColor)
    Q_PROPERTY(QColor fillColor MEMBER m_fillColor)
    Q_PROPERTY(QColor fillSelectedColor MEMBER m_fillSelectedColor)
    Q_PROPERTY(QColor textSameValueColor)
    Q_PROPERTY(QColor fillSameValueColor MEMBER m_fillSameValueColor)

    Q_PROPERTY(bool helpersVisible MEMBER m_helpersVisible NOTIFY onhelpersVisibleChanged)

    Q_PROPERTY(unsigned numberOfRevealed READ getNumberOfRevealed NOTIFY numberOfRevealedChanged)

    Q_ENUMS(Roles)

signals:
    void wrongTry();
    void gameSolved();
    void numberOfRevealedChanged();

public slots:
    void onhelpersVisibleChanged();

public:
    enum Roles {
        CellRole,
        TextColorRole,
        FillColorRole,
        SameValeRole
    };

    GameOfSudokuModel();

    QHash<int, QByteArray> roleNames() const override {
        return {
            { CellRole, "value" },
            { TextColorRole, "textColor" },
            { FillColorRole, "fillColor" },
            { SameValeRole, "sameValue" }
        };
    }


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    unsigned getNumberOfRevealed();

    Q_INVOKABLE void newBoard();
    Q_INVOKABLE void solve(/*std::function<void(int, int, GameOfSudoku::GridData::value_type)> tryCallback
                           = [](int, int, GameOfSudoku::GridData::value_type){}*/);
    Q_INVOKABLE void selectCell(const QVariant & indexValue);
    Q_INVOKABLE bool showHint();
    Q_INVOKABLE void insert(const QVariant & nativeText);
    Q_INVOKABLE void insertIfEmpty(const QVariant & nativeText);
    Q_INVOKABLE bool hasAllNumbers(int);
    Q_INVOKABLE void moveLeft();
    Q_INVOKABLE void moveRight();
    Q_INVOKABLE void moveUp();
    Q_INVOKABLE void moveDown();

private:
    // properties
    QColor m_textColor;
    QColor m_textOriginColor;
    QColor m_fillColor;
    QColor m_fillSelectedColor;
    QColor m_fillSameValueColor;
    bool m_helpersVisible = false;

    GameOfSudoku m_game;
    GameOfSudoku m_gameOrigin;
    GameOfSudoku m_gameSolution;
    int m_selectedCell = 0;
};


#endif // GAMEOFSUDOKUMODEL_H

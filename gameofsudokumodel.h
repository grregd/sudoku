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

    Q_ENUMS(Roles)
public:
    enum Roles {
        CellRole,
        TextColorRole,
        FillColorRole
    };

    GameOfSudokuModel();

    QHash<int, QByteArray> roleNames() const override {
        return {
            { CellRole, "value" },
            { TextColorRole, "textColor" },
            { FillColorRole, "fillColor" }
        };
    }


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE void newBoard();
    Q_INVOKABLE void solve(/*std::function<void(int, int, GameOfSudoku::GridData::value_type)> tryCallback
                           = [](int, int, GameOfSudoku::GridData::value_type){}*/);
    Q_INVOKABLE void selectCell(const QVariant & value);

private:
    QColor m_textColor;
    QColor m_textOriginColor;
    QColor m_fillColor;
    QColor m_fillSelectedColor;
    QColor m_fillSameValueColor;

    GameOfSudoku m_game;
    GameOfSudoku m_gameOrigin;
    int m_selectedCell;

};


#endif // GAMEOFSUDOKUMODEL_H

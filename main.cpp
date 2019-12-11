#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <algorithm>
#include <iterator>

#include "gameofsudokumodel.h"
#include "gameofsudoku.h"

#include <QDebug>



void test()
{
    //    GameOfSudoku game;
    //    game.read(
    //                " 4 _ 7 6 5 _ _ _ 3 "
    //                " _ 8 _ 9 _ 7 _ _ _ "
    //                " 9 _ _ 4 3 2 _ _ 7 "
    //                " _ _ 1 _ _ 6 _ 4 _ "
    //                " _ _ _ 5 _ _ _ 2 _ "
    //                " 2 _ 6 _ _ _ _ _ _ "
    //                " _ 3 _ 8 6 5 _ _ _ "
    //                " 5 9 _ 3 _ _ 1 _ _ "
    //                " _ _ _ _ 2 _ _ _ 8 " );
    //    game.read(
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
    //        game.read(
    //                    " 1 2 3 4 5 6 7 8 9 "
    //                    " _ _ _ _ _ _ _ 1 _ "
    //                    " _ _ _ _ _ _ 2 _ _ "
    //                    " _ _ _ _ _ 3 _ _ _ "
    //                    " _ _ _ _ 4 1 _ _ _ "
    //                    " _ _ _ 6 _ _ _ _ _ "
    //                    " _ _ 5 _ _ _ _ _ _ "
    //                    " _ 7 _ _ _ _ _ _ _ "
    //                    " 8 _ _ _ _ _ _ _ _ "
    //                    " 2 3 4 5 6 7 8 9 1 ");
    //    game.print();
    //    game.solve();
    //    game.print();

    //    for(const auto & solution: solutions)
    //    {
    //        GameOfSudoku::print(solution);
    //    }
    //    qDebug() << solutions.size();

    //    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<GameOfSudokuModel>("GameOfSudokuModel", 1, 0, "GameOfSudokuModel");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

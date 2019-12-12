import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import GameOfSudokuModel 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Game of Sudoku")

    Grid {
        anchors.fill: parent
        rows: 3; columns: 3; spacing: 0
        Repeater {
            model: 9
            Rectangle {
                width: 123; height: 123
                border.width: 1
//                color: "#000000ff"
            }
        }
    }

    TableView {
        id: tableView
        anchors.fill: parent

        rowSpacing: 1
        columnSpacing: 1

        ScrollBar.horizontal: ScrollBar {}
        ScrollBar.vertical: ScrollBar {}

        delegate: Rectangle {
            id: cell
            implicitWidth: 40
            implicitHeight: 40

            Rectangle {
                anchors.fill: parent
                color: model.fillColor
                Text {
                    text: model.value ? model.value : "";
                    color: model.textColor
                    font.pointSize: 20;
                    anchors.centerIn: parent
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: gameOfSudokuModel.selectCell(index)
            }
        }

        // NOTE: when 'sequences' is used both 'portableText' and 'nativetext' are empty
        Shortcut { sequence: "1";  onActivated: gameOfSudokuModel.insert(portableText) }
        Shortcut { sequence: "2";  onActivated: gameOfSudokuModel.insert(portableText) }
        Shortcut { sequence: "3";  onActivated: gameOfSudokuModel.insert(portableText) }
        Shortcut { sequence: "4";  onActivated: gameOfSudokuModel.insert(portableText) }
        Shortcut { sequence: "5";  onActivated: gameOfSudokuModel.insert(portableText) }
        Shortcut { sequence: "6";  onActivated: gameOfSudokuModel.insert(portableText) }
        Shortcut { sequence: "7";  onActivated: gameOfSudokuModel.insert(portableText) }
        Shortcut { sequence: "8";  onActivated: gameOfSudokuModel.insert(portableText) }
        Shortcut { sequence: "9";  onActivated: gameOfSudokuModel.insert(portableText) }
        Shortcut { sequence: "0";  onActivated: gameOfSudokuModel.insert(portableText) }

        model: GameOfSudokuModel {
            id: gameOfSudokuModel
            textColor: "blue"
            textOriginColor: "black"
            fillColor: "#c0c0c0"//"lightgrey"
            fillSelectedColor: "#a0a0a4"//"grey"
            fillSameValueColor: "#808080"//"darkgrey"
        }
    }

    footer: Rectangle {

        height: 50
        color: "grey"

        RowLayout {
            anchors.centerIn: parent


            CheckBox {
                text: qsTr("Pomoce")
                checked: gameOfSudokuModel.helpersVisible
                onClicked: gameOfSudokuModel.helpersVisible = !gameOfSudokuModel.helpersVisible
            }
            Button {
                text: qsTr("Podpowiedz")
                onClicked: gameOfSudokuModel.showHint()
            }
            Button {
                text: qsTr("Nowa gra")
                onClicked: gameOfSudokuModel.newBoard()
            }
            Button {
                text: qsTr("Rozwiąż")
                onClicked: gameOfSudokuModel.solve()
            }
        }
    }
}

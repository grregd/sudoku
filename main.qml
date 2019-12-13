import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import GameOfSudokuModel 1.0

ApplicationWindow {
    id: mainWin
    visible: true
    width: 640
    height: 480
    title: qsTr("Game of Sudoku")

    property var hintCountLeft: 3
    property var wrongTriesCount: 0


    function handleWrongTry()
    {
        ++wrongTriesCount;
        wrongAnswersCount.text = wrongTriesCount;
    }

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
                    font.pointSize: helpers1.checked && model.sameValue ? 25 : 20;
                    font.bold: helpers1.checked && model.sameValue
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
        Shortcut { sequence: "Esc";  onActivated: gameOfSudokuModel.insert("0") }
        Shortcut { sequence: "Left";  onActivated: gameOfSudokuModel.moveLeft() }
        Shortcut { sequence: "Right";  onActivated: gameOfSudokuModel.moveRight() }
        Shortcut { sequence: "Up";  onActivated: gameOfSudokuModel.moveUp() }
        Shortcut { sequence: "Down";  onActivated: gameOfSudokuModel.moveDown() }

        model: GameOfSudokuModel {
            id: gameOfSudokuModel
            textColor: "blue"
            textOriginColor: "black"
            fillColor: "#e0e0e0"
            fillSelectedColor: "#a0a0a0"
            fillSameValueColor: "#c0c0c0"
            onWrongTry: mainWin.handleWrongTry()
        }
    }

    footer: Rectangle {

        height: 50
        color: "grey"

        RowLayout {
            anchors.centerIn: parent

            CheckBox {
                id: helpers1
                text: qsTr("Pomoce 1")
                checked: false
            }
            CheckBox {
                text: qsTr("Pomoce 2")
                checked: gameOfSudokuModel.helpersVisible
                onClicked: gameOfSudokuModel.helpersVisible = !gameOfSudokuModel.helpersVisible
            }
            Button {
                id: buttonHints
                text: qsTr("Podpowiedz") + " (" + hintCountLeft + ")"
                onClicked: {
                    if (hintCountLeft > 0)
                    {
                        if (gameOfSudokuModel.showHint())
                        {
                            --hintCountLeft;
                            text = qsTr("Podpowiedz") + " (" + hintCountLeft + ")"

                            if (hintCountLeft == 0)
                                enabled = false;
                        }
                    }
                }
            }
            Button {
                text: qsTr("Nowa gra")
                onClicked: {
                    hintCountLeft = 3
                    wrongTriesCount = 0
                    buttonHints.text = qsTr("Podpowiedz") + " (" + hintCountLeft + ")"
                    gameOfSudokuModel.newBoard()
                }
            }
            Button {
                text: qsTr("Rozwiąż")
                onClicked: gameOfSudokuModel.solve()
            }
            Label {
                visible: wrongTriesCount > 0
                text: qsTr("Błędy: ")
                color: "red"
                font.pointSize: 20;
            }
            Label {
                id: wrongAnswersCount
                visible: wrongTriesCount > 0
                text: "0"
                color: "red"
                font.pointSize: 20;
            }
        }
    }
}

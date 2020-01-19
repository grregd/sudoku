import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import GameOfSudokuModel 1.0

ApplicationWindow {
    id: mainWin
    visible: true
    width: 540
    height: 760
    title: qsTr("Game of Sudoku")

    property var hintCountLeft: 3
    property var wrongTriesCount: 0
    property var theSize: 39

    function handleWrongTry()
    {
        ++wrongTriesCount;
        wrongAnswersCount.text = wrongTriesCount;
    }

    function handleGameSolved()
    {
        gameTimer.running = false;
        buttonSolve.enabled = false;
        buttonHints.enabled = false;
    }

    Grid {
        anchors.fill: parent
        rows: 3; columns: 3; spacing: 0
        Repeater {
            model: 9
            Rectangle {
                width: 3*theSize+3; height: 3*theSize+3
                border.width: 1
//                color: "#000000ff"
            }
        }
    }

    TableView {
        id: tableView
        anchors.fill: parent

        columnWidthProvider: function (column) { return theSize; }

        rowSpacing: 1
        columnSpacing: 1

        ScrollBar.horizontal: ScrollBar {}
        ScrollBar.vertical: ScrollBar {}

        delegate: Rectangle {
            id: cell
            implicitWidth: theSize
            implicitHeight: theSize
            width: theSize
            height: theSize

            Rectangle {
                anchors.fill: parent
                color: model.fillColor
                Text {
                    text: model.value ? model.value : "";
                    color: model.textColor
                    font.pointSize: helpers1.checked && model.sameValue ? 0.9*theSize : 0.8*theSize;
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
        Shortcut { sequence: "1";  onActivated: { gameOfSudokuModel.insertIfEmpty(portableText); numbersButtonsRepeater.updateState(); } }
        Shortcut { sequence: "2";  onActivated: { gameOfSudokuModel.insertIfEmpty(portableText); numbersButtonsRepeater.updateState(); } }
        Shortcut { sequence: "3";  onActivated: { gameOfSudokuModel.insertIfEmpty(portableText); numbersButtonsRepeater.updateState(); } }
        Shortcut { sequence: "4";  onActivated: { gameOfSudokuModel.insertIfEmpty(portableText); numbersButtonsRepeater.updateState(); } }
        Shortcut { sequence: "5";  onActivated: { gameOfSudokuModel.insertIfEmpty(portableText); numbersButtonsRepeater.updateState(); } }
        Shortcut { sequence: "6";  onActivated: { gameOfSudokuModel.insertIfEmpty(portableText); numbersButtonsRepeater.updateState(); } }
        Shortcut { sequence: "7";  onActivated: { gameOfSudokuModel.insertIfEmpty(portableText); numbersButtonsRepeater.updateState(); } }
        Shortcut { sequence: "8";  onActivated: { gameOfSudokuModel.insertIfEmpty(portableText); numbersButtonsRepeater.updateState(); } }
        Shortcut { sequence: "9";  onActivated: { gameOfSudokuModel.insertIfEmpty(portableText); numbersButtonsRepeater.updateState(); } }
        Shortcut { sequence: "0";  onActivated: { gameOfSudokuModel.insertIfEmpty(portableText); numbersButtonsRepeater.updateState(); } }
        Shortcut { sequence: "Esc";  onActivated: { gameOfSudokuModel.insertIfEmpty("0"); numbersButtonsRepeater.updateState(); } }
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
            onGameSolved: mainWin.handleGameSolved();
        }
        MouseArea {
            anchors.fill: parent
            onWheel: {
                theSize += wheel.angleDelta.y/20
                if (theSize < 10)
                    theSize = 10
                console.log(theSize)
                tableView.forceLayout()
            }
        }
    }

    footer: Rectangle {

        height: 220
        color: "grey"

        GridLayout {
//            anchors.centerIn: parent
            anchors.fill: parent
            rows: 2; columns: 1

            RowLayout {
                Layout.margins: 10
                Grid {
                    anchors.fill: parent
                    rows: 1; columns: 10; spacing: 10
                    Repeater {
                        id: numbersButtonsRepeater
                        model: 10
                        Rectangle {
                            color: "white"
                            implicitWidth: 26; implicitHeight: 26
                            Text {
                                id: text
                                enabled: parent.updateState()
                                visible: enabled
                                text: index == 9 ? "0" : index+1
                                font.pointSize: 32-10
                                font.bold: true
                                anchors.centerIn: parent
                            }
                            function updateState() {
                                text.enabled =
                                        text.visible =
                                        !gameOfSudokuModel.hasAllNumbers(index+1)
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    gameOfSudokuModel.insertIfEmpty(index == 9 ? "0" : (index+1).toString());
                                    numbersButtonsRepeater.updateState();
                                }
                            }
                        }
                        function updateState() {
                            console.log("updateState")
                            for(let i = 0; i < count; ++i)
                                itemAt(i).updateState();
                        }
                    }
                }
            }

            ColumnLayout {
                RowLayout {
                    Layout.margins: 10
                    CheckBox {
                        id: helpers1
                        text: qsTr("Pomoce 1")
                        checked: false
                    }
                    Button {
                        property int countUsed: 0
                        property string title: qsTr("Pomoce 2")
                        text: title
                        checked: gameOfSudokuModel.helpersVisible
                        onPressed: {
                            gameOfSudokuModel.helpersVisible = true
                            countUsed++
                            text = title + " (" + countUsed + ")"
                        }
                        onReleased: gameOfSudokuModel.helpersVisible = false
                    }
                }
                RowLayout {
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
                            timeElapsed.value = 0
                            buttonSolve.enabled = true
                            buttonHints.enabled = true
                            buttonHints.text = qsTr("Podpowiedz") + " (" + hintCountLeft + ")"
                            gameOfSudokuModel.newBoard()
                            gameTimer.running = true;
                        }
                    }
                    Button {
                        id: buttonSolve
                        text: qsTr("Rozwiąż")
                        onClicked: gameOfSudokuModel.solve()
                    }
                }
                RowLayout {
                    Label {
                        property var value: 0
                        id: timeElapsed
                        text: "Czas:    "
                        font.pointSize: 20;

                        function format() {
                            let tmp = value
                            text = "Czas: "
                            if (tmp >= 24*60*60)
                                text += Math.floor(tmp/(24*60*60)) + "d"
                            tmp %= 24*60*60
                            if (tmp >= 60*60)
                                text += Math.floor(tmp/(60*60)) + "g"
                            tmp %= (60*60)
                            if (tmp >= 60)
                                text += Math.floor(tmp/60) + "m"
                            tmp %= 60
                            text += tmp + "s"
                        }


                        function tickSecond() {
                            value++
                            format()
                        }

                        function reset() {
                            value++
                            format()
                        }
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
    }

    Item {
        Timer {
            id: gameTimer
            interval: 1000; running: true; repeat: true
            onTriggered: timeElapsed.tickSecond()
        }
    }
}

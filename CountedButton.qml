import QtQuick 2.12
import QtQuick.Controls 2.2


Button
{
    property int count: 0
    property string title: qsTr("Title")
    property int delta: 1
    property int alarmValue

    signal alarm;

    function updateText() {
        text = title;
        if (count > 0) {
            text += " (" + count + ")";
        }
    }

    onTitleChanged: updateText()

    onCountChanged: updateText()

    onPressed: {
        count += delta
        if (count === alarmValue)
            alarm();
    }
}


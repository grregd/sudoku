import QtQuick 2.12
import QtQuick.Controls 2.2


Button
{
    property int count: 0
    property string title: qsTr("Title")
    property int delta: 1
    property int alarmValue

    property int currentCount: count

    signal alarm;

    function reset() { currentCount = count }

    function updateText() {
        text = title
        if (currentCount > 0) {
            text = title + " (" + currentCount + ")";
        }
    }

    onTitleChanged: updateText()

    onCurrentCountChanged: updateText()

    onPressed: {
        currentCount += delta
        if (currentCount === alarmValue)
            alarm();
    }
}


import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

// !!!tmp import com.darebit.blisterocv 1.0

Item {
    id: mainPanel

    // properties
    // -------------------------------------------------------------
    property bool   imagesPaused: false
    property string pathRNDSufix: ""
    property string pathFailRNDSufix: ""

    // visual properties
    // -------------------------------------------------------------
    width:  parent.width
    height: parent.height

    // signals
    // -------------------------------------------------------------
    signal menuItemSelected(string menuItemId)
    signal imageUpdated(bool passed)

    // event handling
    // -------------------------------------------------------------
    Component.onCompleted: {
        statusTimer.running = true;
        mainWindow.onStatusPlcRunningChanged.connect(runningChanged);
        uiController.newMessage.connect(newMessage);
        uiController.imageReady.connect(imageUpdated);
    }

    // .............................................................
    Component.onDestruction: {
        statusTimer.running = false;
    }

    // .............................................................
    onImageUpdated: {
        if (imagesPaused)
            return;

        pathRNDSufix = "-" + Math.random(10);
        if (!passed) {
            pathFailRNDSufix = "-" + Math.random(10);
        }
    }

    // .............................................................
    function runningChanged() {
        startButton.confirm(statusPlcRunning);

        delayedExecution.acknowledge(statusPlcRunning ? "runMode" : "editMode")
    }

    // .............................................................
    function newMessage(message) {
        messageArea.newMessage(message);
    }

    // PAGES
    // -------------------------------------------------------------
    // !!!tmp
    /*
    property Component editBatchConfig: BatchConfigurationView {
        onScreenReady: {
            delayedExecution.acknowledge("edit");
        }
    }
    */

    // Timer to ask for plc status
    // ---------------------------------------------------------------------
    Timer {
        id: statusTimer
        running: false
        repeat: true
        interval: pollingPeriod
        onTriggered: uiController.requestPLCStatus();
    }

    // .............................................................
    ColumnLayout {
        anchors.fill: parent

        // TOP MENU -----------------------------------------------------------------------
        TopMenu {
            height: 50
            Layout.alignment: Qt.AlignTop
            Layout.bottomMargin: 10

            onItemSelected: {
                menuItemSelected(menuItem);
            }
        }

        // MAIN PANEL ---------------------------------------------------------------------
        RowLayout {
            width: parent.width

            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: true

            spacing: 20

            // INSPECTION IMAGES ----------------------------------------------------------
            ColumnLayout {
                id: inspectionControls

                width: 500
                Layout.alignment: Qt.AlignTop
                Layout.fillHeight: true

                Rectangle {
                    id: imagewrapper

                    color: "transparent"
                    anchors.margins: 5
                    width: parent.width
                    Layout.preferredHeight: 300

                    border.color: "#666666"
                    border.width: 1

                    Image {
                        id: inspectedSample
                        Layout.alignment: Qt.AlignTop
                        width: 496
                        height: 296
                        anchors.centerIn: parent

                        cache: false
                        asynchronous: false

                        source: "image://buffer/snapshot" + mainPanel.pathRNDSufix
                        fillMode: Image.PreserveAspectCrop
                        mipmap: true
                    }

                    Image {
                        id: previewSample
                        Layout.alignment: Qt.AlignTop
                        width: 496
                        height: 296
                        anchors.centerIn: parent

                        cache: false
                        asynchronous: false

                        fillMode: Image.PreserveAspectCrop
                        mipmap: true
                        visible: false
                    }
                }

                RowLayout {
                    spacing: 10

                    Button {
                        id: pausebutton
                        enabled: ! imagesPaused
                        opacity: enabled ? 0.8 : 0.4
                        iconSource: "../images/pause.png"
                        style: BlackButtonStyle { }
                        Layout.preferredHeight: 50
                        onClicked: {
                            imagesPaused = true;
                        }
                    }
                    Button {
                        id: playbutton
                        enabled: imagesPaused
                        opacity: enabled ? 0.8 : 0.4
                        iconSource: "../images/play.png"
                        style: BlackButtonStyle { }
                        Layout.preferredHeight: 50
                        onClicked: {
                            imagesPaused = false;
                            previewSample.visible = false;
                            inspectedSample.visible = true;
                            imageUpdated(false);
                            imagewrapper.border.color = "#666666";
                        }
                    }
                    Button {
                        id: clearbutton
                        enabled: ! statusPlcRunning
                        visible: ! statusPlcRunning
                        opacity: enabled ? 0.8 : 0.4
                        iconSource: "../images/remove.png"
                        style: BlackButtonStyle {}
                        Layout.preferredHeight: 50
                        onClicked: {
                            uiController.clearFailedImages();
                            pathFailRNDSufix = "-" + Math.random(10);
                        }
                    }
                    Button {
                        id: resultsbutton
                        enabled: statusPlcRunning
                        visible: statusPlcRunning
                        opacity: enabled ? 0.8 : 0.4
                        iconSource: "../images/results.png"
                        style: BlackButtonStyle {}
                        Layout.preferredHeight: 50
                        onClicked: {
                            menuItemSelected("MENU_RESULTS");
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        height: 1
                        Layout.fillWidth: true
                    }

                    // Edit
                    // -------------------------------------------------------------
                    Button {
                        id: editbutton
                        enabled: ! statusPlcRunning

                        style: BlackButtonStyle {}
                        iconSource: "../images/edit.png"
                        Layout.preferredHeight: 50

                        text: activeConfigId

                        onClicked: {
                            delayedExecution.execute(
                                "edit",
                                (function(activeConfigId) {
                                    stackView.push({item: editBatchConfig, properties: { configId: activeConfigId } });
                                    stackView.currentItem.forceActiveFocus();
                                })
                                .bind(this, activeConfigId),
                                true
                            );
                        }
                    }
                }

                // Failed Images
                // -------------------------------------------------------------------------
                RowLayout {
                    width: parent.width
                    spacing: 10

                    Repeater {
                        model: 5

                        Rectangle {
                            height: 92
                            width: 90
                            Layout.fillWidth: true
                            border.width: 1
                            border.color: "#666666"
                            color: "transparent"

                            Image {
                                width: 88
                                height: 88
                                anchors.centerIn: parent

                                cache: false
                                asynchronous: false

                                source: "image://buffer/f" + index + mainPanel.pathFailRNDSufix;
                                fillMode: Image.PreserveAspectCrop
                                mipmap: true

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        if (!imagesPaused) return;
                                        imageUpdated(false);
                                        previewSample.source = parent.source;
                                        previewSample.visible = true;
                                        imagewrapper.border.color = "#ff6666";
                                    }
                                }
                            }
                        }
                    }
                }

                // !!!tmp
                /*
                MessageView {
                    id: messageArea

                    anchors.bottom: parent.bottom
                    Layout.fillWidth: true
                    height: 50
                    openHeight: 350
                    opacity: 0.9
                }
                */
            }

            // STATUS PANEL ---------------------------------------------------------------
            ColumnLayout {
                id: statusMonitor
                width: 260
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                spacing: 10

                RowLayout {
                    id: statusControls
                    width: parent.width
                    Layout.alignment: Qt.AlignTop

                    // !!!tmp
                    /*
                    IndicatorLed {
                        id: runningStatus
                        name: "Running"
                        height: 50
                        width: 55
                        on: statusPlcRunning
                    }

                    IndicatorLed {
                        id: warningStatus
                        name: "Warning"
                        height: 50
                        width: 55
                        indicatorColor: "#f0a30a"
                        on: statusPlcWarning
                    }

                    IndicatorLed {
                        id: errorStatus
                        name: "Error"
                        height: 50
                        width: 55
                        indicatorColor: "red"
                        on: statusPlcError
                    }

                    ToggleDelayButton {
                        id: startButton
                        enabled: statusPlcConnected
                        turnedOn: statusPlcRunning
                        Layout.preferredHeight: 80
                        onActivated: {
                            mainWindow.statusJobReady = false;

                            if (turnedOn === false) {
                                // run mode requests reloading (download) of Job to camera
                                // check for the necessity of asynchronous response
                                delayedExecution.execute(
                                    "runMode",
                                    (function() {
                                        uiController.setRunMode();
                                        uiController.requestPLCStart();
                                        this.enabled = false;
                                    })
                                    .bind(this),
                                    true
                                );
                            }
                            else {
                                delayedExecution.execute(
                                    "editMode",
                                    (function() {
                                        uiController.requestPLCStop();
                                        uiController.setEditMode();
                                        enabled = false;
                                    })
                                    .bind(this),
                                    true
                                );
                            }
                        }
                    }
                    */
                }

                TabView {
                    id: counterLists
                    objectName: "counterLists"

                    Layout.fillWidth: true
                    Layout.preferredHeight: 360
                    Layout.alignment: Qt.AlignTop

                    style: TouchTabViewStyle {}

                    Tab {
                        title: "Batch"
                        // !!!tmp
                        /*
                        CounterList {
                            prefix: "batch";
                            isActive: counterLists.currentIndex === 0
                        }
                        */
                    }
                    Tab {
                        title: "Daily"
                        // !!!tmp
                        /*
                        CounterList {
                            prefix: "daily";
                            isActive: counterLists.currentIndex === 1
                        }
                        */
                    }
                    Tab {
                        title: "Global"
                        // !!!tmp
                        /*
                        CounterList {
                            prefix: "global";
                            isActive: counterLists.currentIndex === 2
                        }
                        */
                    }
                }

                RowLayout {
                    Button {
                        style: BlackButtonStyle {}
                        Layout.preferredHeight: 50
                        text: "Errors"
                        enabled: statusPlcError || statusPlcWarning
                        onClicked: {
                            menuItemSelected("MENU_ERRORS");
                        }
                    }

                    Rectangle { Layout.fillWidth: true }

                    Button {
                        enabled: statusPlcConnected
                        style: BlackButtonStyle { }
                        Layout.preferredHeight: 50
                        text: "Clear Counters"
                        onClicked: {
                            uiController.resetCounters(counterLists.currentIndex);
                        }
                    }
                }
            }
        }
    }

    // Delay Execution
    // -----------------------------------------------------------------------
    // !!!tmp
    /*
    DelayedExecution {
        id: delayedExecution

        onCancel: {
            stackView.pop();
        }
    }
    */
}

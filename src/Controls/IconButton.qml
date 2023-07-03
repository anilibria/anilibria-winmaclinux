/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.12
import QtQuick.Controls 2.12
import CustomStyle 1.0

Item {
    id: _button
    property alias iconPath: iconImage.source
    property alias iconWidth: iconImage.width
    property alias iconHeight: iconImage.height
    property string hoverColor: "lightgray"
    property bool hovered: false
    property alias showCrossIcon: crossIcon.visible
    property color backgroundColor: "transparent"
    property string tooltipMessage: ""
    property bool enablePulseAnimation: false

    signal buttonPressed()
    signal rightButtonPressed()
    signal buttonHoverEnter()
    signal buttonHoverExit()

    onEnablePulseAnimationChanged: {
        if (enablePulseAnimation) {
            pulseAnimation.start();
        } else {
            pulseAnimation.stop();
            pulseAnimation.complete();
        }
    }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onEntered: {
            imageRectangle.color = _button.hoverColor;
            buttonHoverEnter();
            hovered = true;
        }
        onExited: {
            imageRectangle.color = _button.backgroundColor;
            buttonHoverExit();
            hovered = false;
        }
        onClicked: {
            if (mouse.button === Qt.LeftButton) {
                _button.buttonPressed();
            } else {
                _button.rightButtonPressed();
            }
        }
    }

    Rectangle {
        id: imageRectangle
        anchors.fill: parent
        color: "transparent"

        Image {
            id: iconImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 29
            height: 29
            mipmap: true
            asynchronous: true
            transform: [
                Scale {
                    id: scaleIconImage
                    origin.x: iconImage.width / 2
                    origin.y: iconImage.height / 2
                    xScale: 1
                    yScale: 1
                }
            ]

            SequentialAnimation {
                id: pulseAnimation
                running: enablePulseAnimation
                loops: Animation.Infinite
                ParallelAnimation {
                    NumberAnimation { target: scaleIconImage; property: "xScale"; to: 1.2; duration: 300 }
                    NumberAnimation { target: scaleIconImage; property: "yScale"; to: 1.2; duration: 300 }
                }
                ParallelAnimation {
                    NumberAnimation { target: scaleIconImage; property: "xScale"; to: 1; duration: 300 }
                    NumberAnimation { target: scaleIconImage; property: "yScale"; to: 1; duration: 300 }
                }
                ParallelAnimation {
                    NumberAnimation { target: scaleIconImage; property: "xScale"; to: 1; duration: 1200 }
                    NumberAnimation { target: scaleIconImage; property: "yScale"; to: 1; duration: 1200 }
                }
            }
        }

        Canvas {
            id: crossIcon
            visible: false
            anchors.fill: parent
            opacity: .5
            onPaint: {
                const ctx = getContext(`2d`);
                const halfWidth = imageRectangle.width / 2;
                const offset = 14;

                ctx.lineWidth = 2;
                ctx.strokeStyle = `#68b0ab`;
                ctx.beginPath();
                ctx.moveTo(imageRectangle.width - offset, offset);
                ctx.lineTo(offset, imageRectangle.height - offset);
                ctx.closePath();
                ctx.stroke();

                ctx.beginPath();
                ctx.moveTo(offset, offset);
                ctx.lineTo(imageRectangle.width - offset, imageRectangle.height - offset);
                ctx.closePath();
                ctx.stroke();
            }
        }        
    }

    ToolTip {
        delay: 2000
        visible: tooltipMessage && hovered
        text: tooltipMessage
        CustomStyle.tooltipColor: applicationThemeViewModel.currentItems.colorTooltipBackground
        CustomStyle.colorTooltipText: applicationThemeViewModel.currentItems.colorTooltipText
    }
}

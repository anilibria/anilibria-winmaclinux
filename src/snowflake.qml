import QtQuick 2.12

Image {
    id: showflake
    x: 0
    y: 0
    source: assetsLocation.iconsPath + "snowflake.svg"
    opacity: .9
    width: 40
    height: 40

    signal startRotating()

    onStartRotating: {
        movingAnimation.running = Qt.binding(function() { return showflake.y < window.height - 200; })
    }

    RotationAnimation on rotation {
        loops: Animation.Infinite
        duration: 10000
        from: 0
        to: 360
    }

    ParallelAnimation {
        id: movingAnimation
        SequentialAnimation {
            loops: Animation.Infinite
            NumberAnimation {
                easing.type: Easing.OutQuad;
                target: showflake
                property: "x"
                to: showflake.x + 90
                duration: 2000
            }
            NumberAnimation {
                easing.type: Easing.InQuad;
                target: showflake
                property: "x"
                to: showflake.x - 90
                duration: 2000
            }
            NumberAnimation {
                easing.type: Easing.InQuad;
                target: showflake
                property: "x"
                to: showflake.x
                duration: 2000
            }
            NumberAnimation {
                easing.type: Easing.OutQuad;
                target: showflake
                property: "x"
                to: showflake.x + 90
                duration: 2000
            }
            NumberAnimation {
                easing.type: Easing.OutQuad;
                target: showflake
                property: "x"
                to: showflake.x
                duration: 3000
            }
        }
        NumberAnimation {
            target: showflake
            property: "y"
            from: 0
            to: window.height + showflake.height
            duration: 10000
        }
        onRunningChanged: {
            if (!running) showflake.destroy();
        }
    }
}

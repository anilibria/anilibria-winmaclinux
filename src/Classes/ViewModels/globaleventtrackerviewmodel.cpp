#include "globaleventtrackerviewmodel.h"
#include <QMouseEvent>

GlobalEventTrackerViewModel::GlobalEventTrackerViewModel(QObject *parent)
    : QObject{parent}
{
    QGuiApplication::instance()->installEventFilter(this);
}

bool GlobalEventTrackerViewModel::eventFilter(QObject *watched, QEvent *event)
{
    QEvent::Type t = event->type();
    if (t == QEvent::MouseButtonRelease && event->spontaneous()) {
        auto mouseEvent = static_cast<QMouseEvent*>(event);
        auto button = mouseEvent->button();
        if (button == Qt::XButton1) {
            emit backButtonPressed();
        }
        if (button == Qt::XButton2) {
            emit forwardButtonPressed();
        }
    }

    return QObject::eventFilter(watched, event);
}

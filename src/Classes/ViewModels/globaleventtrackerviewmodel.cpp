#include "globaleventtrackerviewmodel.h"
#include <QMouseEvent>
#include <QKeyEvent>

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

    if (t == QEvent::KeyRelease && event->spontaneous()) {
        auto keyEvent = static_cast<QKeyEvent*>(event);
        auto button = keyEvent->key();
        if (button == Qt::Key_Minus || button == Qt::Key_Underscore) {
            emit backButtonPressed();
        }
        if (button == Qt::Key_Plus || button == Qt::Key_Equal) {
            emit forwardButtonPressed();
        }
    }

    return QObject::eventFilter(watched, event);
}

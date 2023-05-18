#ifndef GLOBALEVENTTRACKERVIEWMODEL_H
#define GLOBALEVENTTRACKERVIEWMODEL_H

#include <QGuiApplication>
#include <QObject>

class GlobalEventTrackerViewModel : public QObject
{
    Q_OBJECT
public:
    explicit GlobalEventTrackerViewModel(QObject *parent = nullptr);

    bool eventFilter(QObject* watched, QEvent* event);

signals:
    void backButtonPressed();
    void forwardButtonPressed();

};

#endif // GLOBALEVENTTRACKERVIEWMODEL_H

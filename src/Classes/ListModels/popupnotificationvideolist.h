#ifndef POPUPNOTIFICATIONVIDEOLIST_H
#define POPUPNOTIFICATIONVIDEOLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QTimer>
#include "../Models/notificationmodel.h"

class PopupNotificationVideoList : public QAbstractListModel
{
    Q_OBJECT

private:
    QList<NotificationModel*>* m_popupNotifications;
    QTimer* m_timer;

    enum NotificationRoles {
        IdRole = Qt::UserRole + 1,
        MessageRole
    };

public:
    explicit PopupNotificationVideoList(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
    Q_INVOKABLE void addNewNotification(const int& type, const QString& message);

signals:

private slots:
    void timeout();

};

#endif // POPUPNOTIFICATIONVIDEOLIST_H

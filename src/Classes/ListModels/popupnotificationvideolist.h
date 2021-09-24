#ifndef POPUPNOTIFICATIONVIDEOLIST_H
#define POPUPNOTIFICATIONVIDEOLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QTimer>
#include "../Models/notificationmodel.h"

class PopupNotificationVideoList : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool showNotifications READ showNotifications NOTIFY showNotificationsChanged)

private:
    QList<NotificationModel*>* m_popupNotifications;
    QTimer* m_timer;
    bool m_showNotifications;

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

    bool showNotifications() const { return m_showNotifications; }
    void setShowNotifications(const bool& showNotifications);

signals:
    void showNotificationsChanged();

private slots:
    void timeout();

};

#endif // POPUPNOTIFICATIONVIDEOLIST_H

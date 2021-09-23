#ifndef NOTIFICATIONVIEWMODEL_H
#define NOTIFICATIONVIEWMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include "../Models/notificationmodel.h"
#include "../ListModels/popupnotificationvideolist.h"

class NotificationViewModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int countNotifications READ countNotifications NOTIFY countNotificationsChanged)
    Q_PROPERTY(PopupNotificationVideoList* popupNotifications READ popupNotifications NOTIFY popupNotificationsChanged)

private:
    QList<NotificationModel*>* m_notifications;
    PopupNotificationVideoList* m_popupNotifications;

    const static int InfoType;
    const static int ErrorType;
    const static int NewsType;

    enum NotificationRoles {
        TimeStampRole = Qt::UserRole + 1,
        TypeRole,
        MessageRole,
        IdRole
    };    

public:
    explicit NotificationViewModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
    int countNotifications() { return m_notifications->count(); }
    PopupNotificationVideoList* popupNotifications() { return m_popupNotifications; }

    Q_INVOKABLE void sendInfoNotification(const QString& message);
    Q_INVOKABLE void sendErrorNotification(const QString& message);
    Q_INVOKABLE void clearAllNotifications();
    Q_INVOKABLE void clearNotification(const QString& id);

private:
    QString getType(const int& type) const noexcept;
    void addNewNotification(const int& type, const QString& message);

signals:
    void countNotificationsChanged();
    void popupNotificationsChanged();

};

#endif // NOTIFICATIONVIEWMODEL_H

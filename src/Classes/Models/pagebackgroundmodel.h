#ifndef PAGEBACKGROUNDMODEL_H
#define PAGEBACKGROUNDMODEL_H

#include <QObject>

class PageBackgroundModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString image READ image NOTIFY imageChanged FINAL)
    Q_PROPERTY(int positionMode READ positionMode NOTIFY positionModeChanged FINAL)
    Q_PROPERTY(int imageAlignment READ imageAlignment NOTIFY imageAlignmentChanged FINAL)
    Q_PROPERTY(int opacity READ opacity NOTIFY opacityChanged FINAL)

private:
    QString m_image { "" };
    int m_positionMode { 0 };
    int m_imageAlignment { 0 };
    int m_opacity { 100 };

public:
    explicit PageBackgroundModel(QObject *parent = nullptr);

    QString image() const noexcept { return m_image; }

    int positionMode() const noexcept { return m_positionMode; }

    int imageAlignment() const noexcept { return m_imageAlignment; }

    int opacity() const noexcept { return m_opacity; }

signals:
    void imageChanged();
    void positionModeChanged();
    void imageAlignmentChanged();
    void opacityChanged();
};

#endif // PAGEBACKGROUNDMODEL_H

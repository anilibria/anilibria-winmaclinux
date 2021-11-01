#ifndef IMAGEBACKGROUNDVIEWMODEL_H
#define IMAGEBACKGROUNDVIEWMODEL_H

#include <QObject>
#include <QDebug>
#include "../../globalhelpers.h"
#include "../ListModels/commoncomboboxlistmodel.h"

class ImageBackgroundViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY(int imageMode READ imageMode WRITE setImageMode NOTIFY imageModeChanged)
    Q_PROPERTY(bool isHasImage READ isHasImage NOTIFY isHasImageChanged)
    Q_PROPERTY(int alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)
    Q_PROPERTY(CommonComboBoxListModel* imageModes READ imageModes NOTIFY imageModesChanged)
    Q_PROPERTY(CommonComboBoxListModel* alignmentModes READ alignmentModes NOTIFY alignmentModesChanged)
    Q_PROPERTY(int opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(int containerWidth READ containerWidth WRITE setContainerWidth NOTIFY containerWidthChanged)
    Q_PROPERTY(int containerHeight READ containerHeight WRITE setContainerHeight NOTIFY containerHeightChanged)
    Q_PROPERTY(int imageWidth READ imageWidth NOTIFY imageWidthChanged)
    Q_PROPERTY(int imageHeight READ imageHeight NOTIFY imageHeightChanged)
    Q_PROPERTY(int imageX READ imageX NOTIFY imageXChanged)
    Q_PROPERTY(int imageY READ imageY NOTIFY imageYChanged)

private:
    CommonComboBoxListModel* m_imageModes { new CommonComboBoxListModel(this) };
    CommonComboBoxListModel* m_alignmentModes { new CommonComboBoxListModel(this) };
    QString m_imagePath { "" };
    QString m_optionFilePath { "" };
    int m_imageMode { 0 };
    int m_alignment { 0 };
    int m_opacity { 100 };
    int m_containerWidth { 0 };
    int m_containerHeight { 0 };

    QString m_imagePathOption { "imagePath" };
    QString m_imageModeOption { "imageMode" };
    QString m_alignmentOption { "alignment" };
    QString m_opacityOption { "opacity" };

public:
    explicit ImageBackgroundViewModel(QObject *parent = nullptr);

    void setOptionFilePath(const QString& optionFilePath);

    QString imagePath() const noexcept { return m_imagePath; }
    void setImagePath(const QString& imagePath) noexcept;

    int imageMode() const noexcept { return m_imageMode; }
    void setImageMode(int imageMode) noexcept;

    bool isHasImage() const noexcept { return !m_imagePath.isEmpty(); }

    int alignment() const noexcept { return m_alignment; }
    void setAlignment(int alignment);

    int opacity() const noexcept { return m_opacity; }
    void setOpacity(int opacity) noexcept;

    int containerWidth() const noexcept { return m_containerWidth; }
    void setContainerWidth(int containerWidth) noexcept;

    int containerHeight() const noexcept { return m_containerHeight; }
    void setContainerHeight(int containerHeight) noexcept;

    int imageWidth() const noexcept;
    int imageHeight() const noexcept;

    int imageX() const noexcept;
    int imageY() const noexcept;

    CommonComboBoxListModel* imageModes() { return m_imageModes; }
    CommonComboBoxListModel* alignmentModes() { return m_alignmentModes; }

    Q_INVOKABLE void restoreToSavedState();
    Q_INVOKABLE void saveCurrentState();

private:
    void saveOptions();
    void loadOptions();

private slots:
    void imageModesItemsChanged();
    void aligmentModeChanged();

signals:
    void imagePathChanged();
    void imageModeChanged();
    void isHasImageChanged();
    void alignmentChanged();
    void imageModesChanged();
    void alignmentModesChanged();
    void opacityChanged();
    void containerWidthChanged();
    void containerHeightChanged();
    void imageWidthChanged();
    void imageHeightChanged();
    void imageXChanged();
    void imageYChanged();

};

#endif // IMAGEBACKGROUNDVIEWMODEL_H

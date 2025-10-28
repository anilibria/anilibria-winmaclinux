#include "imagebackgroundviewmodel.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

ImageBackgroundViewModel::ImageBackgroundViewModel(QObject *parent) : QObject(parent)
{
    m_imageModes->addItem(0, "Растягивать");
    m_imageModes->addItem(1, "Сохранять пропорции");
    m_imageModes->addItem(2, "Сохранять пропорции с обрезкой");
    m_imageModes->addItem(3, "Размножить автоматически");
    m_imageModes->addItem(4, "Размножить по вертикали");
    m_imageModes->addItem(5, "Размножить по горизонтали");
    m_imageModes->addItem(6, "Без изменений");

    m_alignmentModes->addItem(0, "Без выравнивания");
    m_alignmentModes->addItem(1, "Левый верхний угол");
    m_alignmentModes->addItem(2, "Правый верхний угол");
    m_alignmentModes->addItem(3, "Левый нижний угол");
    m_alignmentModes->addItem(4, "Правый нижний угол");
    m_alignmentModes->addItem(5, "Центр");

    m_pages->addItem(0, "Каталог релизов");
    m_pages->addItem(1, "Видеоплеер");
    m_pages->addItem(2, "Моя Анилибрия");
    m_pages->addItem(3, "Кинозал");
    m_pages->addItem(4, "Связанные релизы");
    m_pages->addItem(5, "TorrentStream");
    m_pages->addItem(6, "Менеджер тем");
    m_pages->addItem(7, "Youtube");
    m_pages->addItem(8, "Обслуживание");
    m_pages->addItem(9, "Приложения");
    m_pages->addItem(10, "Расширения");

    connect(m_imageModes, &CommonComboBoxListModel::selectedItemChanged, this, &ImageBackgroundViewModel::imageModesItemsChanged);
    connect(m_alignmentModes, &CommonComboBoxListModel::selectedItemChanged, this, &ImageBackgroundViewModel::aligmentModeChanged);
}

void ImageBackgroundViewModel::setOptionFilePath(const QString &optionFilePath)
{
    if (!m_optionFilePath.isEmpty()) return;

    m_optionFilePath = optionFilePath;

    createIfNotExistsFile(getCachePath(m_optionFilePath + ".cache"), "{}");
    loadOptions();
}

void ImageBackgroundViewModel::setImagePath(const QString &imagePath) noexcept
{
    if (m_imagePath == imagePath) return;

    m_imagePath = imagePath;
    emit imagePathChanged();
    emit processedImagePathChanged();
    emit isHasImageChanged();
}

void ImageBackgroundViewModel::setImageMode(int imageMode) noexcept
{
    if (m_imageMode == imageMode) return;

    m_imageMode = imageMode;
    emit imageModeChanged();
}

void ImageBackgroundViewModel::setAlignment(int alignment)
{
    if (m_alignment == alignment) return;

    m_alignment = alignment;
    emit alignmentChanged();
    emit imageWidthChanged();
    emit imageHeightChanged();
    emit imageXChanged();
    emit imageYChanged();
}

void ImageBackgroundViewModel::setOpacity(int opacity) noexcept
{
    if (m_opacity == opacity) return;

    m_opacity = opacity;
    emit opacityChanged();
}

void ImageBackgroundViewModel::setContainerWidth(int containerWidth) noexcept
{
    if (m_containerWidth == containerWidth) return;

    m_containerWidth = containerWidth;   
    emit containerWidthChanged();
    emit imageWidthChanged();
    emit imageXChanged();
}

void ImageBackgroundViewModel::setContainerHeight(int containerHeight) noexcept
{
    if (m_containerHeight == containerHeight) return;

    m_containerHeight = containerHeight;
    emit containerHeightChanged();
    emit imageHeightChanged();
    emit imageYChanged();
}

int ImageBackgroundViewModel::imageWidth() const noexcept
{
    return m_alignment > 0 ? m_containerWidth / 2 : m_containerWidth;
}

int ImageBackgroundViewModel::imageHeight() const noexcept
{
    return m_alignment > 0 ? m_containerHeight / 2 : m_containerHeight;
}

int ImageBackgroundViewModel::imageX() const noexcept
{
    switch (m_alignment) {
        case 2:
        case 4:
            return m_containerWidth / 2;
        default:
            return 0;
    }
}

int ImageBackgroundViewModel::imageY() const noexcept
{
    switch (m_alignment) {
        case 3:
        case 4:
            return m_containerHeight / 2;
        default:
            return 0;
    }
}

QString ImageBackgroundViewModel::processedImagePath()
{
    if (m_imagePath.isEmpty()) return "";
    if (m_imagePath.startsWith("http")) return m_imagePath;

#ifdef Q_OS_WIN
    return "file:///" + m_imagePath;
#else
    return "file://" + m_imagePath;
#endif
}

void ImageBackgroundViewModel::restoreToSavedState()
{
    loadOptions();

    emit imagePathChanged();
    emit imageModeChanged();
    emit alignmentChanged();
    emit opacityChanged();
    emit imageHeightChanged();
    emit imageWidthChanged();
    emit imageXChanged();
    emit imageYChanged();
}

void ImageBackgroundViewModel::saveCurrentState()
{
    saveOptions();
}

void ImageBackgroundViewModel::saveOptions()
{
    QJsonObject object;
    object[m_imagePathOption] = m_imagePath;
    object[m_imageModeOption] = m_imageMode;
    object[m_alignmentOption] = m_alignment;
    object[m_opacityOption] = m_opacity;

    QFile file(getCachePath(m_optionFilePath + ".cache"));
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(QJsonDocument(object).toJson());
    file.close();
}

void ImageBackgroundViewModel::loadOptions()
{
    QFile file(getCachePath(m_optionFilePath + ".cache"));
    file.open(QFile::ReadOnly | QFile::Text);
    auto json = file.readAll();
    file.close();

    auto document = QJsonDocument::fromJson(json);
    auto object = document.object();
    m_imagePath = object.contains(m_imagePathOption) ? object[m_imagePathOption].toString() : "";
    m_imageMode = object.contains(m_imageModeOption) ? object[m_imageModeOption].toInt() : 0;
    m_alignment = object.contains(m_alignmentOption) ? object[m_alignmentOption].toInt() : 0;
    m_opacity = object.contains(m_opacityOption) ? object[m_opacityOption].toInt() : 100;

    m_imageModes->setSelectedItem(m_imageMode);
    m_alignmentModes->setSelectedItem(m_alignment);
}

void ImageBackgroundViewModel::imageModesItemsChanged()
{
    setImageMode(m_imageModes->selectedItem());
}

void ImageBackgroundViewModel::aligmentModeChanged()
{
    setAlignment(m_alignmentModes->selectedItem());
}

/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef VLCQT_QMLVIDEOOUTPUT_H_
#define VLCQT_QMLVIDEOOUTPUT_H_

#include <memory>

#include <QQuickItem>
#include <QPointer>

#include "../core/Enums.h"

struct VlcYUVVideoFrame;
class VlcQmlSource;

class VlcQmlVideoOutput : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(VlcQmlSource *source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged)
    Q_PROPERTY(int aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged)
    Q_PROPERTY(int cropRatio READ cropRatio WRITE setCropRatio NOTIFY cropRatioChanged)

private:
    Vlc::FillMode _fillMode;
    Vlc::Ratio _aspectRatio;
    Vlc::Ratio _cropRatio;

    QPointer<VlcQmlSource> _source;

    bool _frameUpdated;
    std::shared_ptr<const VlcYUVVideoFrame> _frame;

public:
    VlcQmlVideoOutput();
    ~VlcQmlVideoOutput();

    VlcQmlSource *source() const;
    void setSource(VlcQmlSource *source);

    int fillMode() const;
    void setFillMode(int mode);

    int aspectRatio() const;
    void setAspectRatio(int aspectRatio);

    int cropRatio() const;
    void setCropRatio(int cropRatio);

public slots:
    void presentFrame(const std::shared_ptr<const VlcYUVVideoFrame> &frame);

signals:
    void sourceChanged();
    void fillModeChanged();
    void aspectRatioChanged();
    void cropRatioChanged();

private:
    virtual QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data);

};

#endif // VLCQT_QMLVIDEOOUTPUT_H_

/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2014 Tadej Novak <tadej@tano.si>
*
* Based on Phonon multimedia library
* Copyright (C) 2012 Harald Sitter <sitter@kde.org>
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

#ifndef VLCQT_VIDEOMEMORYSTREAM_H_
#define VLCQT_VIDEOMEMORYSTREAM_H_

#include "SharedExportCore.h"

#include <vlc/plugins/vlc_common.h>
#include <vlc/plugins/vlc_fourcc.h>

class VlcMediaPlayer;

/*!
    \class VlcVideoMemoryStream VideoMemoryStream.h VLCQtCore/VideoMemoryStream.h
    \ingroup VLCQtCore
    \brief Video memory stream (deprecated)

    VlcVideoMemoryStream is a template class for creating own video rendering engines.

    \see VlcAbstractVideoStream
    \deprecated Deprecated since VLC-Qt 1.1, will be removed in 2.0
 */
class Q_DECL_DEPRECATED VlcVideoMemoryStream
{
public:
    explicit VlcVideoMemoryStream();
    virtual ~VlcVideoMemoryStream();
    
    /*!
        \brief Set required information for rendering video

        \returns overall buffersize needed
     */
    static unsigned setPitchAndLines(const vlc_chroma_description_t *chromaDescription,
                                     unsigned width,
                                     unsigned height,
                                     unsigned *pitches,
                                     unsigned *lines,
                                     unsigned *visiblePitches = 0,
                                     unsigned *visibleLines = 0);

    /*!
        \brief Set VlcMediaPlayer callbacks
        \param player media player (VlcMediaPlayer *)
     */
    void setCallbacks(VlcMediaPlayer *player);

    /*!
        \brief Unset VlcMediaPlayer callbacks
        \param player media player (VlcMediaPlayer *)
     */
    void unsetCallbacks(VlcMediaPlayer *player);

protected:
    /*!
        \brief Lock callback
     */
    virtual void *lockCallback(void **planes) = 0;

    /*!
        \brief Unlock callback
     */
    virtual void unlockCallback(void *picture,
                                void *const *planes) = 0;

    /*!
        \brief Display callback
     */
    virtual void displayCallback(void *picture) = 0;

    /*!
        \brief Format callback
     */
    virtual unsigned formatCallback(char *chroma,
                                    unsigned *width,
                                    unsigned *height,
                                    unsigned *pitches,
                                    unsigned *lines) = 0;

    /*!
        \brief Format cleanup callback
     */
    virtual void formatCleanUpCallback() = 0;

private:
    static void *lockCallbackInternal(void *opaque,
                                      void **planes);
    static void unlockCallbackInternal(void *opaque,
                                       void *picture,
                                       void *const *planes);
    static void displayCallbackInternal(void *opaque,
                                        void *picture);

    static unsigned formatCallbackInternal(void **opaque,
                                           char *chroma,
                                           unsigned *width,
                                           unsigned *height,
                                           unsigned *pitches,
                                           unsigned *lines);
    static void formatCleanUpCallbackInternal(void *opaque);
};

#endif // VLCQT_VIDEOMEMORYSTREAM_H_

/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
* Copyright (C) 2014-2015, Sergey Radionov <rsatom_gmail.com>
* Copyright (C) 2011 Harald Sitter <sitter@kde.org>
*
* This file is based on QmlVlc and Phonon multimedia library
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

#include "VideoMaterial.h"
#include "VideoMaterialShader.h"

VideoMaterialShader::VideoMaterialShader()
{
    setShaderFileName(VertexStage, QLatin1String("qrc:/Shaders/vlc.frag.qsb"));
    setShaderFileName(FragmentStage, QLatin1String("qrc:/Shaders/vlc.vert.qsb"));
}

/*char const *const *VideoMaterialShader::attributeNames() const
{
    static const char *names[] = {
        "targetVertex",
        "textureCoordinates",
        0};
    return names;
}

const char *VideoMaterialShader::vertexShader() const
{
    return "attribute highp vec4 targetVertex;"
           "attribute highp vec2 textureCoordinates;"
           "uniform highp mat4 positionMatrix;"
           "varying highp vec2 textureCoord;"
           "void main(void)"
           "{"
           "    gl_Position = positionMatrix * targetVertex;"
           "    textureCoord = textureCoordinates;"
           "}";
}

const char *VideoMaterialShader::fragmentShader() const
{
    return "uniform sampler2D texY;"
           "uniform sampler2D texU;"
           "uniform sampler2D texV;"
           "uniform mediump mat4 colorMatrix;"
           "varying highp vec2 textureCoord;"
           "uniform lowp float opacity;"
           "void main(void)"
           "{"
           "    highp vec4 color = vec4("
           "           texture2D(texY, textureCoord.st).r,"
           "           texture2D(texU, textureCoord.st).r,"
           "           texture2D(texV, textureCoord.st).r,"
           "           1.0);"
           "    gl_FragColor = colorMatrix * color * opacity;"
           "}";
}*/

/*void VideoMaterialShader::updateState(const RenderState &state,
                                      QSGMaterial *newMaterial,
                                      QSGMaterial *oldMaterial)
{
    Q_UNUSED(oldMaterial)

    if (state.isOpacityDirty())
        program()->setUniformValue(_opacityId, GLfloat(state.opacity()));

    if (state.isMatrixDirty())
        program()->setUniformValue(_positionMatrixId, state.combinedMatrix());

    static const QMatrix4x4 colorMatrix(
        1.164383561643836f, 0.000000000000000f, 1.792741071428571f, -0.972945075016308f,
        1.164383561643836f, -0.213248614273730f, -0.532909328559444f, 0.301482665475862f,
        1.164383561643836f, 2.112401785714286f, 0.000000000000000f, -1.133402217873451f,
        0.000000000000000f, 0.000000000000000f, 0.000000000000000f, 1.000000000000000f);

    program()->setUniformValue(_colorMatrixId, colorMatrix);

    VideoMaterial *material = static_cast<VideoMaterial *>(newMaterial);

    program()->setUniformValue(_texYId, 0);
    program()->setUniformValue(_texUId, 1);
    program()->setUniformValue(_texVId, 2);

    material->bindPlanes();
}*/

bool VideoMaterialShader::updateUniformData(RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    Q_UNUSED(oldMaterial)

    static const QMatrix4x4 colorMatrix(
        1.164383561643836f, 0.000000000000000f, 1.792741071428571f, -0.972945075016308f,
        1.164383561643836f, -0.213248614273730f, -0.532909328559444f, 0.301482665475862f,
        1.164383561643836f, 2.112401785714286f, 0.000000000000000f, -1.133402217873451f,
        0.000000000000000f, 0.000000000000000f, 0.000000000000000f, 1.000000000000000f);
    static const int texY = 0;
    static const int texU = 1;
    static const int texV = 2;

    bool changed = false;
    QByteArray *buf = state.uniformData();

    if (state.isMatrixDirty()) {
        const QMatrix4x4 m = state.combinedMatrix();
        memcpy(buf->data(), m.constData(), 64);
        changed = true;
    }

    if (state.isOpacityDirty()) {
        const float opacity = state.opacity();
        memcpy(buf->data() + 64, &opacity, 4);
        changed = true;
    }

    VideoMaterial *material = static_cast<VideoMaterial *>(newMaterial);

    if (oldMaterial != newMaterial /*|| material->uniforms.dirty*/) {
        memcpy(buf->data() + 68, &texY, 4); // texY
        memcpy(buf->data() + 72, &texU, 8); // texU
        memcpy(buf->data() + 80, &texV, 4); // texV
        memcpy(buf->data() + 80, &colorMatrix, 4); // texV
        changed = true;
    }

    material->bindPlanes();

    return changed;
}

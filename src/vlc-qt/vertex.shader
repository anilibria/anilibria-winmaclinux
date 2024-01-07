#version 440

layout(location = 0) in vec4 targetVertex;
layout(location = 1) in vec2 textureCoordinates;
layout(location = 0) out vec2 textureCoord;
layout(std140, binding = 0) uniform buf {
    mat4 positionMatrix;
} ubuf;
out gl_PerVertex { vec4 gl_Position; };
void main() {
    gl_Position = ubuf.positionMatrix * targetVertex;
    textureCoord = textureCoordinates;
}
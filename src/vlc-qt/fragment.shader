#version 440

layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 fragColor;
layout(std140, binding = 0) uniform buf {
    mat4 colorMatrix;
	float opacity;
} ubuf;
layout(binding = 1) uniform sampler2D texY;
layout(binding = 2) uniform sampler2D texU;
layout(binding = 3) uniform sampler2D texV;

void main(void)
{
	vec4 color = vec4(
       texture(texY, texCoord).r,
       texture(texU, texCoord).r,
       texture(texV, texCoord).r,
       1.0		
	);
    fragColor = colorMatrix * color * opacity;
};
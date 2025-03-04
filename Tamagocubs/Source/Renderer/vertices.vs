
#version 460 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform vec2 offsetInd;
uniform vec2 size;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = vec2(size.x * vertex.z + size.x * offsetInd.x, size.y * 1.0 - size.y * offsetInd.y + size.y * vertex.w);
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
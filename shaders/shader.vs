#version 410

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color_in;

out vec3 color_frag;

void main() {
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
	color_frag = color_in;
}
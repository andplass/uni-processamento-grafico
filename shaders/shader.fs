#version 410

in vec3 color_frag;
out vec4 color_out;

void main() {
    color_out = vec4(color_frag, 1.0);
}
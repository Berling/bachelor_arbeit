#version 330

in vec2 texcoord_;

out vec4 frag_color;

uniform sampler2D sky;

void main() {
	frag_color = texture(sky, texcoord_);
}

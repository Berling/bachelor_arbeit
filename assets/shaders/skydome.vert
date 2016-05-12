#version 330

in vec3 _position;
in vec2 _texcoord;
in vec3 _normal;

out vec2 texcoord_;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	texcoord_ = _texcoord;
	gl_Position = projection * view * model * vec4(_position, 1.f);
}

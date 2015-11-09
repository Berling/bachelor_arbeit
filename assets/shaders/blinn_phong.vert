#version 330

in vec3 _position;
in vec3 _normal;

out vec3 position_;
out vec3 normal_;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(_position, 1.f);
	position_ = (model * vec4(_position, 1.f)).xyz;
	normal_ = (model * vec4(_normal, 1.f)).xyz;
}
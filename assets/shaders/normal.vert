#version 330

in vec3 _position;
in vec3 _normal;

out output_vertex {
	vec3 position;
	vec3 normal;
} vertex_out;

void main() {
	vertex_out.position = _position;
	vertex_out.normal = _normal;
}

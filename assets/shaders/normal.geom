#version 330

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float normal_length;

in output_vertex {
	vec3 position;
	vec3 normal;
} vertex_input[];

void main() {
	for (int i = 0; i < 3; ++i) {
		gl_Position = projection * view * model * vec4(vertex_input[i].position, 1.f);
		EmitVertex();
		gl_Position = projection * view * model * vec4(vertex_input[i].position + vertex_input[i].normal * normal_length, 1.f);
		EmitVertex();
		EndPrimitive();
	}
}

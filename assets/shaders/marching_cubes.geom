#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;

in cell {
	vec4 c00;
	vec4 c01;
	vec4 c10;
	vec4 c11;
} cell_in[];

out output_vertex {
	vec3 position;
} cell_out;

void main() {
	vec4 center = cell_in[0].c00 + cell_in[0].c01 + cell_in[0].c10 + cell_in[0].c11;

	cell_out.position = (center + vec4(-0.5f, -0.5f, 0.f, 1.f)).xyz;
	EmitVertex();
	cell_out.position = (center + vec4(0.5f, -0.5f, 0.f, 1.f)).xyz;
	EmitVertex();
	cell_out.position = (center + vec4(0.f, 0.5f, 0.f, 1.f)).xyz;
	EmitVertex();
}

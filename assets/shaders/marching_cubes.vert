#version 330

in vec4 _c00;
in vec4 _c01;
in vec4 _c10;
in vec4 _c11;

out cell {
	vec4 c00;
	vec4 c01;
	vec4 c10;
	vec4 c11;
} cell_out;

void main() {
	cell_out.c00 = _c00;
	cell_out.c01 = _c01;
	cell_out.c10 = _c10;
	cell_out.c11 = _c11;
}

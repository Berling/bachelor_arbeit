#version 330

in vec3 position_;
in vec3 normal_;

out vec4 frag_color;

uniform vec3 view_vector;
uniform vec3 color;
uniform vec3 light_direction;
uniform vec3 light_color;
uniform float light_energy;
uniform vec3 ambient_term;

vec3 blinn_phong(vec3 N, vec3 L, vec3 V, vec3 light_color, vec3 diff_color, float shininess) {
	vec3 H = normalize(L + V);

	float NdotL = max(0.0, dot(N, L));
	float NdotH = max(0.0, dot(N, H));

	return light_color * diff_color * NdotL + light_color * diff_color * pow(NdotH, shininess);
}

void main() {
	vec3 N = normalize(normal_);
	vec3 V = normalize(view_vector - position_);
	vec3 L = normalize(light_direction);

	frag_color = vec4(ambient_term * color, 1.f) + vec4(blinn_phong(N, L, V, light_color, color, 50.f), 1.f) * light_energy;
}
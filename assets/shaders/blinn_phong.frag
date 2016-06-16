#version 430

in vec3 position_;
in vec3 normal_;

out vec4 frag_color;

uniform vec3 view_vector;
uniform vec3 color;
uniform vec3 light_direction;
uniform vec3 light_color;
uniform float light_energy;

uniform sampler2D rock_diffuse;
uniform sampler2D grass_diffuse;
uniform float tex_scale;

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

	vec3 flip = vec3(N.x < 0.0, N.y >= 0.0, N.z < 0.0);
	vec3 blend_weights = clamp(abs(N) - 0.5, 0.f, 1.f);
	blend_weights *= blend_weights;
	blend_weights *= blend_weights;

	blend_weights /= dot(blend_weights, vec3(1.0, 1.0, 1.0));

	vec3 blended_color;

	vec2 coord1 = vec2(flip.x == 1 ? -position_.y : position_.y, position_.z) * tex_scale;
	vec2 coord2 = vec2(flip.y == 1 ? -position_.x : position_.x, position_.z) * tex_scale;
	vec2 coord3 = vec2(flip.z == 1 ? -position_.x : position_.x, position_.y) * tex_scale;

	vec3 color1 = texture(rock_diffuse, coord1).xyz;
	vec3 color2 = texture(grass_diffuse, coord2).xyz;
	vec3 color3 = texture(rock_diffuse, coord3).xyz;

	blended_color = color1 * blend_weights.xxx + color2 * blend_weights.yyy + color3 * blend_weights.zzz;

	frag_color = vec4(blinn_phong(N, L, V, light_color, blended_color, 5.f), 1.f) * light_energy;
}

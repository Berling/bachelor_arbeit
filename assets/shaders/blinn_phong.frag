#version 430

in vec3 normal_;
in vec3 position_;
in vec3 ltan1_;
in vec3 vtan1_;
in vec2 ltan2_;
in vec2 vtan2_;

out vec4 frag_color;

uniform vec3 color;
uniform vec3 light_color[2];
uniform float light_energy[2];

uniform sampler2D rock_albedo;
uniform sampler2D rock_material;
uniform sampler2D rock_normal;
uniform sampler2D grass_albedo;
uniform sampler2D grass_material;
uniform sampler2D grass_normal;
uniform float tex_scale;

vec3 blinn_phong(float NdotL, float NdotH, vec3 light_color, vec3 diff_color, float gloss, float shininess) {
	return diff_color * NdotL + gloss * diff_color * pow(NdotH, shininess);
}

void main() {
	vec3 N = normalize(normal_);
	vec3 flip = vec3(N.x < 0.0, N.y >= 0.0, N.z < 0.0);
	vec3 blend_weights = clamp(abs(N) - 0.5, 0.f, 1.f);
	blend_weights *= blend_weights;
	blend_weights *= blend_weights;

	blend_weights /= dot(blend_weights, vec3(1.0, 1.0, 1.0));

	vec2 coord1 = vec2(flip.x == 1 ? -position_.y : position_.y, position_.z) * tex_scale;
	vec2 coord2 = vec2(flip.y == 1 ? -position_.x : position_.x, position_.z) * tex_scale;
	vec2 coord3 = vec2(flip.z == 1 ? -position_.x : position_.x, position_.y) * tex_scale;

	vec3 color1 = texture(rock_albedo, coord1).xyz;
	vec3 color2 = texture(grass_albedo, coord2).xyz;
	vec3 color3 = texture(rock_albedo, coord3).xyz;

	vec3 blended_color = color1 * blend_weights.xxx + color2 * blend_weights.yyy + color3 * blend_weights.zzz;

	vec3 material1 = texture(rock_material, coord1).xyz;
	vec3 material2 = texture(grass_material, coord2).xyz;
	vec3 material3 = texture(rock_material, coord3).xyz;

	vec3 blended_material = material1 * blend_weights.xxx + material2 * blend_weights.yyy + material3 * blend_weights.zzz;
	blended_material.g *= 128;

	vec3 l1 = normalize(ltan1_);
	vec3 l2 = normalize(vec3(ltan2_.x, ltan2_.y, ltan1_.z));

	vec3 h1 = normalize(l1 + normalize(vtan1_));
	vec3 h2 = normalize(l2 + normalize(vec3(vtan2_.x, vtan2_.y, vtan1_.z)));

	vec3 nx = 2.f * texture(rock_normal, coord1).xyz - 1.f;
	vec3 ny = 2.f * texture(grass_normal, coord2).xyz - 1.f;
	vec3 nz = 2.f * texture(rock_normal, coord3).xyz - 1.f;

	vec3 temp;
	temp.x = clamp(dot(nx, l1), 0.f, 1.f);
	temp.y = clamp(dot(ny, l1), 0.f, 1.f);
	temp.z = clamp(dot(nz, l2), 0.f, 1.f);
	float NdotL = dot(temp, blend_weights);

	temp.x = clamp(dot(nx, h1), 0.f, 1.f);
	temp.y = clamp(dot(ny, h1), 0.f, 1.f);
	temp.z = clamp(dot(nz, h2), 0.f, 1.f);
	float NdotH = dot(temp, blend_weights);

	frag_color = vec4(blinn_phong(NdotL, NdotH, light_color[0], blended_color, blended_material.r, blended_material.g), 1.f) * light_energy[0];
}

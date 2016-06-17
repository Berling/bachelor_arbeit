#version 430

in vec3 _position;
in vec3 _normal;

out vec3 normal_;
out vec3 position_;
out vec3 ltan1_;
out vec3 vtan1_;
out vec2 ltan2_;
out vec2 vtan2_;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 mit;
uniform vec3 view_vector;
uniform vec3 light_direction[2];

void main() {
	gl_Position = projection * view * model * vec4(_position, 1.f);
	vec3 position = (model * vec4(_position, 1.f)).xyz;
	vec3 normal = (mit * vec4(_normal, 1.f)).xyz;
	normal_ = normal;
	position_ = position;
	vec3 L = normalize(light_direction[0]);
	vec3 V = normalize(view_vector - position);

	{
		vec3 mag = normal * normal;
		mag.xy = mag.x + mag.yz;
		mag.xy = max(mag.xy, 0.03125f);

		vec3 vxn = cross(L, normal);
		vec4 temp;
		temp.xy = normal.xz * L.yx - normal.yx * L.xz;
		temp.zw = normal.xz * vxn.yx - normal.yx * vxn.xz;

		ltan1_.xy = temp.xz * inversesqrt(mag.x);
		ltan2_ = temp.yw * inversesqrt(mag.y);

		ltan1_.z = dot(normal, L);
	}
	{
		vec3 mag = normal * normal;
		mag.xy = mag.x + mag.yz;
		mag.xy = max(mag.xy, 0.03125f);

		vec3 vxn = cross(V, normal);
		vec4 temp;
		temp.xy = normal.xz * V.yx - normal.yx * V.xz;
		temp.zw = normal.xz * vxn.yx - normal.yx * vxn.xz;

		vtan1_.xy = temp.xz * inversesqrt(mag.x);
		vtan2_ = temp.yw * inversesqrt(mag.y);

		vtan1_.z = dot(normal, V);
	}
}

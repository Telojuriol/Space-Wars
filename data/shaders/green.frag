#version 330

in vec3 v_normal;
in vec3 v_camera;
in vec2 v_uv;

uniform vec3 u_light;

uniform sampler2D u_texture_diffuse;
uniform sampler2D u_texture_specular;
//uniform vec3 u_camera_position;

out vec4 fragColor;

void main(void)
{

	vec3 N = normalize(v_normal);

	vec3 L = normalize(u_light);

	vec3 E = normalize(v_camera);

	vec3 final_color = vec3(0.0, 1.0, 0.0);


	
	fragColor =  vec4(final_color, 1.0);

}

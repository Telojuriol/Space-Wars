#version 330


in vec2 v_uv;

uniform sampler2D u_texture_diffuse;

out vec4 fragColor;

void main(void)
{

	vec3 final_color = vec3(0.0, 0.0, 0.0);
	
	vec3 diffuse = texture2D(u_texture_diffuse,v_uv).xyz;
		
	final_color = diffuse;
		
	fragColor =  vec4(final_color, 1.0);

}

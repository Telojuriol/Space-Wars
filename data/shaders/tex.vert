#version 330
 
// the following attributes are supported by this engine
// Mesh::render searches for, and binds them, if they exist

in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_uv;

uniform mat4 u_mvp;
uniform vec3 u_camera_position;

out vec2 v_uv;

void main()
{

	v_uv = a_uv;

	gl_Position = u_mvp * vec4( a_vertex, 1.0 );

}





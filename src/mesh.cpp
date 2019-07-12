#include "mesh.h"
#include <cassert>
#include "includes.h"
#include "shader.h"
#include "extra/textparser.h"


using namespace std;
Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
    indices_vbo_id = 0;
    vao = 0;
}

Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
    indices = m.indices;
    vao = m.vao;
}

Mesh::~Mesh()
{
	if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &indices_vbo_id);
    if(vao)glDeleteVertexArrays(1, &vao);
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
    indices.clear();
}

void Mesh::genBuffers() {
    assert(vertices.size() && "No vertices in this mesh");
    

    //delete old
    if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
    if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
    if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    
    //bind vao to store state
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers( 1, &vertices_vbo_id ); //generate one handler (id)
    glBindBuffer( GL_ARRAY_BUFFER, vertices_vbo_id ); //bind the handler
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW ); //upload data
    
    //shader must a_vertex attribute, error if not
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    
    if(normals.size())
    {
        glGenBuffers( 1, &normals_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, normals_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), &normals[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_normal attribute
        glEnableVertexAttribArray(NORMAL_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(NORMAL_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );

    }
    
    if(uvs.size())
    {
        glGenBuffers( 1, &uvs_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, uvs_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 2 * sizeof(float), &uvs[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_uv attribute
        glEnableVertexAttribArray(UV_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(UV_ATTRIBUTE_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if(colors.size())
    {
        glGenBuffers( 1, &colors_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, colors_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 4 * sizeof(float), &colors[0], GL_STATIC_DRAW ); //upload data

        //shader must have a_color attribute
        glEnableVertexAttribArray(COLOR_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(COLOR_ATTRIBUTE_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if (indices.size() == 0){
        //if there is no index buffer, make an 'emergency' serial one here
        for (size_t i = 0; i < vertices.size(); i++) {
            indices.push_back((unsigned int)i);
        }
    }
    
    //create index array
    glGenBuffers(1, &indices_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0); //unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
    
}

void Mesh::render( int primitive, Shader* sh )
{
    
    glBindVertexArray(vao);
    
    glDrawElements(primitive, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    
}


bool Mesh::loadASE(const char* filename){
	
	TextParser t;
	
	if(t.create(filename) == false){
		cout << "couldn't open file" << endl;
		return false;

	}

	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	indices.clear();
	
	t.seek("*MESH_NUMVERTEX");
	int num_vertex = t.getint();

	t.seek("*MESH_NUMFACES");
	int num_faces = t.getint();

	vector<Vector3> unique_verts;
	unique_verts.resize(num_faces * 3);

	vector<Vector3> unique_faces;
	unique_faces.resize(num_faces);


	for(int i =0; i < num_vertex;i++){
		t.seek("*MESH_VERTEX");
		int new_index = t.getint();
		float new_x = t.getfloat();
		float new_y = t.getfloat();
		float new_z = t.getfloat();

		//Vector3 new_vec3(new_x,new_y,new_z);
		Vector3 new_vec3(-new_x,new_z,new_y);
		unique_verts[new_index] = new_vec3;

	}


	

	
	


	for(int i =0; i < num_faces;i++){

		t.seek("*MESH_FACE");

		t.seek("A:");
		int vert1 = t.getint();
		vertices.push_back(unique_verts[vert1]);

		t.seek("B:");
		int vert2 = t.getint();
		vertices.push_back(unique_verts[vert2]);

		t.seek("C:");
		int vert3 = t.getint();
		vertices.push_back(unique_verts[vert3]);

		Vector3 new_vec3(vert1,vert2,vert3);

		unique_faces[i] = new_vec3;
		

	}



	vector<Vector2> unique_uvs;
	t.seek("*MESH_NUMTVERTEX");
	float num_tvertex = t.getfloat();
	//unique_uvs.resize(num_tvertex);

	for(int i = 0; i < num_tvertex; i++){
		t.seek("*MESH_TVERT");
		int index = t.getint();
		float u = t.getfloat();

		float v = t.getfloat();

		Vector2 new_uv = Vector2(u,v);
		unique_uvs.push_back(new_uv);

	}


	t.seek("*MESH_NUMTVFACES");
	int num_tfaces = t.getint();

	for(int i =0; i < num_tfaces;i++){

		t.seek("*MESH_TFACE");
		
		int newindex = t.getint();

		int vert1 = t.getint();
		uvs.push_back(unique_uvs[vert1]);

		int vert2 = t.getint();
		uvs.push_back(unique_uvs[vert2]);

		int vert3 = t.getint();
		uvs.push_back(unique_uvs[vert3]);

	}


	vector<Vector3> unique_normals;

	unique_normals.resize(num_faces * 3);
	t.seek("*MESH_NORMALS");
	
	for(int i = 0; i < num_faces * 3; i++){
		t.seek("*MESH_VERTEXNORMAL");
		int index = t.getint();
		float nX = t.getfloat();
		float nY = t.getfloat();
		float nZ = t.getfloat();
		Vector3 new_norm = Vector3(nX,nZ,nY);
		//Vector3 new_norm = Vector3(-nX,nZ,-nY);
		unique_normals.push_back(new_norm);
		normals.push_back(new_norm);
	}
	
	for(int i = 0;i < num_faces * 3;i++){
			indices.push_back(i);

	}


	

	this->genBuffers();

	return true;


}


void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	indices.clear();

	vertices.resize(4);
	uvs.resize(4);
	//create four vertices for corner of quad

	vertices[0] =  Vector3(-1,-1,0); uvs[0] = Vector2(0,0);
	vertices[1] =  Vector3(1,-1,0); uvs[1] = Vector2(1,0);
	vertices[2] =  Vector3(1,1,0); uvs[2] = Vector2(1,1);
	vertices[3] =  Vector3(-1,1,0); uvs[3] = Vector2(0,1);

	

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );

	indices.resize(6);
    indices[0] = (0);
    indices[1] = (1);
    indices[2] = (2);
	indices[3] = (0);
	indices[4] = (2);
	indices[5] = (3);


    
    this->genBuffers();
}

void Mesh::setCollisionModel(){
	
	collision_model = newCollisionModel3D();
	collision_model->setTriangleNumber(indices.size() / 3);

	for(int i = 0; i < indices.size() / 3; i++){

		Vector3 v1 = vertices[indices[i * 3]];
		Vector3 v2 = vertices[indices[i * 3 + 1]];
		Vector3 v3 = vertices[indices[i * 3 + 2]];

		collision_model->addTriangle(v1.x,v1.y,v1.z,v2.x,v2.y,v2.z,v3.x,v3.y,v3.z);
	}
	collision_model->finalize();
	
}


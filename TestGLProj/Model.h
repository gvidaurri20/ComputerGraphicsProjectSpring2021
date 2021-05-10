#ifndef MODEL_H
#define MODEL_H
/*
 * Model.h
 * GitHub link: https://github.com/shejan0/BruhComputerGraphicsProject
 * Authors:
 *    Shejan Shuza
 *    Gabriel Vidaurri
 *    Joseph Daau
 *    Tyler Everett
 *    Christopher Urista
 * Description: Model holds the basic elements for storing vertices, indices, and textures (majorly copied from EC Assignment 4 and reworked)
 *
 *
 *
 *
 */

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "tiny_obj_loader.h"
#include <string>
#include "Shader.h"
#include "Error.h"

 /*This class represents a mesh - a collection of triangles. It stores a list of vertices, which are interpreted by openGL as every 3 vertices makes a triangle.*/
class Model {

public:

	/*This struct represents a vertex in 3D space*/
	struct Vertex
	{
		Vertex() : Position(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 1.0f) {} // default values
		glm::vec3 Position; /// x,y,z 
		glm::vec3 Normal; // determines which way the vertex is 'facing'

	};
	struct Textures {
		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
		GLuint normal;
	};

	Model(Shader* shader, const char* filename, const char* materialPath = NULL); //load a model

	Model(Shader* shader);
	~Model(void) {} // default destructor
	void render(glm::mat4 ModelView, glm::mat4 Projection, bool useObjMaterial = false); // render the model
	void setOverrideDiffuseMaterial(glm::vec4 color);
	void setOverrideSpecularMaterial(glm::vec4 color);
	void setOverrideSpecularShininessMaterial(float shine);
	void setOverrideAmbientMaterial(glm::vec4 color);
	void setOverrideEmissiveMaterial(glm::vec4 color);
	friend class BoundingBox;
protected:
	static std::map<std::string, GLuint> textureManager;
	glm::vec4 diffuseOverride;
	glm::vec4 specularOverride;
	float shininessOverride;
	glm::vec4 ambientOverride;
	glm::vec4 emissiveOverride;
	Shader* m_shader; // shader program
	std::vector<tinyobj::shape_t> shapes; //a list of meshes and their respective materials
	std::vector<struct Textures> textures;
	std::vector<GLuint> m_VBO;// vertex buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_NBO;// normal buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_IBO;// index buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_TCBO;// texture coord buffer IDs, each corresponding to a shape
	void updateBuffers(); //initialize your VBO and update when triangles are added
	GLuint LoadTexture(const char* filename);

};
#endif // !1


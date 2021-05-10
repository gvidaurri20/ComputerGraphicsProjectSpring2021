#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "Model.h"
class BoundingBox : public Model{
public:
	BoundingBox(Shader* shader, Model* model);
	void updateBuffers();
	void render(glm::mat4 ModelView, glm::mat4 Projection);
	bool FindBoundaries();
	void checkError(const char* functionName);
	friend class Model;
protected:
	Model* inmodel;
	//float arrx[2], arry[2], arrz[2];
	float vertex3d[8][3];
	//float index3d[24][3];
	GLuint index[24] = { 0,1,1,2,2,3,3,0,0,4,1,5,2,6,3,7,4,5,5,6,6,7,7,4 }; //preconstruct indices
	//GLuint indexbruh[24] = { 0,1,1,2,2,3,3,0,0,4,1,5,2,0,3,7,4,5,5,6,6,7,7,4 };
	float xmin, xmax, ymin, ymax, zmin, zmax;
	
};
#endif
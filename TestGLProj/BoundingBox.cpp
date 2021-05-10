#include "BoundingBox.h"

BoundingBox::BoundingBox(Shader* shader, Model* model):Model(shader){
	BoundingBox::m_shader = shader;
	inmodel = model;
	
	xmax = std::numeric_limits<float>::min();
	//std::cout << std::numeric_limits<float>::min() << std::endl;
	ymax = std::numeric_limits<float>::min();
	zmax = std::numeric_limits<float>::min();
	xmin = std::numeric_limits<float>::max();
	ymin = std::numeric_limits<float>::max();
	zmin = std::numeric_limits<float>::max();
	/*for (int n = 0; n < 8; n++) {
		BoundingBox::m_VBO.push_back(0); //making space inside of the buffer for the vertices
		BoundingBox::m_NBO.push_back(0); //making space inside of the buffer for the normals
		BoundingBox::m_IBO.push_back(0); //making space inside of the buffer for the indices
		glGenBuffers(1, &BoundingBox::m_VBO[n]); // generate a 'name' for the VBO
		glGenBuffers(1, &BoundingBox::m_NBO[n]); // generate a 'name' for the NBO
		glGenBuffers(1, &BoundingBox::m_IBO[n]); // generate a 'name' for the IBO
	}*/

	BoundingBox::m_VBO.push_back(0); //making space inside of the buffer for the vertices
	BoundingBox::m_NBO.push_back(0); //making space inside of the buffer for the normals
	BoundingBox::m_IBO.push_back(0); //making space inside of the buffer for the indices
	glGenBuffers(1, &BoundingBox::m_VBO[0]); // generate a 'name' for the VBO
	glGenBuffers(1, &BoundingBox::m_NBO[0]); // generate a 'name' for the NBO
	glGenBuffers(1, &BoundingBox::m_IBO[0]); // generate a 'name' for the IBO
	/*if (!FindBoundaries()) {
		std::cerr << "Unable to find the boundaries of box" << std::endl;
	}*/
	updateBuffers();
}
void BoundingBox::checkError(const char* functionName)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::cerr << "GL error 0x" << std::hex << error << " detected in BoundingBox::" << functionName << std::endl;
		gluErrorString(error);
	}
}
void BoundingBox::render(glm::mat4 ModelView, glm::mat4 Projection) {
	BoundingBox::m_shader->Activate();
	BoundingBox::m_shader->SetUniform("Projection", Projection);
	BoundingBox::m_shader->SetUniform("ModelView", ModelView);
	BoundingBox::m_shader->SetUniform("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0));
	/*for (int n = 0; n < 8; n++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[n]);
		glEnableVertexAttribArray((*m_shader)["vertexPosition"]);
		glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	}
	for (int n = 0; n < 12; n++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_IBO[n]);
		glDrawElements(GL_LINES, 12, GL_FLOAT, 0);
	}*/
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]); //Bind VBO, there is only one
	glEnableVertexAttribArray((*BoundingBox::m_shader)["vertexPosition"]);
	glVertexAttribPointer((*BoundingBox::m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	//glEnableVertexAttribArray((*BoundingBox::m_shader)["vertexNormal"]);
	//glVertexAttribPointer((*BoundingBox::m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[0]); // Bind IBO, there is only one
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT,0); //draw lines from indices
	BoundingBox::m_shader->DeActivate();
	checkError("render");
}
bool BoundingBox::FindBoundaries() {
	try {
		//printf("x%f x%f y%f y%f z%f z%f\n", xmin, xmax, ymin, ymax, zmin, zmax);
		//std::cout << xmin << xmax << ymin << ymax << zmin << zmax << std::endl;
		for (unsigned int n = 0; n < BoundingBox::inmodel->shapes[0].mesh.positions.size(); n += 3) {
			float curx = BoundingBox::inmodel->shapes[0].mesh.positions[n];
			float cury = BoundingBox::inmodel->shapes[0].mesh.positions[n + 1];
			float curz = BoundingBox::inmodel->shapes[0].mesh.positions[n + 2];

			if (curx > xmax) {
				xmax = curx;
			}
			if (curx < xmin) {
				xmin = curx;
			}
			if (cury > ymax) {
				ymax = cury;
			}
			if (cury < ymin) {
				ymin = cury;
			}
			if (curz > zmax) {
				zmax = curz;
			}
			if (curz < zmin) {
				zmin = curz;
			}
		}
		printf("x%f x%f y%f y%f z%f z%f\n", xmin, xmax, ymin, ymax, zmin, zmax);
		return true;
	}
	catch (...) {
		return false;
	}
}

void BoundingBox::updateBuffers() {
	if (!FindBoundaries()) {
		std::cerr << "Unable to find the boundaries of box" << std::endl;
	}
	//return;
	/*arrx[0] = xmin;
	arrx[1] = xmax;
	arry[0] = ymin;
	arry[1] = ymax;
	arrz[0] = zmin;
	arrz[1] = zmax;
	
	
	arrpos[0] = xmin;
	arrpos[1] = xmax;
	arrpos[2] = ymin;
	arrpos[3] = ymax;
	arrpos[4] = zmin;
	*/
	//Construct vertex buffer
	//Bottom square
	vertex3d[0][0] = xmin;
	vertex3d[0][1] = ymin;
	vertex3d[0][2] = zmin;

	vertex3d[1][0] = xmax;
	vertex3d[1][1] = ymin;
	vertex3d[1][2] = zmin;

	vertex3d[2][0] = xmax;
	vertex3d[2][1] = ymin;
	vertex3d[2][2] = zmax;

	vertex3d[3][0] = xmin;
	vertex3d[3][1] = ymin;
	vertex3d[3][2] = zmax;

	//Top Square
	vertex3d[4][0] = xmin;
	vertex3d[4][1] = ymax;
	vertex3d[4][2] = zmin;

	vertex3d[5][0] = xmax;
	vertex3d[5][1] = ymax;
	vertex3d[5][2] = zmin;

	vertex3d[6][0] = xmax;
	vertex3d[6][1] = ymax;
	vertex3d[6][2] = zmax;

	vertex3d[7][0] = xmin;
	vertex3d[7][1] = ymax;
	vertex3d[7][2] = zmax;
//rintf("%d", sizeof(vertex3d));
	/*
	//Construct indices
	//Line 1,right
	index3d[0][0] = xmin;
	index3d[0][1] = ymin;
	index3d[0][2] = zmin;

	index3d[1][0] = xmax;
	index3d[1][1] = ymin;
	index3d[1][2] = zmin;

	//Line 2,forward
	index3d[2][0] = xmax;
	index3d[2][1] = ymin;
	index3d[2][2] = zmin;

	index3d[3][0] = xmax;
	index3d[3][1] = ymin;
	index3d[3][2] = zmax;

	//Line 3,left
	index3d[4][0] = xmax;
	index3d[4][1] = ymin;
	index3d[4][2] = zmax;
	
	index3d[5][0] = xmin;
	index3d[5][1] = ymin;
	index3d[5][2] = zmax;
	
	//Line 4,backward (bottom square done)
	index3d[6][0] = xmin;
	index3d[6][1] = ymin;
	index3d[6][2] = zmax;

	index3d[7][0] = xmin;
	index3d[7][1] = ymin;
	index3d[7][2] = zmin;

	//Line 5,up
	index3d[8][0] = xmin;
	index3d[8][1] = ymin;
	index3d[8][2] = zmin;

	index3d[9][0] = xmin;
	index3d[9][1] = ymax;
	index3d[9][2] = zmin;

	//Line 6, up
	index3d[10][0] = xmin;
	index3d[10][1] = ymin;
	index3d[10][2] = zmax;

	index3d[11][0] = xmin;
	index3d[11][1] = ymax;
	index3d[11][2] = zmax;

	//Line 7,up
	index3d[12][0] = xmax;
	index3d[12][1] = ymin;
	index3d[12][2] = zmax;
	
	index3d[13][0] = xmax;
	index3d[13][1] = ymax;
	index3d[13][2] = zmax;

	//Line 8, up (all upward legs finished)
	index3d[14][0] = xmax;
	index3d[14][1] = ymin;
	index3d[14][2] = zmin;
	
	index3d[15][0] = xmax;
	index3d[15][1] = ymax;
	index3d[15][2] = zmin;

	//Line 9,top-left corner to right
	index3d[16][0] = xmin;
	index3d[16][1] = ymax;
	index3d[16][2] = zmin;
	
	index3d[17][0] = xmax;
	index3d[17][1] = ymax;
	index3d[17][2] = zmin;

	//Line 10, forward
	index3d[18][0] = xmax;
	index3d[18][1] = ymax;
	index3d[18][2] = zmin;
	
	index3d[19][0] = xmax;
	index3d[19][1] = ymax;
	index3d[19][2] = zmax;
	
	//Line 11,left
	index3d[20][0] = xmax;
	index3d[20][1] = ymax;
	index3d[20][2] = zmax;

	index3d[21][0] = xmin;
	index3d[21][1] = ymax;
	index3d[21][2] = zmax;

	//Line 12, backward, top square done
	index3d[22][0] = xmin;
	index3d[22][1] = ymax;
	index3d[22][2] = zmax;

	index3d[23][0] = xmin;
	index3d[23][1] = ymax;
	index3d[23][2] = zmin;
	*/
	/*for (int n = 0; n < 8; n++) {
		glBindBuffer(GL_ARRAY_BUFFER, BoundingBox::m_VBO[n]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), vertex3d[n], GL_STATIC_DRAW);
	}
	
	for (int n = 0; n < 12; n++) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoundingBox::m_IBO[n]);
		std::cerr << BoundingBox::m_IBO.size()<<std::endl;
		//This line initiates the error
		
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 *sizeof(float), index3d[n], GL_STATIC_DRAW);
	}*/
	glBindBuffer(GL_ARRAY_BUFFER, BoundingBox::m_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), &vertex3d[0][0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoundingBox::m_IBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 2 * sizeof(GLuint), index, GL_STATIC_DRAW);
	checkError("updateBuffers");
}

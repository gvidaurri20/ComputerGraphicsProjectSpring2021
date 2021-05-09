#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
 * Camera.h
 *
 * Authors:
 *    Gabriel Vidaurri
 *    Joseph Daau
 *
 * Last Modified Date: 4/4/21
 *
 * Header file for Camera.cpp
 *
 */

class Camera
{
public:
	Camera(){};

	struct CameraMovement
	{
		glm::vec3 eyeReturn; // Represents the camera eye
		glm::vec3 centerReturn; // Represents the camera center
		glm::vec3 lookatdirReturn;  // Represents the direction the camera is looking at
	};

	// This will be called in main()'s keyboard()
	CameraMovement CustomCameraKeyboard(unsigned char key, glm::vec3 move, glm::vec3 center);
	CameraMovement FlyCameraKeyboard(int key, glm::vec3 move, glm::vec3 center);

	~Camera(void) {} // Default destructor

	virtual bool OnKeyboard(int Key){return false;};

	virtual void OnMouse(int x, int y){};

	virtual void OnRender(){};

    glm::vec3& GetPos() 
    {
        return m_pos;
    }

    glm::vec3& GetTarget() 
    {
        return m_pos+m_lookat;
    }

    glm::vec3& GetUp() 
    {
        return m_up;
    }
	

protected:
	glm::vec3 m_pos;    
	glm::vec3 m_lookat;
    glm::vec3 m_up;

};

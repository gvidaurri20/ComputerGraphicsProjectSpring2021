#include "Camera.h"

/*
 * Camera.cpp
 * 
 * Authors:
 *    Gabriel Vidaurri
 *    Joseph Daau
 *
 * Last Modified Date: 5/8/21
 *
 * Represents the camera for our Doom game. Has functions for our first-person camera, in addition to 
 * allowing the user to switch to a fly mode camera for debugging purposes.
 *
 */


Camera::CameraMovement sendValCamcustom = Camera::CameraMovement();
Camera::CameraMovement sendValCamcustomFly = Camera::CameraMovement();

glm::vec4 vec4Eye; // Converts eye from vec3 to vec4 temporarily in order to rotate it with glm::translate
glm::vec4 vec4Center; // Converts center from vec3 to vec4 temporarily in order to rotate it with glm::rotate
glm::vec3 lookUp = glm::vec3(0.0f, 1.0f, 0.0f); // vec3 to rotate camera upwards and downwards in camera fly mode

int degreeCounter = 0; // Limits how far up and down you can look in fly mode. (at +10 and -10 respectively)


// Points for our camera
// Our Doom camera movement and rotations
Camera::CameraMovement Camera::CustomCameraKeyboard(unsigned char key, glm::vec3 eye, glm::vec3 center)
{
	glm::vec3 x1 = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), center - eye));
	glm::vec3 lookatdir = glm::normalize(center - eye); // Normalizes the direction vector that the camera is looking at
	vec4Center = glm::vec4(center, 1.0f); // Converts center from vec3 to vec4 temporarily in order to rotate it with glm::rotate
	vec4Eye = glm::vec4(eye, 1.0f);

	glm::vec3 strafeVec = glm::normalize(glm::cross(lookatdir, glm::cross(lookatdir, x1)));
	

	// Activates each case depending on which key on the keyboard is pressed
	switch (key) 
	{
		case 27: // This is an ASCII value respresenting the ESC key
			exit(0);
			break;
		
		case 'q': // Strafe camera left //DOES NOT WORK
			//eye += x1;
			//center += x1;
			printf("strafeVec: %f %f %f\n", strafeVec.x, strafeVec.y, strafeVec.z);
			vec4Eye = glm::translate(strafeVec) * vec4Eye;
			vec4Center = glm::translate(strafeVec) * vec4Center;
			eye = glm::vec3(vec4Eye);
			center = glm::vec3(vec4Center);
			break;

		case 'e': // Strafe camera right //DOES NOT WORK
			//eye -= x1;
			//center -= x1;
			printf("-strafeVec: %f %f %f\n", -strafeVec.x, -strafeVec.y, -strafeVec.z);
			vec4Eye = glm::translate(-strafeVec) * vec4Eye;
			vec4Center = glm::translate(-strafeVec) * vec4Center;
			eye = glm::vec3(vec4Eye);
			center = glm::vec3(vec4Center);
			break;

		case 'w': // Moves camera forward
			//eye += lookatdir;
			//center += lookatdir;
			vec4Eye = glm::translate(lookatdir) * vec4Eye;
			vec4Center = glm::translate(lookatdir) * vec4Center;
			eye = glm::vec3(vec4Eye);
			center = glm::vec3(vec4Center);
			break;

		case 's': // Moves camera back
			//eye -= lookatdir;
			//center -= lookatdir;
			vec4Eye = glm::translate(-lookatdir) * vec4Eye;
			vec4Center = glm::translate(-lookatdir) * vec4Center;
			eye = glm::vec3(vec4Eye);
			center = glm::vec3(vec4Center);
			break;

		case 'a': // Rotates camera to the left
			// Applies rotation to the vector for our center so our camera can rotate
			vec4Center = glm::translate(eye.x, eye.y, eye.z)
				* glm::rotate(5.0f, 0.0f, 1.0f, 0.0f)
				* glm::translate(-eye.x, -eye.y, -eye.z)
				* vec4Center;
			center = glm::vec3(vec4Center);
			break;

		case 'd': // Rotates camera to the right
			// Applies rotation to the vector for our center so our camera can rotate
			vec4Center = glm::translate(eye.x, eye.y, eye.z)
				* glm::rotate(-5.0f, 0.0f, 1.0f, 0.0f)
				* glm::translate(-eye.x, -eye.y, -eye.z)
				* vec4Center;
			center = glm::vec3(vec4Center);
			break;

		// The following cases are for flyby camera controls only
		case 'f': // Moves fly camera forward in the direction the camera is looking
			eye += lookatdir;
			center += lookatdir;
			break;

		case 'v': // Moves fly camera backwards in the direction the camera is looking
			eye -= lookatdir;
			center -= lookatdir;
			break;

	}

	// Return our center and eye values back to the caller
	sendValCamcustom.centerReturn = center;
	sendValCamcustom.eyeReturn = eye;
	sendValCamcustom.lookatdirReturn = lookatdir;
	return sendValCamcustom;
}


// Handles our special key input (basically the directional arrow keys)
Camera::CameraMovement Camera::FlyCameraKeyboard(int key, glm::vec3 eye, glm::vec3 center)
{
	// Makes the center a vec4 for easy calculations
	vec4Center = glm::vec4(center, 1.0f); 

	// x2 is the distance to move forward in 1 key which is the crossproduct of what we call up and the lookat direction
	glm::vec3 x2 = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), center - eye));

	glm::vec3 lookatdir = glm::normalize(center - eye); // Normalizes the direction vector that the camera is looking at

	// Activates each case depending on which directional arrow key on the keyboard is pressed
	switch (key)
	{
		case GLUT_KEY_UP: // Makes our fly cam look up to a 45 degree max
			// Updates the degree counter if we can to maximize it up to 10. (10 represents a positive 45 degree angle)
			if (degreeCounter < 10)
				degreeCounter += 1;
			if (degreeCounter * 5 <= 45 && degreeCounter * 5 >= -45)
				center = center + lookUp;
			break;

		case GLUT_KEY_DOWN: // Makes our fly cam look down to a -45 degree min
			// Updates the degree counter if we can to minimize it down to -10. (-10 represents a negative 45 degree angle)
			if (degreeCounter > -10)
				degreeCounter -= 1;
			if (degreeCounter * 5 <= 45 && degreeCounter * 5 >= -45)
				center = center - lookUp;
			break;

		case GLUT_KEY_LEFT: // Rotates our fly cam to the left
			eye += x2;
			center += x2;
			break;

		case GLUT_KEY_RIGHT: // Rotates our fly cam to the right
			eye -= x2;
			center -= x2;
			break;
	}

	// Return our center and eye values back to the caller
	sendValCamcustomFly.centerReturn = center;
	sendValCamcustomFly.eyeReturn = eye;
	return sendValCamcustomFly;
}

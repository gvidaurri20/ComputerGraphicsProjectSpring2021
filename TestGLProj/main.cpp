#define PROJECT_NAME "Final Project for CS 4383 Quarles-Spring 2021"
#define GROUP_NUM "Group 42"
#define LAST_EDIT_DATE "5/9/21 2:52AM"
#define LAST_EDITOR "Gabe Vidaurri"

#include <GL/glew.h>
#include <GL/freeglut.h>

//glm library
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp> // Needed to debug glm things in output window

#include "Model.h"
#include "Shader.h"
#include "QuatCamera.h"
#include "BoundingBox.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <windows.h> // Needed to play music
#include <mmsystem.h> // Needed to play music

/*
 * main.cpp
 * Authors: Gabriel Vidaurri (wre774)
 *          Joseph Daau (ijr855)
 *          Christopher Urista (tgm563)
 *          Shejan Shuza (ovv180)
 * Date: 5/9/21
 *
 * Description: Defines the main() for the program and acts as controller for many core elements.
 *
 */

 /* -- Shader and Model Declarations -- */
Shader shader; // loads our vertex and fragment shaders
Model* ground; // The ground
Model* ceiling; // The ceiling
Model* sphere; // The floating monkey sphere in the center of the scene
Model* sphereLight;
Model* cylinder; // The floating cylinder above the monkey sphere
Model* gun; // The gun
Model* gunMuzzleLight; // The gun light above the muzzle 
Model* demon;
Model* torch;
/* -- Shader and Model Declarations End Here -- */
/* -- Enemy model Declarations -- */
Model* demonModel;
Model* obamidModel;
/* -- Enemy model Declarations End Here -- */
float angle2;
/* -- Wall Model Declarations -- */
Model* wall1, * wall2, * wall3, * wall4, * wall5, * wall6, * wall7,
* wall8, * wall9, * wall10, * wall11, * wall12, * wall13, * wall14,
* wall15, * wall16, * wall17, * wall18, * wall19, * wall20, * wall21;


//mat4 record of each wall
glm::mat4 wallMat[32];
Model* wallModelArr[32];



/* -- Wall Model Declarations End Here -- */

bool collisionDetection = false;


/* -- Matrix Declarations -- */
glm::mat4 projectionMatrix; // Projection matrix
glm::mat4 viewMatrix; // Where the camera is looking
glm::mat4 modelMatrix; // Where the overall model is located with respect to the camera
glm::mat4 prevModelMatrix; //previous version of the model matrix


glm::mat4 headModelMatrix; // Model matrix representing the head's position

glm::mat4 sphereTransMatrix; // Where the sphere model is located wrt the camera
glm::mat4 cubeTransMatrix; // Where the cube model is located wrt the camera
glm::mat4 cylinderTransMatrix; // Where the cylinder model is located wrt the camera
glm::mat4 planeTransMatrix; // (The ground) Where the plane model is located wrt the camera
/* -- Matrix Declarations End Here -- */
/*-- used for the rotation of our objects -- */
glm::mat4 demonsMatrix, demons2Matrix;
glm::mat4 obamid;
/*--used for the rotation of our objects --*/

/* -- Point Light Declarations -- */
float rotation = 0.0f;
glm::vec4 lightPosition1 = glm::vec4(0.0f, 3.0f, 0.0f, 1.0f);
glm::vec4 lightPosition2 = glm::vec4(40.0f, 3.0f, 0.0f, 1.0f);
glm::vec4 lightPosition3 = glm::vec4(-40.0f, 3.0f, 0.0f, 1.0f);
//glm::vec4 lightPosition4 = glm::vec4(0.0f, 0.0f, -60.0f, 1.0f);
/* -- Point Light Declarations End Here -- */

/* -- Camera Variable Declarations -- */
//QuatCamera * camera; // Represents the first-person camera in this scene
Camera::CameraMovement retValCamcustom = Camera::CameraMovement(); // Struct to hold our values for 1st person camera
Camera::CameraMovement retValCamcustomFly = Camera::CameraMovement(); // Struct to hold our values for fly mode camera
Camera customCam;
glm::vec3 eye(0.0f, 0.0f, 0.0f);  // The eye of the camera in first-person
glm::vec3 center(0.0f, 0.0f, 1.0f); // The center of the camera's focus in first-person

glm::vec3 eyeFly; // The eye of the camera in fly mode
glm::vec3 centerFly; // The center of the camera's focus in fly mode
glm::vec3 lookatdirH;


bool isFirstPersonCamera = true; // Bool for camera being first person or not
/* -- Camera Variable Declarations End Here -- */

/* -- Boolean Toggle Variables Declarations -- */
bool isSpotlightOnGun = false; // Toggle for position of the spotlight in the scene. True = on gun. False = on monkey sphere pointing up.
bool isSpotlightOn = true; // Toggle for whether the spotlight in the scene is on/off. Default on at the start.
/* -- Boolean Toggle Variables Declarations End Here -- */

/*Bounding box declaration*/


void initShader(void)
{
	shader.InitializeFromFile("shaders/phong3.vert", "shaders/phong3.frag");
	//shader.AddAttribute("vertexPosition");
	//shader.AddAttribute("vertexNormal");

	checkError("initShader");
}


void initRendering(void)
{
	glClearColor(0.117f, 0.565f, 1.0f, 0.0f); // Dodger Blue
	checkError("initRendering");
}


void init(void)
{
	// View looking into the screen.
	glm::vec3 initpos = glm::vec3(0.0f, 0.0f, -10.0f);
	glm::vec3 initlookatpnt = glm::vec3(.0f, .0f, -1.0f);
	//camera = new QuatCamera(800,600,initpos, initlookatpnt, glm::vec3(0.0f, 1.0f, 0.0f));

	// Perspective projection matrix.
	projectionMatrix = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 1000.0f);

	// Load identity matrix into model matrix.
	modelMatrix = glm::mat4();

	// Calculates the initial model matrix of the head in order to be able to set the eye of the first-person camera at the robot's starting position.
	modelMatrix = sphereTransMatrix;
	headModelMatrix = modelMatrix;
	eye = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

	// Initializes the data for our struct for our camera
	retValCamcustom.eyeReturn = eye;
	retValCamcustom.centerReturn = center;

	initShader();
	initRendering();
}


/* This prints in the console when you start the program */
void dumpInfo(void)
{
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	checkError("dumpInfo");
}




/*Function for Collision Detection
* Input: 
* glm::mat4 playerMatrix - this is the model matrix for the player camera
* glm::mat4 wall - wall matrix to detect
* 
* Purpose: Check for collision detection between player and a wall
* 
* Return:
* True - there is a detection
* False - no detection
*/
bool CheckDetection(glm::mat4 playerMatrix, Model* wall, glm::mat4 wallMath)
{
	//player and wall position
	/*
	glm::vec3 playerPos = glm::vec3(playerMatrix[3].x, playerMatrix[3].y, playerMatrix[3].z);
	glm::vec3 WallPos = glm::vec3(wall[3].x, wall[3].y, wall[3].z);

	//get the distance of the two positions
	float dist = glm::distance(playerPos, WallPos);




	//if we are close to the wall, we have a collision detection
	if (dist < 5.0)
	{
		return true;
	}
	*/

	//get our bounding box and find its boundaries
	BoundingBox box = BoundingBox::BoundingBox(&shader, wall);
	box.FindBoundaries();

	//player position
	glm::vec3 playerPos = glm::vec3(playerMatrix[3].x, playerMatrix[3].y, playerMatrix[3].z);

	//take in a mat4 of each wall
	//multiply the mins and maxes by the wall material
	glm::vec4 minValues = glm::vec4(box.xmin, box.ymin, box.zmin, 1.0f);
	minValues = wallMath * minValues;

	glm::vec4 maxValues = glm::vec4(box.xmax, box.ymax, box.zmax, 1.0f);
	maxValues = wallMath * maxValues;

	if (playerPos.x - 2 <= maxValues.x && playerPos.y <= maxValues.y && playerPos.z <= maxValues.z && playerPos.x +2 >= minValues.x && playerPos.y >= minValues.y && playerPos.z >= minValues.z)
	{
		return true;
	}




	return false;
}





/* Renders all Walls in the Level */
void renderWalls()
{
	// Sets up wall1 things for shader
	wall1->setOverrideDiffuseMaterial(glm::vec4(0.18, 0.09, 0.2, 1.0));
	wall1->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	//wall1->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	//wall1->setOverrideSpecularShininessMaterial(90.0f);
	wall1->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));

	//furthest right walls, to make the outside border
	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(-100.0f, 0.2f, 0.24f), projectionMatrix, false);

	//wall1 material 
	wallMat[0] = glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(-100.0f, 0.2f, 0.24f);
	wallModelArr[0] = wall1;


	//wall2 material and render
	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(-100.0f, 0.2f, -.24f), projectionMatrix, false);
	wallMat[1] = glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(-100.0f, 0.2f, -.24f);
	wallModelArr[1] = wall1;


	//vertical walls within the border walls. positioned from left to right 
	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-67.0f, 0.2f, -1.3f), projectionMatrix, false);
	wallMat[2] = glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-67.0f, 0.2f, -1.3f);
	wallModelArr[2] = wall1;



	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-25.0f, 0.2f, -1.3f), projectionMatrix, false);
	wallMat[3] =  glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-25.0f, 0.2f, -1.3f);
	wallModelArr[3] = wall1;



	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-20.0f, 0.2f, 0.0f), projectionMatrix, false);
	wallMat[4] =  glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-20.0f, 0.2f, 0.0f);
	wallModelArr[4] = wall1;


	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-60.0f, 0.2f, 0.0f), projectionMatrix, false);
	wallMat[5] = glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(-60.0f, 0.2f, 0.0f);
	wallModelArr[5] = wall1;


	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(20.0f, 0.2f, 0.0f), projectionMatrix, false);
	wallMat[6] = glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(20.0f, 0.2f, 0.0f);
	wallModelArr[6] = wall1;



	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(30.0f, 0.2f, -1.3f), projectionMatrix, false);
	wallMat[7] = glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(30.0f, 0.2f, -1.3f);
	wallModelArr[7] = wall1;


	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(60.0f, 0.2f, 0.0f), projectionMatrix, false);
	wallMat[8] = glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(60.0f, 0.2f, 0.0f);
	wallModelArr[8] = wall1;



	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(70.0f, 0.2f, -1.3f), projectionMatrix, false);
	wallMat[9] = glm::scale(1.0f, 20.0f, 135.0f) * glm::translate(70.0f, 0.2f, -1.3f);
	wallModelArr[9] = wall1;


	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 300.0f) * glm::translate(90.0f, 0.2f, 0.0f), projectionMatrix, false);
	wallMat[10] =  glm::scale(1.0f, 20.0f, 300.0f) * glm::translate(90.0f, 0.2f, 0.0f);
	wallModelArr[10] = wall1;



	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 150.0f) * glm::translate(30.0f, 0.2f, 1.0f), projectionMatrix, false);
	wallMat[11] =  glm::scale(1.0f, 20.0f, 150.0f) * glm::translate(30.0f, 0.2f, 1.0f);
	wallModelArr[11] = wall1;


	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 150.0f) * glm::translate(-30.0f, 0.2f, 1.0f), projectionMatrix, false);
	wallMat[12] =  glm::scale(1.0f, 20.0f, 150.0f) * glm::translate(-30.0f, 0.2f, 1.0f);
	wallModelArr[12] = wall1;


	//horizontal walls within the border border 
	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(0.3f, 0.2f, 10.5f), projectionMatrix, false);
	wallMat[13] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(0.3f, 0.2f, 10.5f);
	wallModelArr[13] = wall1;


	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-0.3f, 0.2f, -10.5f), projectionMatrix, false);
	wallMat[14] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-0.3f, 0.2f, -10.5f);
	wallModelArr[14] = wall1;


	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-0.3f, 0.2f, -10.5f), projectionMatrix, false);
	wallMat[15] = glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-0.3f, 0.2f, -10.5f);
	wallModelArr[15] = wall1;


	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-0.3f, 0.2f, -40.5f), projectionMatrix, false);
	wallMat[16] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-0.3f, 0.2f, -40.5f);
	wallModelArr[16] = wall1;



	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-0.3f, 0.2f, 40.5f), projectionMatrix, false);
	wallMat[17] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-0.3f, 0.2f, 40.5f);
	wallModelArr[17] = wall1;


	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(0.3f, 0.8f, 10.5f), projectionMatrix, false);
	wallMat[18] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(0.3f, 0.8f, 10.5f);
	wallModelArr[18] = wall1;


	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(0.3f, 0.8f, -10.5f), projectionMatrix, false);
	wallMat[19] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(0.3f, 0.8f, -10.5f);
	wallModelArr[19] = wall1;


	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-0.3f, 0.3f, -10.5f), projectionMatrix, false);
	wallMat[20] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-0.3f, 0.3f, -10.5f);
	wallModelArr[20] = wall1;


	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(1.3f, 0.3f, -10.5f), projectionMatrix, false);
	wallMat[21] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(1.3f, 0.3f, -10.5f);
	wallModelArr[21] = wall1;


	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-1.3f, 0.3f, -10.5f), projectionMatrix, false);
	wallMat[22] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-1.3f, 0.3f, -10.5f);
	wallModelArr[22] = wall1;


	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-1.3f, 0.3f, 10.5f), projectionMatrix, false);
	wallMat[23] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-1.3f, 0.3f, 10.5f);
	wallModelArr[23] = wall1;


	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-1.3f, 0.3f, 20.5f), projectionMatrix, false);
	wallMat[24] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-1.3f, 0.3f, 10.5f);
	wallModelArr[24] = wall1;

	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-1.3f, 0.3f, 20.5f), projectionMatrix, false);
	wallMat[25] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-1.3f, 0.3f, 20.5f);
	wallModelArr[25] = wall1;

	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-2.3f, 0.3f, 20.5f), projectionMatrix, false);
	wallMat[26] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(-2.3f, 0.3f, 20.5f);
	wallModelArr[26] = wall1;

	wall1->render(viewMatrix * glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(2.3f, 0.3f, 20.5f), projectionMatrix, false);
	wallMat[27] =  glm::scale(40.0f, 20.0f, 5.0f) * glm::translate(2.3f, 0.3f, 20.5f);
	wallModelArr[27] = wall1;

	//horizontal walls to make top and bottom border 
	wall1->render(viewMatrix * glm::scale(200.0f, 20.0f, 5.0f) * glm::translate(0.0f, 0.2f, 60.5f), projectionMatrix, false);
	wallMat[28] =  glm::scale(200.0f, 20.0f, 5.0f) * glm::translate(0.0f, 0.2f, 60.5f);
	wallModelArr[28] = wall1;

	wall1->render(viewMatrix * glm::scale(200.0f, 20.0f, 5.0f) * glm::translate(0.0f, 0.2f, -60.5f), projectionMatrix, false);
	wallMat[29] =  glm::scale(200.0f, 20.0f, 5.0f) * glm::translate(0.0f, 0.2f, -60.5f);
	wallModelArr[29] = wall1;

	//furthest left walls, makes the outside border
	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(100.0f, 0.2f, 0.24f), projectionMatrix, false);
	wallMat[30] =  glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(100.0f, 0.2f, 0.24f);
	wallModelArr[30] = wall1;

	wall1->render(viewMatrix * glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(100.0f, 0.2f, -.24f), projectionMatrix, false);
	wallMat[31] =  glm::scale(1.0f, 20.0f, 400.0f) * glm::translate(100.0f, 0.2f, -.24f);
	wallModelArr[31] = wall1;
}
void renderDemons()
{
	//demon
	demonsMatrix = demonsMatrix * glm::rotate(1.0f, 1.0f, angle2 += 4.5, 0.0f);
	demonModel->render(viewMatrix * demonsMatrix * glm::translate(-3.0f, 0.0f, 0.0f), projectionMatrix, false);

	//obamid
	obamid = obamid * glm::rotate(1.0f, 1.0f, angle2 += 4.5, 0.0f);
	obamidModel->render(viewMatrix * obamid * glm::translate(13.0f, 0.0f, 0.0f), projectionMatrix, false);


}

/* Loads all Wall models in the Level */
void wallModels()
{
	wall1 = new Model(&shader, "models/unitcube.obj", "models/");
	wall2 = new Model(&shader, "models/unitcube.obj", "models/");
	wall3 = new Model(&shader, "models/unitcube.obj", "models/");
	wall4 = new Model(&shader, "models/unitcube.obj", "models/");
	wall5 = new Model(&shader, "models/unitcube.obj", "models/");
	wall6 = new Model(&shader, "models/unitcube.obj", "models/");
	wall7 = new Model(&shader, "models/unitcube.obj", "models/");
	wall8 = new Model(&shader, "models/unitcube.obj", "models/");
	wall9 = new Model(&shader, "models/unitcube.obj", "models/");
	wall10 = new Model(&shader, "models/unitcube.obj", "models/");
	wall11 = new Model(&shader, "models/unitcube.obj", "models/");
	wall12 = new Model(&shader, "models/unitcube.obj", "models/");
	wall13 = new Model(&shader, "models/unitcube.obj", "models/");
	wall14 = new Model(&shader, "models/unitcube.obj", "models/");
	wall15 = new Model(&shader, "models/unitcube.obj", "models/");
	wall16 = new Model(&shader, "models/unitcube.obj", "models/");
	wall17 = new Model(&shader, "models/unitcube.obj", "models/");
	wall18 = new Model(&shader, "models/unitcube.obj", "models/");
	wall19 = new Model(&shader, "models/unitcube.obj", "models/");
}


/* This gets called when the OpenGL is asked to display. This is where all the main rendering calls go. */
void display(void)
{
	//camera->OnRender();

	/* The transformation heirarchy is cylinder -> sphere -> cube */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the framebuffer data from the last frame

	// Camera view for our Doom game
	// Changes the view matrix depending on which camera type is being used in the program.
	if (isFirstPersonCamera == true)
		viewMatrix = glm::lookAt(retValCamcustom.eyeReturn, retValCamcustom.centerReturn, glm::vec3(0.0f, 1.0f, 0.0f));
	else
		viewMatrix = glm::lookAt(retValCamcustomFly.eyeReturn, retValCamcustomFly.centerReturn, glm::vec3(0.0f, 1.0f, 0.0f));

	//viewMatrix = glm::lookAt(camera->GetPos(), camera->GetLookAtPoint(), camera->GetUp());

	rotation += 0.05f; // Update rotation angle if rotation is enabled.

	// Info for First Light at Start of Level
	glm::vec4 lightPos1 = /*glm::rotate(rotation,0.0f, 0.0f, 1.0f) */ lightPosition1;
	shader.Activate(); // Bind shader.
	shader.SetUniform("lightPosition[0]", viewMatrix * lightPos1);
	shader.SetUniform("lightDiffuse[0]", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightSpecular[0]", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightAmbient", glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Info for Second Light
	glm::vec4 lightPos2 = lightPosition2;
	shader.SetUniform("lightPosition[1]", viewMatrix * lightPos2);
	shader.SetUniform("lightDiffuse[1]", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightSpecular[1]", glm::vec4(1.0, 1.0, 1.0, 1.0));
	//shader.SetUniform("lightAmbient[1]", glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Info for Third Light
	glm::vec4 lightPos3 = lightPosition3;
	shader.SetUniform("lightPosition[2]", viewMatrix * lightPos3);
	shader.SetUniform("lightDiffuse[2]", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightSpecular[2]", glm::vec4(1.0, 1.0, 1.0, 1.0));
	//shader.SetUniform("lightAmbient[2]", glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Info for Fourth Light
	//glm::vec4 lightPos4 = lightPosition4;
	//shader.SetUniform("lightPosition[3]", viewMatrix * lightPos4);
	//shader.SetUniform("lightDiffuse[3]", glm::vec4(1.0, 1.0, 1.0, 1.0));
	//shader.SetUniform("lightSpecular[3]", glm::vec4(1.0, 1.0, 1.0, 1.0));
	//shader.SetUniform("lightAmbient[3]", glm::vec4(1.0, 1.0, 1.0, 1.0));

	shader.SetUniform("howManyLights", 3); // Total point lights in the game

	// This section of code determines whether or not the spotlight will be on the gun or in the center
	// monkey sphere point up. It does this by moving the spotlight's position and direction and also 
	// slightly changes the cutoff angle of the cone produced by the spotlight in order to better display
	// on the scene.
	if (isSpotlightOnGun == true) // When spotlight is on the gun pointing forwards
	{
		shader.SetUniform("isSpotlightOnGunVert", true);
		shader.SetUniform("isSpotlightOnGunFrag", true);
		shader.SetUniform("spotlightPosition", glm::vec4(0.0, 0.0, 0.0, 0.0));
		shader.SetUniform("spotlightDirection", glm::vec4(0.0, 0.0, -1.0, 1.0));
		shader.SetUniform("cutoffAngle", 5.0f);
	}
	else // Default Spotlight pointed upwards at floating cylinder
	{
		shader.SetUniform("isSpotlightOnGunVert", false);
		shader.SetUniform("isSpotlightOnGunFrag", false);
		shader.SetUniform("spotlightPosition", glm::vec4(0.0, 0.0, 0.0, 0.0));
		shader.SetUniform("spotlightDirection", glm::vec4(0.0, 1.0, 0.0, 1.0));
		shader.SetUniform("cutoffAngle", 20.0f);
	}

	// This section of code determines whether or not the spotlight will be on or off by changing
	// the spotlight's diffuse and specular.
	if (isSpotlightOn == true) // If the spotlight in the scene is on
	{
		shader.SetUniform("spotlightDiffuse", glm::vec4(2.0, 2.0, 2.0, 1.0));
		shader.SetUniform("spotlightSpecular", glm::vec4(1.0, 1.0, 1.0, 1.0));
	}
	else // If the spotlight in the scene is off
	{
		shader.SetUniform("spotlightDiffuse", glm::vec4(0.0, 0.0, 0.0, 1.0));
		shader.SetUniform("spotlightSpecular", glm::vec4(0.0, 0.0, 0.0, 1.0));
	}
	shader.SetUniform("spotlightExponent", 0.001f);

	headModelMatrix = modelMatrix;

	/* -- Renders objects in the scene -- */
	bool useMat = false; // Whether to use the matrix of not

	// Sphere
	sphere->setOverrideDiffuseMaterial(glm::vec4(0.0, 0.0, 1.0, 1.0));
	sphere->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	sphere->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	sphere->setOverrideSpecularShininessMaterial(90.0f);
	sphere->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	sphere->render(viewMatrix, projectionMatrix, false); // Render sphere in the middle of the area.

	// Sphere Lights
	//sphereLight->render(viewMatrix * glm::translate(lightPos1.x, lightPos1.y, lightPos1.z) * glm::scale(.1f, .1f, .1f), projectionMatrix, true); // Render sphere light at beginning of level
	//sphereLight->setOverrideEmissiveMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	//sphereLight->render(viewMatrix * glm::translate(lightPos2.x, lightPos2.y, lightPos2.z) * glm::scale(.1f, .1f, .1f), projectionMatrix, true); // Render sphere light in left room
	torch->render(viewMatrix * glm::translate(lightPos2.x, lightPos2.y - 2.0f, lightPos2.z), projectionMatrix, false);
	sphereLight->setOverrideEmissiveMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	//sphereLight->render(viewMatrix * glm::translate(lightPos3.x, lightPos3.y, lightPos3.z) * glm::scale(.1f, .1f, .1f), projectionMatrix, true); // Render rotating sphere light
	torch->render(viewMatrix * glm::translate(lightPos3.x, lightPos3.y - 2.0f, lightPos3.z), projectionMatrix, false);
	sphereLight->setOverrideEmissiveMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	//sphereLight->render(viewMatrix * glm::translate(lightPos4.x, lightPos4.y, lightPos4.z) * glm::scale(.1f, .1f, .1f), projectionMatrix, false); // Render rotating sphere light
	//sphereLight->setOverrideEmissiveMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Gun
	gun->setOverrideDiffuseMaterial(glm::vec4(.3, 0.3, 0.3, 1.0));
	gun->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	gun->setOverrideSpecularShininessMaterial(40.0f);
	gun->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	gun->render(glm::translate(1.0f, -1.0f, -2.0f) * glm::scale(.05f, .05f, .05f) * glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f), projectionMatrix, false); // Render the gun

	// Floating Cylinder
	cylinder->setOverrideDiffuseMaterial(glm::vec4(1.0, 0.0, 0.0, 1.0));
	cylinder->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	cylinder->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	cylinder->setOverrideSpecularShininessMaterial(90.0f);
	cylinder->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	cylinder->render(viewMatrix * glm::translate(0.0f, 5.0f, 0.0f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projectionMatrix, useMat);

	// Renders the ground.
	//ground->render(viewMatrix * glm::translate(0.0f, -5.0f, 0.0f) * glm::scale(100.0f, 100.0f, 300.0f), projectionMatrix);
	// Ground
	ground->setOverrideDiffuseMaterial(glm::vec4(0.0, 0.0, 1.0, 1.0));
	ground->setOverrideAmbientMaterial(glm::vec4(0.3, 0.0, 0.0, 1.0));
	//ground->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	//ground->setOverrideSpecularShininessMaterial(90.0f);
	ground->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	ground->render(viewMatrix * glm::translate(0.0f, -2.0f, 0.0f) * glm::scale(100.0f, 100.0f, 300.0f), projectionMatrix, useMat);

	// Roof
	ceiling->setOverrideDiffuseMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	ceiling->setOverrideAmbientMaterial(glm::vec4(0.2, 0.0, 0.0, 1.0));
	ceiling->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	ceiling->render(viewMatrix * glm::translate(0.0f, 20.0f, 0.0f) * glm::scale(100.0f, 100.0f, 300.0f), projectionMatrix, useMat);




	// Render all the Walls
	renderWalls();
	// Render the enemies
	renderDemons();




	// Gun Muzzle Light
	gunMuzzleLight->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	gunMuzzleLight->setOverrideSpecularShininessMaterial(40.0f);
	gunMuzzleLight->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	gunMuzzleLight->render(glm::translate(0.75f, -0.55f, -3.6f) * glm::scale(0.05f, 0.05f, 0.05f) * glm::rotate(90.0f, 1.0f, 0.0f, 0.0f), projectionMatrix, false);
	
	/* -- Done rendering objects in the scene -- */
	glutSwapBuffers(); // Swap the buffers.

	checkError("display");
}


/* This gets called when nothing is happening (OFTEN) */
void idle()
{
	glutPostRedisplay(); // create a display event. Display calls as fast as CPU will allow when put in the idle function
}


/* Called when the window is resized */
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	checkError("reshape");
}


/* Called when a special key is pressed */
void specialKeyboard(int Key, int x, int y)
{
	//camera->OnKeyboard(Key);
}


// Fly Camera calls are done here
void SpecialKeyHandler(int key, int x, int y)
{
	if (isFirstPersonCamera == false)
	{
		// Call our custom keyboard camera
		retValCamcustomFly = customCam.FlyCameraKeyboard(key, retValCamcustomFly.eyeReturn, retValCamcustomFly.centerReturn);
	}
}


/* Called when a normal key is pressed */
void keyboard(unsigned char key, int x, int y)
{

	// Toggles between first-person and fly camera with 'c'
	if (key == 'c' && isFirstPersonCamera == true)
	{
		// Makes it so that the fly camera always starts viewing at angle towards where the player currently is
		retValCamcustomFly.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 20.0f, headModelMatrix[3].z - 40.0f); // The eye starting point will be further back
		retValCamcustomFly.centerReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y + 5.0f, headModelMatrix[3].z + 10.0f);
		isFirstPersonCamera = false;
	}
	else if (key == 'c' && isFirstPersonCamera == false)
	{
		isFirstPersonCamera = true;
	}

	glm::vec3 x1 = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), center - eye));
	glm::vec3 lookatdir = glm::normalize(center - eye);
	glm::vec3 strafeVec = glm::normalize(glm::cross(lookatdir, glm::cross(lookatdir, x1)));

	// Activates each case depending on which key on the keyboard is pressed
	switch (key)
	{
	case 27: // This is an ASCII value respresenting the ESC key
		exit(0);
		break;

	case 'q': // Strafes left //DOES NOT WORK
		// Sets up the values to send to our camera
		retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

		// Calls our custom keyboard camera
		retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);

		// Updates the model matrix of our character to strafe to the left
		modelMatrix = glm::translate(strafeVec) * modelMatrix;
		break;

	case 'e': // Strafes right //DOES NOT WORK
		// Sets up the values to send to our camera
		retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

		// Calls our custom keyboard camera
		retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);

		// Updates the model matrix of our character to strafe to the right
		modelMatrix = glm::translate(-strafeVec) * modelMatrix;
		break;



	case 'w': // Moves our character forward
		// Sets up the values to send to our camera
		retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

		// Calls our custom keyboard camera
		retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);
		

		//keep a previous marker of our matrix
		prevModelMatrix = modelMatrix;
		
		collisionDetection = false;




		//printf("Previous ModelMat pos is %f %f %f \n", modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);


		modelMatrix = glm::translate(retValCamcustom.lookatdirReturn) * modelMatrix;

		//printf("New ModelMat pos is %f %f %f \n", modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);

		//check if we have ANY detection
		for (int i = 0; i < 32; i++)
		{
			collisionDetection = CheckDetection(modelMatrix, wallModelArr[i], wallMat[i]);

			if (collisionDetection)
			{
				break;
			}

		}

		

		printf("Collision is %d \n", collisionDetection);
		//if we don't have a collision then update modelMatrix accordingly
		if (collisionDetection)
		{
			modelMatrix = prevModelMatrix;
		}





		break;

	case 's': // Moves our character back
		// Sets up the values to send to our camera
		retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

		// Calls our custom keyboard camera
		retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);

		//keep a previous marker of our matrix
		prevModelMatrix = modelMatrix;

		collisionDetection = false;

		modelMatrix = glm::translate(-retValCamcustom.lookatdirReturn) * modelMatrix;

		//check if we have ANY detection
		for (int i = 0; i < 32; i++)
		{
			collisionDetection = CheckDetection(modelMatrix, wallModelArr[i], wallMat[i]);

			if (collisionDetection)
			{
				break;
			}

		}


		

		printf("Collision is %d \n", collisionDetection);
		//if we don't have a collision then update modelMatrix accordingly
		if (collisionDetection)
		{
			modelMatrix = prevModelMatrix;
		}

		break;

	case 'a': // Rotates our character to the left
		// Sets up the values to send to our camera
		retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

		// Calls our custom keyboard camera
		retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);

		// Rotates the model matrix of our character to the left
		modelMatrix = modelMatrix * glm::rotate(5.0f, 0.0f, 1.0f, 0.0f);

		break;

	case 'd': // Rotates our character to the right	
		// Sets up the values to send to our camera
		retValCamcustom.eyeReturn = glm::vec3(headModelMatrix[3].x, headModelMatrix[3].y, headModelMatrix[3].z - 10.0f);

		// Calls our custom keyboard camera
		retValCamcustom = customCam.CustomCameraKeyboard(key, retValCamcustom.eyeReturn, retValCamcustom.centerReturn);

		// Rotates the model matrix of our character to the right
		modelMatrix = modelMatrix * glm::rotate(-5.0f, 0.0f, 1.0f, 0.0f);

		break;



	case 'f': // Zooms in (Fly camera)
		retValCamcustomFly = customCam.CustomCameraKeyboard(key, retValCamcustomFly.eyeReturn, retValCamcustomFly.centerReturn);
		break;

	case 'v': // Zooms out (Fly camera)
		retValCamcustomFly = customCam.CustomCameraKeyboard(key, retValCamcustomFly.eyeReturn, retValCamcustomFly.centerReturn);
		break;




	case 'y': // Toggles Position of Spotlight (Either coming from center monkey sphere or gun)
		if (isSpotlightOnGun == false)
			isSpotlightOnGun = true;
		else if (isSpotlightOnGun == true)
			isSpotlightOnGun = false;
		break;
	}

}


/* Called when the camera needs to move due to mouse movement */
static void passiveMouse(int x, int y)
{
	//camera->OnMouse(x, y);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Simple Doom Game");

	glewInit();
	dumpInfo();
	init();

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeyHandler); // Calls additional function to detect special keys
	glutPassiveMotionFunc(passiveMouse);  // Calls additional function to detect mouse movement

	glEnable(GL_DEPTH_TEST);

	// Loads the models to use in the program.
	ground = new Model(&shader, "models/plane.obj", "models/");
	//ground->setOverrideDiffuseMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ceiling = new Model(&shader, "models/plane.obj", "models/");
	//ceiling->setOverrideDiffuseMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	sphere = new Model(&shader, "models/sphere.obj", "models/");
	sphere->setOverrideDiffuseMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	sphereLight = new Model(&shader, "models/sphere.obj", "models/");
	cylinder = new Model(&shader, "models/cylinder.obj", "models/");
	gun = new Model(&shader, "models/m16_1.obj", "models/");
	gunMuzzleLight = new Model(&shader, "models/cylinder.obj", "models/");
	demonModel = new Model(&shader, "models/cacodemon.obj", "models/");
	obamidModel = new Model(&shader, "models/obamid.obj", "models/");
	demon = new Model(&shader, "models/cacodemon.obj", "models/");
	torch = new Model(&shader, "models/torch.obj", "models/");
	wallModels(); // Loads all wall models in our program

	//PlaySound(TEXT("audio/E1M1.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP); // Plays the theme song from the first level of Doom
	

	glutMainLoop();

	return 0;
}
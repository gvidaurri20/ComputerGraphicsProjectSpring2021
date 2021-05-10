#version 440 core
/*
 * phong3.vert
 * Author: Gabriel Vidaurri (wre774)
 * Date: 4/15/21
 *
 * Represents the vertex shader for our program.
 * Calculates the necessary vectors to pass to the fragment shader.
 *
 */

out vec3 N;
out vec3 L;
out vec3 E;
out vec3 H;
out vec4 eyePosition;
out vec2 texCoordsInterpolated;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTextureCoordinates;
//in vec3 vertexPosition;
//in vec3 vertexNormal;

uniform vec4 lightPosition;
uniform mat4 Projection;
uniform mat4 ModelView;

uniform vec4 lightDiffuse;
uniform vec4 lightSpecular; 
uniform vec4 lightAmbient;
uniform vec4 surfaceDiffuse;
uniform vec4 surfaceSpecular;
uniform float shininess;
uniform vec4 surfaceAmbient;
uniform vec4 surfaceEmissive;
uniform float useTexture;

// Spotlight uniform variables
uniform float cutoffAngle; // Represents the cutoff angle of our cone for our spotlight
uniform vec4 spotlightDiffuse; // Represents the spotlight diffuse
uniform vec4 spotlightSpecular; // Represents the spotlight specular
uniform float spotlightExponent; // Represents the spotlight exponent
uniform vec4 spotlightPosition; // Represents the position of the spotlight
uniform vec4 spotlightDirection; // Represents the direction the spotlight is looking
uniform bool isSpotlightOnGunVert; // Boolean for whether or not the spotlight is on the center monkey sphere or gun

// Vectors related to the spotlight
out vec3 HofSpotlight; // Halfway vector of the spotlight
out vec3 LofSpotlight; // Light of the spotlight
out vec3 VofSpotlight; // Viewing vertex vector of the spotlight


void main()
{
    //if(useTexture > 0.0){
		texCoordsInterpolated = vertexTextureCoordinates;
	//}
    gl_Position = Projection * ModelView * vec4(vertexPosition, 1.0);

    eyePosition = ModelView * vec4(vertexPosition, 1.0);

    vec4 eyeLightPos = lightPosition;
	
	N = normalize(ModelView * vec4(vertexNormal,0.0)).xyz;
    L = normalize(eyeLightPos.xyz - eyePosition.xyz);
    E = -normalize(eyePosition.xyz);
    H = normalize(L + E);


    // If the spotlight is on the gun, then we only need to calculate the L and H of the spotlight given the 
    // eyePosition.
    if (isSpotlightOnGunVert == true)
    {
        LofSpotlight = normalize(spotlightPosition.xyz - eyePosition.xyz);
        HofSpotlight = normalize(LofSpotlight + E);       
    }
}


#version 440 core
/*
 * phong3.frag
 * Author: Gabriel Vidaurri (wre774)
 * Date: 4/15/21
 *
 * Represents the fragment shader for our program.
 * Ultimately calculates the color on a surface for multiple lights. 
 * (Namely the spotlight from the gun and the point light at the beginning of the level.)
 *
 */

in vec3 N;
in vec3 L[4];
in vec3 E;
in vec3 H[4];
in vec4 eyePosition;
in vec2 texCoordsInterpolated;

uniform vec4 lightPosition[4];
uniform mat4 Projection;
uniform mat4 ModelView;

uniform int howManyLights;

uniform vec4 lightDiffuse[4];
uniform vec4 lightSpecular[4]; 
uniform vec4 lightAmbient;
uniform vec4 surfaceDiffuse;
uniform vec4 surfaceSpecular;
uniform float shininess;
uniform vec4 surfaceAmbient;
uniform vec4 surfaceEmissive;
uniform float useTexture;
uniform sampler2D diffuseTexture;
uniform float linearAttenuationCoefficient;

// Spotlight uniform variables
uniform float cutoffAngle; // Represents the cutoff angle of our cone for our spotlight
uniform vec4 spotlightDiffuse; // Represents the spotlight diffuse
uniform vec4 spotlightSpecular; // Represents the spotlight specular
uniform float spotlightExponent; // Represents the spotlight exponent
uniform vec4 spotlightPosition; // Represents the position of the spotlight
uniform vec4 spotlightDirection; // Represents the direction the spotlight is looking
uniform bool isSpotlightOnGunFrag; // Boolean for whether or not the spotlight is on the center monkey sphere or gun

in vec3 HofSpotlight; // Halfway vector of the spotlight received from vertex shader
in vec3 LofSpotlight; // Light of the spotlight received from vertex shader
in vec3 VofSpotlight; // Viewing vertex vector of the spotlight received from vertex shader


void main()
{
    vec3 Normal = normalize(N);
    vec3 Light[4];
    Light[0] = normalize(lightPosition[0] - eyePosition).xyz;
    Light[1] = normalize(lightPosition[1] - eyePosition).xyz;
    Light[2] = normalize(lightPosition[2] - eyePosition).xyz;
    //Light[3] = normalize(lightPosition[3] - eyePosition).xyz;
    vec3 Eye    = normalize(E);
    vec3 Half[4];
    Half[0] = normalize(H[0]);
    Half[1] = normalize(H[1]);
    Half[2] = normalize(H[2]);
    //Half[3] = normalize(H[3]);
	
    float Kd[4];
    Kd[0] = max(dot(Normal, Light[0]), 0.0);
    Kd[1] = max(dot(Normal, Light[1]), 0.0);
    Kd[2] = max(dot(Normal, Light[2]), 0.0);
    //Kd[3] = max(dot(Normal, Light[3]), 0.0);
    float Ks[4];
    Ks[0] = pow(max(dot(reflect(-Light[0], Normal),Eye), 0.0), shininess);
    Ks[1] = pow(max(dot(reflect(-Light[1], Normal),Eye), 0.0), shininess);
    Ks[2] = pow(max(dot(reflect(-Light[2], Normal),Eye), 0.0), shininess);
    //Ks[3] = pow(max(dot(reflect(-Light[3], Normal),Eye), 0.0), shininess);
    float Ka = 1.0;

    vec4 diffuse[4];
    diffuse[0] = Kd[0] * lightDiffuse[0] * surfaceDiffuse;
    diffuse[1] = Kd[1] * lightDiffuse[1] * surfaceDiffuse;
    diffuse[2] = Kd[2] * lightDiffuse[2] * surfaceDiffuse;
    //diffuse[3] = Kd[3] * lightDiffuse[3] * surfaceDiffuse;
    vec4 specular[4];
    specular[0] = Ks[0] * lightSpecular[0] * surfaceSpecular;
    specular[1] = Ks[1] * lightSpecular[1] * surfaceSpecular;
    specular[2] = Ks[2] * lightSpecular[2] * surfaceSpecular;
    //specular[3] = Ks[3] * lightSpecular[3] * surfaceSpecular;
    
    vec4 ambient = Ka * lightAmbient * surfaceAmbient;
    //vec4 ambient[4];
    //ambient[0] = Ka * lightAmbient[0] * surfaceAmbient;
    //ambient[1] = Ka * lightAmbient[1] * surfaceAmbient;
    //ambient[2] = Ka * lightAmbient[2] * surfaceAmbient;
    //ambient[3] = Ka * lightAmbient[3] * surfaceAmbient;

    float linearAttenuation[4];
    linearAttenuation[0] = min(1.0, 1.0/ (linearAttenuationCoefficient * length(lightPosition[0] - eyePosition)));
    linearAttenuation[1] = min(1.0, 1.0/ (linearAttenuationCoefficient * length(lightPosition[1] - eyePosition)));
    linearAttenuation[2] = min(1.0, 1.0/ (linearAttenuationCoefficient * length(lightPosition[2] - eyePosition)));
    //linearAttenuation[3] = min(1.0, 1.0/ (linearAttenuationCoefficient * length(lightPosition[3] - eyePosition)));
    //float linearAttenuations;
    //for(int i = 0; i < howManyLights; i++)
    //{
    //    linearAttenuations = linearAttenuations + linearAttenuation[i];
    //}

    vec4 LightsStuff[4];
    LightsStuff[0] = linearAttenuation[0] * (diffuse[0] + specular[0]);
    LightsStuff[1] = linearAttenuation[1] * (diffuse[1] + specular[1]);
    LightsStuff[2] = linearAttenuation[2] * (diffuse[2] + specular[2]);
    //LightsStuff[3] = linearAttenuation[3] * (diffuse[3] + specular[3]);


    vec4 totalLights;
    for(int i = 0; i < howManyLights; i++)
    {
        totalLights = totalLights + LightsStuff[i];
    }

    float angle; // Dot product of the V of the spotlight and the direction. Will later need it for the actual 
                 // angle which will be calculated using arccos.

    // If the spotlight is on the gun, then we need to recalculate the V here so that way the spotlight effect 
    // is rendered per pixel, as opposed to per vertex.  Regardless, we will be calculating the angle (dot product)
    // to be used later.
    if (isSpotlightOnGunFrag == true)
    {
        vec3 VofSpotlightNew = normalize(eyePosition.xyz - spotlightPosition.xyz);
        angle = dot(VofSpotlightNew, spotlightDirection.xyz);
    }
    else
        angle = dot(VofSpotlight, spotlightDirection.xyz);

    float cutoff = radians(cutoffAngle); // Calculates the cutoff given the cutoffAngle passed in with degrees.

    float spotlightEffect; // Float for the spotlight effect

    float kdOfSpotlight = max(dot(Normal, LofSpotlight), 0.0);
    float ksOfSpotlight = pow(max(dot(reflect(-LofSpotlight, Normal), Eye), 0.0), shininess);

    vec4 diffuseOfSpotlight = kdOfSpotlight * spotlightDiffuse * surfaceDiffuse;
    vec4 specularOfSpotlight = ksOfSpotlight * spotlightSpecular * surfaceSpecular;

    // Calculates the arccos of the given cosine scalar angle and compares it to the cutoff.
    // If it is less, then that means that the given vertex is within the spotlight .
    // Otherwise it is outside of the spotlight so no effect should be used.
    if(acos(angle) < cutoff) 
        spotlightEffect = pow(max(angle, 0), spotlightExponent);
    else
        spotlightEffect = 0;


    vec4 texColor = vec4(0.0,0.0,0.0,1.0);
	if(useTexture > 0.0){
		texColor = texture2D(diffuseTexture,texCoordsInterpolated);
	}

    // Calculates the color of the surface given all the lights using the Phong Illumination Model
    //gl_FragColor = surfaceEmissive + ambient + /*linearAttenuation * */(diffuse + specular) + texColor + (spotlightEffect * (diffuseOfSpotlight + specularOfSpotlight));
    gl_FragColor = surfaceEmissive + ambient + totalLights + texColor + (spotlightEffect * (diffuseOfSpotlight + specularOfSpotlight));
}

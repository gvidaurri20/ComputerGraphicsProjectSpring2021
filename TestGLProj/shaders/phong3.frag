#version 440 core
/*
 * phong3.frag
 * Author: Gabriel Vidaurri (wre774)
 * Date: 4/15/21
 *
 * Represents the fragment shader for our program.
 * Ultimately calculates the color on a surface for multiple lights.
 *
 */

in vec3 N;
in vec3 L;
in vec3 E;
in vec3 H;
in vec4 eyePosition;
in vec2 texCoordsInterpolated;

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
uniform bool useTexture;
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
    vec3 Light  = normalize(lightPosition - eyePosition).xyz;
    vec3 Eye    = normalize(E);
    vec3 Half   = normalize(H);
	
    float Kd = max(dot(Normal, Light), 0.0);
    float Ks = pow(max(dot(reflect(-Light, Normal),Eye), 0.0), shininess);
    float Ka = 1.0;
    float linearAttenuation = min(1.0, 1.0/ (linearAttenuationCoefficient * length(lightPosition - eyePosition)));
    vec4 diffuse  = Kd * lightDiffuse * surfaceDiffuse;
    vec4 specular = Ks * lightSpecular * surfaceSpecular;
    vec4 ambient  = Ka * lightAmbient * surfaceAmbient;
    vec4 texColor = vec4(1.0,1.0,1.0,1.0);
    
    //gl_FragColor = surfaceEmissive + ambient + (diffuse + specular);

    float angle; // Dot product of the V of the spotlight and the direction. Will later need it for the actual 
                 // angle which will be calculated using arccos.

    // If the spotlight is on the gun, then we need to recalculate the V here so that way the spotlight effect 
    // is rendered per pixel, as opposed to per vertex.  Regardless, we will be calculating the angle (dot product)
    // to be used later.
    if(useTexture){
            texColor = texture2D(diffuseTexture,texCoordsInterpolated);
    }
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

    // Calculates the color of the surface given all the lights using the Phong Illumination Model
    gl_FragColor = surfaceEmissive + ambient + linearAttenuation*(diffuse + specular) + linearAttenuation*(spotlightEffect * (diffuseOfSpotlight + specularOfSpotlight))*texColor ;
    
}

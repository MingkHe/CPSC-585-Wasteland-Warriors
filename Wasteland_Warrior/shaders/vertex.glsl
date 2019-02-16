// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColour;
layout(location = 2) in vec3 Normal;

// output to be interpolated between vertices and passed to the fragment stage
uniform mat4 modelViewProjection;
uniform vec3 light;
uniform vec3 cameraPos;
uniform mat4 transform;
out vec3 Colour;
out vec3 normal;
out vec3 lightVec;
out vec3 cameraVec;

void main()
{
    // assign vertex position without modification
    gl_Position = modelViewProjection*transform*vec4(VertexPosition, 1.0);

    // assign output colour to be interpolated
    Colour = VertexColour;
	lightVec = light - (transform*vec4(VertexPosition, 1.0)).xyz;
	cameraVec = cameraPos - (transform*vec4(VertexPosition, 1.0)).xyz;
	normal = Normal;//(modelViewProjection*vec4(Normal, 1.0)).xyz;
}

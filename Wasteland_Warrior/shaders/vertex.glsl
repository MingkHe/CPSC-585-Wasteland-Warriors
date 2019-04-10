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
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec2 VertexUV;

// output to be interpolated between vertices and passed to the fragment stage
uniform mat4 modelViewProjection;
uniform mat4 transform;
uniform mat4 depthViewProjection;
uniform mat4 depthViewProjectiontwo;
uniform mat4 depthViewProjectionthree;

out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragVert;
out vec4 shadowCoord;
out vec4 shadowCoordtwo;
out vec4 shadowCoordthree;

void main()
{
    // assign vertex position without modification
    gl_Position = modelViewProjection*transform*vec4(VertexPosition, 1.0);

    // assign output colour to be interpolated
	
	fragTexCoord = VertexUV;
	//fragNormal = VertexNormal;
	//fragVert = VertexPosition;
	//fragNormal = normalize((transform*vec4(VertexNormal.x, VertexNormal.z, -VertexNormal.y , 0)).xyz);
	fragNormal = normalize((transform*vec4(VertexNormal, 0)).xyz);
	fragVert = (transform*vec4(VertexPosition, 1)).xyz;
	
	shadowCoord = depthViewProjection * transform*vec4(VertexPosition, 1.0);
	shadowCoordtwo = depthViewProjectiontwo * transform*vec4(VertexPosition, 1.0);
	shadowCoordthree = depthViewProjectionthree * transform*vec4(VertexPosition, 1.0);
}

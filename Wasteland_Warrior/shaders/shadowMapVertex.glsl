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

// output to be interpolated between vertices and passed to the fragment stage
uniform mat4 modelViewProjection;
uniform mat4 transform;

out vec3 pos;

void main()
{
    // assign vertex position without modification
    gl_Position = modelViewProjection*transform*vec4(VertexPosition, 1.0);
}

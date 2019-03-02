// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec2 VertexPosition;
layout(location = 3) in vec2 VertexUV;

// output to be interpolated between vertices and passed to the fragment stage
out vec2 uv;

void main()
{
    // assign vertex position without modification
    gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);

    // assign output colour to be interpolated
    uv = VertexUV;
}

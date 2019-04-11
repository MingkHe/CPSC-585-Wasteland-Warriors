// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

out vec4 finalColor;

void main() {
	gl_FragDepth = gl_FragCoord.z;
	finalColor = vec4(gl_FragCoord.z, gl_FragCoord.z*gl_FragCoord.z, 0, 1);
}

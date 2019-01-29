// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec3 Colour;
//in vec2 texCoord;
//uniform sampler2DRect image;

in vec3 normal;
in vec3 lightVec;
in vec3 cameraVec;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void) {
    // write colour output without modification
    FragmentColour = vec4(Colour, 0);
	//FragmentColour = texture(image, texCoord);
	
	vec3 normLightVec = normalize(lightVec);
	vec3 reflection = 2*dot(normLightVec, normal) * normal - normLightVec;

	float shading = dot(normal, normLightVec)//*5.f/pow(length(lightVec), 2.f)						//diffuse
	+ pow(max(0.f, dot(reflection, normalize(cameraVec))), 100)//*5.f/pow(length(lightVec), 2.f)	//specular
	+ .1f;																							//ambient
	
	//FragmentColour *= shading;
	//FragmentColour = vec4(1.f, 1.f, 1.f, 0.f);
}

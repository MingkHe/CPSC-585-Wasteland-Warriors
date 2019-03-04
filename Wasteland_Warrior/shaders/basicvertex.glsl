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

out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragVert;

void main()
{
    // assign vertex position without modification
    gl_Position = vec4(VertexPosition, 1.0);

    // assign output colour to be interpolated
	fragTexCoord = VertexUV;
	fragNormal = VertexNormal;
	fragVert = VertexPosition;
	
	
}
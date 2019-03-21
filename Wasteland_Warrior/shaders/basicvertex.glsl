#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 3) in vec2 VertexUV;

out vec2 fragTexCoord;
out vec3 fragVert;

void main()
{
    // assign vertex position without modification
    gl_Position = vec4(VertexPosition, 1.0);

    // assign output colour to be interpolated
	fragTexCoord = VertexUV;
	fragVert = VertexPosition;
	
}

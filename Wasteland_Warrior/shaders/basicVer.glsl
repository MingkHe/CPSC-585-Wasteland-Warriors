#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 Color;

out vec3 fragColor;

uniform mat4 modelViewProjection;

void main()
{
    // assign vertex position without modification
    gl_Position = modelViewProjection*vec4(VertexPosition, 1.0);

    // assign output colour to be interpolated
	fragColor = Color;
	
}

#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 3) in vec2 uv;
out vec2 position;

uniform int numenemies;
uniform vec2 playerpos;

void main()
{
    // assign vertex position without modification
    gl_Position = vec4(VertexPosition, 1.0);
	position = uv;
}

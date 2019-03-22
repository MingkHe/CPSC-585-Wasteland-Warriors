#version 410

#define OFFSET 5.f*3.1415926536/4.f
#define SPEED_SCALE .05f

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColour;

uniform float speed;

uniform vec2 center = vec2(.75f, -.75f);
uniform vec2 scale = vec2(.125f);

out vec3 fragVert;
out vec3 fragColour;

void main()
{
    // assign vertex position without modification
	mat2 trans = mat2(
		cos(OFFSET + speed*SPEED_SCALE), sin(OFFSET + speed*SPEED_SCALE),
		-sin(OFFSET + speed*SPEED_SCALE), cos(OFFSET + speed*SPEED_SCALE));
	gl_Position = vec4((VertexPosition.xy * trans) * scale + center, 0.f, 1.f);

	// assign output colour to be interpolated
	fragVert = VertexPosition;
	fragColour = VertexColour;

}

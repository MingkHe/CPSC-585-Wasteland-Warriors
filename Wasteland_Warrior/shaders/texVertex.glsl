#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 vertex;
layout(location = 3) in vec2 uv;

// output to be interpolated between vertices and passed to the fragment stage

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex, 1.0);
    TexCoords = uv;
}  



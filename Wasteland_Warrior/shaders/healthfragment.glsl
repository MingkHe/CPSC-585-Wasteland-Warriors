#version 410
#define xmargin 0.025f
#define ymargin 0.1f

// interpolated colour received from vertex stage
in vec2 position;

uniform float health;
uniform float maxhealth;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void) {
    // write colour output without modification
	if(position.x > 1-xmargin || position.x < xmargin || position.y > 1-ymargin || position.y < ymargin) {
		FragmentColour = vec4(0, 0, 0, 1);
	} else if(position.x > 1-(health/maxhealth)) {
		FragmentColour = vec4(1, 0, 0, 1);
	} else {
	    FragmentColour = vec4(.1, 0, 0, 1);
	}
}

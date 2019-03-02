#version 410
#define xmargin 0.1f
#define ymargin 0.1f
#define RADAR_SCALE (1.f/30.f)

// interpolated colour received from vertex stage
in vec2 position;

uniform vec2 enemies[50];
uniform int numenemies;
uniform vec2 playerpos;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

float radius_squared(vec2 point1, vec2 point2) {
	vec2 tmp = (point1-point2)*(point1-point2);
	return tmp.x + tmp.y;
}

void main(void) {
    // write colour output without modification
	FragmentColour = vec4(.3, .3, .3, 0);
	for(int i = 0; i < numenemies; i++) {
		vec2 point = enemies[i]-playerpos;
		point.y *= -1;
		if(radius_squared(RADAR_SCALE*point, position) < 0.005) {
			FragmentColour = vec4(.9, 0, 0, 0);
		}
	}
	if(radius_squared(vec2(0), position) < 0.005) {
		FragmentColour = vec4(0, .9, 0, 0);
	}
	if(position.x > 1-xmargin || position.x < xmargin-1 || position.y > 1-ymargin || position.y < ymargin-1) {
		FragmentColour = vec4(0, 0, 0, 0);
	}
}

#version 410
#define xmargin 0.1f
#define ymargin 0.1f
//#define RADAR_SCALE (1.f/30.f)

// interpolated colour received from vertex stage
in vec2 position;

uniform vec2 enemies[50];
uniform vec2 highlights[50];
uniform float enemy_health[50];
uniform float highlight_health[50];
uniform int numenemies;
uniform int numhighlights;
uniform vec2 playerpos;
uniform vec2 playerdir;
uniform float radar_dist;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

float radius_squared(vec2 point1, vec2 point2) {
	vec2 tmp = (point1-point2)*(point1-point2);
	return tmp.x + tmp.y;
}

void main(void) {
    // write colour output without modification
	vec2 norm = normalize(playerdir);
	mat2 rot = mat2(norm.x, norm.y, norm.y, -norm.x);
	FragmentColour = vec4(.3, .3, .3, 1);
	for(int i = 0; i < numenemies; i++) {
		vec2 point = enemies[i]-playerpos;
		point = (point*rot).yx;
		point.y *= -1;
		if(radius_squared(radar_dist*point, position) < 0.005) {
			if(enemy_health[i] > .75) {
				FragmentColour = vec4(0, .9, 0, 1);
			} else if(enemy_health[i] > .5) {
				FragmentColour = vec4(.9, .9, 0, 1);
			} else if(enemy_health[i] > .25) {
				FragmentColour = vec4(.9, .45, 0, 1);
			} else {
				FragmentColour = vec4(.9, 0, 0, 1);
			}
		}
	}
	for(int i = 0; i < numhighlights; i++) {
		vec2 point = highlights[i]-playerpos;
		point = (point*rot).yx;
		point.y *= -1;
		if(radius_squared(radar_dist*point, position) < 0.005) {
			if(highlight_health[i] > .75) {
				FragmentColour = vec4(0, .9, 0, 1);
			} else if(highlight_health[i] > .5) {
				FragmentColour = vec4(.9, .9, 0, 1);
			} else if(highlight_health[i] > .25) {
				FragmentColour = vec4(.9, .45, 0, 1);
			} else {
				FragmentColour = vec4(.9, 0, 0, 1);
			}
		}
		if(radius_squared(radar_dist*point, position) < 0.002) {
			FragmentColour = vec4(.5, 0, .5, 1);
		}
	}
	if(radius_squared(vec2(0), position) < 0.005) {
		FragmentColour = vec4(0, .9, .9, 1);
	}
	if(position.x > 1-xmargin || position.x < xmargin-1 || position.y > 1-ymargin || position.y < ymargin-1) {
		FragmentColour = vec4(0, 0, 0, 1);
	}
}

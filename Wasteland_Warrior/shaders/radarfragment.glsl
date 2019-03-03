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

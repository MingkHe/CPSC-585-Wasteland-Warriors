// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec3 Colour;
in vec3 normal;
in vec3 lightVec;
in vec3 cameraVec;
in vec2 uv;


uniform sampler2D imageTexture;
uniform float distance;

uniform int shade;



// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void) {

	//FragmentColour = (texture(imageTexture, uv)*(dot(normal,-pos)))*0.075*(distance);

    // write colour output without modification
    FragmentColour = vec4(Colour, 0);
	//FragmentColour = texture(image, texCoord);
	
	vec3 normLightVec = normalize(lightVec);
	vec3 reflection = 2*dot(normLightVec, normal) * normal - normLightVec;

	float shading = max(abs(dot(normalize(normal), normLightVec)), 0)//*5.f/pow(length(lightVec), 2.f)			//diffuse
	+ pow(max(0.f, dot(reflection, normalize(cameraVec))), 10)*5.f//pow(length(lightVec), 2.f)	//specular
	+ .05f;																						//ambient
	if(shade == 0) {
		FragmentColour *= shading;
	} else if(shade == 1) {
		if(shading > .95f) {
			FragmentColour *= 1.f;
		} else if(shading > .6f) {
			FragmentColour *= .8f;
		} else if(shading > .4f) {
			FragmentColour *= .8f;
		} else {
			FragmentColour *= .5f;
		}
	}
	
	/*if(normal.x == 0.f && normal.y == 1.f && normal.z == 0.f) {
		FragmentColour = vec4(0, 1, 0, 0);
	}*/
}

/*uniform mat4 model;
uniform vec3 cameraPosition;

// material settings
uniform sampler2D materialTex;
uniform float materialShininess;
uniform vec3 materialSpecularColor;

uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
   float attenuation;
   float ambientCoefficient;
} light;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 finalColor;

void main() {
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
    vec3 surfacePos = vec3(model * vec4(fragVert, 1));
    vec4 surfaceColor = texture(materialTex, fragTexCoord);
    vec3 surfaceToLight = normalize(light.position - surfacePos);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);
    
    //ambient
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;
    
    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;
    
    //attenuation
    float distanceToLight = length(light.position - surfacePos);
    float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

    //linear color (color before gamma correction)
    vec3 linearColor = ambient + attenuation*(diffuse + specular);
    
    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}
*/
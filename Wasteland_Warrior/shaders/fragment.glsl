// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410


//uniform mat4 model;  //=transform
uniform mat4 transform;  //=transform
uniform vec3 cameraPosition; // =cameraPosition

// material settings
uniform sampler2D materialTex;
uniform sampler2D shadowTex;
uniform float materialShininess;
uniform vec3 materialSpecularColor;

uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform float lightAttenuation;
uniform float lightAmbientCoeff;
uniform int isSkybox;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;
in vec4 shadowCoord;

layout(location = 0) out vec4 finalColor;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

void main() {
	//vec3 normal = normalize(transform*vec4(VertexNormal, 0)).xyz;
	//vec3 normal = normalize(mat3(transform) *fragNormal);
	//vec3 normal = normalize(transpose(inverse(mat3(transform))) *fragNormal);
	//vec3 surfacePos = (transform*vec4(VertexPosition, 1)).xyz;
    //vec3 surfacePos = vec3(transform * vec4(fragVert, 1));
	vec3 normal = fragNormal;
	vec3 surfacePos = fragVert;
    vec4 surfaceColor = texture(materialTex, fragTexCoord);
    vec3 surfaceToLight = normalize(lightPosition - surfacePos);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);
	vec3 linearColor = surfaceColor.rgb;
	if (isSkybox != 1){
		//ambient
		vec3 ambient = lightAmbientCoeff * surfaceColor.rgb * lightColour;

		//diffuse
		float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
		vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * lightColour;
    
		//specular
		float specularCoefficient = 0;
		if(diffuseCoefficient > 0.0)
		    specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
		vec3 specular = specularCoefficient * materialSpecularColor * lightColour;
    
		//attenuation
		float distanceToLight = length(lightPosition - surfacePos);
		//float attenuation = 1.0/(1.0 + lightAttenuation * distanceToLight);
		float attenuation = 1.0 / (1.0 + lightAttenuation * pow(distanceToLight, 2));

		//linear color (color before gamma correction)
			linearColor = ambient + attenuation*(diffuse + specular);

		//shadow
		/*float visibility = 0.f;
		float bias = 0.005;
		for (int i=0;i<4;i++){
			visibility += texture(shadowTex, vec2(shadowCoord.xy + poissonDisk[i]/700.0)).a;
		}
		visibility /= 4;
		if(visibility - bias < distanceToLight) {
			linearColor *= .5f;
		}*/
	}
    

    finalColor = vec4(linearColor, surfaceColor.a);
    //final color (after gamma correction)
    //vec3 gamma = vec3(1.0/2.2);
    //finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}

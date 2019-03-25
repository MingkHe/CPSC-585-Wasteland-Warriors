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


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.x < 0 || projCoords.x > 1 || projCoords.y < 0 || projCoords.y > 1) {
		return 0.0;
	}
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowTex, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	vec3 surfaceToLight = normalize(lightPosition - fragVert);
	float bias = max(0.0005 * (1.0 - dot(fragNormal, surfaceToLight)), 0.0005);
    //float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowTex, 0);
	for(float x = -1; x <= 1; x+=.5)
	{
		for(float y = -1; y <= 1; y+=.5)
		{
			float pcfDepth = texture(shadowTex, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 25.0;

    return shadow;
}

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


		//shadow
		float shadow = ShadowCalculation(shadowCoord);

		//linear color (color before gamma correction)
		linearColor = ambient + (1.0-shadow)*attenuation*(diffuse + specular);
	}
    

    finalColor = vec4(linearColor, surfaceColor.a);
    //final color (after gamma correction)
    //vec3 gamma = vec3(1.0/2.2);
    //finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}

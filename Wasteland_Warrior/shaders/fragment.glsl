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
uniform float materialShininess;
uniform vec3 materialSpecularColor;

uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform float lightAttenuation;
uniform float lightAmbientCoeff;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 finalColor;

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
    vec3 linearColor = ambient + attenuation*(diffuse + specular);

    finalColor = vec4(linearColor, surfaceColor.a);
    //final color (after gamma correction)
    //vec3 gamma = vec3(1.0/2.2);
    //finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}

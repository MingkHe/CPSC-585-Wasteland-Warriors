// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

#define SAMPLE_NUM 100
#define SAMPLE_RAD 4.0
#define FAR_SAMPLE_RAD 3.0

//uniform mat4 model;  //=transform
uniform mat4 transform;  //=transform
uniform vec3 cameraPosition; // =cameraPosition

// material settings
uniform sampler2D materialTex;
uniform sampler2D shadowTex;
uniform sampler2D shadowTextwo;
uniform sampler2D shadowTexthree;
uniform float materialShininess;
uniform vec3 materialSpecularColor;
uniform float transparent;
uniform float bias_scale;

uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform float lightAttenuation;
uniform float lightAmbientCoeff;
uniform int isSkybox;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;
in vec4 shadowCoord;
in vec4 shadowCoordtwo;
in vec4 shadowCoordthree;

layout(location = 0) out vec4 finalColor;

uniform vec2 poissonDisk[SAMPLE_NUM] = 
{ 
vec2( 0.460925730073f, 0.701913292718f ), 
vec2( 0.929008298005f, 0.20026975351f ), 
vec2( 0.963264554467f, 0.736957494123f ), 
vec2( 0.412382064601f, 0.218607270356f ), 
vec2( 0.674756574432f, 0.44478071009f ), 
vec2( 0.695648278344f, 0.949220936527f ), 
vec2( 0.205347798856f, 0.991876641932f ), 
vec2( 0.189262808402f, 0.515052855282f ), 
vec2( 0.435090438016f, 0.954806058697f ), 
vec2( 0.959676167373f, 0.46198456764f ), 
vec2( 0.176563169941f, 0.219310418173f ), 
vec2( 0.695131381868f, 0.204656501007f ), 
vec2( 0.254631559797f, 0.776109828632f ), 
vec2( 0.435052628717f, 0.450270974178f ), 
vec2( 0.976348505486f, 0.971166121255f ), 
vec2( 0.776699569637f, 0.66666824719f ), 
vec2( 0.538685328097f, 0.102468627531f ), 
vec2( 0.624475263787f, 0.7864821297f ), 
vec2( 0.0948970236818f, 0.84803754344f ), 
vec2( 0.635742634764f, 0.592780437297f ), 
vec2( 0.829664042729f, 0.0757751862705f ), 
vec2( 0.822354700653f, 0.818279024389f ), 
vec2( 0.0412111865327f, 0.310100743028f ), 
vec2( 0.291638180252f, 0.345833037041f ), 
vec2( 0.0393526791446f, 0.594188219374f ), 
vec2( 0.541070459833f, 0.318066033195f ), 
vec2( 0.803166878508f, 0.313605250558f ), 
vec2( 0.337968427614f, 0.591984817041f ), 
vec2( 0.0269817851388f, 0.105942186506f ), 
vec2( 0.341813875902f, 0.0887860730375f ), 
vec2( 0.908319549083f, 0.593389046893f ), 
vec2( 0.473502277611f, 0.586581723507f ), 
vec2( 0.556084504599f, 0.92497261871f ), 
vec2( 0.222561998591f, 0.652143714015f ), 
vec2( 0.167144535393f, 0.362495312338f ), 
vec2( 0.329996339885f, 0.886779319358f ), 
vec2( 0.820808977933f, 0.457553890798f ), 
vec2( 0.954433855414f, 0.852781175423f ), 
vec2( 0.843639767884f, 0.940082597481f ), 
vec2( 0.136852971683f, 0.737052571156f ), 
vec2( 0.21202655525f, 0.109959989137f ), 
vec2( 0.671307608434f, 0.0731796615905f ), 
vec2( 0.913732504101f, 0.34179017771f ), 
vec2( 0.288780157638f, 0.216113085609f ), 
vec2( 0.328156064067f, 0.452303689495f ), 
vec2( 0.422448228562f, 0.3328871057f ), 
vec2( 0.736350563932f, 0.543690655266f ), 
vec2( 0.497291697896f, 0.820831940158f ), 
vec2( 0.103421689897f, 0.95450391609f ), 
vec2( 0.0759820676203f, 0.504445380274f ), 
vec2( 0.684557625745f, 0.320717886837f ), 
vec2( 0.570647454245f, 0.685582255198f ), 
vec2( 0.583235658089f, 0.200871772727f ), 
vec2( 0.546388281799f, 0.479576759928f ), 
vec2( 0.723806131635f, 0.756242134668f ), 
vec2( 0.327944660092f, 0.695906018579f ), 
vec2( 0.730706580916f, 0.8511816241f ), 
vec2( 0.202548473614f, 0.884822686881f ), 
vec2( 0.121787233783f, 0.0538044584578f ), 
vec2( 0.46056499752f, 0.0453503003769f ), 
vec2( 0.338133585237f, 0.981169477355f ), 
vec2( 0.0586230459921f, 0.40539734938f ), 
vec2( 0.929237932128f, 0.0968417227625f ), 
vec2( 0.749482109733f, 0.123683495556f ), 
vec2( 0.759108178881f, 0.390350655535f ), 
vec2( 0.813328068791f, 0.192930455634f ), 
vec2( 0.0747768157517f, 0.209909792052f ), 
vec2( 0.850611614602f, 0.732743048339f ), 
vec2( 0.462592416729f, 0.131562043441f ), 
vec2( 0.218888002678f, 0.432575164439f ), 
vec2( 0.360023012544f, 0.80464074992f ), 
vec2( 0.14783860945f, 0.596130780316f ), 
vec2( 0.0531858063549f, 0.749045563357f ), 
vec2( 0.132842013515f, 0.139739880977f ), 
vec2( 0.559317563088f, 0.0201641226899f ), 
vec2( 0.0211669381261f, 0.816031169802f ), 
vec2( 0.254934136438f, 0.574325117377f ), 
vec2( 0.675961481235f, 0.681343964991f ), 
vec2( 0.759225085247f, 0.0148408777836f ), 
vec2( 0.0949437431267f, 0.670781588875f ), 
vec2( 0.601078889033f, 0.402047627866f ), 
vec2( 0.975359194286f, 0.647625723782f ), 
vec2( 0.0468489666039f, 0.00578655926909f ), 
vec2( 0.503180235557f, 0.407320519409f ), 
vec2( 0.398882734517f, 0.533238273921f ), 
vec2( 0.02582087367f, 0.906716181533f ), 
vec2( 0.883257145386f, 0.512322031089f ), 
vec2( 0.421882725895f, 0.867074383503f ), 
vec2( 0.632296765158f, 0.869583369003f ), 
vec2( 0.205850467195f, 0.297755764782f ), 
vec2( 0.34751253656f, 0.25974391219f ), 
vec2( 0.624690914025f, 0.968546983361f ), 
vec2( 0.547547646641f, 0.612823968942f ), 
vec2( 0.631763907403f, 0.267809648389f ), 
vec2( 0.866560350758f, 0.266635446699f ), 
vec2( 0.499264746589f, 0.213683034174f ), 
vec2( 0.268132285691f, 0.0556700561769f ), 
vec2( 0.344265618563f, 0.164528953947f ), 
vec2( 0.830954058162f, 0.592161057714f ), 
vec2( 0.614506797369f, 0.135917402293f )
};

float ShadowCalculationtwo(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.x < 0 || projCoords.x > 1 || projCoords.y < 0 || projCoords.y > 1) {
		return 0.f;
	}
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowTextwo, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	vec3 surfaceToLight = normalize(lightPosition - fragVert);
	float bias = max(0.005*bias_scale * (1.0 - dot(fragNormal, surfaceToLight)), 0.005*bias_scale);
    //float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowTextwo, 0);
	for(int i = 0; i < SAMPLE_NUM; i++)
	{
		float pcfDepth = texture(shadowTextwo, projCoords.xy + FAR_SAMPLE_RAD*poissonDisk[i] * texelSize).r;
		shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
	}
	shadow /= float(SAMPLE_NUM);

    return shadow;
}

float ShadowCalculationthree(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.x < 0 || projCoords.x > 1 || projCoords.y < 0 || projCoords.y > 1) {
		return ShadowCalculationtwo(shadowCoordtwo);
	}
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowTexthree, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	vec3 surfaceToLight = normalize(lightPosition - fragVert);
	float bias = max(0.002*bias_scale * (1.0 - dot(fragNormal, surfaceToLight)), 0.002*bias_scale);
    //float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowTexthree, 0);
	for(int i = 0; i < SAMPLE_NUM; i++)
	{
		float pcfDepth = texture(shadowTexthree, projCoords.xy + SAMPLE_RAD*poissonDisk[i] * texelSize).r;
		shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
	}
	shadow /= float(SAMPLE_NUM);

    return shadow;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.x < 0 || projCoords.x > 1 || projCoords.y < 0 || projCoords.y > 1) {
		return ShadowCalculationthree(shadowCoordthree);
	}
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowTex, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	vec3 surfaceToLight = normalize(lightPosition - fragVert);
	float bias = max(0.0005*bias_scale * (1.0 - dot(fragNormal, surfaceToLight)), 0.0005*bias_scale);
    //float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowTex, 0);
	for(int i = 0; i < SAMPLE_NUM; i++)
	{
		float pcfDepth = texture(shadowTex, projCoords.xy + SAMPLE_RAD*poissonDisk[i] * texelSize).r;
		shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
	}
	shadow /= float(SAMPLE_NUM);

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

		float shadow = 0.0;
		//shadow
		shadow = ShadowCalculation(shadowCoord);
		

		//linear color (color before gamma correction)
		linearColor = ambient + (1.0-shadow)*attenuation*(diffuse + specular);
	}
    

    finalColor = vec4(linearColor, transparent);
    //final color (after gamma correction)
    //vec3 gamma = vec3(1.0/2.2);
    //finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}

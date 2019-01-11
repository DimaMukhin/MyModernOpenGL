#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct DirectionalLight 
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition; // in world coord

void main()
{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	// this depends on direction and surface normal (look at theory)
	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

	vec4 specularColour = vec4(0, 0, 0, 0);

	// if there is no diffuse lighting, there is definately no specular light, se we can save a calculation
	if (diffuseFactor > 0.0f)
	{
		// see theory video for more details about the below logic
		vec3 fragToEye = normalize(eyePosition - FragPos); // vertex between the two
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);

		// if we have specular lighting at all
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	color = texture(theTexture, TexCoord) * (ambientColour + diffuseColour + specularColour);
}
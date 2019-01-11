#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);

	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	TexCoord = tex;
	Normal = mat3(transpose(inverse(model))) * norm;	// we need to multiply by model transformation because the object moves. 
														// However, we only care about rotation and scale, translation is on the 4th column so we can convert "model" into mat3 and remove translation
														// we need transpose and inverse because we want to eliminate non-uniform scaling
}
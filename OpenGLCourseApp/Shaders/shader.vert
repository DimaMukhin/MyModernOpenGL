#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos; // the position of the "fragment" in world coords

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
	FragPos = (model * vec4(pos, 1.0f)).xyz;	// we do this because we need the world coords of the gragment
												// this is because FragPos is used for specular light. we use the camera position in world coords and so we also need the frag position in world coord
}
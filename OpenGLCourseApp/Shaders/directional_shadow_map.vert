#version 330

layout (location = 0) in vec3 pos;

uniform mat4 model; // we will work in world space only
uniform mat4 directionalLightSpaceTransform; // this is how we convert pos to light origin and direct it with projection

void main()
{
	gl_Position = directionalLightSpaceTransform * model * vec4(pos, 1.0);
}
#version 330 core

out vec4 FragColour;

in vec2 texCoord;

// texture sampler
uniform sampler2D ourTexture;

void main()
{
	FragColour = texture(ourTexture, texCoord);
}
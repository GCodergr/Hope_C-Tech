#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

// Texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{    
    // Linearly interpolate between both textures (80% bricks, 20% duck)
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
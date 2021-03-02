#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
    // Output texture color
    //FragColor = texture(ourTexture, TexCoord);
    // Mix vertex's color and the texture's color 
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0); 

    // Linearly interpolate between both textures (80% bricks, 20% duck)
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
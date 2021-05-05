#include <cassert>
#include <iostream>

#include "Shader.h"

#include "SDL.h"
#include "utilities/stb_image.h"

#include "utilities/glm/glm.hpp"
#include "utilities/glm/gtc/matrix_transform.hpp"
#include "utilities/glm/gtc/type_ptr.hpp"

typedef uint32_t u32;
typedef int32_t b32;

const int WINDOWS_WIDTH = 800;
const int WINDOWS_HEIGHT = 600;

int main(int argv, char** args) {

	u32 WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	SDL_Window* window = SDL_CreateWindow("OpenGL Test",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOWS_WIDTH,
		WINDOWS_HEIGHT,
		WindowFlags);

	if (window == nullptr)
	{
		std::cout << "Error::SDL2::Error Failed to create SDL window" << std::endl;
		return -1;
	}
	assert(window);

	// Use OpenGL 3.3 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		std::cout << "Error::GLEW::Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Build and compile shader program
	Shader ourShader(".\\assets\\shaders\\3.3.shader.vert", ".\\assets\\shaders\\3.3.shader.frag");

	// Set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
			 // Positions			// colors			// texture coordinates  
		     0.5f,   0.5f,  0.0f,   1.0f, 0.0f, 0.0f,	1.0f,  1.0f,      // top right 
			 0.5f,  -0.5f,  0.0f,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f,      // bottom right 
			-0.5f,  -0.5f,  0.0f,	0.0f, 0.0f, 1.0f,   0.0f,  0.0f,      // bottom left 
			-0.5f,   0.5f,  0.0f,	1.0f, 1.0f, 0.0f,	0.0f,  1.0f       // top left
	};
	unsigned int indices[] = {
		0, 1, 3,	// first triangle
		1, 2, 3	// second triangle
	};
	
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// 1. Bind the Vertex Array Object
	glBindVertexArray(VAO);
	// 2. Bind and set vertex buffer(s)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// 3. Configure vertex attributes
	// position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Load and create a textures 
	//
	// Texture 1
	//
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set the texture filtering parameters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps 
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// Load image, create texture and generate mipmaps
	unsigned char* data = stbi_load(".\\assets\\textures\\Bricks042_1K_Color.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error::Failed to load Brick texture" << std::endl;
	}
	stbi_image_free(data); 

	//
    // Texture 2
    //
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set the texture filtering parameters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	data = stbi_load(".\\assets\\textures\\duck.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error::Failed to load Duck texture" << std::endl;
	}	
	stbi_image_free(data);

	// Tell OpenGL for each sampler to which texture unit it belongs to (only has to be done once)
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so...
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// ...or set it via the texture class
	ourShader.setInt("texture2", 1);
	
	b32 Running = 1;
	b32 FullScreen = 0;
	
	while (Running)
	{
		SDL_Event Event;
		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_KEYDOWN)
			{
				switch (Event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					Running = 0;
					break;
				case 'f':
					FullScreen = !FullScreen;
					if (FullScreen)
					{
						SDL_SetWindowFullscreen(window, WindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
					}
					else
					{
						SDL_SetWindowFullscreen(window, WindowFlags);
					}
					break;
				default:
					break;
				}
			}
			else if (Event.type == SDL_QUIT)
			{
				Running = 0;
			}
		}

		glViewport(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);
		glClearColor(100.f / 255.f, 149.f / 255.f, 237.f / 255.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Note: Uncomment for wire-frame rendering
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	    // bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		// Render container
		ourShader.use();		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
				
		SDL_GL_SwapWindow(window);
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	
	return 0;
}
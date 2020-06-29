#include <cassert>
#include <iostream>

#include "Shader.h"

#include "SDL.h"

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
	Shader ourShader(".\\src\\shaders\\3.3.shader.vert", ".\\src\\shaders\\3.3.shader.frag");
	
	float vertices[] = {
			 // Positions			// colors			 
			 0.5f,  -0.5f,  0.0f,	1.0f, 0.0f, 0.0f,	// bottom right
			-0.5f,  -0.5f,  0.0f,	0.0f, 1.0f, 0.0f,   // bottom left
			 0.0f,   0.5f,  0.0f,	0.0f, 0.0f, 1.0f	// top
	};
	
	GLuint VBO, VAO; // 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);	
	// 1. Bind the Vertex Array Object
	glBindVertexArray(VAO);
	// 2. Bind and set vertex buffer(s)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// 3. Configure vertex attributes
	// position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
		
	
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
		
		// render the triangle
		ourShader.use();		
		glBindVertexArray(VAO);		
		glDrawArrays(GL_TRIANGLES, 0, 3);
			
		SDL_GL_SwapWindow(window);
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	return 0;
}
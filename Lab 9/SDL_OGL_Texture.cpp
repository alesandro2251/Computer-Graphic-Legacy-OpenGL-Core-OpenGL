// SDL_OGL1.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

//#define GLEW_STATIC
#include <GL\glew.h>

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <gl\GLU.h>
#define STB_IMAGE_IMPLEMENTATION //if not defined the function implementations are not included
#include "stb_image.h"
#include "Shader.h"

#include <stdio.h>

bool init();
bool initGL();
void render();
bool LoadTexture(const char* filename, GLuint& texID);
GLuint CreateCube(float, GLuint&, GLuint&);
void DrawCube(GLuint id);
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

Shader shader;

GLuint gVAO, gVBO, gEBO;
GLuint texID1;

float previousTick;
float deltaSum;
float alpha;

void HandleKeyUp(const SDL_KeyboardEvent& key);


int main(int argc, char* args[])
{
	init();

	SDL_Event e;
	//While application is running
	bool quit = false;
	previousTick = SDL_GetTicks() / 1000.0f;
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYUP:
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}
				else {
					HandleKeyUp(e.key);
				}
				break;
			}
		}

		//Render quad
		render();

		float currentTick = SDL_GetTicks() / 1000.0f;

		deltaSum += currentTick - previousTick;
		alpha = sinf(deltaSum) * sinf(deltaSum);

		printf("%f\n", alpha);

		previousTick = currentTick;
		//Update screen
		SDL_GL_SwapWindow(gWindow);
	}

	close();

	return 0;
}

void HandleKeyUp(const SDL_KeyboardEvent& key)
{

}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Use OpenGL 3.3
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);


		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN /*| SDL_WINDOW_FULLSCREEN*/);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				//Initialize OpenGL
				if (!initGL())
				{
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}

	return success;
}

bool initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	glewExperimental = true;
	glewInit();

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		success = false;
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!LoadTexture("smiley.jpg", texID1))
	{
		printf("Could not load texture \"smiley.jpg\"");
	}

	shader.Load(".\\shaders\\vertex.vs", ".\\shaders\\fragment.fs");
	shader.use();
	shader.setInt("Texture1", 0); //tell the shader that the sampler Texture1 should take its data from texture unit 0 (GL_TEXTURE0)
	gVAO = CreateCube(1.0f, gVBO, gEBO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //other modes GL_FILL, GL_POINT

	return success;
}

bool LoadTexture(const char* filename, GLuint& texID)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); //these are the default values for warping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// read the texture
	GLint width, height, channels;
	stbi_set_flip_vertically_on_load(true); //flip the image vertically while loading
	unsigned char* img_data = stbi_load(filename, &width, &height, &channels, 0); //read the image data

	if (img_data)
	{   //3 channels - rgb, 4 channels - RGBA
		GLenum format;
		switch (channels)
		{
		case 4:
			format = GL_RGBA;
			break;
		default:
			format = GL_RGB;
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(img_data);

	return true;
}

void close()
{
	//delete GL programs, buffers and objects
	glDeleteProgram(shader.ID);
	glDeleteVertexArrays(1, &gVAO);
	glDeleteBuffers(1, &gVBO);

	//Delete OGL context
	SDL_GL_DeleteContext(gContext);
	//Destroy window	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void render()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//not actually necessary because GL_TEXTURE0 is active by default
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID1);
	//if multiple textures have to be bound at the same time they are placed in different texture units (slots) -
	//available are GL_TEXTURE0 - GL_TEXTURE15, to be used they have to be activated
	shader.use();
	shader.setFloat("Alpha", alpha);
	DrawCube(gVAO);
}

GLuint CreateCube(float width, GLuint& VBO, GLuint& EBO)
{
	GLfloat vertices[] = {
		//vertex position //vertex color
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left 
	};
	//indexed drawing - we will be using the indices to point to a vertex in the vertices array
	GLuint indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


	GLuint VAO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//we have to change the stride to 6 floats, as each vertex now has 6 attribute values
	//the last value (pointer) is still 0, as the position values start from the beginning
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //the data comes from the currently bound GL_ARRAY_BUFFER
	glEnableVertexAttribArray(0);

	//here the pointer is the size of 3 floats, as the color values start after the 3rd value from the beginning
	//in other words we have to skip the first 3 floats of the vertex attributes to get to the color values
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	//the elements buffer must be unbound after the vertex array otherwise the vertex array will not have an associated elements buffer array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}

void DrawCube(GLuint vaoID)
{
	glUseProgram(shader.ID);
	glBindVertexArray(vaoID);

	//glDrawElements uses the indices in the EBO to get to the vertices in the VBO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
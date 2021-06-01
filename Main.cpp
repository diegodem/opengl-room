#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_GLContext maincontext;

int init()
{
	// Initialize SDL 
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;
	SDL_GL_LoadLibrary(NULL);
	// Request an OpenGL 3.3 context (should be core)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	window = SDL_CreateWindow("OpenGL Room", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return -1;
	}
	maincontext = SDL_GL_CreateContext(window);
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	glViewport(0, 0, 800, 600);
	return 0;
}

int main(int argc, char *args[])
{
	if (init() == -1)
	{
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);

	unsigned int textures[3];
	glGenTextures(3, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	int width, height, nrChannels;
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data = stbi_load("baldosa.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("azulejo.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("concrete.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);

	Shader ourShader("shader.vert", "shader.frag");

	float vertices[] = {
		// floor
		0.f, 0.f, 0.f,		0.f, 0.f,		1.f, 0.f, 0.f,
		10.f, 0.f, 0.f,		10.f, 0.f,		1.f, 0.f, 0.f,
		0.f, 0.f, -6.f,		0.f, 6.f,		1.f, 0.f, 0.f,
		10.f, 0.f, 0.f,		10.f, 0.f,		1.f, 0.f, 0.f,
		0.f, 0.f, -6.f,		0.f, 6.f,		1.f, 0.f, 0.f,
		10.f, 0.f, -6.f,	10.f, 6.f,		1.f, 0.f, 0.f,
		// right wall
		0.f, 0.f, 0.f,		0.f, 0.f,		0.f, 1.f, 0.f,
		10.f, 0.f, 0.f,		20.f, 0.f,		0.f, 1.f, 0.f,
		0.f, 3.f, 0.f,		0.f, 6.f,		0.f, 1.f, 0.f,
		10.f, 0.f, 0.f,		20.f, 0.f,		0.f, 1.f, 0.f,
		0.f, 3.f, 0.f,		0.f, 6.f,		0.f, 1.f, 0.f,
		10.f, 3.f, 0.f,		20.f, 6.f,		0.f, 1.f, 0.f,
		// left wall
		0.f, 0.f, -6.f,		0.f, 0.f,		0.f, 1.f, 0.f,
		10.f, 0.f, -6.f,	20.f, 0.f,		0.f, 1.f, 0.f,
		0.f, 3.f, -6.f,		0.f, 6.f,		0.f, 1.f, 0.f,
		10.f, 0.f, -6.f,	20.f, 0.f,		0.f, 1.f, 0.f,
		0.f, 3.f, -6.f,		0.f, 6.f,		0.f, 1.f, 0.f,
		10.f, 3.f, -6.f,	20.f, 6.f,		0.f, 1.f, 0.f,
		// front wall
		10.f, 0.f, 0.f,		0.f, 0.f,		0.f, 1.f, 0.f,
		10.f, 0.f, -6.f,	12.f, 0.f,		0.f, 1.f, 0.f,
		10.f, 3.f, 0.f,		0.f, 6.f,		0.f, 1.f, 0.f,
		10.f, 0.f, -6.f,	12.f, 0.f,		0.f, 1.f, 0.f,
		10.f, 3.f, 0.f,		0.f, 6.f,		0.f, 1.f, 0.f,
		10.f, 3.f, -6.f,	12.f, 6.f,		0.f, 1.f, 0.f,
		// back wall
		0.f, 0.f, 0.f,		0.f, 0.f,		0.f, 1.f, 0.f,
		0.f, 0.f, -6.f,		12.f, 0.f,		0.f, 1.f, 0.f,
		0.f, 3.f, 0.f,		0.f, 6.f,		0.f, 1.f, 0.f,
		0.f, 0.f, -6.f,		12.f, 0.f,		0.f, 1.f, 0.f,
		0.f, 3.f, 0.f,		0.f, 6.f,		0.f, 1.f, 0.f,
		0.f, 3.f, -6.f,		12.f, 6.f,		0.f, 1.f, 0.f,
		// ceiling
		0.f, 3.f, 0.f,		0.f, 0.f,		0.f, 0.f, 1.f,
		10.f, 3.f, 0.f,		10.f, 0.f,		0.f, 0.f, 1.f,
		0.f, 3.f, -6.f,		0.f, 6.f,		0.f, 0.f, 1.f,
		10.f, 3.f, 0.f,		10.f, 0.f,		0.f, 0.f, 1.f,
		0.f, 3.f, -6.f,		0.f, 6.f,		0.f, 0.f, 1.f,
		10.f, 3.f, -6.f,	10.f, 6.f,		0.f, 0.f, 1.f,
	};


	glm::mat4 model = glm::mat4(1.0f);

	float cameraSpeed;

	glm::vec3 cameraPos = glm::vec3(1.0f, 1.6f, -3.0f);
	glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 direction;

	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glm::mat4 projection;
	float fov = 60.f;

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	ourShader.setInt("texture3", 2);


	int modelLoc = glGetUniformLocation(ourShader.ID, "model");
	int viewLoc = glGetUniformLocation(ourShader.ID, "view");
	int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");

	SDL_Event event;
	bool quit = false;

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = (float)SDL_GetTicks() / 1000.f; // Time of last frame
	SDL_SetRelativeMouseMode(SDL_TRUE);
	int x, y;
	float pitch = 0.f, yaw = 0.f;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			/*SDL_GetMouseState(&x, &y);
			printf("%d %d\n", x, y);*/
			if (event.type == SDL_MOUSEMOTION)
			{
				x = event.motion.xrel;
				y = event.motion.yrel;
				printf("%d %d\n", x, y);
				yaw += x * 0.1f;
				pitch -= y * 0.1f;
				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;
			}
			if (event.type == SDL_MOUSEWHEEL)
			{
				fov -= (float)event.wheel.y;
				if (fov < 1.0f)
					fov = 1.0f;
				if (fov > 75.0f)
					fov = 75.0f;
			}


		}
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
		const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
		float currentFrame = (float)SDL_GetTicks() / 1000.f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		cameraSpeed = 2.5f * deltaTime;
		if (keyboardState[SDL_SCANCODE_W])
		{
			cameraPos += cameraSpeed * glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			cameraPos -= cameraSpeed * glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
		}
		if (keyboardState[SDL_SCANCODE_A])
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ourShader.use();

		projection = glm::perspective(glm::radians(fov), 16.0f / 12.0f, 0.1f, 100.0f);

		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glBindVertexArray(VAO);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		SDL_GL_SwapWindow(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	ourShader.close();
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return 0;

}
#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processKeyboardInput(GLFWwindow* window)
{
	static bool wireframe = false;

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		std::cout << "Esc key pressed, closing window" << std::endl;
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if(wireframe == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = false;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = true;
		}
	}		
}


int main(int argc, char **argv)
{
	if (!glfwInit())
	{
		std::cout << "GLFW failed to initialise" << std::endl;
	}
	else
	{
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		GLFWwindow *window = glfwCreateWindow(800, 600, "My Window", NULL, NULL);
		
		if(window != NULL)
		{
			std::cout << "Window created!" << std::endl;

			glfwMakeContextCurrent(window);
			glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

			if(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{							

				Shader ourShader = Shader("../shaders/vertex.vs", "../shaders/fragment.fs");

				float vertices[] = {
				    // positions        	// texture coords 
					-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,  	// top left 
				     0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 	// top right
				    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 	// bottom left
				     0.5f, -0.5f, 0.0f,		1.0f, 0.0f		// bottom right
				}; 

				unsigned int indices[] = {
					0, 1, 2,	// first triangle 
					1, 2, 3		// seconf triangle
				};				
				unsigned int VBO, VAO, EBO;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);

				glBindVertexArray(VAO); 

				glBindBuffer(GL_ARRAY_BUFFER, VBO);  
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

				// position attribute
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				// texture attribute
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
				glEnableVertexAttribArray(1);

				// Generate texture
				// ----------------
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);				
				// set the texture wrapping/filtering options (on the currently bound texture)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// load and generate the texture
				int width, height, nrChannels;
				stbi_set_flip_vertically_on_load(true);
				unsigned char *data = stbi_load("../textures/wall.jpg", &width, &height, &nrChannels, 0);
				if(data)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else
				{
					std::cout << "Failed to load texture" << std::endl;
				}
				stbi_image_free(data);

				// GLM: maths tests
				// ----------------
				glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
				glm::mat4 trans = glm::mat4(1.0f);

				trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

				vec = trans * vec;

				std::cout << vec.x << vec.y << vec.z << std::endl;

				// render loop
				// -----------
				while(!glfwWindowShouldClose(window))
				{					
					// input
					// -----
					processKeyboardInput(window);	

					// render
					// ------
					glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT);

					// bind texture
					glBindTexture(GL_TEXTURE_2D, texture);

					// render container
					ourShader.use();
					glBindVertexArray(VAO);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

					// glfw: swap buffers and poll IO events
					glfwSwapBuffers(window);
					glfwPollEvents();
				}

				// de-allocate all resources once they've outlived their purpose
				// -------------------------------------------------------------
				glDeleteVertexArrays(1, &VAO);
				glDeleteBuffers(1, &VBO);
				glDeleteBuffers(1, &EBO);
			}
			else
			{
				std::cout << "Failed to initialise GLAD" << std::endl;
			}		
		}
		else
		{
			std::cout << "Window failed to create" << std::endl;
		}

		// glfw: terminate, clearing all previously allocated GLFW resources
		// -----------------------------------------------------------------
		glfwTerminate();
		return 0;
	}
}

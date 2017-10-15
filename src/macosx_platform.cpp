#include <stdio.h>
#include <GLFW/glfw3.h>

void KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
	if(Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
	{
		printf("Esc key pressed, closing window\n");
		glfwSetWindowShouldClose(Window, GLFW_TRUE);
	}		
}

int main(int argc, char **argv)
{
	if (!glfwInit())
	{
		printf("GLFW failed to initialise\n");
	}
	else
	{
		
		GLFWwindow *Window = glfwCreateWindow(1280, 720, "My Window", NULL, NULL);
		
		if(Window != NULL)
		{
			printf("Window created!\n");

			glfwSetKeyCallback(Window, KeyCallback);

			//main loop
			while(!glfwWindowShouldClose(Window))
			{
				//render	
				

				glfwSwapBuffers(Window);
				glfwPollEvents();
			}
		}
		else
		{
			printf("Window failed to create\n");
		}

		glfwTerminate();
	}
}

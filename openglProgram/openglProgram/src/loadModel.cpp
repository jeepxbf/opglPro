/*

*/
#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define FREEIMAGE_LIB
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include "Shader.h"
#include <stb\stb_image.h>
#include "TextureManager.h"

//glm
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "common.h"
#include "Camera.h"
#include "Model.h"

glm::vec3 cameraPos(0.0, 0.f, 3.f);
glm::vec3 cameraFront(0.f, 0.f, -1.f);
glm::vec3 cameraUp(0.f, 1.f, 0.f);

bool keys[1024];

GLfloat deltaTime = 0.f;
GLfloat lastFrame = 0.0f;
Camera mainCamera(cameraPos, cameraFront);

glm::mat4 glmTest()
{
	glm::mat4 trans(1);
	trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.f));
	trans = glm::rotate(trans, glm::radians((GLfloat)glfwGetTime()*50.0f), glm::vec3(0, 0, 1));

	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 1));
	return trans;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	if (action == GLFW_RELEASE)
		keys[key] = false;
	if (key == GLFW_KEY_ESCAPE&&action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
void do_movement()
{
	static GLfloat speed = 0.05f;
	glm::vec3 right = glm::cross(cameraFront, cameraUp);
	if (keys[GLFW_KEY_W])
		mainCamera.processKeyboardInput(Camera_Movement::UP, deltaTime);
	if (keys[GLFW_KEY_S])
		mainCamera.processKeyboardInput(Camera_Movement::DOWN, deltaTime);
	if (keys[GLFW_KEY_A])
		mainCamera.processKeyboardInput(Camera_Movement::LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		mainCamera.processKeyboardInput(Camera_Movement::RIGHT, deltaTime);
}
void mouse_callback(GLFWwindow *window, int key, int action, int mode)
{
	switch (key)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		std::cout << "left button" << std::endl;
		break;
	default:
		break;
	}
}

void wheel_callback(GLFWwindow *window, double offx, double offy)
{
	GLfloat speed = 0.3f;
	std::cout << offy << std::endl;
	mainCamera.processWheel(offy);
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
	static float lastX = xpos, lastY = ypos;
	static bool first = true;
	static GLfloat yaw = 0.0f;
	static GLfloat pitch = 0.f;
	if (first)
	{
		first = false;
		return;
	}

	float offx = lastX - xpos;
	float offy = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	mainCamera.processCursor(offx, offy);
}

int main(int argc, char **argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "helloWorld", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "windows faild" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew faild" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	//glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, wheel_callback);
	//glfwSetCursorPosCallback(window, cursor_callback);

	GLchar* modelPath = "imageSource/nanosuit/nanosuit.obj";
	Model* mModel = new Model(modelPath);

	const char* vertexPath = "Shader/loadModel.vert";
	const char* fragPath = "Shader/loadModel.frag";
	Shader* modelShader = new Shader(vertexPath, fragPath);
	if (!modelShader)
	{
		std::cout << "ModelShader ERROR" << std::endl;
	}
	

	glEnable(GL_DEPTH_TEST);
	const glm::vec3 lightSourcePos(2.f, 2.f, 0.0f);
	//定向光方向
	const glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f);

	while (!glfwWindowShouldClose(window))
	{
		//lalalal
		GLfloat curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;
		static int viewWidth, viewheight;
		glfwGetWindowSize(window, &viewWidth, &viewheight);
		glViewport(0, 0, viewWidth, viewheight);
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		do_movement();

		modelShader->use();

		//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 view(1);
		view = mainCamera.getCamearMatrix();
		glm::mat4 projection(1);
		if (viewheight != 0)
			projection = glm::perspective(glm::radians(45.f), (GLfloat)viewWidth / viewheight, 0.1f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(modelShader->program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(modelShader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));	

		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(modelShader->program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		mModel->draw(modelShader);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

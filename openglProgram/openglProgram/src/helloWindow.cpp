/*
hellowWindow 是opengl的入门小例子 里面包含了绘制一个纹理立方体和摄像机 包含一个处理着色器的类,摄像机类，纹理类
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

#include "Camera.h"
enum enumTexture
{
	box,
	face,
};
float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {

	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)

};
//GLfloat vertices[] = {
//	-0.5f,-0.5f,0.f,1.0f,0.f,0.f,
//	0.5f,-0.5f,0.f, 0.f,1.0f,0.f,
//	0.f,0.5f,0.f,0.f,0.f,1.f,
//};
//GLfloat vertices[] = {
//	-0.5f, 0.5f,0.f, 1.0f,0.f,0.f, 0.0f,1.0f,			//left top
//	-0.5f,-0.5f,0.f,  0.f,1.f,0.f,  0.0f,0.f,			//left bottom
//	 0.5f, 0.5f,0.f,  0.f,1.f,0.f,  1.f,1.f,			//right top
//	 0.5f,-0.5f,0.f,  0.f,0.f,1.f,  1.f,0.f,			//right bottom
//};
GLuint indices[] = {
	0,1,3,
	0,3,2,
};

glm::vec3 cameraPos(0.0, 0.f, 3.f);
glm::vec3 cameraFront(0.f, 0.f, -1.f);
glm::vec3 cameraUp(0.f, 1.f, 0.f);

bool keys[1024];
void loadImage(const char* path,GLint format);

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
//顶点坐标gl_Position*model 转变为世界坐标
// Vclip = Mprojection ⋅ Mview ⋅ Mmodel ⋅ Vlocal
glm::mat4 image3d(GLfloat width,GLfloat height,GLint i)
{
	//模型矩阵
	glm::mat4 model(1);
	//model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.0, 1.f, 0.f));
	model = glm::translate(model, cubePositions[i]);
	model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime()*50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	//摄像机矩阵
	glm::mat4 view(1);
	/*view = glm::translate(view, glm::vec3(0.f, 0.f, -3.5));*/
	//创建一个lookat摄像机 绕着中心点旋转
	/*GLfloat radius = 20.f;
	GLfloat camX = radius * sin((GLfloat)glfwGetTime());
	GLfloat camZ = radius * cos((GLfloat)glfwGetTime());*/
	//view = glm::lookAt(glm::vec3(camX, 0, camZ), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	view = mainCamera.getCamearMatrix();

	//视图矩阵
	glm::mat4 projection(1);
	if(height != 0) //判断除数 要不然会引发abort()
		//第一个参数 摄像机视角宽度
		projection = glm::perspective(glm::radians(45.f), width / height, 0.1f, 100.f);
	//projection = glm::ortho(0.f, width, 0.f, height, 0.1f, 100.f);
	glm::mat4 _3d = projection * view * model;
	return _3d;
}

void key_callback(GLFWwindow *window,int key,int scancode,int action,int mode)
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
		//cameraPos += glm::cross(right, cameraFront) * speed;
	if(keys[GLFW_KEY_S])
		mainCamera.processKeyboardInput(Camera_Movement::DOWN, deltaTime);
		//cameraPos -= glm::cross(right, cameraFront) * speed;
	if(keys[GLFW_KEY_A])
		mainCamera.processKeyboardInput(Camera_Movement::LEFT, deltaTime);
		//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp))*speed;
	if(keys[GLFW_KEY_D])
		mainCamera.processKeyboardInput(Camera_Movement::RIGHT, deltaTime);
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))*speed;
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
	//cameraPos += cameraFront * (GLfloat)offy * speed;
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
	//GLfloat sensitivity = 0.05;
	//offx *= sensitivity;
	//offy *= sensitivity;

	//pitch += offy;
	//yaw += offx;

	//if (pitch >= 89)
	//	pitch = 89;
	//if (pitch <= -89)
	//	pitch = -89;
	//glm::vec3 front;

	//front.x = -glm::sin(glm::radians(yaw));
	//front.y = glm::sin(glm::radians(pitch));
	//front.z = -glm::cos(glm::radians(pitch))*glm::cos(glm::radians(yaw));

	//*front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	//front.y = sin(glm::radians(pitch));
	//front.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw));*/
	//cameraFront = glm::normalize(front);
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
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, wheel_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	/*  使用shader类创建shader 编译链接	*/
	const char* vPath = "Shader/vertexShader.vs";
	const char* fPath = "Shader/flagmentShader.fs";
	Shader* myShder = new Shader(vPath, fPath);

	//载入纹理 把纹理数据存下来
	const char* imagePath = "imageSource/box.png";
	const char* imagePath2 = "imageSource/222.jpg";
	//loadImage(imagePath,GL_RGBA);
	//loadImage(imagePath2, GL_RGB);
	TextureManager::Inst()->LoadTexture(imagePath, enumTexture::box, GL_BGRA, GL_RGBA, 0, 0); //box
	TextureManager::Inst()->LoadTexture(imagePath2, enumTexture::face, GL_BGR, GL_RGB, 0, 0); //笑脸
	//在设置uniform前要使用glUseProgram(program)
	myShder->use();
	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->BindTexture(enumTexture::box);
	glUniform1i(glGetUniformLocation(myShder->program, "ourTexture0"), 0);

	glActiveTexture(GL_TEXTURE1);
	TextureManager::Inst()->BindTexture(enumTexture::face);
	GLint index = glGetUniformLocation(myShder->program, "ourTexture1");
	glUniform1i(index, 1);

	GLint temp;
	glGetUniformiv(myShder->program, glGetUniformLocation(myShder->program, "ourTexture1"), &temp);
	
	
	/*if (!state)
		std::cout << "load image faild" << std::endl;*/
	//绑定顶点buffer数组对象
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); //返回1个未使用的对象名到数组vbo中
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),(GLvoid*)0);
	glEnableVertexAttribArray(0);   //location = 0 enable
	//颜色
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	//纹理坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);   //解绑
	
	glBindVertexArray(VAO);
	glEnable(GL_DEPTH_TEST);
	std::cout << glfwGetTime() << std::endl;
	while (!glfwWindowShouldClose(window))
	{
		//std::cout << glfwGetTime() << std::endl;
		GLfloat curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		static int viewWidth, viewheight;
		glfwGetWindowSize(window, &viewWidth, &viewheight);
		glViewport(0, 0, viewWidth, viewheight);
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		do_movement();
		//glm::mat4 trans = glmTest();
		//glUniformMatrix4fv(glGetUniformLocation(myShder->program, "trans"), 1, GL_FALSE, glm::value_ptr(trans));
		//myShder->use();
		for (GLint i = 0; i < 10; i++)
		{
			glm::mat4 _3d = image3d(viewWidth, viewheight, i);
			glUniformMatrix4fv(glGetUniformLocation(myShder->program, "trans"), 1, GL_FALSE, glm::value_ptr(_3d));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
			glBindVertexArray(0);
		}		
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void loadImage(const char* path,GLint format)//,int* width,int* height,int* channels)
{	
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//设置包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//加载和生成纹理
	int width, height, channels;
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "load image ERROR" << std::endl;
	}
	
	stbi_image_free(data);
}
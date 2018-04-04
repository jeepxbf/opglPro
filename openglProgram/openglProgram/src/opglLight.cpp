/*
log：2018-3-10 增加camera class

http://devernay.free.fr/cours/opengl/materials.html  定义了很多材质参数

点光源的衰减参数
F = (1.0)/c + k1*d + k2*d*d   c是常数项，d是距离，k1是一次项，k2是二次项
我们希望覆盖大约50米的距离，那么我们可以选择距离为50的参数
距离	常数项	一次项	二次项
7		1.0		0.7		1.8
13		1.0		0.35	0.44
20		1.0		0.22	0.20
32		1.0		0.14	0.07
50		1.0		0.09	0.032
65		1.0		0.07	0.017
100		1.0		0.045	0.0075
160		1.0		0.027	0.0028
200		1.0		0.022	0.0019
325		1.0		0.014	0.0007
600		1.0		0.007	0.0002
3250	1.0		0.0014	0.000007

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
	specularBox,
};
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {

	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
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
glm::mat4 image3d(GLfloat width, GLfloat height, GLint i,GLboolean isscale)
{
	//模型矩阵
	glm::mat4 model(1);
	model = glm::translate(model, cubePositions[i]);
	model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime()*50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	if(isscale)
		model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2f));	

	//摄像机矩阵
	glm::mat4 view(1);
	view = mainCamera.getCamearMatrix();

	//视图矩阵
	glm::mat4 projection(1);
	if (height != 0) //判断除数 要不然会引发abort()  第一个参数 摄像机视角宽度
		projection = glm::perspective(glm::radians(45.f), width / height, 0.1f, 100.f);
	glm::mat4 _3d = projection * view * model;
	return _3d;
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


	const char* lightVertex = "Shader/lightsource.vert";
	const char* lightfrag = "Shader/lightsource.frag";
	Shader* lightShader = new Shader(lightVertex, lightfrag);
	if (!lightShader)
	{
		std::cout << "light shader ERROR" << std::endl;
	}
	lightShader->use();
	//绑定顶点buffer数组对象
	GLuint VBO;
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); //返回1个未使用的对象名到数组vbo中

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);   //location = 0 enable
	glBindVertexArray(0);   //解绑
	

	const char* verpath = "Shader/lightcube.vert";
	const char* flagpath = "Shader/lightcube.frag";
	Shader* cubeShader = new Shader(verpath, flagpath);
	if (!cubeShader)
	{
		std::cout << "light shader ERROR" << std::endl;
		return -1;
	}
	cubeShader->use();
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	TextureManager *diffuseTex = TextureManager::Inst();
	if (diffuseTex)
	{
		const char* texPath = "imageSource/box.png";
		bool result = diffuseTex->LoadTexture(texPath, enumTexture::box, GL_BGRA, GL_RGBA, 0, 0);
		if (!result)
			std::cout << "load pic ERROR" << std::endl;
		glUniform1i(glGetUniformLocation(cubeShader->program, "material.diffuse"), 0);

		const char* specularTexPath = "imageSource/container2_specular.png";
		result = diffuseTex->LoadTexture(specularTexPath, enumTexture::specularBox, GL_BGRA, GL_RGBA, 0, 0);
		if (!result)
			std::cout << "load pic2 ERROR" << std::endl;
		glUniform1i(glGetUniformLocation(cubeShader->program, "material.specular"), 1);
	}

	glEnable(GL_DEPTH_TEST);

	
	GLint lightColorLoc = glGetUniformLocation(cubeShader->program, "lightColor");
	
	glUniform3f(lightColorLoc, 1.f, 1.f, 1.f);
	glBindVertexArray(0);

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

		cubeShader->use();
	
		//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 view(1);
		view = mainCamera.getCamearMatrix();
		glm::mat4 projection(1);
		if (viewheight != 0)
			projection = glm::perspective(glm::radians(45.f), (GLfloat)viewWidth / viewheight, 0.1f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(cubeShader->program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//glUniform3f(glGetUniformLocation(cubeShader->program, "light.direction"), lightDirection.x, lightDirection.y, lightDirection.z); //lightsource position
		glUniform3f(glGetUniformLocation(cubeShader->program, "viewPos"), mainCamera.cameraPosition.x, mainCamera.cameraPosition.y, mainCamera.cameraPosition.z);
		glUniform3f(glGetUniformLocation(cubeShader->program, "lightPos"), lightSourcePos.x, lightSourcePos.y, lightSourcePos.z);
		
		glActiveTexture(GL_TEXTURE0);
		diffuseTex->BindTexture(enumTexture::box);
		
		
		glActiveTexture(GL_TEXTURE1);
		diffuseTex->BindTexture(enumTexture::specularBox);
		GLint ambient = glGetUniformLocation(cubeShader->program, "material.ambient");
		GLint diffuse = glGetUniformLocation(cubeShader->program, "material.diffuse");
		GLint specular = glGetUniformLocation(cubeShader->program, "material.specular");
		GLint shininess = glGetUniformLocation(cubeShader->program, "material.shininess");

		glUniform3f(ambient, 1.0f, 0.5f, 0.31f);
		glUniform3f(diffuse, 1.0f, 0.5f, 0.31f);
		glUniform3f(specular, 0.5f, 0.5f, 0.5f);
		glUniform1f(shininess, 32.0f);

		GLint ambientLight = glGetUniformLocation(cubeShader->program, "light.ambient");
		GLint diffuseLight = glGetUniformLocation(cubeShader->program, "light.diffuse");
		GLint specularLight = glGetUniformLocation(cubeShader->program, "light.specular");
		GLint constantLight = glGetUniformLocation(cubeShader->program, "light.constant");
		GLint linearLight = glGetUniformLocation(cubeShader->program, "light.linear");
		GLint quadtaticLight = glGetUniformLocation(cubeShader->program, "light.quadtatic");

		glUniform3f(ambientLight, 0.2f, 0.2f, 0.2f);
		glUniform3f(diffuseLight, 0.5f, 0.5f, 0.5f);
		glUniform3f(specularLight, 1.f, 1.f, 1.f);
		//50		1.0		0.09	0.032
		glUniform1f(constantLight, 1.0);
		glUniform1f(linearLight, 0.09);
		glUniform1f(quadtaticLight, 0.032);


		glBindVertexArray(lightVAO);
		glm::mat4 model;
		for (int i = 0; i < 10; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f*i), glm::vec3(1.0f, 0.3f, 0.5f));
			//model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime()*50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(cubeShader->program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glBindVertexArray(0);
		

		lightShader->use();
		glm::mat4 lightMat(1);
		lightMat = glm::translate(lightMat, lightSourcePos);
		//lightMat = glm::rotate(lightMat, glm::radians(45.f), glm::vec3(0.5f, 1.0f, 0.0f));
		lightMat = glm::scale(lightMat, glm::vec3(0.2, 0.2, 0.2));

		glUniformMatrix4fv(glGetUniformLocation(lightShader->program, "model"), 1, GL_FALSE, glm::value_ptr(lightMat));
		glUniformMatrix4fv(glGetUniformLocation(lightShader->program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightShader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

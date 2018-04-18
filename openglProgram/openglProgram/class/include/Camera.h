#pragma once
/*
	在这里写一个opengl的摄像机类，这个类的是基于欧拉角的
	这里使用的右手坐标系，
	定义一个摄像机需要一个摄像机的位置和一个观察的方向  这就是前轴/负z轴，还需要一个右轴和上轴，其实
	我们实际上创建了一个三个单位轴相互垂直的、以摄像机的位置为原点的坐标系

	我们观察的方向是从观察点指向摄像机的，cameradir = 观察的位置 - 摄像机的位置
	计算右轴时候 定义一个垂直向上的向量updir = （0，1，0）， right = corss(cameradir,updir) 叉乘根据右手定则，
	大拇指指向右轴，手握的方向就是叉乘的顺序

	我们用lookat函数定义一个摄像机矩阵

	这里有个模型
	1 摄像机面向的方向是z轴的负轴方向
	2 俯仰角 pitch 也是从z轴的负轴开始沿着x轴 逆时针转动的 是摄像机与z轴的夹角，
	  在转动偏航角的时候z轴（前轴）是变化的，它指向的是摄像机看向的方向，所以这个
	  夹角一直都是与z轴的夹角 所以z = -cos(pitch)    y = sin(pitch)
	3 偏航角 yaw是从z轴的负轴开始绕着y轴逆时针转动的，也是与z轴的夹角，由于是逆
	  时针转动的，所以x的值是负值，z = -cos(yaw) x = -sin(yaw)

	整合起来之后就是 x = -sin(yaw)  y = sin(pitch) z = -cos(pitch)*-cos(yaw)

*/

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>

enum Camera_Movement
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

const GLfloat YAW = 0.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.05f;
const GLfloat ZOOM = 45.0f;

class Camera
{
public:
	
	Camera();
	Camera(glm::vec3 position,glm::vec3 front);
	~Camera();

	glm::vec3 cameraPosition;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	void setYaw(GLfloat yaw) { this->yaw = yaw; }
	void setPitch(GLfloat pitch) { this->pitch = pitch; }
	void setZoom(GLfloat zoom) { this->zoom = zoom; }
	void setSensitivity(GLfloat sen) { mouseSensitivity = sen; }
	void setSpeed(GLfloat speed) { movementSpeed = speed; }

	void processKeyboardInput(Camera_Movement moveDir,GLfloat deltaTime);
	void processWheel(double offy);
	void processCursor(GLdouble offx, GLdouble offy);

	glm::mat4 getCamearMatrix();
private:
	
	glm::vec3 worldUp;
	

	GLfloat yaw;
	GLfloat pitch;
	GLfloat zoom;
	GLfloat mouseSensitivity;
	GLfloat movementSpeed;

	void updateCameraDir();
};
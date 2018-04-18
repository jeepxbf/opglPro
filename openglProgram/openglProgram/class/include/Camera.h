#pragma once
/*
	������дһ��opengl��������࣬�������ǻ���ŷ���ǵ�
	����ʹ�õ���������ϵ��
	����һ���������Ҫһ���������λ�ú�һ���۲�ķ���  �����ǰ��/��z�ᣬ����Ҫһ����������ᣬ��ʵ
	����ʵ���ϴ�����һ��������λ���໥��ֱ�ġ����������λ��Ϊԭ�������ϵ

	���ǹ۲�ķ����Ǵӹ۲��ָ��������ģ�cameradir = �۲��λ�� - �������λ��
	��������ʱ�� ����һ����ֱ���ϵ�����updir = ��0��1��0���� right = corss(cameradir,updir) ��˸������ֶ���
	��Ĵָָ�����ᣬ���յķ�����ǲ�˵�˳��

	������lookat��������һ�����������

	�����и�ģ��
	1 ���������ķ�����z��ĸ��᷽��
	2 ������ pitch Ҳ�Ǵ�z��ĸ��Ὺʼ����x�� ��ʱ��ת���� ���������z��ļнǣ�
	  ��ת��ƫ���ǵ�ʱ��z�ᣨǰ�ᣩ�Ǳ仯�ģ���ָ��������������ķ����������
	  �н�һֱ������z��ļн� ����z = -cos(pitch)    y = sin(pitch)
	3 ƫ���� yaw�Ǵ�z��ĸ��Ὺʼ����y����ʱ��ת���ģ�Ҳ����z��ļнǣ���������
	  ʱ��ת���ģ�����x��ֵ�Ǹ�ֵ��z = -cos(yaw) x = -sin(yaw)

	��������֮����� x = -sin(yaw)  y = sin(pitch) z = -cos(pitch)*-cos(yaw)

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
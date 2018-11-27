#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"

Camera::Camera(glm::vec3 position,
    glm::vec3 forward, 
    glm::vec3 up, 
    float fovAngleY, 
    float width, 
    float height,
    float nearZ, 
    float farZ, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed)
{
    this->position = position;
    this->forward = forward; 
    this->up = up;

    //does this calculation once
    this->fov = fovAngleY * glm::pi<float>() / 180.0f;
    this->width = width;
    this->height = height;
    this->nearZ = nearZ;
    this->farZ = farZ;

	pitch = startPitch;
	yaw = startYaw;
	//calculate the front of the camera 
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	movementSpeed = startMovementSpeed;
	turnSpeed = startTurnSpeed;
}

//the basics of the camera needs to create its local axis
Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	up = startUp; //worldUp
	pitch = startPitch;
	yaw = startYaw;
	front= glm::vec3(0.0f, 0.0f, -1.0f);
	movementSpeed = startMovementSpeed;
	turnSpeed = startTurnSpeed;
}

Camera::~Camera()
{
}

//incorporates deltaTime updates the position of the camera
void Camera::Update(GLfloat deltaTime)
{
    //TODO - cache the pointer to the Input single instance instead of calling 
    //       GetInstance() multiple times
	GLfloat velocity = movementSpeed * deltaTime;
    if (Input::GetInstance()->IsKeyDown(GLFW_KEY_A))
    {
		position -= right * velocity;
    } 
     

	//changes the local axis with the right and front position
    else if (Input::GetInstance()->IsKeyDown(GLFW_KEY_D))
    {
		position += right * velocity;
    } 
	else if (Input::GetInstance()->IsKeyDown(GLFW_KEY_Q))
	{
		position -= cameraUp * velocity;
	}
	else if (Input::GetInstance()->IsKeyDown(GLFW_KEY_E))
	{
		position += cameraUp * velocity;
	}
	else if (Input::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		position -= front * velocity;
	}
	else if (Input::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		position += front * velocity;
	}
	//mouse input
	else if (Input::GetInstance()->IsKeyDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		
		mouseControl(Input::GetInstance()->getXChange(), Input::GetInstance()->getYChange());
	}

	right = glm::normalize(glm::cross(front, up));
	//camera's right and front to get cameraup
	cameraUp = glm::normalize(glm::cross(right, front));
    
	//view matrix
    //we use forward here instead of a lookTarget because it's easier to track
    //and conceptualize.
    viewMatrix = glm::lookAt(
        position,               //where the camera is
        position + front,     //where the camera is looking at , point of camera to at, look in front (forward)
        cameraUp                      //what is 'up' for the camera of the world (up)
    );

    //create the projection matrix
    projectionMatrix = glm::perspectiveFovLH<GLfloat>(
        fov,
        width,                  //the width of the window in float
        height,                 //the height of the window in float
        nearZ,                  //the near Z-plane
        farZ                    //the far Z-plane
    );
}

//mouse controls yaw with the x changes 
//pitch with the y changes
//stops the user from pitches too forward or backwards for things to be upside down 

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	UpdateMouse();
}
//if there are changes in the yaw or pitch 
//the camera's front changes
//x depends on yaw and pitch
//y depends on pitch
//z depends on yaw and pitch but the sin of yaw
void Camera::UpdateMouse()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); 
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	//cross of the front and world up to find the right of the camera
	right = glm::normalize(glm::cross(front, up));
	//camera's right and front to get the camera's up
	cameraUp = glm::normalize(glm::cross(right, front));

}


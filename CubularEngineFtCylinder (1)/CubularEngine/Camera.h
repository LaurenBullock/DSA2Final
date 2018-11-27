#pragma once
#include "stdafx.h"
//Haley
//This camera moves on its own axis using a right and front vectors to move around in the vector space
//updating its position with deltaTime
class Camera
{
private:
    glm::vec3 position;     //position of camera
    glm::vec3 forward;      //where the camera is facing
    glm::vec3 up;           //what 'up' is for the camera, world cross product of direction and right
	//tutorial
	glm::vec3 front;
	glm::vec3 right; //calculate with the cross product of the direction and the "up" vector[0,1,0]
	glm::vec3 cameraUp; //
	GLfloat yaw;
	GLfloat pitch;
	GLfloat movementSpeed;
	GLfloat turnSpeed;


    float fov;      //field of view
    float width;    //width of screen
    float height;   //height of screen
    float nearZ;    //near Z-plane
    float farZ;     //far  Z-plane

    glm::mat4 viewMatrix;       //cached view matrix
    glm::mat4 projectionMatrix; //cached projection matrix

public:

    /// <summary>
    /// Constructor where you supply everything you need for the 2 matrices
	//HR: adding startYaw, StartPitch, startMovementSpeed, StartTurnSpeed
    /// </summary>
    Camera(
        glm::vec3 position,
        glm::vec3 forward,
        glm::vec3 up,
        float fovAngleY,
        float width,
        float height,
        float nearZ,
        float farZ, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed
    );

	Camera(
		glm::vec3 startPosition, 
		glm::vec3 startUp, 
		GLfloat startYaw, 
		GLfloat startPitch, 
		GLfloat startMovementSpeed, 
		GLfloat startTurnSpeed);
    /// <summary>
    /// Destruction
    /// </summary>
    ~Camera();

    /// <summary>
    /// Queries input and updates the matrices of the camera
    /// </summary>
    void Update();
	//uses deltatime instead of small increments to control speed
	void Update(GLfloat deltaTime);
	
	void mouseControl(GLfloat xChange, GLfloat yChange);
	void UpdateMouse();

    /// <summary>
    /// Gets the view matrix
    /// </summary>
    glm::mat4 GetView() const { return viewMatrix; }
    
    /// <summary>
    /// Gets the projection matrix
    /// </summary>
    glm::mat4 GetProjection() const { return projectionMatrix; }


    //TODO - maybe having getters & setters for other private variables would be
    //       useful for you
};


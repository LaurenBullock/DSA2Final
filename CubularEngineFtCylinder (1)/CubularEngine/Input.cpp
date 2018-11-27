#include "Input.h"

//for singleton
Input* Input::instance = nullptr;

Input::Input()
{
}

Input::~Input()
{
}

Input* Input::GetInstance()
{
    //basic singleton stuff

    if (instance == nullptr)
    {
        instance = new Input();
    }
    return instance;
}

void Input::Release()
{
    //TODO - check if instance is instance is a nullptr?
    delete instance;
    instance = nullptr;
}

void Input::Init(GLFWwindow * window)
{
    //TODO - honestly this feels pretty weird that a singleton will require
    //       an Init before it works. Some people wouldn't consider this a 
    //       true singleton. (Maybe it shouldn't ba singleton?)
    this->window = window;
	//set the mouse to the window
	glfwSetWindowUserPointer(window, this);
	//this method hides the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	createCallBacks();
}
//get input if the key is down
//if the mouse's left button is press return true
bool Input::IsKeyDown(int key)  
{
    //TODO - maybe we should assert if window is nullptr
	if (key == GLFW_MOUSE_BUTTON_LEFT)
	{
		return glfwGetMouseButton(window, key) == GLFW_PRESS;
	}
	return glfwGetKey(window, key) == GLFW_PRESS;
}

//get the mouse's x position change 
GLfloat Input::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

//get the mouse's y position change
GLfloat Input::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

//create the mouse callback 
void Input::createCallBacks()
{
	glfwSetCursorPosCallback(window, handleMouse);
}

//set the mouse to the window
//if the mouse moves find the last x/y and the current x/y, subtract them 
void Input::handleMouse(GLFWwindow * window, double xPos, double yPos)
{
	Input* theWindow = static_cast<Input*>(glfwGetWindowUserPointer(window));
	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;

	}
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

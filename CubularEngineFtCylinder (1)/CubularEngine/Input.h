#pragma once
#include "stdafx.h"
//Haley Rogers
//This cpp handles the keyboard and mouse input for the program
//using video tutorial, "The Camera" by TutsGalaxy.com on modern GL/C++ as guide for the mouse controls
/// <summary>
/// Singleton that does basic input handling
/// </summary>

//TODO - refactor a few things for this class to handle 'isKeyPressed' or
//       'isKeyReleased'.

//TODO - refactor a few things for this class to handle mouse input

class Input
{
private:      
    /// <summary>
    /// Singleton implementation (private constructor & destructor)
    /// </summary>
    Input();
    ~Input();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

    static Input* instance;     //singleton stuff
    GLFWwindow* window;         //window (required for input)
public:
    /// <summary>
    /// Singleton reference to the instance
    /// </summary>
    static Input* GetInstance();

    /// <summary>
    /// De-allocation
    /// </summary>
    static void Release();

    /// <summary>
    /// Initializes the input singleton before input can be detected properly
    /// </summary>
    void Init(GLFWwindow* window);

    /// <summary>
    /// Checks if the key is down
    /// </summary>
    bool IsKeyDown(int key);

	//tutorial
	GLfloat getXChange();
	GLfloat getYChange();
	void createCallBacks();
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};


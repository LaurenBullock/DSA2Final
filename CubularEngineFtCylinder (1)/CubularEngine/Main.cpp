#include "stdafx.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "GameEntity.h"
#include "Material.h"
#include "Input.h"
#include <glm/gtc/matrix_transform.hpp>
//Haley Rogers
//Project Work
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
//  Adds a triangle to the list of vertices
//  CCW by default
//  3- 2    
//  |/      
//  1       
void addTriangle(std::vector<GLfloat> &vertices, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3 ) {

    vertices.push_back( static_cast<GLfloat>( p1.x ) );
    vertices.push_back( static_cast<GLfloat>( p1.y ) );
    vertices.push_back( static_cast<GLfloat>( p1.z ) );

    vertices.push_back( static_cast<GLfloat>( p2.x ) );
    vertices.push_back( static_cast<GLfloat>( p2.y ) );
    vertices.push_back( static_cast<GLfloat>( p2.z ) );

    vertices.push_back( static_cast<GLfloat>( p3.x ) );
    vertices.push_back( static_cast<GLfloat>( p3.y ) );
    vertices.push_back( static_cast<GLfloat>( p3.z ) );
}
//HR: This method creates a line basedon two points within the space
void createLine(std::vector<GLfloat> &vertices, glm::vec3 p1, glm::vec3 p2)
{
	vertices.push_back(static_cast<GLfloat>(p1.x));
	vertices.push_back(static_cast<GLfloat>(p1.y));
	vertices.push_back(static_cast<GLfloat>(p1.z));

	vertices.push_back(static_cast<GLfloat>(p2.x));
	vertices.push_back(static_cast<GLfloat>(p2.y));
	vertices.push_back(static_cast<GLfloat>(p2.z));
}
//  Adds a quad to a vertex array
//  CCW by default
//  3- 2
//  |/ |
//  1- 4
void addQuad(std::vector<GLfloat> &vertices, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4 ) {
    addTriangle( vertices, p1, p2, p3 );
    addTriangle( vertices, p1, p4, p2 );
}
/// <summary>
/// Fills a vertex array with floats representing a cylinder
/// </summary>
/// <param name="vertices">vertex array </param>
/// <param name="height">height of cylinder</param>
/// <param name="radius">radius of cylinder</param>
/// <param name="subdivisions">subdivisions of the cylinder</param>
void generateCylinder(std::vector<GLfloat> &vertices, float height, float radius, int subdivisions) {
    //how many radians to increment to rotate around a unit circle
    float angleIncrement = 2 * glm::pi<float>() / subdivisions;

    //cache the top center and bottom points for the cylinder
    glm::vec3 topCenter( 0, height / 2.f, 0 );
    glm::vec3 bottomCenter( 0, -height / 2.f, 0 );

    float currAngle = 0.f;
    for ( int i = 0; i < subdivisions; ++i, currAngle += angleIncrement)
    {
        //calculates the sin and cos of the current angle and the next one
        float sinCurr = sinf( currAngle ) * radius;
        float cosCurr = cosf( currAngle ) * radius;
        float sinNext = sinf( currAngle + angleIncrement ) * radius;
        float cosNext = cosf( currAngle + angleIncrement ) * radius;

        //generate the points (view this from the side)
        glm::vec3 bottomLeft(  sinCurr, -height / 2.f, cosCurr );
        glm::vec3 topLeft(     sinCurr,  height / 2.f, cosCurr );
        glm::vec3 bottomRight( sinNext, -height / 2.f, cosNext );
        glm::vec3 topRight(    sinNext,  height / 2.f, cosNext );

        //add the side quad
        addQuad( vertices, bottomLeft, topRight, topLeft, bottomRight );

        //add the top & bot tri
        addTriangle( vertices, topLeft, topRight, topCenter );
        addTriangle( vertices, bottomLeft, bottomCenter, bottomRight );
    }
};

//TODO - maybe make some #define macro for a print if debug?
//TODO - make an Engine class with a specific Init() and Run() function such that
//       our Main.cpp is kept clean and tidy

int main()
{
    {
        //init GLFW
        {
            if (glfwInit() == GLFW_FALSE)
            {
#ifdef _DEBUG
                std::cout << "GLFW failed to initialize" << std::endl;
                _CrtDumpMemoryLeaks();
                std::cin.get();
#endif
                return 1;
            }
        }
#ifdef _DEBUG
        std::cout << "GLFW successfully initialized!" << std::endl;
#endif // _DEBUG

        //create & init window, set viewport
        int width = 800;
        int height = 600;
        GLFWwindow* window = glfwCreateWindow(width, height, "Haley Rogers: Midterm", nullptr, nullptr);
        {
            if (window == nullptr)
            {
#ifdef _DEBUG
                std::cout << "GLFW failed to create window" << std::endl;
                _CrtDumpMemoryLeaks();
                std::cin.get();
#endif
                glfwTerminate();
                return 1;
            }

            //tells OpenGL to use this window for this thread
            //(this would be more important for multi-threaded apps)
            glfwMakeContextCurrent(window);

            //gets the width & height of the window and specify it to the viewport
            int windowWidth, windowHeight;
            glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);
        }
#ifdef _DEBUG
        std::cout << "Window successfully initialized!" << std::endl;
#endif // _DEBUG

        //init GLEW
        {
            if (glewInit() != GLEW_OK)
            {
#ifdef _DEBUG
                std::cout << "GLEW failed to initialize" << std::endl;
                _CrtDumpMemoryLeaks();
                std::cin.get();
#endif
                glfwTerminate();
                return 1;
            }
        }
#ifdef _DEBUG
        std::cout << "GLEW successfully initialized!" << std::endl;
#endif // _DEBUG

        //init the shader program
        //TODO - this seems like a better job for a shader manager
        //       perhaps the Shader class can be refactored to fit a shader program
        //       rather than be a thing for vs and fs
        GLuint shaderProgram = glCreateProgram();
        {

            //create vS and attach to shader program
            Shader *vs = new Shader();
            vs->InitFromFile("assets/shaders/vertexShader.glsl", GL_VERTEX_SHADER);
            glAttachShader(shaderProgram, vs->GetShaderLoc());

            //create FS and attach to shader program
            Shader *fs = new Shader();
            fs->InitFromFile("assets/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
            glAttachShader(shaderProgram, fs->GetShaderLoc());

            //link everything that's attached together
            glLinkProgram(shaderProgram);

            GLint isLinked;
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
            if (!isLinked)
            {
                char infolog[1024];
                glGetProgramInfoLog(shaderProgram, 1024, NULL, infolog);
#ifdef _DEBUG
                std::cout << "Shader Program linking failed with error: " << infolog << std::endl;
                std::cin.get();
#endif

                // Delete the shader, and set the index to zero so that this object knows it doesn't have a shader.
                glDeleteProgram(shaderProgram);
                glfwTerminate();
                _CrtDumpMemoryLeaks();
                return 1;
            }

            //everything's in the program, we don't need this
            delete fs;
            delete vs;
        }

#ifdef _DEBUG
        std::cout << "Shaders compiled attached, and linked!" << std::endl;
#endif // _DEBUG

			GLfloat vertices[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
			};
        std::vector<GLfloat> cylinder;
        generateCylinder( cylinder, 2.f, 1.f, 7 );

		Mesh* squareMesh = new Mesh();

        //myMesh->InitWithVertexArray( &cube[0], cube.size(), shaderProgram);
		//myMesh->InitWithVertexArray(&cylinder[0], cylinder.size(), shaderProgram);

		//Cube mesh
		squareMesh->InitWithVertexArray(vertices, _countof(vertices), shaderProgram);
        Material* myMaterial = new Material(shaderProgram);

        //HR: Made a vector of all the 10 cubes within the space, randomly placed
		std::vector<GameEntity*> cubeManager;		
		
		GameEntity* cubeTen = new GameEntity(
			squareMesh,
			myMaterial,
			glm::vec3(-2.f, -1.5f, 0.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.2f, 0.2f, 0.2f)
		);
		cubeManager.push_back(cubeTen);
        GameEntity* cubeOne = new GameEntity(
            squareMesh,
            myMaterial,
            glm::vec3(-2.5f, 0.3f, 0.f),
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(0.4f)
        );
		cubeManager.push_back(cubeOne);
		GameEntity* cubeTwo = new GameEntity(
			squareMesh,
			myMaterial,
			glm::vec3(-0.8f, -1.2f, 0.f), 
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.4f)
		);
		cubeManager.push_back(cubeTwo);
		GameEntity* cubeThree = new GameEntity(
			squareMesh,
			myMaterial,
			glm::vec3(-0.5f, -2.4f, 0.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.4f)
		);
		cubeManager.push_back(cubeThree);
		GameEntity* cubeFour = new GameEntity(
			squareMesh,
			myMaterial,
			glm::vec3(-1.5f, 1.7f, 0.f), 
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.4f)
		);
		cubeManager.push_back(cubeFour);
		GameEntity* cubeFive = new GameEntity(
			squareMesh,
			myMaterial,
			glm::vec3(-0.4f, 0.3f, 0.f), 
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.4f)
		);
		cubeManager.push_back(cubeFive);
		GameEntity* cubeSix = new GameEntity(
			squareMesh,
			myMaterial,
			glm::vec3(0.9f, -1.2f, 0.f), 
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.4f)
		);
		cubeManager.push_back(cubeSix);
		GameEntity* cubeSeven = new GameEntity(
			squareMesh,
			myMaterial,
			glm::vec3(1.8f, 1.6f, 0.f), 
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.4f)
		);
		cubeManager.push_back(cubeSeven);
		GameEntity* cubeEight = new GameEntity(
			squareMesh,
			myMaterial,
			glm::vec3(2.3f, -2.1f, 0.f), 
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.4f)
		);
		cubeManager.push_back(cubeEight);
		GameEntity* cubeNine = new GameEntity(
			squareMesh,
			myMaterial,
			glm::vec3(2.3f, 0.5f, 0.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.4f)
		);
		cubeManager.push_back(cubeNine);
        //TODO - maybe a CameraManager?
        Camera* myCamera = new Camera(
            glm::vec3(0.2f, 1.1f, -6.9f),    //position of camera
            glm::vec3(0.0f, 0.0f, 1.f),     //the 'forward' of the camera
            glm::vec3(0.0f, 1.f, 0.0f),     //what 'up' is for the camera
            60.0f,                          //the field of view in radians
            (float)width,                   //the width of the window in float
            (float)height,                  //the height of the window in float
            0.01f,                          //the near Z-plane
            100.f, -84.9f, 9.6f, 0.5f, 0.05f                           //the far Z-plane
        );
        Input::GetInstance()->Init(window);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable( GL_CULL_FACE );
        glCullFace( GL_FRONT );
		std::vector<glm::vec3> cubeMovement;
		for (int i = 0; i < 10; i++)
		{
			glm::vec3 vector;
			if (i < 5)
			{
				vector.x = (rand() % 10 + 1.0f) / 10.0f;
				vector.y = (rand() % 10 + 1.0f) / 10.0f;
			}
			else
			{
				vector.x = -(rand() % 10 + 1.0f) / 10.0f;
				vector.y = -(rand() % 10 + 1.0f) / 10.0f;
			}
			vector.z = 0.0f;
			cubeMovement.push_back(vector);
		}
        //main loop
        while (!glfwWindowShouldClose(window))
        {
			GLfloat now = glfwGetTime();
			deltaTime = now - lastTime; //divided by get performance frequency * 1000 for seconds
			lastTime = now;

            /* INPUT */
            {
                //checks events to see if there are pending input
                glfwPollEvents();
                //breaks out of the loop if user presses ESC
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                {
                    break;
                }
            }

            /* GAMEPLAY UPDATE */
			//HR:cubes will be Updated based on their position, scale and angle
			//cubes will move with a given vector(velocity that will be updated through deltaTime) at the start of the program
			//when two cubes collide with each other, bounce away from each other
   //         cubeOne->Update();
			//cubeOne->rotatingCube(1.0f, deltaTime);
			//cubeTwo->Update();
			//cubeTwo->translatingCube(cubeMovement[1], deltaTime);
			//cubeThree->Update();
			//cubeFour->Update();
			//cubeFive->Update();
			//cubeSix->Update();
			//cubeSeven->Update();
			//cubeEight->Update();
			//cubeNine->Update();
			//cubeTen->Update();
			
			for (int i = 0; i < 10; i++)
			{
				cubeManager[i]->Update();
				cubeManager[i]->translatingCube(cubeMovement[i], deltaTime);
			}
			//HR: the camera is updating with delta time
            myCamera->Update(deltaTime);

            /* PRE-RENDER */
            {
                //start off with clearing the 'color buffer'
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                //clear the window to have c o r n f l o w e r   b l u e
                glClearColor(0, 0, 0, 1.0f);
            }

            /* RENDER */
			//cubes
   //         cubeOne->Render(myCamera);
			//cubeTwo->Render(myCamera);
			//cubeThree->Render(myCamera);
			//cubeFour->Render(myCamera);
			//cubeFive->Render(myCamera);
			//cubeSix->Render(myCamera);
			//cubeSeven->Render(myCamera);
			//cubeEight->Render(myCamera);
			//cubeNine->Render(myCamera);
			//cubeTen->Render(myCamera);
			for (int i = 0; i < 10; i++)
			{
				cubeManager[i]->Render(myCamera);
			}
			myCamera->Update(deltaTime);
            /* POST-RENDER */
            {
                //'clear' for next draw call
                glBindVertexArray(0);
                glUseProgram(0);
                //swaps the front buffer with the back buffer
                glfwSwapBuffers(window);
            }
        }

        //de-allocate our mesh for the squares and lines
        delete squareMesh;
        delete myMaterial;
		
		//HR: cubes
        delete cubeOne;
		delete cubeTwo;
		delete cubeThree;
		delete cubeFour;
		delete cubeFive;
		delete cubeSix;
		delete cubeSeven;
		delete cubeEight;
		delete cubeNine;
		delete cubeTen;
		
		//HR: lines
		//HR: camera
        delete myCamera;
		//delete tutCamera;
        Input::Release();
    }

    //clean up
    glfwTerminate();
#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif // _DEBUG
    return 0;
}
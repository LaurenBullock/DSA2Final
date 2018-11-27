#pragma once
#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
//Haley Rogers
//Represents one 'renderable' object
//added a rotation method, distance from the start and what their index is the vector lists of cubes
//and a rendering method for the graph lines

class GameEntity
{
private:
    Mesh* mesh;        
    Material* material;

    //TODO - maybe this should be in a transform class?
    glm::vec3 scale;
    glm::mat4 worldMatrix;
public: 
    /// <summary>
    /// Basic paramterized constructor for most of our private vars
    /// </summary>
	glm::vec3 position;
	glm::vec3 eulerAngles;

	//distance from the start vertex
	float distStart;
	//this for the original list of cubes
	int unOrderIndex;
	//this for the ordered list of cubes
	int orderIndex;
	//saves the dist
	void distSaved(float dist);

	float getDistance() const;
    GameEntity(
        Mesh* mesh,
        Material* material,
        glm::vec3 position,
        glm::vec3 eulerAngles,
        glm::vec3 scale
    );

    /// <summary>
    /// Destruction
    /// </summary>
    virtual ~GameEntity();

	//HR: added a rotation method for each cube
	void rotatingCube(float multiplier, GLfloat deltaTime);
	void translatingCube(glm::vec3 velocity, GLfloat deltaTime);
	void screenWrap();
	/// <summary>
    /// Updates the worldMatrix
    /// </summary>
    virtual void Update();

    /// <summary>
    /// Renders the gameEntity based on a camera
    /// </summary>
    void Render(Camera* camera);

	//add a method for rendering a line with GLLines instead of triangles
	void RenderingLine(Camera * camera);

};


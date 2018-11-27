#include "GameEntity.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Input.h"

void GameEntity::distSaved(float dist)
{
	distStart = dist;
}

float GameEntity::getDistance() const
{
	return distStart;
}

GameEntity::GameEntity( Mesh * mesh,
    Material * material,
    glm::vec3 position,
    glm::vec3 eulerAngles,
    glm::vec3 scale )
{
    this->mesh = mesh;
    this->material = material;
    this->position = position;
    this->eulerAngles = eulerAngles;
    this->scale = scale;
	distStart = 0;
	orderIndex = 0;
	unOrderIndex = 0;
    worldMatrix = glm::identity<glm::mat4>();
}

GameEntity::~GameEntity()
{
}
//if visited rotated this cube
void GameEntity::rotatingCube(float multiplier, GLfloat deltaTime)
{
 eulerAngles.x += deltaTime * multiplier;
}
//velocity in the x,y axis
void GameEntity::translatingCube(glm::vec3 velocity, GLfloat deltaTime)
{
	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
	position.z += velocity.z * deltaTime;
}
//This method keeps all the cubes on the screen
void GameEntity::screenWrap()
{
	if (position.x > 5)
	{

	}
	if (position.x < 0)
	{

	}
	if (position.y > 5)
	{

	}
	if (position.y < 0)
	{

	}
}
void GameEntity::Update()
{
    Input* input = Input::GetInstance();

    float multiplier = 1.f;


    //construction of world matrix
    worldMatrix = glm::identity<glm::mat4>();

    worldMatrix = glm::translate( worldMatrix, position );

    //rotate along y
    worldMatrix = glm::rotate( worldMatrix,
        this->eulerAngles.x, glm::vec3( 1.f, 0.f, 0.f ) );

    //rotate along x
    worldMatrix = glm::rotate( worldMatrix,
        this->eulerAngles.y, glm::vec3( 0.f, 1.f, 0.f ) );
	//rotate along z
    worldMatrix = glm::rotate( worldMatrix,
        this->eulerAngles.z, glm::vec3( 0.f, 0.f, 1.f ) );

    worldMatrix = glm::scale( worldMatrix, scale );
}

void GameEntity::Render( Camera* camera )
{
    material->Bind( camera, worldMatrix );
    mesh->Render();

}

void GameEntity::RenderingLine(Camera* camera)
{
	material->Bind(camera, worldMatrix);
	mesh->RenderLine();

}

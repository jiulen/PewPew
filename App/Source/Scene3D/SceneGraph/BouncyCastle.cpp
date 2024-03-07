/**
 CBouncyCastle
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "BouncyCastle.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/SolidObjectManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CBouncyCastle::CBouncyCastle(void)
{
}

/**
 @brief Destructor
 */
CBouncyCastle::~CBouncyCastle(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CBouncyCastle::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(0);
	cSceneNode->SetShader("Shader3D_Model");
	cSceneNode->Init();
	cSceneNode->sceneGraphType = CSceneNode::SG_Type::BOUNCY;
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(10.f, -10.f);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(10.f, fCheckHeight, -10.f));
	cSceneNode->SetLocalScaleMtx(glm::vec3(1, 1, 1));
	cSceneNode->SetPosition(glm::vec3(10.f, fCheckHeight, -10.f));
	cSceneNode->InitModels("Models/BouncyCastle/BouncyCastle.obj", "Models/BouncyCastle/purple.tga");
	cSceneNode->bContinuousUpdate = true;
	cSceneNode->SetUpdateScaleMtx(glm::vec3(1, 1.05, 1));
	cSceneNode->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-1.5, 0, -1.5), glm::vec3(1.5, 1.f, 1.5));
	cSceneNode->SetType(CEntity3D::TYPE::STRUCTURE);

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(1);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->sceneGraphType = CSceneNode::SG_Type::BOUNCY;
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(0, 1.35, 0)); //translate is scale of parent + 0.35
	cSceneNodeChild->SetPosition(glm::vec3(10.f, fCheckHeight, -10.f));
	cSceneNodeChild->InitModels("Models/BouncyCastle/ball.obj", "Models/BouncyCastle/ball.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(2.f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f));
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);

	CSolidObjectManager::GetInstance()->Add(cSceneNode);
	return true;
}

/**
 @brief PrintSelf
 */
void CBouncyCastle::PrintSelf(void)
{
	cout << "CBouncyCastle::PrintSelf()" << endl;
}
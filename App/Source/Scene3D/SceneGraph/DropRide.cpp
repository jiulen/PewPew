/**
 CDropRide
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "DropRide.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/SolidObjectManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CDropRide::CDropRide(void)
{
}

/**
 @brief Destructor
 */
CDropRide::~CDropRide(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CDropRide::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(0);
	cSceneNode->SetShader("Shader3D_Model");
	cSceneNode->Init();
	cSceneNode->sceneGraphType = CSceneNode::SG_Type::DROP_RIDE;
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(-10.f, -10.f);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(-10.f, fCheckHeight, -10.f));
	cSceneNode->SetPosition(glm::vec3(-10.f, fCheckHeight, -10.f));
	cSceneNode->InitModels("Models/DropRide/DropRideTower_H.obj", "Models/DropRide/white_H.tga",
							"Models/DropRide/DropRideTower_M.obj", "Models/DropRide/green_M.tga",
							"Models/DropRide/DropRideTower_L.obj", "Models/DropRide/pink_L.tga");
	cSceneNode->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-1.5, -1.5, -1.5), glm::vec3(1.5, 7, 1.5));
	cSceneNode->SetType(CEntity3D::TYPE::STRUCTURE);
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.f;
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 15.f;
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 30.f;

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(1);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->sceneGraphType = CSceneNode::SG_Type::DROP_RIDE;
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(0, 0.6, 0));
	cSceneNodeChild->SetPosition(glm::vec3(-10.f, fCheckHeight, -10.f));
	cSceneNodeChild->InitModels("Models/DropRide/DropRideSeats_H.obj", "Models/DropRide/white_H.tga",
								"Models/DropRide/DropRideSeats_M.obj", "Models/DropRide/green_M.tga",
								"Models/DropRide/DropRideSeats_L.obj", "Models/DropRide/pink_L.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateTranslateMtx(glm::vec3(0, 0.05, 0));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f));
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	CSolidObjectManager::GetInstance()->Add(cSceneNode);
	return true;
}

/**
 @brief PrintSelf
 */
void CDropRide::PrintSelf(void)
{
	cout << "CDropRide::PrintSelf()" << endl;
}
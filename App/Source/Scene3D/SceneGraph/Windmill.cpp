/**
 CWindmill
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Windmill.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/SolidObjectManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CWindmill::CWindmill(void)
{
}

/**
 @brief Destructor
 */
CWindmill::~CWindmill(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CWindmill::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(0);
	cSceneNode->SetShader("Shader3D_Model");
	cSceneNode->Init();
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(-35.f, 12.5f);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(-35.f, fCheckHeight, 12.5f));
	cSceneNode->SetPosition(glm::vec3(-35.f, fCheckHeight, 12.5f));
	cSceneNode->InitModels("Models/Windmill/WindmillStand_H.obj", "Models/Windmill/white_H.tga",
						   "Models/Windmill/WindmillStand_M.obj", "Models/Windmill/green_M.tga",
						   "Models/Windmill/WindmillStand_L.obj", "Models/Windmill/pink_L.tga");
	cSceneNode->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.2f, -0.5f, -0.2f), glm::vec3(0.2f, 6.f, 0.2f));
	//cSceneNode->bIsDisplayed = false;
	cSceneNode->SetType(CEntity3D::TYPE::STRUCTURE);
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.f;
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 40.f;
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 50.f;

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(1);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(0, 6, 0.3));
	cSceneNodeChild->SetPosition(glm::vec3(-35.f, fCheckHeight, 10.f));
	cSceneNodeChild->InitModels("Models/Windmill/WindmillFans_H.obj", "Models/Windmill/Windturbine_H.tga",
								"Models/Windmill/WindmillFans_M.obj", "Models/Windmill/Windturbine_M.tga",
								"Models/Windmill/WindmillFans_L.obj", "Models/Windmill/Windturbine_L.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(2.f), glm::vec3(0.0f, 0.0f, 1.0f));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f));
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 40.f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 50.f;

	CSolidObjectManager::GetInstance()->Add(cSceneNode);
	return true;
}

/**
 @brief PrintSelf
 */
void CWindmill::PrintSelf(void)
{
	cout << "CWindmill::PrintSelf()" << endl;
}
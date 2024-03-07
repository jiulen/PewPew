/**
 CTeacupRide
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "TeacupRide.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/SolidObjectManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CTeacupRide::CTeacupRide(void)
{
}

/**
 @brief Destructor
 */
CTeacupRide::~CTeacupRide(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CTeacupRide::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(0);
	cSceneNode->SetShader("Shader3D_Model");
	cSceneNode->Init();
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(10.f, 10.f);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(10.f, fCheckHeight, 10.f));
	cSceneNode->SetPosition(glm::vec3(10.f, fCheckHeight, 10.f));
	cSceneNode->InitModels("Models/TeacupRide/wooden-plate_H.obj", "Models/TeacupRide/wooden-plate_H.tga",
						   "Models/TeacupRide/wooden-plate_M.obj", "Models/TeacupRide/wooden-plate_M.tga",
						   "Models/TeacupRide/wooden-plate_L.obj", "Models/TeacupRide/wooden-plate_L.tga");
	cSceneNode->bContinuousUpdate = true;
	cSceneNode->SetUpdateRotateMtx(glm::radians(0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNode->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-3.f, 0, -3.f), glm::vec3(3.f, 1.2f, 3.f));
	cSceneNode->SetType(CEntity3D::TYPE::STRUCTURE);
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.f;
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 15.f;
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 30.f;

	// Initialise 1st child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(1);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(0, 0.625, 1.75));
	cSceneNodeChild->SetPosition(glm::vec3(10.f, fCheckHeight, 10.f));
	cSceneNodeChild->InitModels("Models/TeacupRide/teacup_H.obj", "Models/TeacupRide/teacup_H.tga",
								"Models/TeacupRide/teacup_M.obj", "Models/TeacupRide/teacup_M.tga",
								"Models/TeacupRide/teacup_L.obj", "Models/TeacupRide/teacup_L.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3());
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 15.f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 30.f;

	// Initialise 2nd child CSceneNode
	cSceneNodeChild = new CSceneNode(2);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(-1.25, 0.625, -0.75));
	cSceneNodeChild->SetPosition(glm::vec3(10.f, fCheckHeight, 10.f));
	cSceneNodeChild->InitModels("Models/TeacupRide/teacup_H.obj", "Models/TeacupRide/teacup_H.tga",
								"Models/TeacupRide/teacup_M.obj", "Models/TeacupRide/teacup_M.tga",
								"Models/TeacupRide/teacup_L.obj", "Models/TeacupRide/teacup_L.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(-0.3f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 15.f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 30.f;

	// Initialise 3rd child CSceneNode
	cSceneNodeChild = new CSceneNode(3);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(1.5, 0.625, -1.5));
	cSceneNodeChild->SetPosition(glm::vec3(10.f, fCheckHeight, 10.f));
	cSceneNodeChild->InitModels("Models/TeacupRide/teacup_H.obj", "Models/TeacupRide/teacup_H.tga",
								"Models/TeacupRide/teacup_M.obj", "Models/TeacupRide/teacup_M.tga",
								"Models/TeacupRide/teacup_L.obj", "Models/TeacupRide/teacup_L.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(-0.3f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 15.f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 30.f;

	CSolidObjectManager::GetInstance()->Add(cSceneNode);
	return true;
}

/**
 @brief PrintSelf
 */
void CTeacupRide::PrintSelf(void)
{
	cout << "CTeacupRide::PrintSelf()" << endl;
}
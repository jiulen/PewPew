/**
 CFerrisWheel
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "FerrisWheel.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/SolidObjectManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CFerrisWheel::CFerrisWheel(void)
{
}

/**
 @brief Destructor
 */
CFerrisWheel::~CFerrisWheel(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CFerrisWheel::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(0);
	cSceneNode->SetShader("Shader3D_Model");
	cSceneNode->Init();
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(-10.f, 10.f);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNode->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNode->InitModels("Models/FerrisWheel/FerrisWheelBase_H.obj", "Models/FerrisWheel/FerrisWheelBase_H.tga",
						   "Models/FerrisWheel/FerrisWheelBase_M.obj", "Models/FerrisWheel/FerrisWheelBase_M.tga",
						   "Models/FerrisWheel/FerrisWheelBase_L.obj", "Models/FerrisWheel/FerrisWheelBase_L.tga");
	cSceneNode->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-3, 0, -5), glm::vec3(1.5, 12, 5));
	cSceneNode->SetType(CEntity3D::TYPE::STRUCTURE);
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.f;
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 20.f;
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 40.f;

	// Initialise 1st child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(1);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(-0.7f, 6.25f, 0));
	cSceneNodeChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChild->InitModels("Models/FerrisWheel/FerrisWheelArm_H.obj", "Models/FerrisWheel/FerrisWheelArm_H.tga",
								"Models/FerrisWheel/FerrisWheelArm_M.obj", "Models/FerrisWheel/FerrisWheelArm_M.tga",
								"Models/FerrisWheel/FerrisWheelArm_L.obj", "Models/FerrisWheel/FerrisWheelArm_L.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 15.f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 30.f;

	// Initialise 2nd gen child 1
	CSceneNode* cSceneNodeChildChild = new CSceneNode(2);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, 0, -5));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 2
	cSceneNodeChildChild = new CSceneNode(3);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, 0, 5));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 3
	cSceneNodeChildChild = new CSceneNode(4);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, -5, 0));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 4
	cSceneNodeChildChild = new CSceneNode(5);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, 5, 0));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 5
	cSceneNodeChildChild = new CSceneNode(6);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, 4.3, 2.5));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 6
	cSceneNodeChildChild = new CSceneNode(7);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, -4.3, -2.5));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 7
	cSceneNodeChildChild = new CSceneNode(8);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, -4.3, 2.5));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 8
	cSceneNodeChildChild = new CSceneNode(9);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, 4.3, -2.5));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 9
	cSceneNodeChildChild = new CSceneNode(10);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, 2.5, 4.3));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 10
	cSceneNodeChildChild = new CSceneNode(11);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, -2.5, -4.3));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 11
	cSceneNodeChildChild = new CSceneNode(12);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, -2.5, 4.3));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise 2nd gen child 12
	cSceneNodeChildChild = new CSceneNode(13);
	cSceneNodeChildChild->SetShader("Shader3D_Model");
	cSceneNodeChildChild->Init();
	cSceneNodeChildChild->SetLocalTranslateMtx(glm::vec3(0, 2.5, -4.3));
	cSceneNodeChildChild->SetPosition(glm::vec3(-10.f, fCheckHeight, 10.f));
	cSceneNodeChildChild->InitModels("Models/FerrisWheel/cabin_H.obj", "Models/FerrisWheel/cabin_H.tga",
									 "Models/FerrisWheel/cabin_M.obj", "Models/FerrisWheel/cabin_M.tga",
									 "Models/FerrisWheel/cabin_L.obj", "Models/FerrisWheel/cabin_L.tga");
	cSceneNodeChildChild->bContinuousUpdate = true;
	cSceneNodeChildChild->SetUpdateRotateMtx(glm::radians(-0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNodeChildChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f));
	cSceneNodeChildChild->bIsDisplayed = false;
	cSceneNodeChild->AddChild(cSceneNodeChildChild);
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNodeChildChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	CSolidObjectManager::GetInstance()->Add(cSceneNode);
	return true;
}

/**
 @brief PrintSelf
 */
void CFerrisWheel::PrintSelf(void)
{
	cout << "CFerrisWheel::PrintSelf()" << endl;
}
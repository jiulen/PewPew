/**
 CCarousel
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Carousel.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/SolidObjectManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CCarousel::CCarousel(void)
{
}

/**
 @brief Destructor
 */
CCarousel::~CCarousel(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CCarousel::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(0);
	cSceneNode->SetShader("Shader3D_Model");
	cSceneNode->Init();
	cSceneNode->sceneGraphType = CSceneNode::SG_Type::CAROUSEL;
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(0, 0);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(0, fCheckHeight, 0));
	cSceneNode->SetPosition(glm::vec3(0, fCheckHeight, 0));
	cSceneNode->InitModels("Models/Carousel/CarouselMain_H.obj", "Models/Carousel/CarouselMain_H.tga",
							"Models/Carousel/CarouselMain_M.obj", "Models/Carousel/CarouselMain_M.tga",
							"Models/Carousel/CarouselMain_L.obj", "Models/Carousel/CarouselMain_L.tga");
	cSceneNode->bContinuousUpdate = true;
	cSceneNode->SetUpdateRotateMtx(glm::radians(0.075f), glm::vec3(0, 1, 0));
	cSceneNode->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-2.5, -0.5, -2.5), glm::vec3(2.5, 5, 2.5));
	cSceneNode->SetType(CEntity3D::TYPE::STRUCTURE);
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.f;
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 25.f;
	cSceneNode->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 35.f;

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(1);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->sceneGraphType = CSceneNode::SG_Type::CAROUSEL;
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(0.75, 0.65, 1.5)); //height is 0.65 - 1.5
	cSceneNodeChild->SetPosition(glm::vec3(0, fCheckHeight, 0));
	cSceneNodeChild->InitModels("Models/Carousel/CarouselHorse_H.obj", "Models/Carousel/CarouselHorse_H.tga",
								"Models/Carousel/CarouselHorse_M.obj", "Models/Carousel/CarouselHorse_M.tga",
								"Models/Carousel/CarouselHorse_L.obj", "Models/Carousel/CarouselHorse_L.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateTranslateMtx(glm::vec3(0, 0.005, 0));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f));
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 10.f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 20.f;

	// Initialise a child CSceneNode
	cSceneNodeChild = new CSceneNode(2);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->sceneGraphType = CSceneNode::SG_Type::CAROUSEL;
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(-0.75, 1.216, -1.5)); //height is 0.65 - 1.5
	cSceneNodeChild->SetLocalRotateMtx(glm::pi<float>(), glm::vec3(0, 1, 0));
	cSceneNodeChild->SetPosition(glm::vec3(0, fCheckHeight, 0));
	cSceneNodeChild->InitModels("Models/Carousel/CarouselHorse_H.obj", "Models/Carousel/CarouselHorse_H.tga",
								"Models/Carousel/CarouselHorse_M.obj", "Models/Carousel/CarouselHorse_M.tga",
								"Models/Carousel/CarouselHorse_L.obj", "Models/Carousel/CarouselHorse_L.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateTranslateMtx(glm::vec3(0, 0.005, 0));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f));
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 10.f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 20.f;
	
	// Initialise a child CSceneNode
	cSceneNodeChild = new CSceneNode(3);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->sceneGraphType = CSceneNode::SG_Type::CAROUSEL;
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(-1.5, 1.5, 0.75)); //height is 0.65 - 1.5
	cSceneNodeChild->SetLocalRotateMtx(glm::pi<float>() / 2 * 3, glm::vec3(0, 1, 0));
	cSceneNodeChild->SetPosition(glm::vec3(0, fCheckHeight, 0));
	cSceneNodeChild->InitModels("Models/Carousel/CarouselHorse_H.obj", "Models/Carousel/CarouselHorse_H.tga",
								"Models/Carousel/CarouselHorse_M.obj", "Models/Carousel/CarouselHorse_M.tga",
								"Models/Carousel/CarouselHorse_L.obj", "Models/Carousel/CarouselHorse_L.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateTranslateMtx(glm::vec3(0, 0.005, 0));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f));
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 10.f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 20.f;

	// Initialise a child CSceneNode
	cSceneNodeChild = new CSceneNode(4);
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->Init();
	cSceneNodeChild->sceneGraphType = CSceneNode::SG_Type::CAROUSEL;
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(1.5, 0.933, -0.75)); //height is 0.65 - 1.5
	cSceneNodeChild->SetLocalRotateMtx(glm::pi<float>() / 2, glm::vec3(0, 1, 0));
	cSceneNodeChild->SetPosition(glm::vec3(0, fCheckHeight, 0));
	cSceneNodeChild->InitModels("Models/Carousel/CarouselHorse_H.obj", "Models/Carousel/CarouselHorse_H.tga",
								"Models/Carousel/CarouselHorse_M.obj", "Models/Carousel/CarouselHorse_M.tga",
								"Models/Carousel/CarouselHorse_L.obj", "Models/Carousel/CarouselHorse_L.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateTranslateMtx(glm::vec3(0, 0.005, 0));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f));
	cSceneNodeChild->bIsDisplayed = false;
	cSceneNode->AddChild(cSceneNodeChild);
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS] = 0.0f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS] = 10.f;
	cSceneNodeChild->arrLODDistance[CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS] = 20.f;

	CSolidObjectManager::GetInstance()->Add(cSceneNode);
	return true;
}

/**
 @brief PrintSelf
 */
void CCarousel::PrintSelf(void)
{
	cout << "CCarousel::PrintSelf()" << endl;
}
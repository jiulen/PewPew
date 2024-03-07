/**
 CScene3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Scene3D.h"

// Include GLEW
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

// Include filesystem to read from a file
#include "System\filesystem.h"

// Include CShaderManager
#include "RenderControl/ShaderManager.h"

#include "../GameStateManagement/GameStateManager.h"

#include "Entities/Hut_Concrete.h"
#include "Entities/TreeKabak3D.h"

#include "Entities/TreeCypress3D.h"
#include "Entities/TreeElm3D.h"
#include "Entities/TreeLaurel3D.h"
#include "Entities/TreeLobloly3D.h"
#include "Entities/TreeSpruce3D.h"

#include "Entities/Rock3D.h"
#include "Entities/Bush.h"

#include "SceneGraph/SpinTower.h"

#include "SceneGraph/Windmill.h"
#include "SceneGraph/TeacupRide.h"
#include "SceneGraph/FerrisWheel.h"
#include "SceneGraph/DropRide.h"
#include "SceneGraph/Carousel.h"
#include "SceneGraph/BouncyCastle.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: cSettings(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cFPSCounter(NULL)
	, cSoundController(NULL)
	, cCamera(NULL)
	, cSkyBox(NULL)
	, cSolidObjectManager(NULL)
	, cEntityManager(NULL)
	, cPlayer3D(NULL)
	, cProjectileManager(NULL)
	, cGUI_Scene3D(NULL)
	, scoped(false)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	// Destroy the cTerrain
	if (cTerrain)
	{
		cTerrain->Destroy();
		cTerrain = NULL;
	}

	// Destroy the camera
	if (cCamera)
	{
		cCamera->Destroy();
		cCamera = NULL;
	}

	// Destroy the cSoundController
	if (cSoundController)
	{
		// We won't delete this since it was created elsewhere
		cSoundController = NULL;
	}

	if (cSkyBox)
	{
		cSkyBox->Destroy();
		cSkyBox = NULL;
	}

	if (cPlayer3D)
	{
		cSolidObjectManager->Erase(cPlayer3D);
		cPlayer3D->Destroy();
		cPlayer3D = NULL;
	}

	if (cSolidObjectManager)
	{
		cSolidObjectManager->Destroy();
		cSolidObjectManager = NULL;
	}

	if (cProjectileManager)
	{
		cProjectileManager->Destroy();
		cProjectileManager = NULL;
	}

	if (cEntityManager)
	{
		cEntityManager->Destroy();
		cEntityManager = NULL;
	}

	if (cGUI_Scene3D)
	{
		cGUI_Scene3D->Destroy();
		cGUI_Scene3D = NULL;
	}

	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	cMouseController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;
	
	// We won't delete this since it was created elsewhere
	cSettings = NULL;
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CScene3D::Init(void)
{
	cSettings = CSettings::GetInstance();

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Configure the camera
	cCamera = CCamera::GetInstance();
	cCamera->vec3Position = glm::vec3(0.0f, 0.5f, 3.0f);

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	cMouseController = CMouseController::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\PistolShoot.ogg"), 1, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\AKShoot.ogg"), 2, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\SniperShoot.ogg"), 3, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\ShotgunShoot.ogg"), 4, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\PistolReload.ogg"), 5, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\AKReload.ogg"), 6, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\SniperReload.ogg"), 7, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\ShotgunReload.ogg"), 8, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\pop.ogg"), 9, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\hurt.ogg"), 10, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\walking.ogg"), 11, true, false, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\running.ogg"), 12, true, false, true);

	// Load the Environment Entities
	// Load the SkyBox
	cSkyBox = CSkyBox::GetInstance();
	// Set a shader to this class instance of CSkyBox
	cSkyBox->SetShader("Shader3D_SkyBox");
	cSkyBox->Init();

	// Load the Terrain
	cTerrain = CTerrain::GetInstance();
	cTerrain->SetShader("Shader3D_Terrain");
	cTerrain->Init("Image/Terrain/World/terrain.bmp");
	// Set the size of the Terrain
	cTerrain->SetRenderSize(100.0f, 5.0f, 100.0f);

	// Load the movable entities
	// Initialise the cSolidObjectManager
	cSolidObjectManager = CSolidObjectManager::GetInstance();
	cSolidObjectManager->Init();

	cEntityManager = CEntityManager::GetInstance();
	cEntityManager->Init();

	// Initialise the CPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();
	cPlayer3D->SetPosition(glm::vec3(-32.5f, 0.5f, 12.5f));
	cPlayer3D->SetShader("Shader3D");
	cPlayer3D->SetTerrain(cTerrain);
	cPlayer3D->Init();
	cPlayer3D->InitCollider("Shader3D_Line", glm::vec4(0.f, 1.f, 0.f, 1.f));
	cPlayer3D->AttachCamera(cCamera);
	// Assign a cSniper to the cPlayer3D
	CSniper* cSniper = new CSniper();
	// Set the position, rotation and scale of the weapon
	cSniper->SetPosition(glm::vec3(1.f, -1.f, -5.f));
	cSniper->SetRotation(glm::pi<float>(), glm::vec3(0.f, 1.f, 0.f));
	cSniper->SetScale(glm::vec3(10.f, 10.f, 10.f));
	// Initialise the instance
	cSniper->Init();
	cSniper->SetShader("Shader3D_Model");
	cPlayer3D->SetWeapon(cSniper);
	// Assign a cShotgun to the cPlayer3D
	CShotgun* cShotgun = new CShotgun();
	// Set the position, rotation and scale of the weapon
	cShotgun->SetPosition(glm::vec3(1.5f, -2.f, -4.5f));
	cShotgun->SetRotation(glm::pi<float>() / 4 * 2.25f, glm::vec3(0.f, 1.f, 0.f));
	cShotgun->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	// Initialise the instance
	cShotgun->Init();
	cShotgun->SetShader("Shader3D_Model");
	cPlayer3D->SetWeapon(cShotgun);
	// Assign a cAK47 to the cPlayer3D
	CAK47* cAK47 = new CAK47();
	// Set the position, rotation and scale of the weapon
	cAK47->SetPosition(glm::vec3(2.f, -2.f, -7.f));
	cAK47->SetRotation(glm::pi<float>(), glm::vec3(0.f, 1.f, 0.f));
	cAK47->SetScale(glm::vec3(20.f, 20.f, 20.f));
	// Initialise the instance
	cAK47->Init();
	cAK47->SetShader("Shader3D_Model");
	cPlayer3D->SetWeapon(cAK47);
	// Add the cPlayer3D to the cSolidObjectManager
	cSolidObjectManager->Add(cPlayer3D);

	// Init enemies
	// 8 enemies surrounding the exterior
	// Initialise enemy 1
	float fCheckHeight = cTerrain->GetHeight(-13.5f, -12.5f);
	CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(-13.5f, fCheckHeight, -12.5f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 1
	cEnemy3D->AddEnemyWaypoint(-13.5f, -12.5f);
	cEnemy3D->AddEnemyWaypoint(0.25f, -12.75f);
	cEnemy3D->AddEnemyWaypoint(14.f, -13.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 1.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 15.f);
	cEnemy3D->AddEnemyWaypoint(-0.5f, 15.5f);
	cEnemy3D->AddEnemyWaypoint(-15.f, 16.f);
	cEnemy3D->AddEnemyWaypoint(-14.25f, 1.75f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 1
	CPistol* cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 1
	CAK47* cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise enemy 2
	fCheckHeight = cTerrain->GetHeight(14.f, -13.f);
	cEnemy3D = new CEnemy3D(glm::vec3(14.f, fCheckHeight, -13.f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 2
	cEnemy3D->AddEnemyWaypoint(-13.5f, -12.5f);
	cEnemy3D->AddEnemyWaypoint(0.25f, -12.75f);
	cEnemy3D->AddEnemyWaypoint(14.f, -13.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 1.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 15.f);
	cEnemy3D->AddEnemyWaypoint(-0.5f, 15.5f);
	cEnemy3D->AddEnemyWaypoint(-15.f, 16.f);
	cEnemy3D->AddEnemyWaypoint(-14.25f, 1.75f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 2
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 2
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise enemy 3
	fCheckHeight = cTerrain->GetHeight(14.f, 15.f);
	cEnemy3D = new CEnemy3D(glm::vec3(14.f, fCheckHeight, 15.f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 3
	cEnemy3D->AddEnemyWaypoint(-13.5f, -12.5f);
	cEnemy3D->AddEnemyWaypoint(0.25f, -12.75f);
	cEnemy3D->AddEnemyWaypoint(14.f, -13.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 1.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 15.f);
	cEnemy3D->AddEnemyWaypoint(-0.5f, 15.5f);
	cEnemy3D->AddEnemyWaypoint(-15.f, 16.f);
	cEnemy3D->AddEnemyWaypoint(-14.25f, 1.75f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 3
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 3
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise enemy 4
	fCheckHeight = cTerrain->GetHeight(-15.f, 16.f);
	cEnemy3D = new CEnemy3D(glm::vec3(-15.f, fCheckHeight, 16.f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 4
	cEnemy3D->AddEnemyWaypoint(-13.5f, -12.5f);
	cEnemy3D->AddEnemyWaypoint(0.25f, -12.75f);
	cEnemy3D->AddEnemyWaypoint(14.f, -13.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 1.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 15.f);
	cEnemy3D->AddEnemyWaypoint(-0.5f, 15.5f);
	cEnemy3D->AddEnemyWaypoint(-15.f, 16.f);
	cEnemy3D->AddEnemyWaypoint(-14.25f, 1.75f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 4
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 4
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise enemy 5
	fCheckHeight = cTerrain->GetHeight(0.25f, -12.75f);
	cEnemy3D = new CEnemy3D(glm::vec3(0.25f, fCheckHeight, -12.75f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 5
	cEnemy3D->AddEnemyWaypoint(-13.5f, -12.5f);
	cEnemy3D->AddEnemyWaypoint(0.25f, -12.75f);
	cEnemy3D->AddEnemyWaypoint(14.f, -13.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 1.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 15.f);
	cEnemy3D->AddEnemyWaypoint(-0.5f, 15.5f);
	cEnemy3D->AddEnemyWaypoint(-15.f, 16.f);
	cEnemy3D->AddEnemyWaypoint(-14.25f, 1.75f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 5
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 5
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise enemy 6
	fCheckHeight = cTerrain->GetHeight(14.f, 1.f);
	cEnemy3D = new CEnemy3D(glm::vec3(14.f, fCheckHeight, 1.f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 6
	cEnemy3D->AddEnemyWaypoint(-13.5f, -12.5f);
	cEnemy3D->AddEnemyWaypoint(0.25f, -12.75f);
	cEnemy3D->AddEnemyWaypoint(14.f, -13.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 1.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 15.f);
	cEnemy3D->AddEnemyWaypoint(-0.5f, 15.5f);
	cEnemy3D->AddEnemyWaypoint(-15.f, 16.f);
	cEnemy3D->AddEnemyWaypoint(-14.25f, 1.75f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 6
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 6
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise enemy 7
	fCheckHeight = cTerrain->GetHeight(-0.5f, 15.5f);
	cEnemy3D = new CEnemy3D(glm::vec3(-0.5f, fCheckHeight, 15.5f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 7
	cEnemy3D->AddEnemyWaypoint(-13.5f, -12.5f);
	cEnemy3D->AddEnemyWaypoint(0.25f, -12.75f);
	cEnemy3D->AddEnemyWaypoint(14.f, -13.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 1.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 15.f);
	cEnemy3D->AddEnemyWaypoint(-0.5f, 15.5f);
	cEnemy3D->AddEnemyWaypoint(-15.f, 16.f);
	cEnemy3D->AddEnemyWaypoint(-14.25f, 1.75f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 7
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 7
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise enemy 8
	fCheckHeight = cTerrain->GetHeight(-14.25f, 1.75f);
	cEnemy3D = new CEnemy3D(glm::vec3(-14.25f, fCheckHeight, 1.75f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 8
	cEnemy3D->AddEnemyWaypoint(-13.5f, -12.5f);
	cEnemy3D->AddEnemyWaypoint(0.25f, -12.75f);
	cEnemy3D->AddEnemyWaypoint(14.f, -13.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 1.f);
	cEnemy3D->AddEnemyWaypoint(14.f, 15.f);
	cEnemy3D->AddEnemyWaypoint(-0.5f, 15.5f);
	cEnemy3D->AddEnemyWaypoint(-15.f, 16.f);
	cEnemy3D->AddEnemyWaypoint(-14.25f, 1.75f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 8
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 8
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// 2 enemies surrounding the carousel
	// Initialise enemy 1
	fCheckHeight = cTerrain->GetHeight(-3.75f, -3.75f);
	cEnemy3D = new CEnemy3D(glm::vec3(-3.75f, fCheckHeight, -3.75f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 1
	cEnemy3D->AddEnemyWaypoint(-3.75f, -3.75f);
	cEnemy3D->AddEnemyWaypoint(3.75f, -3.75f);
	cEnemy3D->AddEnemyWaypoint(3.75f, 3.75f);
	cEnemy3D->AddEnemyWaypoint(-3.75f, 3.75f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 1
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 1
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise enemy 2
	fCheckHeight = cTerrain->GetHeight(3.75f, 3.75f);
	cEnemy3D = new CEnemy3D(glm::vec3(3.75f, fCheckHeight, 3.75f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 2
	cEnemy3D->AddEnemyWaypoint(-3.75f, -3.75f);
	cEnemy3D->AddEnemyWaypoint(3.75f, -3.75f);
	cEnemy3D->AddEnemyWaypoint(3.75f, 3.75f);
	cEnemy3D->AddEnemyWaypoint(-3.75f, 3.75f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 2
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 2
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// 4 enemies moving from the carousel to outside and back in
	// Initialise enemy 1
	fCheckHeight = cTerrain->GetHeight(-10.f, 3.f);
	cEnemy3D = new CEnemy3D(glm::vec3(-10.f, fCheckHeight, 3.f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 1
	cEnemy3D->AddEnemyWaypoint(-10.f, 3.f);
	cEnemy3D->AddEnemyWaypoint(-5.f, 0.f);
	cEnemy3D->AddEnemyWaypoint(-10.f, -7.f);
	cEnemy3D->AddEnemyWaypoint(-5.f, 0.f);
	cEnemy3D->AddEnemyWaypoint(-10.f, 3.f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 1
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 1
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise enemy 2
	fCheckHeight = cTerrain->GetHeight(9.5f, -7.5f);
	cEnemy3D = new CEnemy3D(glm::vec3(9.5f, fCheckHeight, -7.5f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 2
	cEnemy3D->AddEnemyWaypoint(9.5f, -7.5f);
	cEnemy3D->AddEnemyWaypoint(5.f, 0.f);
	cEnemy3D->AddEnemyWaypoint(11.5f, 5.5f);
	cEnemy3D->AddEnemyWaypoint(5.f, 0.f);
	cEnemy3D->AddEnemyWaypoint(9.5f, -7.5f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 2
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 2
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);
	
	// Initialise enemy 3
	fCheckHeight = cTerrain->GetHeight(-7.5f, -10.f);
	cEnemy3D = new CEnemy3D(glm::vec3(-7.5f, fCheckHeight, -10.f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 3
	cEnemy3D->AddEnemyWaypoint(-7.5f, -10.f);
	cEnemy3D->AddEnemyWaypoint(0.f, -5.f);
	cEnemy3D->AddEnemyWaypoint(7.5f, -9.5f);
	cEnemy3D->AddEnemyWaypoint(0.f, -5.f);
	cEnemy3D->AddEnemyWaypoint(-7.5f, -10.f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 3
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 3
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise enemy 4
	fCheckHeight = cTerrain->GetHeight(-7.f, 14.5f);
	cEnemy3D = new CEnemy3D(glm::vec3(-7.f, fCheckHeight, 14.5f));
	cEnemy3D->SetTerrain(cTerrain);
	cEnemy3D->SetShader("Shader3D_NoColour");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec3(-0.15f, -0.5f, -0.15f), glm::vec3(0.15f, 0.5f, 0.15f));
	cEnemy3D->fear = 1;
	// Set waypoints for enemy 4
	cEnemy3D->AddEnemyWaypoint(-7.f, 14.5f);
	cEnemy3D->AddEnemyWaypoint(0.f, 5.f);
	cEnemy3D->AddEnemyWaypoint(5.f, 11.f);
	cEnemy3D->AddEnemyWaypoint(0.f, 5.f);
	cEnemy3D->AddEnemyWaypoint(-7.f, 14.5f);
	cEnemy3D->FinishAddWaypoints();
	// Assign a cPistol to enemy 4
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(1, cEnemyPistol);
	// Assign a cAK47 to the enemy 4
	cEnemyAK = new CAK47();
	cEnemyAK->SetRotation(glm::half_pi<float>(), glm::vec3(0, 1, 0));
	cEnemyAK->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	cEnemyAK->Init();
	cEnemyAK->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyAK);
	// Add enemy to SolidObjManager
	cSolidObjectManager->Add(cEnemy3D);

	// Init structures
	// Init a CHut_Concrete
	fCheckHeight = cTerrain->GetHeight(-27.0f, 10.0f);
	CHut_Concrete* cHut_Concrete = new CHut_Concrete(glm::vec3(-27.0f, fCheckHeight, 10.0f));
	cHut_Concrete->SetShader("Shader3D_NoColour");
	cHut_Concrete->SetLODStatus(true);
	cHut_Concrete->Init();
	cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// Add the cHut_Concrete to the cSolidObjectManager
	cSolidObjectManager->Add(cHut_Concrete);

	fCheckHeight = cTerrain->GetHeight(-27.0f, 12.5f);
	cHut_Concrete = new CHut_Concrete(glm::vec3(-27.0f, fCheckHeight, 12.5f));
	cHut_Concrete->SetShader("Shader3D_NoColour");
	cHut_Concrete->SetLODStatus(true);
	cHut_Concrete->Init();
	cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// Add the cHut_Concrete to the cSolidObjectManager
	cSolidObjectManager->Add(cHut_Concrete);

	fCheckHeight = cTerrain->GetHeight(-27.0f, 15.0f);
	cHut_Concrete = new CHut_Concrete(glm::vec3(-27.0f, fCheckHeight, 15.0f));
	cHut_Concrete->SetShader("Shader3D_NoColour");
	cHut_Concrete->SetLODStatus(true);
	cHut_Concrete->Init();
	cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// Add the cHut_Concrete to the cSolidObjectManager
	cSolidObjectManager->Add(cHut_Concrete);

	fCheckHeight = cTerrain->GetHeight(-30.0f, 10.0f);
	cHut_Concrete = new CHut_Concrete(glm::vec3(-30.0f, fCheckHeight, 10.0f));
	cHut_Concrete->SetShader("Shader3D_NoColour");
	cHut_Concrete->SetLODStatus(true);
	cHut_Concrete->Init();
	cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	cHut_Concrete->SetRotation(180, glm::vec3(0, 1, 0));

	// Add the cHut_Concrete to the cSolidObjectManager
	cSolidObjectManager->Add(cHut_Concrete);

	fCheckHeight = cTerrain->GetHeight(-30.0f, 12.5f);
	cHut_Concrete = new CHut_Concrete(glm::vec3(-30.0f, fCheckHeight, 12.5f));
	cHut_Concrete->SetShader("Shader3D_NoColour");
	cHut_Concrete->SetLODStatus(true);
	cHut_Concrete->Init();
	cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	cHut_Concrete->SetRotation(180, glm::vec3(0, 1, 0));

	// Add the cHut_Concrete to the cSolidObjectManager
	cSolidObjectManager->Add(cHut_Concrete);

	fCheckHeight = cTerrain->GetHeight(-30.0f, 15.0f);
	cHut_Concrete = new CHut_Concrete(glm::vec3(-30.0f, fCheckHeight, 15.0f));
	cHut_Concrete->SetShader("Shader3D_NoColour");
	cHut_Concrete->SetLODStatus(true);
	cHut_Concrete->Init();
	cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	cHut_Concrete->SetRotation(180, glm::vec3(0, 1, 0));

	// Add the cHut_Concrete to the cSolidObjectManager
	cSolidObjectManager->Add(cHut_Concrete);

	// Init a CTreeCypress3D
	fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	CTreeCypress3D* cTreeCypress3D = new CTreeCypress3D(glm::vec3(-2.0f, fCheckHeight, 2.0f));
	cTreeCypress3D->SetInstancingMode(true);
	if (cTreeCypress3D->IsInstancedRendering() == true)
	{
		cTreeCypress3D->SetScale(glm::vec3(25.f));
		cTreeCypress3D->SetNumOfInstance(40);
		cTreeCypress3D->SetSpreadDistance(100.f);

		cTreeCypress3D->SetShader("Shader3D_Instancing");
	}
	if (cTreeCypress3D->Init() == true)
	{
		cEntityManager->Add(cTreeCypress3D);
	}
	else
	{
		delete cTreeCypress3D;
	}

	// Init a CTreeElm3D
	fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	CTreeElm3D* cTreeElm3D = new CTreeElm3D(glm::vec3(-2.0f, fCheckHeight, 2.0f));
	cTreeElm3D->SetInstancingMode(true);
	if (cTreeElm3D->IsInstancedRendering() == true)
	{
		cTreeElm3D->SetScale(glm::vec3(25.f));
		cTreeElm3D->SetNumOfInstance(40);
		cTreeElm3D->SetSpreadDistance(100.f);

		cTreeElm3D->SetShader("Shader3D_Instancing");
	}
	if (cTreeElm3D->Init() == true)
	{
		cEntityManager->Add(cTreeElm3D);
	}
	else
	{
		delete cTreeElm3D;
	}

	// Init a CTreeLaurel3D
	fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	CTreeLaurel3D* cTreeLaurel3D = new CTreeLaurel3D(glm::vec3(-2.0f, fCheckHeight, 2.0f));
	cTreeLaurel3D->SetInstancingMode(true);
	if (cTreeLaurel3D->IsInstancedRendering() == true)
	{
		cTreeLaurel3D->SetScale(glm::vec3(25.f));
		cTreeLaurel3D->SetNumOfInstance(40);
		cTreeLaurel3D->SetSpreadDistance(100.f);

		cTreeLaurel3D->SetShader("Shader3D_Instancing");
	}
	if (cTreeLaurel3D->Init() == true)
	{
		cEntityManager->Add(cTreeLaurel3D);
	}
	else
	{
		delete cTreeLaurel3D;
	}

	// Init a CTreeLobloly3D
	fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	CTreeLobloly3D* cTreeLobloly3D = new CTreeLobloly3D(glm::vec3(-2.0f, fCheckHeight, 2.0f));
	cTreeLobloly3D->SetInstancingMode(true);
	if (cTreeLobloly3D->IsInstancedRendering() == true)
	{
		cTreeLobloly3D->SetScale(glm::vec3(25.f));
		cTreeLobloly3D->SetNumOfInstance(40);
		cTreeLobloly3D->SetSpreadDistance(100.f);

		cTreeLobloly3D->SetShader("Shader3D_Instancing");
	}
	if (cTreeLobloly3D->Init() == true)
	{
		cEntityManager->Add(cTreeLobloly3D);
	}
	else
	{
		delete cTreeLobloly3D;
	}

	// Init a CTreeSpruce3D
	fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	CTreeSpruce3D* cTreeSpruce3D = new CTreeSpruce3D(glm::vec3(-2.0f, fCheckHeight, 2.0f));
	cTreeSpruce3D->SetInstancingMode(true);
	if (cTreeSpruce3D->IsInstancedRendering() == true)
	{
		cTreeSpruce3D->SetScale(glm::vec3(25.f));
		cTreeSpruce3D->SetNumOfInstance(40);
		cTreeSpruce3D->SetSpreadDistance(100.f);

		cTreeSpruce3D->SetShader("Shader3D_Instancing");
	}
	if (cTreeSpruce3D->Init() == true)
	{
		cEntityManager->Add(cTreeSpruce3D);
	}
	else
	{
		delete cTreeSpruce3D;
	}

	// Init a CRock3D
	CRock3D* cRock3D = new CRock3D();
	cRock3D->SetInstancingMode(true);
	if (cRock3D->IsInstancedRendering() == true)
	{
		cRock3D->SetScale(glm::vec3(2.f));
		cRock3D->SetNumOfInstance(150);
		cRock3D->SetSpreadDistance(100.f);

		cRock3D->SetShader("Shader3D_Instancing");
	}
	if (cRock3D->Init() == true)
	{
		cEntityManager->Add(cRock3D);
	}
	else
	{
		delete cRock3D;
	}

	// Init a CBush
	CBush* cBush = new CBush();
	cBush->SetInstancingMode(true);
	if (cBush->IsInstancedRendering() == true)
	{
		cBush->SetScale(glm::vec3(20.f));
		cBush->SetNumOfInstance(150);
		cBush->SetSpreadDistance(100.f);

		cBush->SetShader("Shader3D_Instancing");
	}
	if (cBush->Init() == true)
	{
		cEntityManager->Add(cBush);
	}
	else
	{
		delete cBush;
	}

	CWindmill::Create();
	CTeacupRide::Create();
	CFerrisWheel::Create();
	CDropRide::Create();
	CCarousel::Create();
	CBouncyCastle::Create();

	cProjectileManager = CProjectileManager::GetInstance();
	cProjectileManager->Init();
	cProjectileManager->SetShader("Shader3D");

	// Load GUI entities
	// Store the cGUI_Scene3D singleton instance here
	cGUI_Scene3D = CGUI_Scene3D::GetInstance();
	cGUI_Scene3D->Init();

	//Start with camera effects
	CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
	CCameraEffectsManager::GetInstance()->Get("CameraShake")->SetStatus(true);

	// Set a shader to this class instance of Minimap
	cMinimap = CMinimap::GetInstance();
	cMinimap->SetShader("Shader3D_RenderToTexture");
	cMinimap->Init();

	scoped = false;
	fullMinimap = false;

	return true;
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
*/
bool CScene3D::Update(const double dElapsedTime)
{
	//Conditions for crosshair
	if (cPlayer3D->GetWeapon() != nullptr)
	{
		switch (cPlayer3D->GetWeapon()->crosshairType)
		{
		case CWeaponInfo::CROSSHAIR::NONE:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::STANDARD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::SPREAD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(true);
			break;
		}
	}

	// Store the current position, if rollback is needed
	cPlayer3D->StorePositionForRollback();

	cPlayer3D->isMoving = false;
	// Get keyboard updates for player3D
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
		cPlayer3D->isMoving = true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
		cPlayer3D->isMoving = true;
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
		cPlayer3D->isMoving = true;
	}		
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
		cPlayer3D->isMoving = true;
	}
		
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE))
	{
		cPlayer3D->SetToJump();
	}		
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		cPlayer3D->sprinting = true;
	}
	else
	{
		cPlayer3D->sprinting = false;
	}
	static bool isPosturePressed = false;
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && !isPosturePressed)
	{
		cPlayer3D->TogglePosture();
		isPosturePressed = true;
	}
	else if (!CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && isPosturePressed)
	{
		isPosturePressed = false;
	}
	switch (cPlayer3D->iCurrentPosture)
	{
	case CPlayer3D::PLAYERPOSTURE::STANDING:
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->vec2Magnitude = glm::vec2(0.05f, 0.05f);
		break;
	case CPlayer3D::PLAYERPOSTURE::CROUCH:
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->vec2Magnitude = glm::vec2(0.025f, 0.025f);
		break;
	case CPlayer3D::PLAYERPOSTURE::PRONE:
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->vec2Magnitude = glm::vec2(0.01f, 0.01f);
		break;
	}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_1))
	{
		cPlayer3D->SetCurrentWeaponIndex(0);
		//Conditions for crosshair
		switch (cPlayer3D->GetWeapon()->crosshairType)
		{
		case CWeaponInfo::CROSSHAIR::NONE:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::STANDARD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::SPREAD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(true);
			break;
		}		
	}		
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_2))
	{
		cPlayer3D->SetCurrentWeaponIndex(1);
		//Conditions for crosshair
		switch (cPlayer3D->GetWeapon()->crosshairType)
		{
		case CWeaponInfo::CROSSHAIR::NONE:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::STANDARD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::SPREAD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(true);
			break;
		}
	}		
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_3))
	{
		cPlayer3D->SetCurrentWeaponIndex(2);
		//Conditions for crosshair
		switch (cPlayer3D->GetWeapon()->crosshairType)
		{
		case CWeaponInfo::CROSSHAIR::NONE:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::STANDARD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::SPREAD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(true);
			break;
		}
	}		

	if (cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET) > 0.0)
	{
		cPlayer3D->IncreaseCurrentWeaponIndex();
		//Conditions for crosshair
		switch (cPlayer3D->GetWeapon()->crosshairType)
		{
		case CWeaponInfo::CROSSHAIR::NONE:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::STANDARD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::SPREAD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(true);
			break;
		}
	}		
	else if (cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET) < 0.0)
	{
		cPlayer3D->DecreaseCurrentWeaponIndex();
		//Conditions for crosshair
		switch (cPlayer3D->GetWeapon()->crosshairType)
		{
		case CWeaponInfo::CROSSHAIR::NONE:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::STANDARD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(false);
			break;
		case CWeaponInfo::CROSSHAIR::SPREAD:
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHair2")->SetStatus(true);
			break;
		}
	}
		

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_R))
	{
		if (cPlayer3D->GetWeapon()->Reload())
			cSoundController->PlaySoundByID(cPlayer3D->GetWeapon()->reloadSoundID);
	}
		

	static bool lmbPressed = false;

	if (cPlayer3D->GetWeapon() != nullptr)
	{
		if (cPlayer3D->GetWeapon()->automatic)
		{
			if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB))
			{
				if (cPlayer3D->DischargeWeapon())
				{
					cSoundController->PlaySoundByID(cPlayer3D->GetWeapon()->shootSoundID);					
				}					
				lmbPressed = true;
			}
			else if (!cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB) && lmbPressed)
				lmbPressed = false;
				
		}
		else
		{
			if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB) && !lmbPressed)
			{
				if (cPlayer3D->DischargeWeapon())
				{
					cSoundController->PlaySoundByID(cPlayer3D->GetWeapon()->shootSoundID);
				}					
				lmbPressed = true;
			}
			else if (!cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB) && lmbPressed)
				lmbPressed = false;				
		}
	}
	else
		lmbPressed = false;

	// Get keyboard and mouse updates for camera
	if (!cPlayer3D->IsCameraAttached())
	{
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_I))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::FORWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_K))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::BACKWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_J))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::LEFT, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_L))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::RIGHT, (float)dElapsedTime);
		// Get mouse updates
		if (scoped)
		{
			cCamera->ProcessMouseMovement((float)cMouseController->GetMouseDeltaX() * 0.5f,
				(float)cMouseController->GetMouseDeltaY() * 0.5f);
		}
		else
		{
			cCamera->ProcessMouseMovement((float)cMouseController->GetMouseDeltaX(),
				(float)cMouseController->GetMouseDeltaY());
		}
		
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController
			::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}
	else
	{
		// Get mouse updates
		cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX(),
			(float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController
			::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}

	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_0))
	{
		if (cPlayer3D->IsCameraAttached())
			cPlayer3D->AttachCamera();
		else
			cPlayer3D->AttachCamera(cCamera);
	}

	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_LEFT_BRACKET))
	{
		cSkyBox->SetCurrentTextureIndex(cSkyBox->GetCurrentTextureIndex() - 1);
	}
	else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_RIGHT_BRACKET))
	{
		cSkyBox->SetCurrentTextureIndex(cSkyBox->GetCurrentTextureIndex() + 1);
	}

	//Scoping
	if (cPlayer3D->GetWeapon() != nullptr)
	{
		if (cPlayer3D->GetWeapon()->canScope && //check if can scope
		   (cPlayer3D->GetWeapon()->GetCanFire() || cPlayer3D->GetWeapon()->GetMagRound() < cPlayer3D->GetWeapon()->GetMaxMagRound())) //check if reloading
		{
			if (cMouseController->IsButtonPressed(CMouseController::RMB))
			{
				// Switch on Scope mode and zoom in
				cCamera->fZoom = 15.0f;
				scoped = true;
				cGUI_Scene3D->scoped = true;
				CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(true);
			}
			else if (cMouseController->IsButtonReleased(CMouseController::RMB))
			{
				// Switch off Scope mode and zoom out
				cCamera->fZoom = 45.0f;
				scoped = false;
				cGUI_Scene3D->scoped = false;
				CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(false);
			}
		}
		else if (CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->GetStatus())
		{
			cCamera->fZoom = 45.0f;
			scoped = false;
			cGUI_Scene3D->scoped = false;
			CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(false);
		}
	}
	
	// View full map
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_TAB))
	{
		fullMinimap = true;
		cMinimap->fullMinimap = true;
		cGUI_Scene3D->fullMinimap = true;
	}
	else
	{
		fullMinimap = false;
		cMinimap->fullMinimap = false;
		cGUI_Scene3D->fullMinimap = false;
	}

	cSolidObjectManager->Update(dElapsedTime);

	cProjectileManager->Update(dElapsedTime);

	cEntityManager->Update(dElapsedTime);

	cSolidObjectManager->CheckForCollision();

	// Post Update the mouse controller
	cMouseController->PostUpdate();

	// Call the cGUI_Scene3D's update method
	cGUI_Scene3D->Update(dElapsedTime);

	cCamera->ProcessMouseMovement(0, 0, false);

	cPlayer3D->enemiesLeft = 14 - cSolidObjectManager->enemiesKilled;
	// Win condition
	if (cSolidObjectManager->enemiesKilled >= 14)
		CGameStateManager::GetInstance()->SetActiveGameState("WinState");

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene3D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 @brief Render Render this instance
 */
void CScene3D::Render(void)
{
	// Part 1: Render for the minimap by binding to framebuffer and render to color texture
	//         But the camera is move to top-view of the scene

	// Backup some key settings for the camera and player
	glm::vec3 storePlayerPosition = cPlayer3D->GetPosition();
	float storeCameraYaw = cCamera->fYaw;
	float storeCameraPitch = cCamera->fPitch;
	glm::vec3 storeCameraPosition = cCamera->vec3Position;
	// Adjust camera yaw and pitch so that it is looking from a top-view of the terrain
	cCamera->fYaw += 180.f;
	cCamera->fPitch = -90.f;
	// We store the player's position into the camera as we want the minimap to focus on the player
	float cameraX, cameraY, cameraZ;
	if (fullMinimap)
	{
		cameraX = 0;
		cameraY = 130.f;
		cameraZ = 0;
	}
	else
	{
		cameraX = storeCameraPosition.x;
		cameraY = 40.f;
		cameraZ = storeCameraPosition.z;
	}
	cCamera->vec3Position = glm::vec3(cameraX, cameraY, cameraZ);
	// Recalculate all the camera vectors
	// We disable pitch constraints for this specific case as we want the camera to look straight down
	cCamera->ProcessMouseMovement(0, 0, false);
	glm::mat4 playerView;
	if (fullMinimap)
	{
		playerView = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), 
								 glm::vec3(cameraX, cameraY, cameraZ) + glm::vec3(0, -1, 0), 
								 glm::vec3(1, 0, 0));
	}
	else
	{
		playerView = cCamera->GetViewMatrix();
	}
	
	
	glm::mat4 playerProjection = glm::perspective(	glm::radians(45.f),
													(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
													0.1f, 1000.f);
	// Set the camera parameters back to the previous values
	cCamera->fYaw = storeCameraYaw;
	cCamera->fPitch = storeCameraPitch;
	cCamera->vec3Position = storeCameraPosition;
	cCamera->ProcessMouseMovement(0, 0, true);

	cMinimap->Activate();
	cMinimap->PreRender();

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// Render the terrain
	cTerrain->SetView(playerView);
	cTerrain->SetProjection(playerProjection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	// Render the entities
	cEntityManager->SetView(playerView);
	cEntityManager->SetProjection(playerProjection);
	cEntityManager->Render();

	glDisable(GL_DEPTH_TEST);

	std::list<CSolidObject*>::iterator it, end;
	end = cSolidObjectManager->lSolidObject.end();
	for (it = cSolidObjectManager->lSolidObject.begin(); it != end; ++it)
	{
		if ((*it)->iTextureID_Icon > 0)
			(*it)->renderIcon = true;
	}
	// Render the entities for the minimap
	cSolidObjectManager->SetView(playerView);
	cSolidObjectManager->SetProjection(playerProjection);
	cSolidObjectManager->Render();
	std::list<CSolidObject*>::iterator it2, end2;
	end2 = cSolidObjectManager->lSolidObject.end();
	for (it2 = cSolidObjectManager->lSolidObject.begin(); it2 != end2; ++it2)
	{
		if ((*it2)->iTextureID_Icon > 0)
			(*it2)->renderIcon = false;
	}
	glEnable(GL_DEPTH_TEST);

	cMinimap->Deactivate();

	// Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(	glm::radians(CCamera::GetInstance()->fZoom),
												(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
												0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the skybox
	cSkyBox->SetView(view);
	cSkyBox->SetProjection(projection);
	cSkyBox->PreRender();
	cSkyBox->Render();
	cSkyBox->PostRender();

	// Render the terrain
	cTerrain->SetView(view);
	cTerrain->SetProjection(projection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	// Render the entities
	cEntityManager->SetView(view);
	cEntityManager->SetProjection(projection);
	cEntityManager->Render();

	// Render the solid objects
	cSolidObjectManager->SetView(view);
	cSolidObjectManager->SetProjection(projection);
	cSolidObjectManager->Render();

	// Render the projectiles
	cProjectileManager->SetView(view);
	cProjectileManager->SetProjection(projection);
	cProjectileManager->PreRender();
	cProjectileManager->Render();
	cProjectileManager->PostRender();

	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	cGUI_Scene3D->SetProjection(projection);
	cGUI_Scene3D->PreRender();
	cGUI_Scene3D->Render();
	cGUI_Scene3D->PostRender();

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}
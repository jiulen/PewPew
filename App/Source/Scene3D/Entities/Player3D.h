/**
 CPlayer3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include CSolidObject
#include "SolidObject.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Camera
#include "../Camera.h"

// Include CTerrain
#include "../Terrain/Terrain.h"

// Include WeaponInfo
#include "../WeaponInfo/WeaponInfo.h"

// Include Physics3D
#include "../Physics3D.h"

// Include InventoryManager
#include "../../Scene2D/InventoryManager.h"

#include <string>
using namespace std;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CPlayer3D : public CSingletonTemplate<CPlayer3D>, public CSolidObject
{
	friend class CSingletonTemplate<CPlayer3D>;

public:
	// Defines several possible options for player movement.
	enum class PLAYERMOVEMENT : unsigned int 
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
		NUM_MOVEMENTS
	};

	// Defines several possible options for player posture. 
	enum class PLAYERPOSTURE : unsigned int
	{
		STANDING = 0,
		CROUCH,
		PRONE,
		NUM_MOVEMENTS
	};
	PLAYERPOSTURE iPreviousPosture, iCurrentPosture, iNextPosture;
	float fPostureOffset;

	// Player Attributes
	glm::vec3 vec3Up;
	glm::vec3 vec3Right;
	glm::vec3 vec3WorldUp;
	// Euler Angles
	float fYaw;
	float fPitch;
	// Player options
	float fMouseSensitivity;

	// Initialise this class instance
	bool Init(void);

	// Set the pointer of the terrain to this player3D class
	void SetTerrain(CTerrain* theNewTerrain);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix(void) const;

	// Attach a camera to this class instance
	void AttachCamera(CCamera* cCamera = NULL);
	// Check if a camera ia attached to this class instance
	bool IsCameraAttached(void);

	// Set Weapon to this class instance
	bool SetWeapon(CWeaponInfo* cWeaponInfo = NULL);

	// Get current weapon of this class instance
	CWeaponInfo* GetWeapon(void) const;

	// Set current weapon
	void SetCurrentWeaponIndex(const int iSlot);
	void IncreaseCurrentWeaponIndex();
	void DecreaseCurrentWeaponIndex();
	//Get current weapon
	int GetCurrentWeaponIndex(void) const;

	// Discharge current weapon
	bool DischargeWeapon(void) const;

	// Set to Jump
	void SetToJump(void);
	// Set to Fall
	void SetToFall(void);
	// Set to Idle
	void SetToIdle(void);

    // Processes input received from any keyboard-like input system as player movements. 
	// Accepts input parameter in the form of Player defined ENUM (to abstract it from windowing systems)
	void ProcessMovement(const PLAYERMOVEMENT direction, const float fDeltaTime);

    // Processes input received from a mouse input system as player rotation. 
	// Expects the offset value in both the x and y direction.
	void ProcessRotate(float fXOffset, float fYOffset, const bool bConstrainPitch = true);

	// Toggle the player's posture
	void TogglePosture(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Print Self
	virtual void PrintSelf(void);

	bool sprinting;
	float stamina;

	float fOriginalSpeed;

	bool isMoving;

	double timeSinceLastHit;

	int enemiesLeft;
protected:

	CInventoryManager* cInventoryManager;

	// The handle to the CCamera class instance
	CCamera* cCamera;

	// The handle to the CTerrain class instance
	CTerrain* cTerrain;

	// Boolean flag to indicate the player had moved
	bool bMoved;

	// Physics
	CPhysics3D cPhysics3D;

	std::vector<CWeaponInfo*> playerWeapon;
	int iCurrentWeapon;

	// Default Constructor
	CPlayer3D(void);

	// Constructor with vectors
	CPlayer3D(	const glm::vec3 vec3Position,
				const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f),
				const float fYaw = -90.0f,
				const float fPitch = 0.0f);

	// Destructor
	virtual ~CPlayer3D(void);

    // Calculates the front vector from the Player's (updated) Euler Angles
	void UpdatePlayerVectors(void);

	// Snap the player's position to the terrain
	void SnapToTerrain(void);

	// Update Jump or Fall
	void UpdateJumpFall(const double dElapsedTime);

	// Update Posture
	void UpdatePosture(const double dElapsedTime);

	// Includes health and stamina
	void UpdateInventory();
};

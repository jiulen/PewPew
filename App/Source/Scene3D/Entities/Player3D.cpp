/**
 CPlayer3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Player3D.h"

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System/ImageLoader.h"

//For allowing creating of Mesh 
#include "Primitives/MeshBuilder.h"

// Include CSolidObjectManager
#include "SolidObjectManager.h"

#include "../../GameStateManagement/GameStateManager.h"

#include "../../SoundController/SoundController.h"

#include "../CameraEffects/CameraEffectsManager.h"

#include "System/LoadOBJ.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CPlayer3D::CPlayer3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(0.0f)
	, fPitch(0.0f)
	, fMouseSensitivity(0.1f)
	, cCamera(NULL)
	, cTerrain(NULL)
	, iCurrentWeapon(0)
	, bMoved(false)
	, timeSinceLastHit(0)
	, enemiesLeft(14)
{
	// Set the default position so it is above the ground
	vec3Position = glm::vec3(0.0f, 0.5f, 0.0f);

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Constructor with vectors
 @param position A const glm::vec3 variable which contains the position of the camera
 @param up A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CPlayer3D::CPlayer3D(	const glm::vec3 vec3Position,
						const glm::vec3 vec3Front,
						const float fYaw,
						const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fMouseSensitivity(0.1f)
	, cCamera(NULL)
	, cTerrain(NULL)
	, iCurrentWeapon(0)
	, bMoved(false)
	, timeSinceLastHit(0)
{
	mesh = NULL;

	this->vec3Position = vec3Position;
	this->vec3Front = vec3Front;
	this->fYaw = fYaw;
	this->fPitch = fPitch;

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Destructor
 */
CPlayer3D::~CPlayer3D(void)
{
	while (playerWeapon.size() > 0)
	{
		if (playerWeapon.back() != nullptr)
			delete playerWeapon.back();
		playerWeapon.pop_back();
	}

	if (cTerrain)
	{
		// We set it to NULL only since it was declared somewhere else
		cTerrain = NULL;
	}
	
	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CPlayer3D::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::PLAYER);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//CS: Create the Quad Mesh using the mesh builder
	/*mesh = CMeshBuilder::GenerateBox(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f, 1.0f, 1.0f);*/

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Player.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Player.tga" << endl;
		return false;
	}

	// Minimap icon
	std::vector<glm::vec3> vertices_icon;
	std::vector<glm::vec2> uvs_icon;
	std::vector<glm::vec3> normals_icon;
	std::vector<ModelVertex> vertex_buffer_data_icon;
	std::vector<GLuint> index_buffer_data_icon;

	std::string file_path_icon = "Models/circle.obj";
	bool success_icon = CLoadOBJ::LoadOBJ(file_path_icon.c_str(), vertices_icon, uvs_icon, normals_icon, true);
	if (!success_icon)
	{
		cout << "Unable to load Models/circle.obj" << endl;
		return false;
	}

	CLoadOBJ::IndexVBO(vertices_icon, uvs_icon, normals_icon, index_buffer_data_icon, vertex_buffer_data_icon);

	glGenVertexArrays(1, &VAO_ICON);
	glBindVertexArray(VAO_ICON);
	glGenBuffers(1, &VBO_ICON);
	glGenBuffers(1, &IBO_ICON);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ICON);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data_icon.size() * sizeof(ModelVertex), &vertex_buffer_data_icon[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ICON);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data_icon.size() * sizeof(GLuint), &index_buffer_data_icon[0], GL_STATIC_DRAW);
	iIndicesSize_Icon = index_buffer_data_icon.size();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// load and create a texture 
	iTextureID_Icon = CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/player_fullmap_icon.tga", false);
	if (iTextureID_Icon == 0)
	{
		cout << "Unable to load Image/GUI/player_fullmap_icon.tga" << endl;
		return false;
	}

	// Store the handler to the CTerrain
	// cTerrain = CTerrain::GetInstance();
	if (cTerrain != NULL)
	{
		// Update the y-axis position of the player
		vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z);
	}

	// Set the Physics to fall status by default
	cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);

	iCurrentWeapon = 0;
	playerWeapon.resize(3);

	// Set the postures
	iPreviousPosture = PLAYERPOSTURE::STANDING;
	iCurrentPosture = PLAYERPOSTURE::STANDING;
	iNextPosture = PLAYERPOSTURE::STANDING;
	fPostureOffset = 0.0f;

	sprinting = false;
	stamina = 100.f;
	health = 500;

	enemiesLeft = 14;

	fMovementSpeed = 5.f;
	fOriginalSpeed = fMovementSpeed;

	// Get the handler to the CInventoryManager instance
	cInventoryManager = CInventoryManager::GetInstance();

	isMoving = false;

	return true;
}

// Set the pointer of the terrain to this player3D class
void CPlayer3D::SetTerrain(CTerrain* theNewTerrain)
{
	cTerrain = theNewTerrain;

	if (cTerrain != NULL)
	{
		// Update the y-axis position of the player
		vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z);
	}
}

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
@return A glm::mat4 variable which contains the view matrix
*/
glm::mat4 CPlayer3D::GetViewMatrix(void) const
{
	return glm::lookAt(vec3Position, vec3Position + vec3Front, vec3Up);
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CPlayer3D::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CPlayer3D::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}

/**
 @brief Set Weapon to this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @param cWeaponInfo A CWeaponInfo* variable which contains the weapon info
 @return true if weapon successfully set, else false
 */
bool CPlayer3D::SetWeapon(CWeaponInfo* cWeaponInfo)
{
	for (int i = 0; i < playerWeapon.size(); ++i)
	{
		if (playerWeapon[i] == nullptr)
		{
			playerWeapon[i] = cWeaponInfo;
			return true;
		}

		if (i == playerWeapon.size() - 1) //replace last weapon
		{
			if (playerWeapon[i] != nullptr)
				delete playerWeapon[i];

			playerWeapon[i] = cWeaponInfo;
			return true;
		}
	}
}

/**
 @brief Get Weapon of this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
CWeaponInfo* CPlayer3D::GetWeapon(void) const
{
	return playerWeapon[iCurrentWeapon];
}

/**
 @brief Set current weapon
 @param iSlot A const int variable which contains the weapon info to check for.
 */
void CPlayer3D::SetCurrentWeaponIndex(const int iSlot)
{
	if (iSlot < 0 || iSlot >= playerWeapon.size())
		return;

	if (playerWeapon[iSlot] != nullptr)
		iCurrentWeapon = iSlot;
}

void CPlayer3D::IncreaseCurrentWeaponIndex()
{
	int originalWeapon = iCurrentWeapon;

	do {
		++iCurrentWeapon;
		if (iCurrentWeapon >= playerWeapon.size())
		{
			iCurrentWeapon = originalWeapon;
		}
	} while (playerWeapon[iCurrentWeapon] == nullptr);
}

void CPlayer3D::DecreaseCurrentWeaponIndex()
{
	int originalWeapon = iCurrentWeapon;

	do {
		--iCurrentWeapon;
		if (iCurrentWeapon < 0)
		{
			iCurrentWeapon = originalWeapon;
		}
	} while (playerWeapon[iCurrentWeapon] == nullptr);
}

// Get current texture Index
int CPlayer3D::GetCurrentWeaponIndex(void) const
{
	return iCurrentWeapon;
}

/**
 @brief Discharge weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @return A bool variable
 */
bool CPlayer3D::DischargeWeapon(void) const
{
	if (playerWeapon.size() > 0) //no need check for out of vector range, already enforced in set weapon index func
		if (playerWeapon[iCurrentWeapon] != nullptr)
			return playerWeapon[iCurrentWeapon]->Discharge(vec3Position, vec3Front, (CSolidObject*)this);

	return false;
}

/**
 @brief Set to Jump
 */
void CPlayer3D::SetToJump(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE && iCurrentPosture == PLAYERPOSTURE::STANDING)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::JUMP);
		cPhysics3D.SetInitialVelocity(glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

/**
 @brief Set to Fall
 */
void CPlayer3D::SetToFall(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
	}
	else if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
	}
}

/**
 @brief Set to Idle
 */
void CPlayer3D::SetToIdle(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::IDLE);
	}
	else if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::IDLE);
	}
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::ProcessMovement(const PLAYERMOVEMENT direction, const float deltaTime)
{
	// Since W-A-S-D was pressed, then set this bool flag to true
	bMoved = true;

	// Calculate the velocity
	float velocity = fMovementSpeed * deltaTime;
	if (sprinting && iCurrentPosture == PLAYERPOSTURE::STANDING && stamina > 0)
		velocity *= 1.5f;

	//slow player down for each weapon they have
	for (int i = 0; i < playerWeapon.size(); ++i)
	{
		if (playerWeapon[i] != nullptr)
		{
			if (i == iCurrentWeapon)
				velocity *= playerWeapon[iCurrentWeapon]->activeSpeedMultiplier;
			else
				velocity *= playerWeapon[iCurrentWeapon]->passiveSpeedMultiplier;
		}
	}

	glm::vec3 playerDirection = glm::vec3(0, 0, 0);
	if (direction == PLAYERMOVEMENT::FORWARD)
		playerDirection += vec3Front;
	if (direction == PLAYERMOVEMENT::BACKWARD)
		playerDirection -= vec3Front;
	if (direction == PLAYERMOVEMENT::LEFT)
		playerDirection -= vec3Right;
	if (direction == PLAYERMOVEMENT::RIGHT)
		playerDirection += vec3Right;

	// Update the player's position
	vec3Position += glm::normalize(glm::vec3(playerDirection.x, 0, playerDirection.z)) * velocity;
	
	//// Snap to the terrain height
	//vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;

}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 @param yoffset A const float variable which contains the y axis of the mouse movement
 @param constrainPitch A const bool variable which indicates if the pitch will be constrained at upright positions
 */
void CPlayer3D::ProcessRotate(float fXOffset, float fYOffset, const bool constrainPitch)
{
	fXOffset *= fMouseSensitivity;
	fYOffset *= fMouseSensitivity;

	fYaw += fXOffset;
	fPitch += fYOffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (fPitch > 89.0f)
			fPitch = 89.0f;
		if (fPitch < -89.0f)
			fPitch = -89.0f;
	}

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdatePlayerVectors();
}

/**
 @brief Toggle the player's posture
 */
void CPlayer3D::TogglePosture(void)
{
	switch (iCurrentPosture)
	{
	case PLAYERPOSTURE::STANDING:
		iNextPosture = PLAYERPOSTURE::CROUCH;
		break;
	case PLAYERPOSTURE::CROUCH:
		if (iPreviousPosture == PLAYERPOSTURE::STANDING)
			iNextPosture = PLAYERPOSTURE::PRONE;
		else if(iPreviousPosture == PLAYERPOSTURE::PRONE)
			iNextPosture = PLAYERPOSTURE::STANDING;
		break;
	case PLAYERPOSTURE::PRONE:
		iNextPosture = PLAYERPOSTURE::CROUCH;
		break;
	default:
		break;
	}
}


/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CPlayer3D::Update(const double dElapsedTime)
{
	if (hurt)
	{
		CSoundController::GetInstance()->PlaySoundByID(10);
		hurt = false;
		timeSinceLastHit = 0;
		CCameraEffectsManager::GetInstance()->Get("RegenHealthScreen")->SetStatus(false);
	}
	else
	{
		timeSinceLastHit += dElapsedTime;
		if (timeSinceLastHit > 5 && health <= 400)
		{
			health += (500 - health) / 100; //regen 1% hp every 0.2s
			timeSinceLastHit -= 0.2;
			CCameraEffectsManager::GetInstance()->Get("RegenHealthScreen")->SetStatus(true);
		}
		if (health > 400)
			CCameraEffectsManager::GetInstance()->Get("RegenHealthScreen")->SetStatus(false);
	}

	if (stamina <= 0)
	{
		stamina = 0;
		sprinting = false;
	}

	if (sprinting && iCurrentPosture == PLAYERPOSTURE::STANDING)
	{
		if (isMoving)
		{
			if (cCamera->fZoom > 30.0f)
			{
				cCamera->fZoom -= 5.0f * dElapsedTime;
				if (cCamera->fZoom < 30.0f)
					cCamera->fZoom = 30.0f;
			}
			//Reduce stamina
			stamina -= 20 * dElapsedTime;

			CSoundController::GetInstance()->PlaySoundByID(12);
			CSoundController::GetInstance()->StopPlayById(11);
		}	
	}
	else
	{
		if (cCamera->fZoom < 45.0f && cCamera->fZoom >= 30.0f)
		{
			cCamera->fZoom += 15.0f * dElapsedTime;
			if (cCamera->fZoom > 45.0f)
				cCamera->fZoom = 45.0f;
		}

		if (isMoving)
		{
			CSoundController::GetInstance()->PlaySoundByID(11);
			CSoundController::GetInstance()->StopPlayById(12);
		}
		else
		{
			stamina += 5 * dElapsedTime;
			if (stamina > 100)
				stamina = 100;
		}
	}

	if (!isMoving)
	{
		CSoundController::GetInstance()->StopPlayById(11);
		CSoundController::GetInstance()->StopPlayById(12);
	}

	if (playerWeapon[iCurrentWeapon] != nullptr)
	{
		playerWeapon[iCurrentWeapon]->Update(dElapsedTime);
	}

	// Update the Jump/Fall
	UpdateJumpFall(dElapsedTime);
	// Update the posture
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
		UpdatePosture(dElapsedTime);

	// Constraint the player's position
	SnapToTerrain();
	

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}

	// Update the parent class
	CSolidObject::Update(dElapsedTime);

	// Reset bMoved to false for the next frame
	bMoved = false;

	fRotationAngle = atan2(-vec3Front.z, vec3Front.x);

	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, fRotationAngle, glm::vec3(0, 1, 0));

	UpdateInventory();

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CPlayer3D::PreRender(void)
{
	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CPlayer3D::Render(void)
{
	// Don't render the player if the camera is attached to it and not in full minimap
	if (cCamera && !renderIcon)
		return;

	CSolidObject::Render();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CPlayer3D::PostRender(void)
{
	CSolidObject::PostRender();
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CPlayer3D::UpdatePlayerVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	vec3Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));  

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Snap the player's position to the terrain
 */
void CPlayer3D::SnapToTerrain(void)
{
	// If the player is not jumping nor falling, then we snap his position to the terrain
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset - fPostureOffset;
	}
	else
	{
		if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
		{
			// If the player is jumping or falling, then we only snap his position 
			// if the player's y-coordinate is below the cTerrain 
			float fCheckHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;

			// If the player is below the terrain, then snap to the terrain height
			if (fCheckHeight > vec3Position.y)
				vec3Position.y = fCheckHeight;
		}
	}	
}

/**
 @brief Update Jump or Fall
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::UpdateJumpFall(const double dElapsedTime)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP)
	{
		// Update the elapsed time to the physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update(dElapsedTime);
		// Get the displacement from the physics engine and update the player position
		vec3Position = vec3Position + cPhysics3D.GetDisplacement();

		// If the player is still jumping and the initial velocity has reached zero or below zero, 
		// then it has reach the peak of its jump
		if ((cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP) && (cPhysics3D.GetDeltaDisplacement().y <= 0.0f))
		{
			// Set status to fall
			cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
		}
	}
	else if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
	{
		// Update the elapsed time to the physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update(dElapsedTime);
		// Get the displacement from the physics engine and update the player position
		vec3Position = vec3Position + cPhysics3D.GetDisplacement();

		// Constaint the player's position to the terrain
		float fCheckHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;

		// Set the Physics to idle status
		if (fCheckHeight > vec3Position.y)
		{
			vec3Position.y = fCheckHeight;
			cPhysics3D.SetStatus(CPhysics3D::STATUS::IDLE);
		}
	}
}

/**
 @brief Update Posture
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::UpdatePosture(const double dElapsedTime)
{
	// Return if there is no chance in posture
	if (iCurrentPosture == iNextPosture)
		return;

	switch (iNextPosture)
	{
	case PLAYERPOSTURE::STANDING:
		fPostureOffset -= 0.01f;
		if (fPostureOffset < 0.0f)
		{
			fPostureOffset = 0.0f;
			iPreviousPosture = iCurrentPosture;
			iCurrentPosture = iNextPosture;
			fMovementSpeed = fOriginalSpeed;
		}
		break;
	case PLAYERPOSTURE::CROUCH:
		if (iCurrentPosture == PLAYERPOSTURE::STANDING)
		{
			fPostureOffset += 0.01f;
			if (fPostureOffset > 0.25f)
			{
				fPostureOffset = 0.25f;
				iPreviousPosture = iCurrentPosture;
				iCurrentPosture = iNextPosture;
				fMovementSpeed = fOriginalSpeed * 0.5f;
			}
		}
		else if (iCurrentPosture == PLAYERPOSTURE::PRONE)
		{
			fPostureOffset -= 0.01f;
			if (fPostureOffset < 0.25f)
			{
				fPostureOffset = 0.25f;
				iPreviousPosture = iCurrentPosture;
				iCurrentPosture = iNextPosture;
				fMovementSpeed = fOriginalSpeed * 0.5f;
			}
		}
		break;
	case PLAYERPOSTURE::PRONE:
		fPostureOffset += 0.01f;
		if (fPostureOffset > 0.35f)
		{
			fPostureOffset = 0.35f;
			iPreviousPosture = iCurrentPosture;
			iCurrentPosture = iNextPosture;
			fMovementSpeed = fOriginalSpeed * 0.2f;
		}
		break;
	default:
		break;
	}

	// Finalise the player's height
	vec3Position.y = vec3Position.y - fPostureOffset;
}

void CPlayer3D::UpdateInventory()
{
	cInventoryManager->GetItem("Health")->Set(health);
	if (health <= 0)
		CGameStateManager::GetInstance()->SetActiveGameState("LoseState");
	else if (health <= 100) // 20% health or lower
	{
		CCameraEffectsManager::GetInstance()->Get("LowHealthScreen")->SetStatus(true);
		CCameraEffectsManager::GetInstance()->Get("RegenHealthScreen")->SetStatus(false);
	}		
	else
		CCameraEffectsManager::GetInstance()->Get("LowHealthScreen")->SetStatus(false);

	cInventoryManager->GetItem("Stamina")->Set((int)(stamina));
}

/**
 @brief Print Self
 */
void CPlayer3D::PrintSelf(void)
{
	cout << "CPlayer3D::PrintSelf()" << endl;
	cout << "========================" << endl;
}

/**
 CEnemy3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Enemy3D.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"
#include "System/LoadOBJ.h"

#include <iostream>
using namespace std;

// Set this to true if printing the debug info
#define _DEBUG_FSM false

/**
 @brief Default Constructor
 */
CEnemy3D::CEnemy3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, cPlayer3D(NULL)
	, cTerrain(NULL)
	, cWaypointManager(NULL)
	, m_iWayPointID(-1)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, +fHeightOffset, 0.0f);

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CEnemy3D::CEnemy3D(	const glm::vec3 vec3Position,
					const glm::vec3 vec3Front,
					const float fYaw,
					const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, cPlayer3D(NULL)
	, cTerrain(NULL)
	, m_iWayPointID(-1)
{
	this->vec3Position = vec3Position;
	this->vec3Position.y += +fHeightOffset;
	this->vec3Front = vec3Front;
	this->fYaw = fYaw;
	this->fPitch = fPitch;

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Destructor
 */
CEnemy3D::~CEnemy3D(void)
{
	if (cWaypointManager)
	{
		// We set it to NULL only since it was declared somewhere else
		cWaypointManager = NULL;
	}

	if (cTerrain)
	{
		// We set it to NULL only since it was declared somewhere else
		cTerrain = NULL;
	}

	if (cPlayer3D)
	{
		// We set it to NULL only since it was declared somewhere else
		cPlayer3D = NULL;
	}

	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}

	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO_ICON);
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CEnemy3D::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::NPC);

	// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();

	//mesh = CMeshBuilder::GenerateBox(glm::vec4(1, 1, 1, 1), 0.5f, 1.f, 0.5f);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = "Models/Enemy/enemy1.obj";
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load Models/Enemy/enemy.obj" << endl;
		return false;
	}

	CLoadOBJ::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(ModelVertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	iIndicesSize = index_buffer_data.size();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/Enemy/enemy.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Models/Enemy/enemy.png" << endl;
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
	iTextureID_Icon = CImageLoader::GetInstance()->LoadTextureGetID("Models/Enemy/enemy_icon.tga", false);
	if (iTextureID_Icon == 0)
	{
		cout << "Unable to load Models/Enemy/enemyIcon.tga" << endl;
		return false;
	}

	// Store the handler to the terrain
	cTerrain = CTerrain::GetInstance();

	// Movement Control
	fMovementSpeed = 1.5f;
	iCurrentNumMovement = 0;
	iMaxNumMovement = 100;

	// Detection distance for player
	fDetectionDistance = 10.f;

	// Init cWaypointManager
	cWaypointManager = new CWaypointManager;
	cWaypointManager->Init();

	return true;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CEnemy3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CEnemy3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CEnemy3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CEnemy3D::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CEnemy3D::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}

/**
 @brief Set Weapon to this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @param cWeaponInfo A CWeaponInfo* variable which contains the weapon info
 */
void CEnemy3D::SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo)
{
	if (iSlot == 0)
		cPrimaryWeapon = cWeaponInfo;
	else if (iSlot == 1)
		cSecondaryWeapon = cWeaponInfo;
}

/**
 @brief Get Weapon of this class instance
 @return The CWeaponInfo* value
 */
CWeaponInfo* CEnemy3D::GetWeapon(void) const
{
	if (iCurrentWeapon == 0)
		return cPrimaryWeapon;
	else if (iCurrentWeapon == 1)
		return cSecondaryWeapon;

	return NULL;
}

/**
 @brief Set current weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
void CEnemy3D::SetCurrentWeapon(const int iSlot)
{
	iCurrentWeapon = iSlot;
}

/**
 @brief Discharge weapon
 @return A bool value
 */
bool CEnemy3D::DischargeWeapon(void) const
{
	if ((iCurrentWeapon == 0) && (cPrimaryWeapon))
	{
		return cPrimaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	else if ((iCurrentWeapon == 1) && (cSecondaryWeapon))
	{
		return cSecondaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	//return NULL;
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CEnemy3D::ProcessMovement(const ENEMYMOVEMENT direction, const float deltaTime)
{
	float velocity = fMovementSpeed* deltaTime;
	if (direction == ENEMYMOVEMENT::FORWARD)
		vec3Position += vec3Front * velocity;
	if (direction == ENEMYMOVEMENT::BACKWARD)
		vec3Position -= vec3Front * velocity;
	if (direction == ENEMYMOVEMENT::LEFT)
		vec3Position -= vec3Right * velocity;
	if (direction == ENEMYMOVEMENT::RIGHT)
		vec3Position += vec3Right * velocity;

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
	}

	// Constraint the player's position
	Constraint();
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 */
void CEnemy3D::ProcessRotate(const float fXOffset)
{
	// Update the yaw
	fYaw += fXOffset;// *fRotationSensitivity;

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdateEnemyVectors();
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CEnemy3D::Update(const double dElapsedTime)
{
	// Don't update if this entity is not active
	if (bStatus == false)
	{
		return false;
	}

	// Store the enemy's current position, if rollback is needed.
	StorePositionForRollback();

	if (hurt)
	{
		if (health <= 25)
			sCurrentFSM = FSM::ATTACK;
		else
			sCurrentFSM = FSM::RETREAT;

		hurt = false;
		iFSMCounter = 0;
	}		

	switch (sCurrentFSM)
	{
	case FSM::IDLE:
		if (iCurrentWeapon == 0) // Check if the weapon mag isnt empty
		{
			if (cPrimaryWeapon->GetMagRound() != cPrimaryWeapon->GetMaxMagRound()) //Check if mag isnt full
				cPrimaryWeapon->Reload();
			else if (cSecondaryWeapon != nullptr)
			{
				if (cSecondaryWeapon->GetMagRound() != cSecondaryWeapon->GetMaxMagRound() && cSecondaryWeapon->GetTotalRound() != 0)
				{
					iCurrentWeapon = 1;
					cSecondaryWeapon->Reload();
				}				
			}				
		}
		else if (iCurrentWeapon == 1)
		{
			if (cSecondaryWeapon->GetMagRound() != cSecondaryWeapon->GetMaxMagRound()) //Check if mag isnt full
				cPrimaryWeapon->Reload();
			else if (cPrimaryWeapon != nullptr)
			{
				if (cPrimaryWeapon->GetMagRound() != 0 && cPrimaryWeapon->GetTotalRound() != 0)
				{
					iCurrentWeapon = 0;
					cPrimaryWeapon->Reload();
				}				
			}
		}
		if (iFSMCounter > iMaxFSMCounter / 3)
		{
			sCurrentFSM = FSM::PATROL;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Rested: Switching to Patrol State" << endl;
		}
		iFSMCounter++;
		break;
	case FSM::ATTACK:
		if (glm::distance(vec3Position, cPlayer3D->GetPosition()) < fDetectionDistance + 5 || health <= 25) //low health enemies will constantly chase player
		{
			if (glm::distance(vec3Position, cPlayer3D->GetPosition()) < 10 && fear == 2) //fearful enemies wont move too close to player
			{
				//do nothing
			}
			else
			{
				vec3Front = glm::normalize((cPlayer3D->GetPosition() - vec3Position));
				UpdateFrontAndYaw();
			}

			// Discharge weapon
			if (DischargeWeapon() == false)
			{
				if (iCurrentWeapon == 0)
				{
					// Check if the weapon mag is empty
					if (cPrimaryWeapon->GetMagRound() == 0)
					{
						if (cSecondaryWeapon != nullptr)
						{
							if (cSecondaryWeapon->GetMagRound() != 0)
							{
								// Use secondary weapon
								iCurrentWeapon = 1;
							}
							else if (cPrimaryWeapon->GetTotalRound() != 0)
							{
								// Reload the weapon
								cPrimaryWeapon->Reload();
							}
						}
						else if (cPrimaryWeapon->GetTotalRound() != 0)
						{
							// Reload the weapon
							cPrimaryWeapon->Reload();
						}
					}
				}
				else if (iCurrentWeapon == 1)
				{
					// Check if the weapon mag is empty
					if (cSecondaryWeapon->GetMagRound() == 0)
					{
						if (cPrimaryWeapon != nullptr)
						{
							if (cPrimaryWeapon->GetMagRound() != 0)
							{
								// Use primary weapon
								iCurrentWeapon = 0;
							}
							else if (cSecondaryWeapon->GetTotalRound() != 0)
							{
								// Reload the weapon
								cSecondaryWeapon->Reload();
							}
							else
							{
								// Swtich to retreat mode
								sCurrentFSM = FSM::RETREAT;
								iFSMCounter = 0;
								if (_DEBUG_FSM == true)
									cout << "Switching to Retreat State" << endl;
							}
						}
						else if (cSecondaryWeapon->GetTotalRound() != 0)
						{
							// Reload the weapon
							cSecondaryWeapon->Reload();
						}
					}
				}
			}

			// Process the movement
			ProcessMovement(ENEMYMOVEMENT::FORWARD, (float)dElapsedTime);
			if (_DEBUG_FSM == true)
				cout << "Attacking now" << endl;
		}
		else
		{
			// If NPC lose track of player, then go back to the nearest waypoint
			vec3Front = glm::normalize((cWaypointManager->GetNearestWaypoint(vec3Position)->GetPosition() - vec3Position));
			UpdateFrontAndYaw();
			// Switch to patrol mode
			sCurrentFSM = FSM::PATROL;
		}
		iFSMCounter++;
		break;
	case FSM::RETREAT:
		vec3Front = glm::normalize((vec3Position - cPlayer3D->GetPosition())); //move away from player
		UpdateFrontAndYaw();
		// Process the movement
		ProcessMovement(ENEMYMOVEMENT::FORWARD, (float)dElapsedTime);
		if (iCurrentWeapon == 0) // Check if the weapon mag isnt empty
		{
			if (cPrimaryWeapon->GetMagRound() != cPrimaryWeapon->GetMaxMagRound()) //Check if mag isnt full
				cPrimaryWeapon->Reload();
			else if (cSecondaryWeapon != nullptr)
			{
				iCurrentWeapon = 1;
				cSecondaryWeapon->Reload();
			}
		}
		else if (iCurrentWeapon == 1)
		{
			if (cSecondaryWeapon->GetMagRound() != cSecondaryWeapon->GetMaxMagRound()) //Check if mag isnt full
				cPrimaryWeapon->Reload();
			else if (cPrimaryWeapon != nullptr)
			{
				iCurrentWeapon = 0;
				cPrimaryWeapon->Reload();
			}
		}
		if (iFSMCounter > iMaxFSMCounter * 5)
		{
			// Swtich to idle mode
			sCurrentFSM = FSM::IDLE;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Switching to Idle State" << endl;
		}
		iFSMCounter++;
		break;
	case FSM::PATROL:
		// Check if the destination position has been reached
		if (cWaypointManager->HasReachedWayPoint(vec3Position))
		{
			vec3Front = glm::normalize((cWaypointManager->GetNextWaypoint()->GetPosition() - vec3Position));
			UpdateFrontAndYaw();

			if (_DEBUG_FSM == true)
				cout << "Reached waypoint: Going to next waypoint" << endl;
		}
		else if (iFSMCounter > iMaxFSMCounter * 4)
		{
			sCurrentFSM = FSM::IDLE;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "FSM Counter maxed out: Switching to Idle State" << endl;
		}
		else if (glm::distance(vec3Position, cPlayer3D->GetPosition()) < fDetectionDistance)
		{
			sCurrentFSM = FSM::ATTACK;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Target found: Switching to Attack State" << endl;
		}
		else
		{
			// Process the movement
			ProcessMovement(ENEMYMOVEMENT::FORWARD, (float)dElapsedTime);
			if (_DEBUG_FSM == true)
				cout << "Patrolling" << endl;
		}
		iFSMCounter++;
		break;
	default:
		break;
	}

	// Update the model
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, glm::radians(fYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	// Update the weapon's position
	if (cPrimaryWeapon)
	{
		cPrimaryWeapon->SetPosition(vec3Position + glm::vec3(0.05f, -0.075f, 0.5f));
		cPrimaryWeapon->Update(dElapsedTime);
		glm::mat4 gunModel = model;
		gunModel = glm::translate(gunModel, glm::vec3(0.05f, -0.075f, 0.5f));
		gunModel = glm::rotate(gunModel, cPrimaryWeapon->GetRotationAngle(), cPrimaryWeapon->GetRotationAxis());
		gunModel = glm::scale(gunModel, cPrimaryWeapon->GetScale());
		cPrimaryWeapon->SetModel(gunModel);
	}
	if (cSecondaryWeapon)
	{
		cSecondaryWeapon->SetPosition(vec3Position + glm::vec3(0.05f, -0.075f, 0.5f));
		cSecondaryWeapon->Update(dElapsedTime);
		glm::mat4 gunModel = model;
		gunModel = glm::translate(gunModel, glm::vec3(0.05f, -0.075f, 0.5f));
		gunModel = glm::rotate(gunModel, cSecondaryWeapon->GetRotationAngle(), cSecondaryWeapon->GetRotationAxis());
		gunModel = glm::scale(gunModel, cSecondaryWeapon->GetScale());
		cSecondaryWeapon->SetModel(gunModel);
	}

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CEnemy3D::PreRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CEnemy3D::Render(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	// Render enemy
	CSolidObject::Render();

	if (!renderIcon)
	{
		//Render weapon
		if ((iCurrentWeapon == 0) && (cPrimaryWeapon))
		{
			cPrimaryWeapon->SetView(view);
			cPrimaryWeapon->SetProjection(projection);
			cPrimaryWeapon->PreRender();
			cPrimaryWeapon->Render();
			cPrimaryWeapon->PostRender();
		}
		else if ((iCurrentWeapon == 1) && (cSecondaryWeapon))
		{
			cSecondaryWeapon->SetView(view);
			cSecondaryWeapon->SetProjection(projection);
			cSecondaryWeapon->PreRender();
			cSecondaryWeapon->Render();
			cSecondaryWeapon->PostRender();
		}
	}
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CEnemy3D::PostRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PostRender();
}

void CEnemy3D::SetTerrain(CTerrain* terrain)
{
	cTerrain = terrain;
}

void CEnemy3D::AddEnemyWaypoint(float fX, float fZ)
{
	float fCheckHeight = cTerrain->GetHeight(fX, fZ);
	if (m_iWayPointID == -1)
		m_iWayPointID = cWaypointManager->AddWaypoint(glm::vec3(fX, fCheckHeight, fZ));
	else
		m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(fX, fCheckHeight, fZ));
}

void CEnemy3D::FinishAddWaypoints()
{
	//cWaypointManager->PrintSelf();

	// Let the NPC face the nearest waypoint
	vec3Front = glm::normalize((cWaypointManager->GetNearestWaypoint(vec3Position)->GetPosition() - vec3Position));
	UpdateFrontAndYaw();
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CEnemy3D::UpdateEnemyVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front = glm::normalize(front);
	
	vec3Front = front;
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));  
	vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Right = vec3Right;
		cCamera->vec3Up = vec3Up;
	}
}

/**
 @brief Constraint the player's position
 */
void CEnemy3D::Constraint(void)
{
	// Get the new height
	float fNewYValue = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	// Smooth out the change in height
	vec3Position.y += (fNewYValue - vec3Position.y) * 0.05f;
}

/**
 @brief Update Front Vector and Yaw
 */
void CEnemy3D::UpdateFrontAndYaw(void)
{
	fYaw = glm::degrees(glm::acos(dot(glm::vec3(1.0f, 0.0f, 0.0f), vec3Front)));
	if (cross(glm::vec3(1.0f, 0.0f, 0.0f), vec3Front).y < 0.0f)
		fYaw *= -1;
}

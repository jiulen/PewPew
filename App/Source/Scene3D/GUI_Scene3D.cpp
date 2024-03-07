/**
 CGUI_Scene3D
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "GUI_Scene3D.h"

// Include CBloodScreen
#include "CameraEffects/BloodScreen.h"
#include "CameraEffects/LowHealthScreen.h"
#include "CameraEffects/RegenHealthScreen.h"
// Include CScopeScreen
#include "CameraEffects/ScopeScreen.h"
// Include CCrossHair
#include "CameraEffects/CrossHair.h"
// Include CCrossHair2
#include "CameraEffects/CrossHair2.h"
#include "CameraEffects/Hitmarker.h"
// Include CCameraShake
#include "CameraEffects/CameraShake.h"

#include "System/ImageLoader.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene3D::CGUI_Scene3D(void)
	: cSettings(NULL)
	, model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
	, m_fProgressBar(0.0f)
	, cCameraEffectsManager(NULL)
	, cMinimap(NULL)
	, cInventoryManager(NULL)
	, cInventoryItem(NULL)
	, cPlayer3D(NULL)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene3D::~CGUI_Scene3D(void)
{
	// Set the cPlayer3D to NULL since it was initialised elsewhere
	cPlayer3D = NULL;
	
	// Destroy the cInventoryManager and set it to NULL
	if (cInventoryManager)
	{
		cInventoryManager->Destroy();
		cInventoryManager = NULL;
	}

	// Cleanup IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Destroy the cMinimap and set it to NULL
	if (cMinimap)
	{
		cMinimap->Destroy();
		cMinimap = NULL;
	}

	// Destroy the cCameraEffects and set it to NULL
	if (cCameraEffectsManager)
	{
		cCameraEffectsManager->Destroy();
		cCameraEffectsManager = NULL;
	}

	// Set the cSettings to NULL since it was initialised elsewhere
	cSettings = NULL;
}

/**
  @brief Initialise this instance
  */
bool CGUI_Scene3D::Init(void)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	// Load the CCameraEffectsManager
	cCameraEffectsManager = CCameraEffectsManager::GetInstance();
	cCameraEffectsManager->Init();

	// Add the CameraEffects
	// Add BloodScreen
	CBloodScreen* cBloodScreen = new CBloodScreen();
	cBloodScreen->Init();
	cBloodScreen->SetShader("Shader2D");
	cCameraEffectsManager->Add("BloodScreen", cBloodScreen);
	// Add Low Health Screen
	CLowHealthScreen* cLowHealthScreen = new CLowHealthScreen();
	cLowHealthScreen->Init();
	cLowHealthScreen->SetShader("Shader2D");
	cCameraEffectsManager->Add("LowHealthScreen", cLowHealthScreen);
	// Add Regen Health Screen
	CRegenHealthScreen* cRegenHealthScreen = new CRegenHealthScreen();
	cRegenHealthScreen->Init();
	cRegenHealthScreen->SetShader("Shader2D");
	cCameraEffectsManager->Add("RegenHealthScreen", cRegenHealthScreen);
	// Add ScopeScreen
	CScopeScreen* cScopeScreen = new CScopeScreen();
	cScopeScreen->Init();
	cScopeScreen->SetShader("Shader2D");
	cCameraEffectsManager->Add("ScopeScreen", cScopeScreen);
	// Add CrossHair
	CCrossHair* cCrossHair = new CCrossHair();
	cCrossHair->Init();
	cCrossHair->SetShader("Shader2D");
	cCameraEffectsManager->Add("CrossHair", cCrossHair);
	// Add CrossHair
	CCrossHair2* cCrossHair2 = new CCrossHair2();
	cCrossHair2->Init();
	cCrossHair2->SetShader("Shader2D");
	cCameraEffectsManager->Add("CrossHair2", cCrossHair2);
	// Add Hitmarker
	CHitmarker* cHitmarker = new CHitmarker();
	cHitmarker->Init();
	cHitmarker->SetShader("Shader2D");
	cCameraEffectsManager->Add("Hitmarker", cHitmarker);
	// Add CameraShake
	CCameraShake* cCameraShake = new CCameraShake();
	cCameraShake->Init();
	cCameraEffectsManager->Add("CameraShake", cCameraShake);

	// Load the Minimap
	cMinimap = CMinimap::GetInstance();
	// Set a shader to this class instance of Minimap
	cMinimap->SetShader("Shader3D_RenderToTexture");
	cMinimap->Init();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(CSettings::GetInstance()->pWindow, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Define the window flags
	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	// Get the handler to the CInventoryManager instance
	cInventoryManager = CInventoryManager::GetInstance();

	// Add a Health icon as one of the inventory items
	cInventoryItem = cInventoryManager->Add("Health", "Image/Scene2D_Health.tga", 500, 500);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	// Add a Stamina icon as one of the inventory items
	cInventoryItem = cInventoryManager->Add("Stamina", "Image/sprinting.tga", 100, 100);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	// Add a pistol as one of the inventory items
	cInventoryItem = cInventoryManager->Add("Pistol", "Image/Scene2D_TreeTile.tga", 5, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	// Get the handler to the CPlayer3D instance
	cPlayer3D = CPlayer3D::GetInstance();

	scoped = false;
	fullMinimap = false;

	return true;
}


/**
 @brief Set model
 @param model A glm::mat4 variable to be assigned to this class instance
 */ 
void CGUI_Scene3D::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable to be assigned to this class instance
 */
void CGUI_Scene3D::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable to be assigned to this class instance
 */
void CGUI_Scene3D::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}


/**
 @brief Update this instance
 @param dElapsedTime A const double containing the elapsed time since the last frame
 */
void CGUI_Scene3D::Update(const double dElapsedTime)
{
	// Update camera effects
	cCameraEffectsManager->Update(dElapsedTime);

	// Calculate the relative scale to our default windows width
	/*const float relativeScale_x = cSettings->iWindowWidth / 800.0f;
	const float relativeScale_y = cSettings->iWindowHeight / 600.0f;*/ //got bug so hardcode this

	const float relativeScale_x = 1;
	const float relativeScale_y = 1;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create an invisible window which covers the entire OpenGL window
	ImGui::Begin("Invisible window", NULL, window_flags);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2((float)cSettings->iWindowWidth, (float)cSettings->iWindowHeight));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	
	ImGuiWindowFlags healthWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;

	ImGuiWindowFlags inventoryWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;

	// Render the FPS
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.75f, 0.75f, 0.75f, 0.75f));  // Set a background color
	ImGui::Begin("FPS Counter", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.f, cSettings->iWindowHeight * 0.f));
	ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	// Display the FPS
	ImGui::TextColored(ImVec4(0, 0, 0, 1), "FPS: %d", cFPSCounter->GetFrameRate());
	ImGui::End();
	ImGui::PopStyleColor();

	// Render the Enemies Left
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.75f, 0.75f, 0.75f, 0.75f));  // Set a background color
	ImGui::Begin("Enemies Counter", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.f, cSettings->iWindowHeight * 0.075f));
	ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	// Display the Enemies Left
	ImGui::TextColored(ImVec4(0, 0, 0, 1), "Enemies: %d", cPlayer3D->enemiesLeft);
	ImGui::End();
	ImGui::PopStyleColor();

	// Render the Health
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.25f, 0.25f, 0.25f, 0.75f));  // Set a background color
	ImGui::Begin("Health", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.315f, cSettings->iWindowHeight * 0.0f));
	ImGui::SetWindowSize(ImVec2(100.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	cInventoryItem = cInventoryManager->GetItem("Health");
	ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
		ImVec2(cInventoryItem->vec2Size.x * relativeScale_x, cInventoryItem->vec2Size.y * relativeScale_y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::ProgressBar(cInventoryItem->GetCount() /
		(float)cInventoryItem->GetMaxCount(), ImVec2(100.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::End();
	ImGui::PopStyleColor();

	// Render the Stamina
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.25f, 0.25f, 0.25f, 0.75f));  // Set a background color
	ImGui::Begin("Stamina", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.5f, cSettings->iWindowHeight * 0.0f));
	ImGui::SetWindowSize(ImVec2(100.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	cInventoryItem = cInventoryManager->GetItem("Stamina");
	ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
		ImVec2(cInventoryItem->vec2Size.x * relativeScale_x, cInventoryItem->vec2Size.y * relativeScale_y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
	ImGui::ProgressBar(cInventoryItem->GetCount() /
		(float)cInventoryItem->GetMaxCount(), ImVec2(100.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::End();
	ImGui::PopStyleColor();

	if (!fullMinimap)
	{
		// Render the player icon on minimap
		ImGui::Begin("PlayerIcon", NULL, healthWindowFlags);
		ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.8215f, cSettings->iWindowHeight * 0.10225f));
		ImGui::SetWindowSize(ImVec2(50.0f * relativeScale_x, 50.0f * relativeScale_y));
		ImGui::Image((void*)(intptr_t)CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/player_minimap_icon.tga", true),
			ImVec2(50.f * relativeScale_x, 50.f * relativeScale_y),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}	

	// Render the inventory items
	cInventoryItem = cInventoryManager->GetItem("Pistol");
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.75f, 0.75f, 0.75f, 0.75f));  // Set a background color
	
	ImGui::Begin("Image", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.825f, cSettings->iWindowHeight * 0.925f));
	ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::Image((void*)(intptr_t)CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/AmmoIcon.tga", true),
		ImVec2(18.f * relativeScale_x, 18.f * relativeScale_y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	ImGui::TextColored(ImVec4(0, 0, 0, 1), "%d / %d",
		cPlayer3D->GetWeapon()->GetMagRound(), cPlayer3D->GetWeapon()->GetTotalRound());
	ImGui::End();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.75f, 0.75f, 0.75f, 0.75f));  // Set a background color
	ImGui::Begin("PlayerPos", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.5f, cSettings->iWindowHeight * 0.925f));
	ImGui::SetWindowSize(ImVec2(300.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	ImGui::TextColored(ImVec4(0, 0, 0, 1), "X:%3.1f Y:%3.1f Z:%3.1f",
		CCamera::GetInstance()->vec3Position.x,
		CCamera::GetInstance()->vec3Position.y,
		CCamera::GetInstance()->vec3Position.z);
	ImGui::End();
	ImGui::PopStyleColor();

	ImGui::End();
}


/**
 @brief Set up the OpenGL display environment before rendering
 */
void CGUI_Scene3D::PreRender(void)
{
}

/**
 @brief Render this instance
 */
void CGUI_Scene3D::Render(void)
{
	// Render the camera effects
	cCameraEffectsManager->Render();

	if (!scoped)
	{
		//glEnable(GL_DEPTH_TEST);
		cPlayer3D->GetWeapon()->SetProjection(projection);
		cPlayer3D->GetWeapon()->PreRender();
		cPlayer3D->GetWeapon()->Render();
		cPlayer3D->GetWeapon()->PostRender();
		//glDisable(GL_DEPTH_TEST);
	}

	// Render the minimap
	cMinimap->PreRender();
	cMinimap->Render();
	cMinimap->PostRender();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CGUI_Scene3D::PostRender(void)
{
	glDisable(GL_DEPTH_TEST);
}

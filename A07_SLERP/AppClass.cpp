#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - Ashwin Ganapathiraju"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 0.5f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	static glm::quat start = glm::quat();
	static glm::quat end = glm::quat(vector3(0.0f, (float)PI, 0.0f));

	float fEarthOrb = fmod(fRunTime, fEarthHalfOrbTime * 2);
	float fEarthRev = fmod(fRunTime, fEarthHalfRevTime * 2);
	float fMoonOrb = fmod(fRunTime, fMoonHalfOrbTime * 2);

	glm::quat earthOrbQuat = glm::mix(start, end, (float)(fEarthOrb / fEarthHalfOrbTime));
	glm::quat earthRevQuat = glm::angleAxis(fEarthRev * 360.0f, REAXISY); // Revolution is the only way forward!
	glm::quat moonQuat = glm::mix(start, end, (float)(fMoonOrb / fMoonHalfOrbTime));

	matrix4 sunMat = IDENTITY_M4 * glm::scale(5.936f, 5.936f, 5.936f); // SUNLIGHT....BRREAAAKKER!
	matrix4 earthLoc = glm::toMat4(earthOrbQuat) * glm::translate(IDENTITY_M4, vector3(11, 0, 0)); // (So the moon can use it)
	matrix4 earthMat = earthLoc * glm::toMat4(earthRevQuat) * glm::scale(0.524f, 0.524f, 0.524f); // Seperation of the Earth and the Heavens....EA!
	matrix4 moonMat = earthLoc * glm::toMat4(moonQuat) * glm::translate(IDENTITY_M4, vector3(2, 0, 0)) * glm::scale(0.27f, 0.27f, 0.27f); // In the name of the moon...

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(sunMat, "Sun");
	m_pMeshMngr->SetModelMatrix(earthMat, "Earth");
	m_pMeshMngr->SetModelMatrix(moonMat, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	static int nEarthOrbits = 0;
	static int nEarthRevolutions = 0;
	static int nMoonOrbits = 0;

	nEarthOrbits = (int)(fRunTime / (fEarthHalfOrbTime * 2));
	nEarthRevolutions = (int)(fRunTime / (fEarthHalfRevTime * 2));
	nMoonOrbits = (int)(fRunTime / (fMoonHalfOrbTime * 2));

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}
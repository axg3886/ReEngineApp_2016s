#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Ganapathiraju, Ashwin - Gimbal Lock");
	m_v4ClearColor = vector4(0.0f, 0.0f, 0.0f, 0.0f);
}
void AppClass::InitVariables(void)
{
	//Loading the model
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");

	//Set the axis of Steve visible
	m_pMeshMngr->SetVisibleAxis(true, "Steve");
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

	//Rotation matrices
	glm::quat rotX = glm::angleAxis(m_v3Orientation.x, REAXISX);
	glm::quat rotY = glm::angleAxis(m_v3Orientation.y, REAXISY);
	glm::quat rotZ = glm::angleAxis(m_v3Orientation.z, REAXISZ);

	//linear combination
	m_mToWorld = glm::toMat4(rotX * rotY * rotZ);

	//Setting the model matrix
	m_pMeshMngr->SetModelMatrix(m_mToWorld, "Steve");

	//Adding the instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("Steve");

	int nFPS = m_pSystem->GetFPS();
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("X:", REYELLOW);
	m_pMeshMngr->PrintLine(std::to_string(m_v3Orientation.x), RERED);
	m_pMeshMngr->Print("Y:", REYELLOW);
	m_pMeshMngr->PrintLine(std::to_string(m_v3Orientation.y), RERED);
	m_pMeshMngr->Print("Z:", REYELLOW);
	m_pMeshMngr->PrintLine(std::to_string(m_v3Orientation.z), RERED);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
	m_pMeshMngr->Render(); //renders the list of meshes in the system.
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release();
}
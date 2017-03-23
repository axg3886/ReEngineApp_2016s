#include "AppClass.h"
void AppClass::Update(void)
{
#pragma region DOES NOT NEED CHANGES
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma endregion
#pragma region FEEL FREE TO USE THIS CLOCK
	//Calculate delta and total times
	static double dTotalTime = 0.0; //Total time of the simulation
	double dDeltaTime = m_pSystem->LapClock(); //time difference between function calls
	dTotalTime += dDeltaTime; //Incrementing the time differences 
#pragma endregion
#pragma region YOUR CODE GOES HERE
	static vector3 start = vector3();
	static vector3 end = vector3(0.0f, 5.0f, 0.0f);
	dTotalTime = fmod(dTotalTime, 10.0f); // Moduluo to every 10 seconds for lerp's sake
	m_m4Steve = glm::mat4(1.0f); // same as m_m4Steve = IDENTITY_M4; setting the identity to steve
	m_m4Steve = glm::rotate(m_m4Steve, (float)dTotalTime * 72.0f, REAXISZ); // 2 laps around the center in 10 seconds - 60 * 60 / 100 * 2
	m_m4Steve = glm::translate(m_m4Steve,
		dTotalTime > 5.0 // if we're half way through
		? glm::lerp(end, start, (float)((dTotalTime - 5.0f) / 5.0f)) // start lerping back down
		: glm::lerp(start, end, (float)(dTotalTime / 5.0f))); // otherwise, lerp up, up, and away!
#pragma endregion
#pragma region DOES NOT NEED CHANGES
	//Set the model matrix
	m_pMeshMngr->SetModelMatrix(m_m4Steve, "Steve");
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//Print info on the screen
	m_pMeshMngr->PrintLine("\n" + m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("Seconds:");
	m_pMeshMngr->PrintLine(std::to_string(dTotalTime), RERED);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}
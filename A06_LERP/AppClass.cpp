#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

	sphereLocation = new vector3[numSpheres] {
		vector3(-4.0f, -2.0f, 5.0f),
		vector3(1.0f, -2.0f, 5.0f),
		vector3(-3.0f, -1.0f, 3.0f),
		vector3(2.0f, -1.0f, 3.0f),
		vector3(-2.0f, 0.0f, 0.0f),
		vector3(3.0f, 0.0f, 0.0f),
		vector3(-1.0f, 1.0f, -3.0f),
		vector3(4.0f, 1.0f, -3.0f),
		vector3(0.0f, 2.0f, -5.0f),
		vector3(5.0f, 2.0f, -5.0f),
		vector3(1.0f, 3.0f, -5.0f),
	};
	// Generate spheres
	spheres = new PrimitiveClass[numSpheres];
	sphereMatrices = new matrix4[numSpheres];
	for (int i = 0; i < numSpheres; i++) {
		spheres[i].GenerateSphere(.1f, 5, vector3(1, 0, 0));
		sphereMatrices[i] = glm::translate(sphereLocation[i]);
	}
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	// Modulo'd time - how far are we through this current loop cycle
	float fTime = fmod(static_cast<float>(fRunTime), fDuration * numSpheres);
	// What's that mapped to between 0 and 1?
	float fPercent = MapValue(fTime, 0.0f, fDuration, 0.0f, 1.0f);
	// What index are we checking
	int index = 0;
	while (fPercent >= 1.0f) {
		// If we're over 100%, subtract and move forward till we're not
		fPercent -= 1.0f;
		index++;
	}
	// Get our start vector
	vector3 start = sphereLocation[index];
	// Out end vector loops with this simple ternary
	vector3 end = (index + 1 >= numSpheres) ? sphereLocation[0] : sphereLocation[index + 1];
	// Lerp and translate and set the model matrix
	m_pMeshMngr->SetModelMatrix(glm::translate(glm::lerp(start, end, fPercent)), "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	for (int i = 0; i < numSpheres; i++) {
		spheres[i].Render(m4Projection, m4View, sphereMatrices[i]);
	}

	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{

	if (sphereLocation != nullptr)
	{
		delete[] sphereLocation;
		sphereLocation = nullptr;
	}
	if (spheres != nullptr)
	{
		delete[] spheres;
		spheres = nullptr;
	}
	if (sphereMatrices != nullptr)
	{
		delete[] sphereMatrices;
		sphereMatrices = nullptr;
	}

	super::Release(); //release the memory of the inherited fields
}
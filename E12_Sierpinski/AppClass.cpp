#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("E12 - Instance Rendering");
	m_v4ClearColor = vector4(REBLACK, 1.0f);
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	m_pMesh = new MyMesh();
	
	//Creating the Mesh points
	m_pMesh->AddVertexPosition(vector3(-triangleSize, triangleSize, 0.0f));
	m_pMesh->AddVertexPosition(vector3(0.0f, -triangleSize, 0.0f));
	m_pMesh->AddVertexPosition(vector3(triangleSize, triangleSize, 0.0f));

	m_pMesh->AddVertexColor(REGREEN);
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexColor(REBLUE);

	m_pMesh->CompileOpenGL3X();

	// Define global vars
	numRows = 16;
	triangleSize = 0.5f;

	// Thank you to http://www.geeksforgeeks.org/pascal-triangle/
	std::vector<matrix4> serpinski;
	for (int n = 1; n <= numRows; n++) {
		int C = 1;
		for (int i = 1; i <= n; i++) {
			if (C % 2 != 0) {
				serpinski.push_back(
					glm::translate(vector3((-0.5f * (n - 1) + (i - 1)) * triangleSize * 2, (-n) * triangleSize * 2, 0.0f)) *
					glm::rotate(IDENTITY_M4, 180.0f, REAXISZ)
				);
			}
			C = C * (n - i) / i;
		}
	}
	numObjects = serpinski.size();

	m_fMatrixArray = new float[numObjects * 16];
	for (int x = 0; x < numObjects; x ++) {
		const float* m4MVP = glm::value_ptr(serpinski[x]);
		memcpy(&m_fMatrixArray[x * 16], m4MVP, 16 * sizeof(float));
	}
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	ClearScreen();
	
	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	m_pMesh->RenderList(m4Projection, m4View, m_fMatrixArray, numObjects);//Rendering nObjects
	
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}
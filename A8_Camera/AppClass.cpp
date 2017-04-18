#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("A8_Camera"); // Window Name
}
void AppClass::InitVariables(void)
{
	m_Camera = new Camera();
	m_Camera->MoveForward(1.0f);

	//Generate the Cylinder
	m_pCylinder = new PrimitiveClass();
	m_pCylinder->GenerateCylinder(1.0f, 1.0f, 10, REGREEN);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	ArcBallFake();

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid
	m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY);

	matrix4 view = m_Camera->GetView();
	//Render the cone
	m_pCylinder->Render(m_Camera->GetProjection(ortho), view, IDENTITY_M4);

	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	//Release the memory of the member fields
	SafeDelete(m_pCylinder);

	SafeDelete(m_Camera);

	//Release the memory of the inherited fields
	super::Release();
}

void AppClass::ArcBallFake(void) {
	// Thank you ArcBall method and Alberto's email!
	if (!arcBallFake)
		return;

	UINT MouseX, MouseY;	// Coordinates for the mouse
	UINT CenterX, CenterY;	// Coordinates for the center of the screen.
							//Initialize the position of the pointer to the middle of the screen
	CenterX = m_pSystem->GetWindowX() + m_pSystem->GetWindowWidth() / 2;
	CenterY = m_pSystem->GetWindowY() + m_pSystem->GetWindowHeight() / 2;

	//Calculate the position of the mouse and store it
	POINT pt;
	GetCursorPos(&pt);
	MouseX = pt.x;
	MouseY = pt.y;

	//Calculate the difference in position
	if (MouseX < CenterX)
		if(shouldRoll)
			m_Camera->ChangeRoll((CenterX - MouseX) * -0.01f);
		else
			m_Camera->ChangePitch((CenterX - MouseX) * -0.01f);

	else if (MouseX > CenterX)
		if (shouldRoll)
			m_Camera->ChangeRoll((MouseX - CenterX) * 0.01f);
		else
			m_Camera->ChangePitch((MouseX - CenterX) * 0.01f);

	if (MouseY < CenterY)
		m_Camera->ChangeYaw((CenterY - MouseY) * -0.01f);

	else if (MouseY > CenterY)
		m_Camera->ChangeYaw((MouseY - CenterY) * 0.01f);

	SetCursorPos(CenterX, CenterY); //Position the mouse in the center
	arcBallFake = false;
	shouldRoll = false;
}
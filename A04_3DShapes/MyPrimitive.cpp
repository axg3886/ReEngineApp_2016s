#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	for (int i = 0; i < a_nSubdivisions; i++) {
		float thetaL = 2 * i * glm::pi<float>() / (float)a_nSubdivisions;
		float thetaR = 2 * (i+1) * glm::pi<float>() / (float)a_nSubdivisions;

		vector3 pointA(0, 0, 0); // bottom
		vector3 pointB(cos(thetaL) * a_fRadius, 0, sin(thetaL) * a_fRadius); // left
		vector3 pointC(cos(thetaR) * a_fRadius, 0, sin(thetaR) * a_fRadius); // right
		vector3 pointD(0, a_fHeight, 0); // top
		AddQuad(pointA, pointB, pointC, pointD); // A-B-C, C-B-D
	}
	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	for (int i = 0; i < a_nSubdivisions; i++) {
		float thetaL = 2 * i * glm::pi<float>() / (float)a_nSubdivisions, cTL = cos(thetaL), sTL = sin(thetaL);
		float thetaR = 2 * (i + 1) * glm::pi<float>() / (float)a_nSubdivisions, cTR = cos(thetaR), sTR = sin(thetaR);

		vector3 pointCT(0, a_fHeight, 0); // center top
		vector3 pointLT(cTL * a_fRadius, a_fHeight, sTL * a_fRadius); // left top
		vector3 pointRT(cTR * a_fRadius, a_fHeight, sTR * a_fRadius); // right top
		vector3 pointCB(0, 0, 0); // center bottom
		vector3 pointLB(cTL * a_fRadius, 0, sTL * a_fRadius); // left bottom
		vector3 pointRB(cTR * a_fRadius, 0, sTR * a_fRadius); // right bottom

		// Top Triangle
		AddQuad(pointCT, pointLT, pointRT, pointCT); // A-B-C, C-B-D
		// Side quad
		AddQuad(pointLT, pointRT, pointLB, pointRB); // A-B-C, C-B-D
		// Bottom triangle
		AddQuad(pointCB, pointLB, pointRB, pointCB); // A-B-C, C-B-D
	}
	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	for (int i = 0; i < a_nSubdivisions; i++) {
		float thetaL = 2 * i * glm::pi<float>() / (float)a_nSubdivisions, cTL = cos(thetaL), sTL = sin(thetaL);
		float thetaR = 2 * (i + 1) * glm::pi<float>() / (float)a_nSubdivisions, cTR = cos(thetaR), sTR = sin(thetaR);

		vector3 pointILT(cTL*a_fInnerRadius, a_fHeight, sTL*a_fInnerRadius); // inner left top
		vector3 pointIRT(cTR*a_fInnerRadius, a_fHeight, sTR*a_fInnerRadius); // inner right top
		vector3 pointOLT(cTL*a_fOuterRadius, a_fHeight, sTL*a_fOuterRadius); // outer left top
		vector3 pointORT(cTR*a_fOuterRadius, a_fHeight, sTR*a_fOuterRadius); // outer right top
		vector3 pointILB(cTL*a_fInnerRadius, 0, sTL*a_fInnerRadius); // inner left bottom
		vector3 pointIRB(cTR*a_fInnerRadius, 0, sTR*a_fInnerRadius); // inner right bottom
		vector3 pointOLB(cTL*a_fOuterRadius, 0, sTL*a_fOuterRadius); // outer left bottom
		vector3 pointORB(cTR*a_fOuterRadius, 0, sTR*a_fOuterRadius); // outer right bottom

		// Top quad
		AddQuad(pointILT, pointIRT, pointOLT, pointORT); // A-B-C, C-B-D
		// Outside quad
		AddQuad(pointOLT, pointORT, pointOLB, pointORB); // A-B-C, C-B-D
		// Inside quad
		AddQuad(pointILB, pointIRB, pointILT, pointIRT); // A-B-C, C-B-D
		// Bottom quad
		AddQuad(pointOLB, pointORB, pointILB, pointIRB); // A-B-C, C-B-D
	}
	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float circleRadius = a_fOuterRadius - a_fInnerRadius, centerRadius = a_fInnerRadius + circleRadius;
	for (int k = 0; k < a_nSubdivisionsA; k++) {
		float phiL = 2 * k * glm::pi<float>() / (float)a_nSubdivisionsA, cPL = cos(phiL), sPL = sin(phiL);
		float phiR = 2 * (k + 1) * glm::pi<float>() / (float)a_nSubdivisionsA, cPR = cos(phiR), sPR = sin(phiR);

		for (int i = 0; i < a_nSubdivisionsB; i++) {
			float thetaL = 2 * i * glm::pi<float>() / (float)a_nSubdivisionsB, cTL = cos(thetaL), sTL = sin(thetaL);
			float thetaR = 2 * (i + 1) * glm::pi<float>() / (float)a_nSubdivisionsB, cTR = cos(thetaR), sTR = sin(thetaR);

			/*
			Torus parametrics:
			x(theta, phi) = (R + r * cos(theta)) * cos(phi)
			y(theta, phi) = (R + r * cos(theta)) * sin(phi)
			z(theta, phi) = r * sin(theta)
			*/
			vector3 pointA((centerRadius + cTL*circleRadius) * cPL, sTL * circleRadius, (centerRadius + cTL*circleRadius) * sPL);
			vector3 pointB((centerRadius + cTR*circleRadius) * cPL, sTR * circleRadius, (centerRadius + cTR*circleRadius) * sPL);
			vector3 pointC((centerRadius + cTL*circleRadius) * cPR, sTL * circleRadius, (centerRadius + cTL*circleRadius) * sPR);
			vector3 pointD((centerRadius + cTR*circleRadius) * cPR, sTR * circleRadius, (centerRadius + cTR*circleRadius) * sPR);

			// The quad at that rotation
			AddQuad(pointA, pointB, pointC, pointD); // A-B-C, C-B-D
		}
	}
	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 25)
		a_nSubdivisions = 25;

	Release();
	Init();

	//Your code starts here
	for (int k = 0; k < a_nSubdivisions; k++) {
		float phiL = k * glm::pi<float>() / (float)a_nSubdivisions, cPL = cos(phiL), sPL = sin(phiL);
		float phiR = (k + 1) * glm::pi<float>() / (float)a_nSubdivisions, cPR = cos(phiR), sPR = sin(phiR);

		for (int i = 0; i < a_nSubdivisions; i++) {
			float thetaL = 2 * i * glm::pi<float>() / (float)a_nSubdivisions, cTL = cos(thetaL), sTL = sin(thetaL);
			float thetaR = 2 * (i + 1) * glm::pi<float>() / (float)a_nSubdivisions, cTR = cos(thetaR), sTR = sin(thetaR);

			/*
			Sphere parametrics:
			x(theta, phi) = r * cos(theta) * sin(phi)
			y(theta, phi) = r * sin(theta) * sin(phi)
			z(theta, phi) = r * cos(phi)
			*/
			vector3 pointA(cTL * sPR * a_fRadius, cPR * a_fRadius, sTL * sPR * a_fRadius);
			vector3 pointB(cTL * sPL * a_fRadius, cPL * a_fRadius, sTL * sPL * a_fRadius);
			vector3 pointC(cTR * sPR * a_fRadius, cPR * a_fRadius, sTR * sPR * a_fRadius);
			vector3 pointD(cTR * sPL * a_fRadius, cPL * a_fRadius, sTR * sPL * a_fRadius);

			// The quad at that rotation
			AddQuad(pointA, pointB, pointC, pointD); // A-B-C, C-B-D
		}
	}
	//Your code ends here
	CompileObject(a_v3Color);
}
#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "RE\ReEngAppClass.h"

class Camera
{
private:
	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 up;
	glm::quat orientation;
	glm::mat4 projPerspective;
	glm::mat4 projOrthographic;

public:
	Camera();
	~Camera();

	glm::mat4 GetView(void);
	glm::mat4 GetProjection(bool ortho);
	void SetPosition(glm::vec3 pos);
	void SetTarget(glm::vec3 target);
	void SetUp(glm::vec3 up);
	void MoveForward(float increment);
	void MoveSideways(float increment);
	void MoveVertical(float increment);
	void ChangePitch(float increment);
	void ChangeRoll(float increment);
	void ChangeYaw(float increment);
};

#endif
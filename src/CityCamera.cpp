#include "CityCamera.hpp"
#include "CityApplication.hpp"
#include "GL/glfw.h"
#include <cmath>

using namespace stein;
using namespace std;

CityCamera::CityCamera() :
	Camera(), m_nextMove(), m_xMousePosition(), m_yMousePosition(), MOVE_STEP(3.0)
{}

CityCamera::~CityCamera()
{}

void CityCamera::cancelMovement() {
	m_nextMove = glm::vec3(0., 0., 0.);
}
void CityCamera::setMouseMovement(int deltaX, int deltaY) {
	m_xMousePosition += 10. * (deltaX / (GLfloat)1024);
	m_yMousePosition += -10. * (deltaY / (GLfloat)1024);
}

void CityCamera::setKeyMovement(Direction to) {
	size_t axis;

	if(to == FORWARD || to == BACKWARD)
		axis = FORWARD;
	else if (to == RIGHT || to == LEFT)
		axis = RIGHT;
	else
		axis = UP;
	m_nextMove[axis] += (to == axis) ? 1. : -1.;
}

void CityCamera::move() {
	glm::vec3 cameraNewPos;

	float moveOnX = -m_nextMove[0] * MOVE_STEP;
	float moveOnY = m_nextMove[1] * MOVE_STEP;
	float moveOnZ = m_nextMove[2] * MOVE_STEP;
	for(size_t iCoord=0; iCoord<3; ++iCoord)
	{
		cameraNewPos[iCoord]=position[iCoord]
			+xAxis[iCoord]*moveOnX
			+yAxis[iCoord]*moveOnY
			+zAxis[iCoord]*moveOnZ;
	}
	
	glm::mat4 rotateAroundX = xRotation(-float(m_yMousePosition * M_PI/2.));
	glm::mat4 rotateAroundY = yRotation(float(m_xMousePosition * M_PI));
	glm::mat4 translate = translation(cameraNewPos);

	view = glm::mat4();
	view =  view * rotateAroundX * rotateAroundY * translate;
	int iCoord;
	for(iCoord=0; iCoord<3; ++iCoord)
	{
		//Updates the axis with values in view
		xAxis[iCoord]=view[iCoord][0];
		yAxis[iCoord]=view[iCoord][1];
		zAxis[iCoord]=view[iCoord][2];
		//Updates the position of the camera c
		position[iCoord] = cameraNewPos[iCoord];
	}
	cancelMovement();
}

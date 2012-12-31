#ifndef _CITYAPPLICATION_H_
#define _CITYAPPLICATION_H_

#include "stein/Application.hpp"
#include "stein/Tools.hpp"
#include "stein/Builders.hpp"
#include "stein/Object.hpp"
#include "stein/FramebufferGL.hpp"
#include "stein/Color.hpp"
#include "City/Grid.hpp"
#include "CityCamera.hpp"

class CityApplication : public stein::Application {
public:
    CityApplication();
    void animate();
    void mouseMotionEvent();
	void mouseButtonEvent();
    void keyEvent();
    CityCamera* initCamera(const float size, glm::vec3 position);
    void loadShaders();
	void loadTextures();
    void loadFrameBuffer();
    void renderFrame();

    int _exMouseXPos; // Keeps the X mouse position of the previous image
	int _exMouseYPos; // Keeps the Y mouse position of the previous image
	bool _leftButtonPressed; // Keeps the left button of the mouse value (PRESS or RELEASE)
	float _bias;
    GLuint shaders[4];
    GLuint textures[4][3];
    FramebufferGL buffers[4];
private:

};

#endif /* CitySAPPLICATION_H_ */

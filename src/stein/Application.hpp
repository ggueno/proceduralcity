#pragma once


#include "Scene.hpp"
#include "GLHeaders.hpp"
#include <GL/glfw.h>

namespace stein {

class Application {
public :
    Application(size_t width, size_t height);
    virtual ~Application();

    void loop();

    size_t getWindowWidth() const;
    size_t getWindowHeight() const;
    void setTitle(const char* title);

protected:
    Scene m_scene;

    // Mouse and keys management
    bool m_bShowMouse; // True if mouse is seeable
    int m_mouseXPos; // Mouse position - horizontal axis
    int m_mouseYPos; // mouse position - vertical axis   
    GLfloat m_pressedMouseXPos; // Mouse position - updated only when left button down
    GLfloat m_pressedMouseYPos; // mouse position - updated only when left button down
    int m_mouseScroll; // scroll value (up : ++, down : --)

    void resize(GLuint w, GLuint h);
    void setBackgroundColor(const Color &color);
    
    void hideCursor(char key);
    void printFPS();

    virtual void mouseMotionEvent();
    virtual void mouseButtonEvent();
    virtual void keyEvent();
    virtual void animate();
    virtual void renderFrame();

    size_t frameCount() const;

    void initGlfw();

    // Window
    size_t m_windowedWidth; // Window dimensions when not fullscreen - horizontal axis
    size_t m_windowedHeight; // Window dimensions when not fullscreen - vertical axis
    size_t m_fullScreenWidth; // Screen dimensions - horizontal axis
    size_t m_fullScreenHeight; // Screen dimensions - vertical axis
    size_t m_width; // Window actual dimensions - horizontal axis
    size_t m_height; // Window actual dimensions - vertical axis

    // Frame management
    GLuint m_frameCount; // Frame counter
    double m_lastStartTime; // Time updated each frame
    double m_lastSecondTime; // Time updated each second
    double m_frameDuration; // Frame time duration
};
}
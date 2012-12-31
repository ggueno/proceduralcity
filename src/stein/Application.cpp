#include "Application.hpp"
#include "GLHeaders.hpp"
#include "Scene.hpp"
#include <iostream>

namespace stein {

Application::Application(size_t width, size_t height) : m_width(width), m_height(height), m_frameCount(0), m_lastStartTime(0.), m_lastSecondTime(0.) {
    // Mouse position, pressed position and scroll data initilaization
    m_bShowMouse = bool(false);
    m_mouseXPos = m_width/2;
    m_mouseYPos = m_height/2;
    m_pressedMouseXPos = 0;
    m_pressedMouseYPos = 0;

    m_mouseScroll = 0;
    m_frameDuration = 1. / 30.;

    // Initialisation of GLFW and creation of OpenGL context
    initGlfw();
    // Customize a few OpenGL and GLFW states (after context creation)
    //customizeStates();
    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos( m_mouseXPos, m_mouseYPos );
}

// Cleans before the application can be closed
Application::~Application() {
    glfwTerminate();
}

// Inits SDL and OpenGL context, sets a few states
void Application::initGlfw() {
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
    
    #ifdef __APPLE__
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
        glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    // Open a window and create its OpenGL context
    if(!glfwOpenWindow(m_width, m_height, 0,0,0,0, 24,0, GLFW_WINDOW)) {
        fprintf(stderr, "Failed to open GLFW window\n");

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    #ifdef __APPLE__
        glewExperimental = GL_TRUE;
    #endif

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
          /* Problem: glewInit failed, something is seriously wrong. */
          fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
          exit(EXIT_FAILURE);
    }

    // Ensure we can capture the escape key being pressed below
    glfwEnable(GLFW_STICKY_KEYS);

    // Enable vertical sync (on cards that support it)
    glfwSwapInterval(1);

    // Depth test
    glEnable(GL_DEPTH_TEST);
    
    // Decides the background color used after this call
    setBackgroundColor(Color::BLACK);
    
    // Sets the width of the lines
    glLineWidth(2);
}

// Adapts the drawing to the new size of the window
// resize doesn't work on Mac os (Windows ?)
void Application::resize(GLuint w, GLuint h) {
    std::cout << "Window resize  : [" << w << "," << h << "]" << std::endl;

    m_width = w;
    m_height = h;
    
    // Update window size
    glfwSetWindowSize(m_width, m_height);
    glViewport(0, 0, m_width, m_height);
}

// Sets the background color
void Application::setBackgroundColor(const Color &color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

// Sets the title of the window
void Application::setTitle(const char* title) {
    glfwSetWindowTitle(title);
}

// Hides cursor by pressing key
void Application::hideCursor(char key) {
    if(glfwGetKey(key) == GLFW_RELEASE)
        m_bShowMouse = !m_bShowMouse;

    if(m_bShowMouse != true )
        glfwDisable(GLFW_MOUSE_CURSOR);
    else
        glfwEnable(GLFW_MOUSE_CURSOR);
    
}

void Application::renderFrame() {
    // Clears the window with current clearing color, clears also the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draws scene
    m_scene.drawObjectsOfScene(0);
    
    // Performs the buffer swap between the current shown buffer, and the one we just worked on
    glfwSwapBuffers();
}

void Application::mouseMotionEvent() {}
void Application::mouseButtonEvent() {}
void Application::keyEvent() {}

void Application::animate() {}

void Application::loop() {
    do {
        double t = glfwGetTime();
        if(t - m_lastStartTime >= m_frameDuration) {
            mouseMotionEvent();
            mouseButtonEvent();
            keyEvent();
            animate();
            renderFrame();
            m_lastStartTime = t;
            ++m_frameCount;
            // Reports any possible glError
            //printGlErrors();
        }

        if(t - m_lastSecondTime >= 1.) {
            m_lastSecondTime = t;
            std::cout << "FPS : " << m_frameCount << std::endl;
            m_frameCount = 0;
        }
    } // Check if the ESC key was pressed or the window was closed
    while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));
}

size_t Application::frameCount() const {
    return m_frameCount;
}

size_t Application::getWindowWidth() const {
    return m_width;
}

size_t Application::getWindowHeight() const {
    return m_height;
}

} // namespace stein
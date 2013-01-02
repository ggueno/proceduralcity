#include "CityApplication.hpp"

using namespace std;
using namespace glm;
using namespace stein;

enum shaderType {
    COLOR, GBUFFER, LACCUM, SHADDOW
};

enum textureType {
    BATIMENT
};

enum  frameBufferType {
    FB_GBUFFER, FB_SHADDOW, FB_LACCUM
};

static inline float frand() {
    return float(rand()) / RAND_MAX;
}

CityApplication::CityApplication() :
    stein::Application(800, 800)
    {

    const float size = .04;
    const float near = 0.1;
    const float far = 100;

    //load shaders

    //m_scene.camera.setPerspectiveProjection(-size, size, -size, size, near, far);
    //m_scene.camera.setPosition(vec3(0, 10, -100));

    CityCamera * camera = initCamera(.06, glm::vec3(0., 0., 0.));
    loadShaders();
    loadTextures();
    loadFrameBuffer();
    //draw beast

    //init finalScreen
    Object &quadObject = m_scene.createObject(GL_TRIANGLES);
    buildSquare(quadObject, 1);
    //m_screen = m_scene.addObjectToDraw(quadObject.id);

    // On charge une grille
    Grid * g = new Grid(vec3(100.0,-1.0,-100.0), 200.0, 200.0,6,5.0,1.0);

    // Pour chaque parcels lot de la grille on ajoute un immeuble de la taille (hauteur, largeur) convenus
    // std::vector<Building *> buildings;
    Lot * l = g->getOneLot();

    // //On créer les buildins de base à construire
    Object &building = m_scene.createObject(GL_TRIANGLES);
    buildCube(building);

    GLuint element;


    while(l != NULL){
        //On ajoute un building à la liste avec ses caractéristiques

        //On applique les transformations sur l'objet
        vec3 p = l->getCenter();
        float lot_width = l->getWidth();
        float lot_length = l->getHeight();
        float lot_height = 10.0;

        element = m_scene.addObjectToDraw(building.id);
        //m_scene.setDrawnObjectColor(element, Color(1.0f, 0.0f, 0.0f));
        m_scene.setDrawnObjectTextureID0(element, textures[BATIMENT][0]);
        m_scene.setDrawnObjectTextureID1(element, textures[BATIMENT][1]);
        m_scene.setDrawnObjectTextureID2(element, textures[BATIMENT][2]);

        m_scene.setDrawnObjectModel(element, 
            translation(vec3(p.x,p.y,p.z)) 
            * scalee(vec3(lot_width, lot_height, lot_length))
        );


    
        //buildings[buildings.size()-1]->scalee(1.0, r, 1.0 );
        l = g->getOneLot();
    }


}

// Load shaders
void CityApplication::loadShaders() {
    shaders[COLOR] = loadProgram("shaders/colorShader.glsl");
    shaders[GBUFFER] = loadProgram("shaders/3_gbuffer.glsl");
    shaders[LACCUM] = loadProgram("shaders/3_laccum_spot.glsl");
    //shaders[SHADDOW] = loadProgram("shaders/3_shadowgen.glsl");
    m_scene.setDefaultShaderID(shaders[GBUFFER]);


}

void CityApplication::loadTextures(){
    textures[BATIMENT][0] = loadTexture("textures/spnza_bricks_a_diff.tga", 0);
    textures[BATIMENT][1] = loadTexture("textures/spnza_bricks_a_spec.tga", 1);
    textures[BATIMENT][2] = loadTexture("textures/spnza_bricks_a_bump.png", 2);
}

void CityApplication::loadFrameBuffer(){
   int status = build_framebuffer(buffers[FB_GBUFFER], m_width, m_height, 2);
    if (status == -1)
    {
        fprintf(stderr, "Error on building framebuffer\n");
        exit( EXIT_FAILURE );
    }

    int size = 512;
    status = build_framebuffer(buffers[FB_SHADDOW],size,size,0);

    if (status == -1)
    {
        fprintf(stderr, "Error on building shadow framebuffer\n");
        exit( EXIT_FAILURE );
    }
}

CityCamera * CityApplication::initCamera(const float size, glm::vec3 position) {
    m_scene.camera = new CityCamera();
    m_scene.camera->setPerspectiveProjection(-size, size, -size, size, .1, 500);
    m_scene.camera->setPosition(position);
    return (CityCamera*)m_scene.camera;
}


void CityApplication::mouseMotionEvent() {
    //Updating mouse data
    _exMouseXPos = m_mouseXPos;
    _exMouseYPos = m_mouseYPos;
    glfwGetMousePos(&m_mouseXPos, &m_mouseYPos);

    if(!m_bShowMouse)
        // Mouse Camera Movement
        ((CityCamera*)m_scene.camera)->setMouseMovement(m_mouseXPos - _exMouseXPos, m_mouseYPos - _exMouseYPos);
}

void CityApplication::mouseButtonEvent() {
    if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) _leftButtonPressed = true;
    else _leftButtonPressed = false;
}

void CityApplication::keyEvent() {
    // Get keboard motion + movement setting
    if(glfwGetKey('Z') == GLFW_PRESS) ((CityCamera*)m_scene.camera)->setKeyMovement(FORWARD);
    if(glfwGetKey('Q') == GLFW_PRESS) ((CityCamera*)m_scene.camera)->setKeyMovement(LEFT);
    if(glfwGetKey('S') == GLFW_PRESS) ((CityCamera*)m_scene.camera)->setKeyMovement(BACKWARD);
    if(glfwGetKey('D') == GLFW_PRESS) ((CityCamera*)m_scene.camera)->setKeyMovement(RIGHT);
    if(glfwGetKey('H') == GLFW_PRESS) hideCursor('H');
}


void CityApplication::animate() {
    ((CityCamera*)m_scene.camera)->move();
}


void CityApplication::renderFrame() {
  
  // Get camera matrices
  glBindFramebuffer(GL_FRAMEBUFFER, buffers[FB_GBUFFER].fbo);
  glDrawBuffers(buffers[FB_GBUFFER].outCount, buffers[FB_GBUFFER].drawBuffers);
  glViewport(0, 0, m_width, m_height);

  // Default states
  glEnable(GL_DEPTH_TEST);

  // Clear the front buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind gbuffer shader
  glUseProgram(shaders[GBUFFER]);

  // Upload uniforms
  m_scene.drawObjectsOfScene(shaders[GBUFFER]);


  //LACCUM
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport( 0, 0, m_width, m_height );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // // Bind laccum shader
  glUseProgram(shaders[LACCUM]);

  // // Compute light positions
  glm::vec3 lightPosition(5.0, 5.0, 5.0);
  //float lightPosition[3] = { sin(t) * 10.0, 5.0, cos(t) * 10.0};
  glm::vec3 lightTarget(0.0, 0.0, 0.0);
  glm::vec3 lightDirection;
  glm::vec3 lightUp(0.0, 1.0, 0.0);
  lightDirection = lightTarget - lightPosition;
  lightDirection = glm::normalize(lightDirection);
  glm::vec3 lightColor(1.0, 1.0, 1.0);
  float lightIntensity = 1.0;

  // // Compute locations for light accumulation shader
  float shadowBias = 0.001f;
  float shadowSamples = 1.0;
  float shadowSampleSpread = 800.0;

  static const float MAT4F_M1_P1_TO_P0_P1array[16] =  
  {
    0.5f, 0.f, 0.f, 0.f,
    0.f, 0.5f, 0.f, 0.f,
    0.f, 0.f, 0.5f, 0.f,
    0.5f, 0.5f, 0.5f, 1.f
  };
  glm::mat4 MAT4F_M1_P1_TO_P0_P1 = glm::make_mat4(MAT4F_M1_P1_TO_P0_P1array);
  // Build shadow matrices
  glm::mat4 worldToLight = glm::lookAt(lightPosition, lightTarget, lightUp);
  glm::mat4 shadowProjection = glm::perspective(60.f, 1.f, 1.0f, 1000.f);    
  glm::mat4 projectionLight = worldToLight * shadowProjection;
  glm::mat4 projectionLightBias = projectionLight * MAT4F_M1_P1_TO_P0_P1;

  // // Upload uniforms
  glm::mat4 projMat = glm::ortho(-400.0f, 400.0f, -400.0f, 400.0f, -1.0f, 1.0f );
  glUniformMatrix4fv(glGetUniformLocation(shaders[LACCUM], "Projection"), 1, 0, glm::value_ptr(projMat));
  glUniform1i(glGetUniformLocation(shaders[LACCUM], "Material"), 0);
  glUniform1i(glGetUniformLocation(shaders[LACCUM], "Normal"), 1);
  glUniform1i(glGetUniformLocation(shaders[LACCUM], "Depth"), 2);
  //glUniform1i(glGetUniformLocation(shader[LACCUM], "Shadow"), 3);
  glUniform3fv(glGetUniformLocation(shaders[LACCUM], "CameraPosition"), 1, glm::value_ptr(m_scene.camera->getPosition()));
  glUniformMatrix4fv(glGetUniformLocation(shaders[LACCUM], "InverseViewProjection"), 1, 0, glm::value_ptr(glm::inverse(m_scene.camera->getView()*m_scene.camera->getProjection())));
  glUniformMatrix4fv(glGetUniformLocation(shaders[LACCUM], "ProjectionLight"), 1, 0,glm::value_ptr(projectionLightBias));
  glUniform1f(glGetUniformLocation(shaders[LACCUM], "ShadowBias"),shadowBias);
  glUniform1f(glGetUniformLocation(shaders[LACCUM], "ShadowSamples"),shadowSamples);
  glUniform1f(glGetUniformLocation(shaders[LACCUM], "ShadowSampleSpread"),shadowSampleSpread);

  // Bind color to unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, buffers[FB_GBUFFER].colorTexId[0]);        
  // Bind normal to unit 1
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, buffers[FB_GBUFFER].colorTexId[1]);    
  // Bind depth to unit 2
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, buffers[FB_GBUFFER].depthTexId);        
  // Bind shadow map to unit 3
  //glActiveTexture(GL_TEXTURE3);
  //glBindTexture(GL_TEXTURE_2D, shadow.depthTexId);        

  // Blit above the rest
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

  // Light uniforms
  glUniform3fv(glGetUniformLocation(shaders[LACCUM], "LightPosition"), 1, glm::value_ptr(lightDirection));
  glUniform3fv(glGetUniformLocation(shaders[LACCUM], "LightDirection"), 1, glm::value_ptr(lightPosition));
  glUniform3fv(glGetUniformLocation(shaders[LACCUM], "LightColor"), 1, glm::value_ptr(lightColor));
  glUniform1f(glGetUniformLocation(shaders[LACCUM], "LightIntensity"), lightIntensity);

  // Draw quad
  m_scene.drawObject(m_screen);

  glDisable(GL_BLEND);

  // Performs the buffer swap between the current shown buffer, and the one we just worked on
  glfwSwapBuffers();
}
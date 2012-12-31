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
    FB_GBUFFER, FB_SHADDOW
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
    //draw beast

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
    shaders[SHADDOW] = loadProgram("shaders/3_shadowgen.glsl");
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
    std::cout << "renderFrame" << std::endl;

            // Get camera matrices

       //  float viewProjection[16];
       //  float iviewProjection[16];

       //  mat4fMul( worldToView, projection, viewProjection);
       //  mat4fInverse(viewProjection, iviewProjection);

        //glBindFramebuffer(GL_FRAMEBUFFER, buffers[FB_GBUFFER].fbo);
        //glDrawBuffers(buffers[FB_GBUFFER].outCount, buffers[FB_GBUFFER].drawBuffers);

        // Viewport 
        //camera.setViewport(0, 0, width, height);

        // Default states
        glEnable(GL_DEPTH_TEST);

        // Clear the front buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


       //  // Default states
       //  glDepthMask(GL_FALSE);
       //  glEnable( GL_BLEND );
       //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

       //  glUseProgram( sky_shader.program );
       //  glUniformMatrix4fv(sky_projectionLocation, 1, 0, projection);
       //  glUniformMatrix4fv(sky_viewLocation, 1, 0, worldToView);
       //  glUniformMatrix4fv(sky_objectLocation, 1, 0, objectToWorld);
       //  glUniform1i( sky_skyTexLocation, 0 );

       //  // Bind sky texture and draw
       //  glActiveTexture( GL_TEXTURE0 );
       //  glBindTexture( GL_TEXTURE_2D, textures[2] );
       //  glBindVertexArray( vao[0] );
       //  glDrawElements( GL_TRIANGLES, sky_triangleCount*3, GL_UNSIGNED_INT, (void*)0 );
       //  glDisable(GL_BLEND);
       //  glDepthMask(GL_TRUE);

        // Bind gbuffer shader
        glUseProgram(shaders[GBUFFER]);
        // Upload uniforms
        m_scene.drawObjectsOfScene(shaders[GBUFFER]);

       //  //b->drawMeshSimple();
       // //g->draw();

       //  //cylinder->drawMeshSimple();
       //  //cone->drawMeshSimple();
       //  //plane->drawObject();

       //  glActiveTexture(GL_TEXTURE0);
       //  glBindTexture(GL_TEXTURE_2D, textures[3]);
       //  s->drawMeshSimple();

       //  glBindFramebuffer(GL_FRAMEBUFFER, 0);
       //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       //  // Compute light positions
       //  //float lightPosition[3] = { 0.0, 25.0, 0.0};
       //  float lightPosition[3] = { 0.0, 50.0, 0.0};
       //  float lightTarget[3] = { 0.0, 5.0, 0.0};
       //  float lightDirection[3];
       //  float lightUp[3] = { 0.0, 1.0, 0.0};
       //  vec3fSub(lightTarget, lightPosition, lightDirection);
       //  vec3fNormalize(lightDirection, vec3fNorm(lightDirection));
       //  float lightColor[3] = {1.0, 1.0, 1.0};
       //  float lightIntensity = 1.0;


       //  // Build shadow matrices
       //  float shadowProjection[16];
       //  float worldToLight[16];
       //  lookAt(lightPosition, lightTarget, lightUp, worldToLight);
       //  perspective(60.f, 1.f, 1.0f, 1000.f, shadowProjection );
       //  float projectionLight[16];
       //  float projectionLightBias[16];
       //  mat4fMul( worldToLight, shadowProjection,  projectionLight);
       //  mat4fMul( projectionLight, MAT4F_M1_P1_TO_P0_P1, projectionLightBias);

       //  glBindFramebuffer(GL_FRAMEBUFFER,shadowBuffer.fbo);
       //  glDrawBuffers(shadowBuffer.outCount, shadowBuffer.drawBuffers);
       //  // Clear the front buffer
       //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       //  // Viewport
       //  glViewport( 0, 0, size, size  );
       //  camera.setViewport(0, 0, size, size);

       //  // Default states
       //  glEnable(GL_DEPTH_TEST);

       //  // Bind gbuffer shader
       //  glUseProgram(shadowgen_shader.program);
       //  // Upload uniforms
       //  glUniformMatrix4fv(shadowgen_projectionLocation, 1, 0, shadowProjection);
       //  glUniformMatrix4fv(shadowgen_viewLocation, 1, 0, worldToLight);
       //  glUniformMatrix4fv(shadowgen_objectLocation, 1, 0, objectToWorld);
       //  glUniform1f(shadowgen_timeLocation, t);

       //  // for(int i=0; i<buildings.size(); ++i){
       //  //     buildings[i]->drawObject(shadowgen_shader.program);
       //  // }


       //  glBindFramebuffer(GL_FRAMEBUFFER, 0);

       //  glViewport( 0, 0, width, height  );

       //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       //  // Bind laccum shader
       //  glUseProgram(laccum_shader.program);
       //  // Upload uniforms
       //  glUniformMatrix4fv(laccum_projectionLocation, 1, 0, orthoProj);
       //  glUniformMatrix4fv(laccum_viewLocation, 1, 0, worldToLight);
       //  glUniformMatrix4fv(laccum_objectLocation, 1, 0, objectToWorld);
       //  glUniform1i(laccum_materialLocation, 0);
       //  glUniform1i(laccum_normalLocation, 1);
       //  glUniform1i(laccum_depthLocation, 2);
       //  glUniform1i(laccum_shadow, 3);
       //  glUniform3fv(laccum_cameraPositionLocation, 1, cameraPosition);
       //  glUniformMatrix4fv(laccum_inverseViewProjectionLocation, 1, 0, iviewProjection);
       //  glUniformMatrix4fv(laccum_projectionLightBias, 1, 0, projectionLightBias);
       //  glUniform1f(laccum_bias,shadowBias);
       //  glUniform1f(laccum_sample,samples);
       //  glUniform1f(laccum_spread,spread);

       //  // Bind color to unit 0
       //  glActiveTexture(GL_TEXTURE0);
       //  glBindTexture(GL_TEXTURE_2D, gbuffer.colorTexId[0]);
       //  // Bind normal to unit 1
       //  glActiveTexture(GL_TEXTURE1);
       //  glBindTexture(GL_TEXTURE_2D, gbuffer.colorTexId[1]);
       //  // Bind depth to unit 2
       //  glActiveTexture(GL_TEXTURE2);
       //  glBindTexture(GL_TEXTURE_2D, gbuffer.depthTexId);

       //  glActiveTexture(GL_TEXTURE3);
       //  glBindTexture(GL_TEXTURE_2D, shadowBuffer.depthTexId);

       //  // Blit above the rest
       //  glDisable(GL_DEPTH_TEST);

       //  glEnable(GL_BLEND);
       //  glBlendFunc(GL_ONE, GL_ONE);

       //  // Light uniforms
       //  glUniform3fv(laccum_lightDirectionLocation, 1, lightDirection);
       //  glUniform3fv(laccum_lightPositionLocation, 1, lightPosition);
       //  glUniform3fv(laccum_lightColorLocation, 1, lightColor);
       //  glUniform1f(laccum_lightIntensityLocation, lightIntensity);
       //  //glUniform1i(laccum_shadow, shadowBuffer.depthTexId);

       //  // Draw quad
       //  quad->drawMeshSimple();
       //  glDisable(GL_BLEND);


     // Performs the buffer swap between the current shown buffer, and the one we just worked on
    glfwSwapBuffers();

}
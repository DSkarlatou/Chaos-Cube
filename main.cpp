#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader_m.h"
#include "objloader.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;
#include <cstdlib>
#include <ctime>

// prototypes
void KeysCheck();
void spawnShape();
void updateAllObjectPositions(float scale, glm::vec3 p, glm::vec3 v, int j);
void updateObjPosition(float scale, vector<vector<glm::vec3>> object, glm::vec3 p, glm::vec3 v, int j);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void checkOnAxis(int shape, double scale, int index);
void makeCube();
void makeSphere();
void makeCylinder();
float getRandomScale();

glm::vec3  movement(float scale, int sh, glm::vec3 p, glm::vec3 v, glm::vec3 c, int i);

// settings
const unsigned int SCR_WIDTH    = 600;
const unsigned int SCR_HEIGHT   = 600;
float TRANSPARENCY = 0.4f; 

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float SPEED = 50.0f;

// camera start settings
glm::vec3 cameraPos   = glm::vec3(180.0f, 150.0f, -130.0f); 
glm::vec3 cameraFront = glm::vec3(50.0f, 50.0f, 50.0f);     
glm::vec3 cameraUp    = glm::vec3(0.0, 0.5f, 0.0f);         

// sphere start position
GLfloat PosSX = 50.0f; 
GLfloat PosSY = 50.0f;
GLfloat PosSZ = 50.0f;

int numberOfCubes = 0;
int numberOfSpheres= 0;
int numberOfCylinders = 0;
vector<vector<glm::vec3>> cubes;
vector<vector<glm::vec3>> spheres;
vector<vector<glm::vec3>> cylinders;

GLFWwindow* window; //global means easier (/~.~)/

// lighting
glm::vec3 lightPos(120.0f, 120.0f, 50.0f);

unsigned int VBO, cubeVAO, lightCubeVAO;

// Read .obj file //THIS BLOCK IS FOR SPHERE OBJECT
std::vector<glm::vec3> objsVerts;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals; 
bool res = loadOBJ("sphere10.obj", objsVerts, uvs, normals);
GLuint vertexbuffer;
GLuint uvbuffer;

// Read .obj file //THIS BLOCK IS FOR CUBE OBJECT
std::vector<glm::vec3> objsVertsCube;
std::vector<glm::vec2> uvsCube;
std::vector<glm::vec3> normalsCube;
bool resCube = loadOBJ("cube10.obj", objsVertsCube, uvsCube, normalsCube);
GLuint vertexbufferCube;
GLuint uvbufferCube;

// Read .obj file //THIS BLOCK IS FOR CYLINDER OBJECT
std::vector<glm::vec3> objsVertsCyl;
std::vector<glm::vec2> uvsCyl;
std::vector<glm::vec3> normalsCyl; 
bool resCyl = loadOBJ("cylinder-r5-h10.obj", objsVertsCyl, uvsCyl, normalsCyl);
GLuint vertexbufferCyl;
GLuint uvbufferCyl;

Shader Shape; //create global shader object, it will be used by all shapes that are added by SPACEBAR

float vertices[] = { // OCD is useful sometimes \(~.~\)
         0.0f,    0.0f,    0.0f,    0.0f,  0.0f, -1.0f,
         100.0f,  0.0f,    0.0f,    0.0f,  0.0f, -1.0f,
         100.0f,  100.0f,  0.0f,    0.0f,  0.0f, -1.0f,
         100.0f,  100.0f,  0.0f,    0.0f,  0.0f, -1.0f,
         0.0f,    100.0f,  0.0f,    0.0f,  0.0f, -1.0f,
         0.0f,    0.0f,    0.0f,    0.0f,  0.0f, -1.0f,

         0.0f,    0.0f,    100.0f,  0.0f,  0.0f,  1.0f,
         100.0f,  0.0f,    100.0f,  0.0f,  0.0f,  1.0f,
         100.0f,  100.0f,  100.0f,  0.0f,  0.0f,  1.0f,
         100.0f,  100.0f,  100.0f,  0.0f,  0.0f,  1.0f,
         0.0f,    100.0f,  100.0f,  0.0f,  0.0f,  1.0f,
         0.0f,    0.0f,    100.0f,  0.0f,  0.0f,  1.0f,

         0.0f,    100.0f,  100.0f, -1.0f,  0.0f,  0.0f,
         0.0f,    100.0f,  0.0f,   -1.0f,  0.0f,  0.0f,
         0.0f,    0.0f,    0.0f,   -1.0f,  0.0f,  0.0f,
         0.0f,    0.0f,    0.0f,   -1.0f,  0.0f,  0.0f,
         0.0f,    0.0f,    100.0f, -1.0f,  0.0f,  0.0f,
         0.0f,    100.0f,  100.0f, -1.0f,  0.0f,  0.0f,

         100.0f,  100.0f,  100.0f,  1.0f,  0.0f,  0.0f,
         100.0f,  100.0f,  0.0f,    1.0f,  0.0f,  0.0f,
         100.0f,  0.0f,    0.0f,    1.0f,  0.0f,  0.0f,
         100.0f,  0.0f,    0.0f,    1.0f,  0.0f,  0.0f,
         100.0f,  0.0f,    100.0f,  1.0f,  0.0f,  0.0f,
         100.0f,  100.0f,  100.0f,  1.0f,  0.0f,  0.0f,

         0.0f,    0.0f,    0.0f,    0.0f, -1.0f,  0.0f,
         100.0f,  0.0f,    0.0f,    0.0f, -1.0f,  0.0f,
         100.0f,  0.0f,    100.0f,  0.0f, -1.0f,  0.0f,
         100.0f,  0.0f,    100.0f,  0.0f, -1.0f,  0.0f,
         0.0f,    0.0f,    100.0f,  0.0f, -1.0f,  0.0f,
         0.0f,    0.0f,    0.0f,    0.0f, -1.0f,  0.0f,

         0.0f,    100.0f,  0.0f,    0.0f,  1.0f,  0.0f,
         100.0f,  100.0f,  0.0f,    0.0f,  1.0f,  0.0f,
         100.0f,  100.0f,  100.0f,  0.0f,  1.0f,  0.0f,
         100.0f,  100.0f,  100.0f,  0.0f,  1.0f,  0.0f,
         0.0f,    100.0f,  100.0f,  0.0f,  1.0f,  0.0f,
         0.0f,    100.0f,  0.0f,    0.0f,  1.0f,  0.0f
};

int main()
{
    // Initialize and set up glfw
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // creating the window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, u8"Colliding Objects", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // displaying the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, key_callback);;

    // initializing glew
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // settings for depth and transparency
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); //CLOCKWISE

    // Shaders
    Shader lightingShader("Shaders/basic_lighting.vs", "Shaders/basic_lighting.fs");    // FOR THE SCENE CUBE
    Shader sphereShader("Shaders/basic_shader.vs", "Shaders/basic_shader.fs");          // FOR THE BIG SPHERE
    Shader Shape("Shaders/basic_shader.vs", "Shaders/basic_shader.fs");                 // FOR NEW SHAPES

    // for the scene cube
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // This little block is to get some random numbers for the cube's color
    srand(static_cast <unsigned> (time(0)));
    float Bred   = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float Bgreen = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float Bblue  = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    printf("\nThe cube's colour constists of these (random) values\n");
    printf("RED: %f, GREEN: %f, BLUE: %f\n", Bred, Bgreen, Bblue);

    // rendering
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
    {
        glm::mat4 model = glm::mat4(1.0f);
        
        // TIMING
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float speed = SPEED * deltaTime;

        // CHECK KEYBOARD INPUT
        KeysCheck(); 

        // BLACK BACKGROUND
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat Aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
        glm::mat4 projection = glm::perspective(45.0f, Aspect, 10.0f, 1000.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);

        makeSphere();// rendering the large sphere

        int totalNumberOfShapes = numberOfCubes + numberOfSpheres + numberOfCylinders;
        int j = 0;
        while (j < totalNumberOfShapes) {

            for (int i = 0; i < numberOfCubes; i++)
            {
                glm::mat4 modelS = glm::mat4(1.0f);
                float red, green, blue;
                float posx, posy, posz;
                float vx, vy, vz;
                float scale;
                // accessing stuff for coloring, position , vectors
                red = cubes[i][0].x;
                green = cubes[i][0].y;
                blue = cubes[i][0].z;
                posx = cubes[i][1].x;
                posy = cubes[i][1].y;
                posz = cubes[i][1].z;
                scale = cubes[i][2].x;

                vx = cubes[i][3].x;
                vy = cubes[i][3].y;
                vz = cubes[i][3].z;

                glm::vec3 p(posx, posy, posz);
                glm::vec3 c(red,green,blue);
                glm::vec3 v(vx, vy, vz);

                // give to movement (returns a direction)
                glm::vec3 vl = movement(scale, 1, p, v,c,i);

                // update direction
                cubes[i][3].x = vl.x;
                cubes[i][3].y = vl.y;
                cubes[i][3].z = vl.z;

                // update position
                cubes[i][1].x += cubes[i][3].x * speed;
                cubes[i][1].y += cubes[i][3].y * speed;
                cubes[i][1].z += cubes[i][3].z * speed;

                Shape.use();
                Shape.setVec4("Color", red, green, blue, 1.0f);
                Shape.setMat4("Projection", projection);
                Shape.setMat4("View", view);
                glm::vec3 posCube(posx, posy, posz);
                modelS = glm::translate(modelS, posCube);
                modelS = glm::scale(modelS, glm::vec3(scale)); // no need to scale
                Shape.setMat4("Model", modelS);

                makeCube();

                j++;
            }
            for (int i = 0; i < numberOfSpheres; i++)
            {
                glm::mat4 modelS = glm::mat4(1.0f);
                float red, green, blue;
                float posx, posy, posz;
                float vx, vy, vz;
                float scale;
                // accessing stuff for coloring, position , vectors
                red = spheres[i][0].x;
                green = spheres[i][0].y;
                blue = spheres[i][0].z;
                posx = spheres[i][1].x;
                posy = spheres[i][1].y;
                posz = spheres[i][1].z;
                scale = spheres[i][2].x;

                vx = spheres[i][3].x;
                vy = spheres[i][3].y;
                vz = spheres[i][3].z;

                glm::vec3 p(posx, posy, posz);
                glm::vec3 c(red, green, blue);
                glm::vec3 v(vx,vy,vz);

                // give to movement (returns a direction)
                glm::vec3 vl = movement(scale, 2, p,v,c,i);

                // update direction
                spheres[i][3].x = vl.x;
                spheres[i][3].y = vl.y;
                spheres[i][3].z = vl.z;

                // update position
                spheres[i][1].x += spheres[i][3].x * speed;
                spheres[i][1].y += spheres[i][3].y * speed;
                spheres[i][1].z += spheres[i][3].z * speed;
                
                Shape.use();
                Shape.setVec4("Color", red, green, blue, 1.0f);
                Shape.setMat4("Projection", projection);
                Shape.setMat4("View", view);
                glm::vec3 posSphere(posx, posy, posz);
                modelS = glm::translate(modelS, posSphere);
                modelS = glm::scale(modelS, glm::vec3(scale)); // no need to scale
                Shape.setMat4("Model", modelS);

                makeSphere();

                j++;
            }
            for (int i = 0; i < numberOfCylinders; i++)
            {
                glm::mat4 modelS = glm::mat4(1.0f);
                float red, green, blue;
                float posx, posy, posz;
                float vx, vy, vz;
                float scale;
                // accessing stuff for coloring, position , vectors
                red = cylinders[i][0].x;
                green = cylinders[i][0].y;
                blue = cylinders[i][0].z;
                posx = cylinders[i][1].x;
                posy = cylinders[i][1].y;
                posz = cylinders[i][1].z;
                scale = cylinders[i][2].x;

                vx = cylinders[i][3].x;
                vy = cylinders[i][3].y;
                vz = cylinders[i][3].z;

                glm::vec3 p(posx, posy, posz);
                glm::vec3 c(red, green, blue);
                glm::vec3 v(vx, vy, vz);

                // give to movement (returns a direction)
                glm::vec3 vl = movement(scale, 3, p, v, c, i);

                // update direction
                cylinders[i][3].x = vl.x;
                cylinders[i][3].y = vl.y;
                cylinders[i][3].z = vl.z;

                // update position
                cylinders[i][1].x += cylinders[i][3].x * speed;
                cylinders[i][1].y += cylinders[i][3].y * speed;
                cylinders[i][1].z += cylinders[i][3].z * speed;

                Shape.use();
                Shape.setVec4("Color", red, green, blue, 1.0f);
                Shape.setMat4("Projection", projection);
                Shape.setMat4("View", view);
                glm::vec3 posCylinder(posx, posy, posz);
                modelS = glm::translate(modelS, posCylinder);
                modelS = glm::scale(modelS, glm::vec3(scale)); // no need to scale
                Shape.setMat4("Model", modelS);

                makeCylinder();

                j++;
            } 
        }
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec4("objectColor", Bred, Bgreen, Bblue, TRANSPARENCY);
        lightingShader.setVec4("lightColor", 1.0f, 1.0f, 1.0f, 1.0f); //White light color
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", 100.0f, 120.0f, 30.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(lightCubeVAO);
  
        // Draw the big sphere (this part is last)
        sphereShader.use();
        sphereShader.setVec4("Color", 1.0f, 0.0f, 0.0f, 1.0f);
        sphereShader.setMat4("Projection", projection);
        sphereShader.setMat4("View", view);
        model = glm::mat4(1.0f);
        glm::vec3 pos(PosSX, PosSY, PosSZ);
        model = glm::translate(model, pos);
        model = glm::scale(model, glm::vec3(3.0f)); // my sphere has radius 5, so 5m*3.0 = 15.0 radius for big sphere
        sphereShader.setMat4("Model", model);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // they've outlived their purpose:
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate
    glfwTerminate();
    return 0;
}

// Updating positions
glm::vec3 movement(float scale, int sh, glm::vec3 p, glm::vec3 v, glm::vec3 c, int index)
{// int sh: tells me the shape, 1->cube, 2->sphere, 3->cylinder 
 // int element tells me the index in the array the shape I'm working with is in
    float distance;
    glm::vec3 separation;
    float distance2;
    glm::vec3 separation2;
    float scale2;
    int totalNumberOfShapes = numberOfCubes + numberOfSpheres + numberOfCylinders;
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float l = (scale * 10) / 2;
    checkOnAxis(sh, scale, index);
    //TODO: refactor
    if (sh == 1) // cube
    {
        if (p.x > 100.0f - l || p.x < 0.0f + l)
        {
            v.x = -v.x;
            cubes[index][0].x = r;
            cubes[index][0].y = g;
            cubes[index][0].z = b;
            
        }
        if (p.y > 100.0f - l || p.y < 0.0f + l)
        {
            v.y = -v.y;
            cubes[index][0].x = r;
            cubes[index][0].y = g;
            cubes[index][0].z = b;
        }
        if (p.z > 100.0f - l || p.z < 0.0f + l)
        {
            v.z = -v.z;
            cubes[index][0].x = r;
            cubes[index][0].y = g;
            cubes[index][0].z = b;
        }
    }

    if (sh == 2) // sphere
    {
        if (p.x > 100.0f - l || p.x < 0.0f + l)
        {
            v.x = -v.x;
            spheres[index][0].x = r;
            spheres[index][0].y = g;
            spheres[index][0].z = b;
        }
        if (p.y > 100.0f - l || p.y < 0.0f + l)
        {
            v.y = -v.y;
            spheres[index][0].x = r;
            spheres[index][0].y = g;
            spheres[index][0].z = b;
        }
        if (p.z > 100.0f - l || p.z < 0.0f + l)
        {
            v.z = -v.z;
            spheres[index][0].x = r;
            spheres[index][0].y = g;
            spheres[index][0].z = b;
        }
    }

    if (sh == 3) // cylinder
    {
        if (p.x > 100.0f - l || p.x < 0.0f + l)
        {
            v.x = -v.x;
            cylinders[index][0].x = r;
            cylinders[index][0].y = g;
            cylinders[index][0].z = b;
        }
        if (p.y > 100.0f - l || p.y < 0.0f + l)
        {
            v.y = -v.y;
            cylinders[index][0].x = r;
            cylinders[index][0].y = g;
            cylinders[index][0].z = b;
        }
        if (p.z > 100.0f - l || p.z < 0.0f + l)
        {
            v.z = -v.z;
            cylinders[index][0].x = r;
            cylinders[index][0].y = g;
            cylinders[index][0].z = b;
        }
    }

    // This block is responsible for handling the big ball collision
    separation.x = p.x + v.x - PosSX;
    separation.y = p.y + v.y - PosSY;
    separation.z = p.z + v.z - PosSZ;
    distance = sqrt(pow(separation.x, 2) + pow(separation.y, 2) + pow(separation.z, 2));
    if (distance < l + 15.0f)
    {
        v.x = -v.x;
        v.y = -v.y;
        v.z = -v.z;
        if (sh == 1)
        {
            cubes[index][0].x = r;
            cubes[index][0].y = g;
            cubes[index][0].z = b;
        }
        if (sh == 2)
        {
            spheres[index][0].x = r;
            spheres[index][0].y = g;
            spheres[index][0].z = b;
        }
        if (sh == 3)
        {
            cylinders[index][0].x = r;
            cylinders[index][0].y = g;
            cylinders[index][0].z = b;
        }
    }


    int j = index + 1;
    updateAllObjectPositions(scale, p, v, j);

    //-----------------------------

    glm::vec3 position(v.x, v.y, v.z);
    return position;
}

void makeCube()
{
    glGenBuffers(1, &vertexbufferCube);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferCube);
    glBufferData(GL_ARRAY_BUFFER, objsVertsCube.size() * sizeof(glm::vec3), &objsVertsCube[0], GL_STATIC_DRAW);
    glGenBuffers(1, &uvbufferCube);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferCube);
    glBufferData(GL_ARRAY_BUFFER, uvsCube.size() * sizeof(glm::vec2), &uvsCube[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferCube);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);// 2nd attribute buffer : UVs
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferCube);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, objsVertsCube.size());       // Draw the triangles !
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void makeSphere() {

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, objsVerts.size() * sizeof(glm::vec3), &objsVerts[0], GL_STATIC_DRAW);
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);// 2nd attribute buffer : UVs
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, objsVerts.size());       // Draw the triangles !

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void makeCylinder() {

    glGenBuffers(1, &vertexbufferCyl);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferCyl);
    glBufferData(GL_ARRAY_BUFFER, objsVertsCyl.size() * sizeof(glm::vec3), &objsVertsCyl[0], GL_STATIC_DRAW);
    glGenBuffers(1, &uvbufferCyl);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferCyl);
    glBufferData(GL_ARRAY_BUFFER, uvsCyl.size() * sizeof(glm::vec2), &uvsCyl[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferCyl);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);// 2nd attribute buffer : UVs
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferCyl);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, objsVertsCyl.size());       // Draw the triangles !

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
// resizing window and all that the window contains
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void KeysCheck()
{
    // for camera movement
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W)) 
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S))
        cameraPos += -cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A))
        cameraPos += cameraSpeed * glm::cross(cameraFront, cameraUp);
    if (glfwGetKey(window, GLFW_KEY_D))
        cameraPos += -cameraSpeed * glm::cross(cameraFront, cameraUp);
    if (glfwGetKey(window, GLFW_KEY_E))
        cameraPos += -(cameraSpeed * 50) * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_X))
        cameraPos += (cameraSpeed * 50) * cameraUp;

    // for sphere movement
    if (glfwGetKey(window, GLFW_KEY_RIGHT))
        if (PosSX <= 0 + 15.0f)
            PosSX = 0.0f + 15.0f;
        else
            PosSX = PosSX -= 1.0f; //Position Sphere on X axis (smart naming explanation \(~.~\) )
    if (glfwGetKey(window, GLFW_KEY_LEFT))
        if (PosSX >= 100 - 15.0f)
            PosSX = 100.0f - 15.0f;
        else
            PosSX = PosSX += 1.0f; 
    if (glfwGetKey(window, GLFW_KEY_DOWN))
        if (PosSY <= 0 + 15.0f)
            PosSY = 0.0f + 15.0f;
        else
            PosSY = PosSY -= 1.0f; 
    if (glfwGetKey(window, GLFW_KEY_UP))
        if (PosSY >= 100 - 15.0f)
            PosSY = 100.0f - 15.0f;
        else
            PosSY = PosSY += 1.0f; 
    if (glfwGetKey(window, GLFW_KEY_KP_ADD))
        if (PosSZ <= 0 + 15.0f)
            PosSZ = 0.0f + 15.0f;
        else
            PosSZ = PosSZ -= 1.0f; 
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT))
        if (PosSZ >= 100 - 15.0f)
            PosSZ = 100.0f - 15.0f;
        else
            PosSZ = PosSZ += 1.0f; 

    // for speed increase
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))
    {
        if (glfwGetKey(window, GLFW_KEY_COMMA))
        {
            if (SPEED <= 0)
            {
                SPEED == 0;
                return; //cannot allow to have a negative speed! Please stop!
            }
            SPEED -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_PERIOD))
        {
            SPEED += 1;
        }
    }
}

float getRandomScale() //for scaling
{
    srand(time(NULL)); //initialize the random seed
    float arrayNum[10] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    int RandIndex = rand() % 10; //generates a random number between 0 and 10
    float result = arrayNum[RandIndex];
    while (result == 0) 
    {
        int RandIndex = rand() % 10; //generates a random number between 0 and 10
        result = arrayNum[RandIndex];
    }
    return result;
}

// this is just for handling space press, only registers one press, even if you hold it down
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        printf("\nYou just hit space, spawning a new shape!\n\n");
        spawnShape();
    }
}

void spawnShape()
{
    int shapeToMake = 1 + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (4 - 1))); //will be randomized
    while (shapeToMake == 4) { shapeToMake = 1 + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (4 - 1))); }

    srand(static_cast <unsigned> (time(0))); 
    // randoms [0.0, 1.0]
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    // randoms [0.1, 0.9]
    float vx = 0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.9 - 0.1)));
    float vy = 0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.9 - 0.1)));
    float vz = 0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.9 - 0.1)));

    float scale =  getRandomScale();
    
    glm::vec3 color(r, g, b);                         // get the random color
    glm::vec3 sc(scale, 0.0f, 0.0f);                  // this will be used when drawing
    glm::vec3 vctr(vx, vy, vz);                       // vector
  
    //TODO: refactor
    if (shapeToMake == 1)
    {
        printf("~~~Spawning a brand new shiny cube ! \n");
        float l = (scale * 10) / 2;;
        numberOfCubes += 1; // increment for the future

        glm::vec3 startPos(l, l, l);

        for (int i = numberOfCubes - 1; i < numberOfCubes; i++)
        {
            cubes.push_back(std::vector<glm::vec3>()); // magic line
            for (int j = 0; j < 1; j++)
            {
                cubes[i].push_back(color);
                cubes[i].push_back(startPos);
                cubes[i].push_back(sc);
                cubes[i].push_back(vctr);
            }
        }
        printf("scale = %f\n", scale);
        printf("CUBE STATS: d = %f, position(%f, %f, %f), color(%f, %f, %f) vector(%f, %f, %f)\n", 2.0 * l, l, l, l, r, g, b, vx,vy,vz);
    }

    if (shapeToMake == 2) 
    {
        printf("~~~Spawning a brand new shiny sphere !\n");
        
        float l = (scale * 10) / 2;
        glm::vec3 startPos(l, l, l);
        numberOfSpheres += 1; // increment for the future
        
        // pushing onto "vector"
        for (int i = numberOfSpheres-1; i < numberOfSpheres; i++)
        {
            spheres.push_back(std::vector<glm::vec3>()); // magic line
            for (int j = 0; j < 1; j++)
            {
                spheres[i].push_back(color);
                spheres[i].push_back(startPos);
                spheres[i].push_back(sc);
                spheres[i].push_back(vctr);
            }
        }
        printf("SPHERE STATS: d = %f, position(%f, %f, %f), color(%f, %f, %f) vector(%f, %f, %f)\n", 2.0 * l, l, l, l, r, g, b, vx, vy, vz);
    }

    if (shapeToMake == 3)
    {
        printf("~~~Spawning a brand new shiny cylinder ! \n");
        
        float h = (scale * 10)/2;
        numberOfCylinders += 1; // increment for the future
        glm::vec3 startPos(h, h, h);
        // pushing onto "vector"
        for (int i = numberOfCylinders - 1; i < numberOfCylinders; i++)
        {
            cylinders.push_back(std::vector<glm::vec3>()); // magic line
            for (int j = 0; j < 1; j++)
            {
                cylinders[i].push_back(color);
                cylinders[i].push_back(startPos);
                cylinders[i].push_back(sc);
                cylinders[i].push_back(vctr);
            }
        }

        printf("CYLINDER STATS: d = %f, position(%f, %f, %f), color(%f, %f, %f) vector(%f, %f, %f)\n", 2.0 * h, h, h, h, r, g, b, vx, vy, vz);

    }
}


// if shape exits the stage cube, place it back inside the cube
void checkOnAxis(int shape, double scale, int index)
{
    float placeBack = scale * 10 / 2;
    //TODO; refactor
    if (shape == 1)
    {
        // checking on X axis
        if (cubes[index][1].x <= placeBack)
            cubes[index][1].x = placeBack;
        if (cubes[index][1].x >= 100.0f - placeBack)
            cubes[index][1].x = 100.0f - placeBack;
        // checking on Y axis
        if (cubes[index][1].y <= placeBack)
            cubes[index][1].y = placeBack;
        if (cubes[index][1].y >= 100.0f - placeBack)
            cubes[index][1].y = 100.0f - placeBack;
        // checking on Z axis
        if (cubes[index][1].z <= placeBack)
            cubes[index][1].z = placeBack;
        if (cubes[index][1].z >= 100.0f - placeBack)
            cubes[index][1].z = 100.0f - placeBack;
    }
    if (shape == 2)
    {
        // checking on X axis
        if (spheres[index][1].x <= placeBack)
            spheres[index][1].x = placeBack;
        if (spheres[index][1].x >= 100.0f - placeBack)
            spheres[index][1].x = 100.0f - placeBack;
        // checking on Y axis
        if (spheres[index][1].y <= placeBack)
            spheres[index][1].y = placeBack;
        if (spheres[index][1].y >= 100.0f - placeBack)
            spheres[index][1].y = 100.0f - placeBack;
        // checking on Z axis
        if (spheres[index][1].z <= placeBack)
            spheres[index][1].z = placeBack;
        if (spheres[index][1].z >= 100.0f - placeBack)
            spheres[index][1].z = 100.0f - placeBack;
    }
    if (shape == 3)
    {
        // checking on X axis
        if (cylinders[index][1].x <= placeBack)
            cylinders[index][1].x = placeBack;
        if (cylinders[index][1].x >= 100.0f - placeBack)
            cylinders[index][1].x = 100.0f - placeBack;
        // checking on Y axis
        if (cylinders[index][1].y <= placeBack)
            cylinders[index][1].y = placeBack;
        if (cylinders[index][1].y >= 100.0f - placeBack)
            cylinders[index][1].y = 100.0f - placeBack;
        // checking on Z axis
        if (cylinders[index][1].z <= placeBack)
            cylinders[index][1].z = placeBack;
        if (cylinders[index][1].z >= 100.0f - placeBack)
            cylinders[index][1].z = 100.0f - placeBack;
    }
}

//NEW FUNCS FOR REFACTORINGS

void updateAllObjectPositions(float scale, glm::vec3 p, glm::vec3 v, int j)
{
    updateObjPosition(scale, cubes, p, v, j);
    updateObjPosition(scale, spheres, p, v, j);
    updateObjPosition(scale, cylinders, p, v, j);
}

void updateObjPosition(float scale, vector<vector<glm::vec3>> object, glm::vec3 p, glm::vec3 v, int j)
{
    for (int i = j; i < object.size(); i++) //checking from i = j, because we don't want to accidentally check the object with itself
    {
        
        float distance2 = sqrt(pow(p.x + v.x - object[i][1].x + object[i][3].x, 2) + 
                               pow(p.y + v.y - object[i][1].y + object[i][3].y, 2) + 
                               pow(p.z + v.z - object[i][1].z + object[i][3].z, 2));

        if (distance2 < (scale * 10) / 2 + (object[i][2].x * 10) / 2)
        {
            v.x = -v.x;
            v.y = -v.y;
            v.z = -v.z;

            object[i][3].x = -object[i][3].x;
            object[i][3].y = -object[i][3].y;
            object[i][3].z = -object[i][3].z;
        }
    }

}



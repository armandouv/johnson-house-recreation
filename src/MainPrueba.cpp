#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

void MouseCallback(GLFWwindow *window, double xPos, double yPos);

void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

float tiempo = 1.0f;
float speed = 1.0f;


// Positions of the point lights
glm::vec3 pointLightPositions[] = {
        glm::vec3(3.0f, 7.5f, 3.0f),
        glm::vec3(3.0f, 7.5f, 10.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
};

float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};


glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;    // Time of last frame

float drawer_translation = -4.5f;
bool is_drawer_opening = false, is_drawer_closing = false;

float chair_translation = 2.2f;
bool is_chair_opening = false, is_chair_closing = false;

float spider_rotation = 0.0f, spider_translation = 0.0f;
bool is_spider_left = true, is_spider_down = true;

float fly_rotation = 0.0f, fly_x_translation = 0.0f, fly_y_translation = 0.0f;

float door_rotation = 0.0f;
bool is_door_opening = false, is_door_closing = false;

int main() {
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Iluminacion 2", nullptr, nullptr);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


    Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
    Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
    Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");

    Model Couch((char *) "Models/Couch/couch.obj");
    Model Dresser((char *) "Models/Dresser/dresser2.obj");
    Model Drawer((char *) "Models/Dresser/drawer.obj");
    Model SmallCouch((char *) "Models/SmallCouch/smallcouch.obj");
    Model Table((char *) "Models/Table/table.obj");
    Model Piso((char *) "Models/Piso/Piso.obj");
    Model Chair((char *) "Models/Chair/chair.obj");
    Model Pot((char *) "Models/Pot/pot.obj");
    Model Dishware((char *) "Models/Dishware/dishware.obj");
    Model House((char *) "Models/House/house6.obj");
    Model Door((char *) "Models/Door/door.obj");
    Model Spider((char *) "Models/Spider/spider.obj");
    Model Fly((char *) "Models/Fly/fly.obj");
    Model Light((char *) "Models/Light/light.obj");



    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    //glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat) SCREEN_WIDTH / (GLfloat) SCREEN_HEIGHT, 0.1f,
                                            100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)) {

        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (is_drawer_opening && drawer_translation < -3.4f) drawer_translation += 0.1f;
        else if (is_drawer_closing && drawer_translation > -4.5f) drawer_translation -= 0.1f;

        if (is_chair_opening && chair_translation > 1.3f) chair_translation -= 0.1f;
        else if (is_chair_closing && chair_translation < 2.2f) chair_translation += 0.1f;

        if (is_door_opening && door_rotation < 90.0f) door_rotation += 5.0f;
        else if (is_door_closing && door_rotation > 0.0f) door_rotation -= 5.0f;

        if (is_spider_down) {
            if (spider_translation > -0.1f) spider_translation -= 0.01f;
            else is_spider_down = false;
        } else {
            if (spider_translation < 0.1f) spider_translation += 0.01f;
            else is_spider_down = true;
        }

        if (is_spider_left) {
            if (spider_rotation > -45.0f) spider_rotation -= 0.5f;
            else is_spider_left = false;
        } else {
            if (spider_rotation < 45.0f) spider_rotation += 0.5f;
            else is_spider_left = true;
        }

        fly_x_translation = sin(currentFrame);
        fly_y_translation = cos(currentFrame);
        if (fly_x_translation >= 0) {
            if (fly_y_translation >= 0) {
                fly_rotation = glm::degrees(180.0f) + abs(asin(fly_x_translation));
            } else {
                fly_rotation = glm::degrees(180.0f) + abs(acos(fly_y_translation));
            }
        } else {
            if (fly_y_translation >= 0) {
                fly_rotation = glm::degrees(180.0f) - abs(asin(fly_x_translation));
            } else {
                fly_rotation = glm::degrees(180.0f) - abs(acos(fly_y_translation));
            }
        }

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // OpenGL options
        glEnable(GL_DEPTH_TEST);



        //Load Model


        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Use();
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.7f, 0.7f, 0.7f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.8f, 0.8f, 0.8f);


        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x,
                    pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), Light1.x, Light1.y,
                    Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), Light1.x, Light1.y,
                    Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 0.1f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.4f);



        // Point light 2
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x,
                    pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), Light1.x, Light1.y,
                    Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), Light1.x, Light1.y,
                    Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 0.1f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.4f);

        // Point light 3
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x,
                    pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

        // Point light 4
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x,
                    pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x,
                    camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x,
                    camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.07f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.17f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
                    glm::cos(glm::radians(15.0f)));

        // Set material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        glm::mat4 model(1);
        glBindVertexArray(0);

        view = camera.GetViewMatrix();
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.0f, 2.3f, 7.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Couch.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.5f, 2.3f, 10.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Dresser.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(drawer_translation, 2.3f, 10.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Drawer.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.5f, 2.3f, 8.0f));
        model = glm::rotate(model, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        SmallCouch.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.3f, 2.3f, 10.0f));
        model = glm::rotate(model, glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        SmallCouch.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.0f, 2.3f, 3.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Table.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.2f, 1.38f + 2.3f, 3.5f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Dishware.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.0f, 2.3f, 10.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Table.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.1f, 0.82f + 2.3f, 10.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Pot.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.2f, 2.3f, chair_translation));
        model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Chair.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.8f, 2.3f, 1.2f));
        model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Chair.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.5f));
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        House.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.8f, 3.6f, 15.6f));
        model = glm::scale(model, glm::vec3(4.1f, 2.5f, 3.0f));
        model = glm::rotate(model, glm::radians(90.0f + door_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Door.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.5f, 9.0f + spider_translation, 10.0f));
        model = glm::rotate(model, glm::radians(spider_rotation), glm::vec3(1.0, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(spider_rotation * 4), glm::vec3(0.0, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Spider.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.0f + fly_x_translation, 6.0f, 3.0f + fly_y_translation));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        model = glm::rotate(model, fly_rotation + glm::degrees(90.0f), glm::vec3(0.0, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Fly.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.0f, 5.93f, 3.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Light.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.0f, 5.93f, 10.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Light.Draw(lightingShader);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }


    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();


    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement() {

    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        camera.ProcessKeyboard(FORWARD, deltaTime);

    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);


    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
        camera.ProcessKeyboard(LEFT, deltaTime);


    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
        camera.ProcessKeyboard(RIGHT, deltaTime);


    }

    if (keys[GLFW_KEY_T]) {
        pointLightPositions[0].x += 0.01f;
    }
    if (keys[GLFW_KEY_G]) {
        pointLightPositions[0].x -= 0.01f;
    }

    if (keys[GLFW_KEY_Y]) {
        pointLightPositions[0].y += 0.01f;
    }

    if (keys[GLFW_KEY_H]) {
        pointLightPositions[0].y -= 0.01f;
    }
    if (keys[GLFW_KEY_U]) {
        pointLightPositions[0].z -= 0.1f;
    }
    if (keys[GLFW_KEY_J]) {
        pointLightPositions[0].z += 0.01f;
    }


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_SPACE]) {
        active = !active;
        if (active) {
            Light1 = glm::vec3(1.0f, 1.0f, 1.0f);
        } else {
            Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
        }
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        if (drawer_translation == -3.4f || is_drawer_opening) {
            is_drawer_closing = true;
            is_drawer_opening = false;
        } else if (drawer_translation == -4.5f || is_drawer_closing) {
            is_drawer_closing = false;
            is_drawer_opening = true;
        }
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        if (chair_translation == 2.2f || is_chair_closing) {
            is_chair_closing = false;
            is_chair_opening = true;
        } else if (chair_translation == 1.3f || is_chair_opening) {
            is_chair_closing = true;
            is_chair_opening = false;
        }
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        if (door_rotation == 0.0f || is_door_closing) {
            is_door_closing = false;
            is_door_opening = true;
        } else if (door_rotation == 90.0f || is_door_opening) {
            is_door_closing = true;
            is_door_opening = false;
        }
    }
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
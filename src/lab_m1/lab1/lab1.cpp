#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */



Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!

}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "head.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

    {
        Mesh* mesh = new Mesh("bunny123");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
        meshes["bunny123"] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes["sphere"] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes["teapot"] = mesh;
    }



    box_position = glm::vec3(-1, 0.5f, 0);
    color = glm::vec3(0, 0, 0);
    changing_object = meshes["box"];
    speeed = 1.0f;
    k = 0;
}


void Lab1::FrameStart()
{
}

void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(color.x, color.y, color.z, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
 
    
    RenderMesh(changing_object, glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(changing_object, box_position);

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

    RenderMesh(meshes["bunny123"], glm::vec3(3, 0.5f, 0), glm::vec3(0.1, 0.1, 0.1));

}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.

    if (window->KeyHold(GLFW_KEY_SPACE)) {
        printf("TIIIN APASAAT\n");
    }

    //5 unitati -> 1 secunda
    //0.5 unitati -> 0.1 secunde


   /* float speed = 1.0f;*/
    if (window->KeyHold(GLFW_KEY_W)) {
        box_position.z += -speeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        box_position.x += -speeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        box_position.z += speeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        box_position.x += speeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        box_position.y += speeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        box_position.y += -speeed * deltaTime;
    }
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        color.x = 1;
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

    if (key == GLFW_KEY_SPACE) {
        printf("s-a apasat space\n");

        k++;
        if (k == 3)
            k = 0;
        if(k == 0)
         changing_object = meshes["box"];
        if (k == 1)
            changing_object = meshes["teapot"];
        if (k == 2)
            changing_object = meshes["sphere"];
    } 

    /*if (key & (1 << GLFW_MOUSE_BUTTON_RIGHT)) {
        speeed = 0;
        printf("!@#!@#!@#!@");
    }*/

}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event

    /*if (key == GLFW_KEY_SPACE) {
        printf("s-a deapasat space\n");
    }*/

    /*if (key == GLFW_MOUSE_BUTTON_RIGHT) {
        speeed = 1.0f;
        printf("depress");
    }*/
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event

    if (button & (1 << GLFW_MOUSE_BUTTON_RIGHT)) {
        speeed = 0;
        printf("!@#!@#!@#!@");
    }
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        speeed = 1.0f;
        printf("depress");
    }
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}

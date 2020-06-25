#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>

#include "../include/ImGui/imgui.h"
#include "../include/ImGui/imgui_impl_glfw.h"
#include "../include/ImGui/imgui_impl_opengl3.h"

#include "../include/obj_loader.h"
#include "../include/shader.h"
#include "../include/camera.h"
#include "../include//structs.h"

int width = 800, height = 600;
float last_x, last_y, delta_time = 0.0f, last_frame_time = 0.0f;;
bool wireframe = false, first_mouse = false, collapse_edge = false, show_model = true;
Camera *camera = new Camera{glm::vec3{0, 0, 1}, glm::vec3{0, 1, 0}};
GLFWwindow *window;

bool lexi_compare(const Edge *a, const Edge *b) {
    if (a->vertex_a->position.x == b->vertex_a->position.x) {
        if (a->vertex_a->position.y == b->vertex_a->position.y) {
            return a->vertex_a->position.z < b->vertex_a->position.z;
        } else {
            return a->vertex_a->position.y < b->vertex_a->position.y;
        }
    } else {
        return a->vertex_a->position.x < b->vertex_a->position.x;
    }
}

// this struct is there to order the edge pointer inside the multiset according to their cost
// NOTE: if the cost is equal a lexicographic compare of an edge vertex is done
struct cost_compare {
    bool operator()(const Edge *a, const Edge *b) const {
        if (a->cost == b->cost) {
            return lexi_compare(a, b);
        }
        return a->cost < b->cost;
    }
};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow *window, double x, double y);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void uploadMatrices(GLuint shader);

void initialize();

// TODO 3: implement the function remove_best_edge
// NOTE: the function should select the edge with the lowest cost and collapse it accordingly
// NOTE: you can implement this function on your own without following the TODOs. If your implementation works it will be graded.
void edge_collapse(std::multiset<Edge *, cost_compare> &edges, std::set<Triangle *> &triangles) {
    // TODO 3.1: find the edge with the lowest cost

    // TODO 3.2: create a vertex pointer to the new vertex that will created by the edge collapse
    // NOTE: you can assume that the position of the new vertex is the position in the middle of the edge that will be collapsed

    // TODO 3.3: find all triangles that would be modified by the edge collapse

    // TODO 3.4: find all edges that would be modified by the edge collapse

    // TODO 3.5: find the border vertices and border edges as shown in the image included in the lab.pdf

    // TODO 3.6: remove the triangles that would be modified from the incident triangles of the border edges

    // TODO 3.7: remove the triangles that would be modified from the incident triangles of the border vertices

    // TODO 3.8: remove the edges that would be modified from the incident edges of the border vertices

    // TODO 3.9: create the edges that will be created when connecting the border vertices with the new vertex (that was created by the edge collapse)
    // NOTE: do not forget to compute and set the cost factor for the edge

    // TODO 3.10: create the new triangles that will be created using the border edges an the new vertex (that was created by the edge collapse)
    // NOTE: make sure the triangle egdes are added to the edges multiset and their cost value is set
    // NOTE: make sure that the edge of a triangle has the triangle itself in it's incident_triangles vector
    // NOTE: make sure that the vertices of a triangle have the triangle itself in their incidents_triangles vector

    // TODO 3.11: delete all triangles that would have been modified by the edge collapse and erase them from the triangles set

    // TODO 3.12: delete all edges that would have been modified by the edge collapse and erase them from the edges multiset
}

int main(int argc, const char *argv[]) {

    initialize();

    Shader normal_shader = Shader{"../shader/base.vert", "../shader/base.frag"};
    auto vertices_indices_pair = ObjLoader::load_obj("../models/triceratops/triceratops.obj");
    std::vector<Vertex *> vertices = vertices_indices_pair.first;
    std::vector<GLuint> indices = vertices_indices_pair.second;

    // NOTES:
    // the following set, map and multiset can be used the pointer to your data
    // the triangles will be drawn in the main loop
    // the edge map can be used to store the edge between index i and index j
    // feel free to use your own data structures for the implementations
    std::set<Triangle *> triangles;
    std::map<GLuint,Vertex*> vertex_map;
    std::map<std::pair<GLuint, GLuint>, Edge *> edge_map;
    std::multiset<Edge *, cost_compare> edges;

    // TODO 1: for every triangle that is encoded in the indices vector create a new triangle and the corresponding edges
    // NOTE: make sure that an edge between the vertices i and j is only created once (you can use the vertex_map for this)
    // NOTE: make sure that the value for the cost of an edge is set. In this case you can take the length of the edge as cost.
    // NOTE: remember that the new triangle has to be part of the incident_triangles of the edges and vertices of the triangle

    // TODO 2: take all edge pointer of the edges that were created and insert them into the edges multiset
    // NOTE: the edges in the multiset are sorted according to their cost value

    // the main loop where the object are drawn
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        // calculate the time it took to compute the new frame to move the camera in a smoother way depending on the framerate
        delta_time = currentFrame - last_frame_time;
        last_frame_time = currentFrame;
        glfwGetFramebufferSize(window, &width, &height);

        // TODO 3: implement the function edge_collapse
        // NOTE: to verify the implementation load the cube.obj that is in the models folder and test the edge collapse
        if (collapse_edge && !edges.empty()) {
            edge_collapse(edges, triangles);
            collapse_edge = false;
        }

        // specify the background color
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        // clear color, depth and stencil buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        uploadMatrices(normal_shader.get_program());
        glUseProgram(normal_shader.get_program());


        // if the edges multiset is not empty the edge with the lowest cost function is being drawn
        // NOTE: if you want to see only the edge with the lowest cost activate the wireframe mode (key 1) and deactivate the drawing of the model (key 3)
        // NOTE: the following way to draw lines and triangles is deprecated in the current version of openGL
        //       it is used here to make it easier for you to focus on the actual edge collapse
        if (!edges.empty()) {
            Edge *min_cost_edge = *edges.begin();
            glBegin(GL_LINES);
            glVertex3f(min_cost_edge->vertex_a->position.x, min_cost_edge->vertex_a->position.y,
                       min_cost_edge->vertex_a->position.z);
            glVertex3f(min_cost_edge->vertex_b->position.x, min_cost_edge->vertex_b->position.y,
                       min_cost_edge->vertex_b->position.z);
            glEnd();
        }

        // if the boolean show_model is true (it can be switched by pressing 3) draw all triangles stored in the triangles vector
        // by pressing 1 the wireframe mode is switched on/off
        if (show_model) {
            glBegin(GL_TRIANGLES);
            for (auto triangle : triangles) {
                Vertex *a = triangle->vertex_a;
                Vertex *b = triangle->vertex_b;
                Vertex *c = triangle->vertex_c;
                glVertex3f(a->position.x, a->position.y, a->position.z);
                glVertex3f(b->position.x, b->position.y, b->position.z);
                glVertex3f(c->position.x, c->position.y, c->position.z);
            }
            glEnd();
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize({350, 100});
        ImGui::Begin("Statistics");
        ImGui::Text("Position: %f, %f, %f", camera->Position.x, camera->Position.y, camera->Position.z);
        ImGui::Text("Triangle count: %d", triangles.size());
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // unbinding shader program
    glUseProgram(NULL);
    // window is no longer required
    glfwDestroyWindow(window);
    // finish glfw
    glfwTerminate();
    return 0;
}

// this function is called when a key is pressed
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // if the escape key is pressed the window will close
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        collapse_edge = true;
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        show_model = !show_model;
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (wireframe) {
            wireframe = false;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            wireframe = true;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }

    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(FORWARD, delta_time * 50);
        }

    }

    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(BACKWARD, delta_time * 50);
        }
    }
    //left and right
    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(RIGHT, delta_time * 50);
        }
    }

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(LEFT, delta_time * 50);
        }
    }
}

void mouse_callback(GLFWwindow *window, double x, double y) {
    if (first_mouse) {
        last_x = x;
        last_y = y;
        first_mouse = false;
    }

    float xoffset = x - last_x;
    float yoffset = last_y - y; // reversed since y-coordinates go from bottom to top

    glfwSetCursorPos(window, last_x, last_y);
    camera->ProcessMouseMovement(xoffset, yoffset);
}

// this function is called when the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// this function uploads the model, view and projection matrix to the shader if they are defined in the shader
void uploadMatrices(GLuint shader) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), width / (float) height, 0.1f, 10000.0f);

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);
}

void initialize() {
    // initialize the GLFW library to be able create a window
    if (!glfwInit()) {
        throw std::runtime_error("Couldn't init GLFW");
    }

    // set the opengl version
    /*
    int major = 3;
    int minor = 3;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    */
    // create the window
    window = glfwCreateWindow(width, height, "Lab 5", NULL, NULL);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Couldn't create a window");
    }

    // set the window to the current context so that it is used
    glfwMakeContextCurrent(window);
    // set the frameBufferSizeCallback so that the window adjusts if it is scaled
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // set the keyCallback function so that keyboard input can be used
    glfwSetKeyCallback(window, key_callback);
    // set the mouse callback so that mouse input can be used
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // try to initialise glew to be able to use opengl commands
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error(
                std::string("Could initialize GLEW, error = ") + (const char *) glewGetErrorString(err));
    }

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;


    // opengl configuration
    glEnable(GL_DEPTH_TEST);        // enable depth-testing
    glDepthFunc(GL_LESS);           // depth-testing interprets a smaller value as "closer"
    glfwSwapInterval(false); // disables VSYNC

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    const char *glsl_version = "#version 130";
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}
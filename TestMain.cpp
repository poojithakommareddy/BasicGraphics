#include <iostream>
#include <cstring>
#include <thread>
#include <algorithm>
#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "MeshShaderGL.hpp"
#include "ModelLoader.hpp"
#include "ModelGL.hpp"
using namespace std;

/*///////////////////////////////////////////////////////////////////////////////
GLOBALS
///////////////////////////////////////////////////////////////////////////////*/

GLFWwindow* window = NULL;
int windowWidth = 800;
int windowHeight = 600;

// NOTE: When running, the window and framebuffer sizes MAY NOT BE the SAME!!! (Looking at you Apple...)
int framebufferWidth = windowWidth;
int framebufferHeight = windowHeight;

bool firstMouseMove = true;
double mouseX = -1;
double mouseY = -1;
bool leftMouseButtonDown = false;
ModelGL *modelGL = NULL;
const float TRANSLATION_INC=0.1;
const float CAMERA_WALK_SPEED = 0.1f;
const float CAMERA_ROTATE_SPEED = 30.0f;
Camera *camera = new Camera(glm::vec3(-2,2,2),glm::vec3(1, 0, 0), glm::vec3(0, 1, 0),60.0f, 0.01f, 30.0f, framebufferWidth, framebufferHeight);
Light *light = new Light(glm::vec3(0, 3, 2), glm::vec3(1, 1, 1), true);
float shiny = 10.0;
MeshShaderGL *shader;

/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - GLFW Callbacks
///////////////////////////////////////////////////////////////////////////////*/

// GLFW error callback
static void error_callback(int error, const char* description) {
	cerr << "ERROR " << error << ": " << description << endl;
}

// GLFW key callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {

		case GLFW_KEY_SPACE:
            modelGL->reset();
			// cout << "SPACEBAR" << endl;
			break;
                
        case GLFW_KEY_W:
            camera->forward(CAMERA_WALK_SPEED);
            break;
                
        case GLFW_KEY_S:
            camera->forward(-CAMERA_WALK_SPEED);
            break;
                
        case GLFW_KEY_D:
            camera->strafeRight(CAMERA_WALK_SPEED);
            break;
                
        case GLFW_KEY_A:
            camera->strafeRight(-CAMERA_WALK_SPEED);
            break;
			
		case GLFW_KEY_U:
            modelGL->rotate(5,glm::vec3(0,0,1));
			break;

		case GLFW_KEY_O:
            modelGL->rotate(-5,glm::vec3(0,0,1));
			break;

		case GLFW_KEY_I:
            modelGL->translate(glm::vec3(0,TRANSLATION_INC,0));
			break;

		case GLFW_KEY_K:
            modelGL->translate(glm::vec3(0,-TRANSLATION_INC,0));
			break;
        case GLFW_KEY_L:
            modelGL->translate(glm::vec3(TRANSLATION_INC,0,0));

            break;
        case GLFW_KEY_J:
            modelGL->translate(glm::vec3(-TRANSLATION_INC,0,0));
            break;
        case GLFW_KEY_C:
                if (!light->getIsPointLight())
                    light->setIsPointLight(true);
                else
                    light->setIsPointLight(false);
            break;
        case GLFW_KEY_0:
                shader->setMaterialChoice(0);
            break;
                
        case GLFW_KEY_1:
                shader->setMaterialChoice(1);
            break;
                
        case GLFW_KEY_V:
                shiny = shiny * 5.0;
                shader->setShininess(shiny);
            break;
                
        case GLFW_KEY_B:
                shiny = shiny / 5.0;
                if (shiny < 10.0)
                {
                    shiny = 10.0;
                }
                    shader->setShininess(shiny);
            break;
        case GLFW_KEY_N:
                light->setColor(glm::vec3(0, 0, 1));
            break;
                
        case GLFW_KEY_M:
                light->setColor(glm::vec3(1, 1, 1));
            break;

		default:
			break;
		}
	}
}

// GLFW mouse movement callback
static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouseMove) {
		mouseX = xpos;
		mouseY = ypos;
		firstMouseMove = false;
	}

	double xDiff = xpos - mouseX;
	double yDiff = ypos - mouseY;

	xDiff /= framebufferWidth;
	yDiff /= framebufferHeight;
    camera->rotateRight((-xDiff * CAMERA_ROTATE_SPEED));
    camera->rotateUp((-yDiff * CAMERA_ROTATE_SPEED));

	// TODO: xDiff and yDiff are normalized changes in mouse
	// cout << "MOUSE: " << xpos << " " << ypos << endl;

	mouseX = xpos;
	mouseY = ypos;
}

// GLFW callback for mouse BUTTONS
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			cout << "MOUSE LEFT DOWN" << endl;
			leftMouseButtonDown = true;
		}
		else if (action == GLFW_RELEASE) {
			cout << "MOUSE LEFT UP" << endl;
			leftMouseButtonDown = false;
		}
	}
}

// GLFW callback when the window changes size
void window_size_callback(GLFWwindow* window, int width, int height) {
	windowWidth = width;
	windowHeight = height;
}

// GLFW callback when the framebuffer changes size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	framebufferWidth = width;
	framebufferHeight = height;
}

/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - Main
///////////////////////////////////////////////////////////////////////////////*/

int main(int argc, char **argv) {

	// If an argument is passed in, load a 3D file.
	// Otherwise, we'll just create a simple quad.
	string modelFilename = "";
	if (argc >= 2) {
		modelFilename = string(argv[1]);
	}

	// GLFW setup //
	
	// Set error callback
	glfwSetErrorCallback(error_callback);

	// (Try to) initialize GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// MAC-SPECIFIC: Convince Mac that, yes, we want to use OpenGL version 3.3 (and NOT 2.1)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	// Create our GLFW window
	window = glfwCreateWindow(	windowWidth, windowHeight, 
								"kommarp_teapot", 
								NULL, NULL);

	// Were we able to make it?
	if (!window) {
		// Kill GLFW and exit program
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// We want to draw to this window, so make the OpenGL context associated with this window our current context.
	glfwMakeContextCurrent(window);

	// Basically, turning VSync on (so we will wait until the screen is updated once before swapping the back and front buffers
	glfwSwapInterval(1);

	// Set our keyboard callback function, so that we can process keyboard input!
	glfwSetKeyCallback(window, key_callback);
	
	// Set our mouse callback function for when the mouse MOVES
	glfwSetCursorPosCallback(window, mouse_position_callback);

	// Set our mouse BUTTON callback function
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Set our callback for when the window changes size
	glfwSetWindowSizeCallback(window, window_size_callback);

	// Set our callback for when the framebuffer changes size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// GLEW setup //
	
	// MAC-SPECIFIC: Some issues occur with using OpenGL core and GLEW; so, we'll use the experimental version of GLEW
	glewExperimental = true;

	// (Try to) initalize GLEW
	GLenum err = glewInit();

	// Call glGetError() to clear it out (can occur after glewInit() for some reason)
	glGetError();

	if (GLEW_OK != err) {
		// We couldn't start GLEW, so we've got to go.
		// Kill GLFW and get out of here
		cout << "ERROR: GLEW could not start: " << glewGetErrorString(err) << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	cout << "GLEW initialized; version ";
	cout << glewGetString(GLEW_VERSION) << endl;

	GLint glMajor, glMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
	glGetIntegerv(GL_MINOR_VERSION, &glMinor);
	cout << "OpenGL context version: ";
	cout << glMajor << "." << glMinor << endl;
	cout << "Supported GLSL version is ";
	cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	// Load shader program
	shader = new MeshShaderGL("../Basic.vs", "../Basic.ps", true);

	// Create/Get mesh data	
	ModelData *modelData = NULL;
	
	if (modelFilename != "") {
		cout << "Loading model: " << modelFilename << endl;

		// Load model		
		modelData = loadModel(modelFilename);

		// Did we load the model?
		if (!modelData) {
			cout << "ERROR: Failed to set up model data." << endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		// Override color per-vertex		
		vector<Vertex>* vertices = modelData->getMesh(0)->getVertices();
		for (int i = 0; i < vertices->size(); i++) {
			glm::vec3 pos = vertices->at(i).pos;
			pos += 1.0;
			pos /= 2.0;
			vertices->at(i).color = glm::vec4(pos.x, pos.y, pos.z, 1.0);
		}		
	}
	else {	
		cout << "Creating mesh from scratch..." << endl;

		// Create the mesh data
		vector<Vertex> verts;
		vector<unsigned int> ind;

		// Add four corners
		verts.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.0), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec4(1, 0, 0, 1)));
		verts.push_back(Vertex(glm::vec3(0.5, -0.5, 0.0), glm::vec4(0.0, 1.0, 0.0, 1.0), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec4(1, 0, 0, 1)));
		verts.push_back(Vertex(glm::vec3(0.5, 0.5, 0.0), glm::vec4(0.0, 0.0, 1.0, 1.0), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec4(1, 0, 0, 1)));
		verts.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec4(1, 0, 0, 1)));

		// Add indices for two triangles
		ind.push_back(0);
		ind.push_back(1);
		ind.push_back(2);

		ind.push_back(0);
		ind.push_back(2);
		ind.push_back(3);

		// Create mesh data
		MeshData *mesh = new MeshData(verts, ind);

		// Create a simple model and add mesh to it
		modelData = new ModelData();
		modelData->addMesh(mesh, 0);
	}
	
	// Get pointers to list of vertices and elements for first mesh
	vector<Vertex>* vertices = modelData->getMesh(0)->getVertices();
	vector<unsigned int>* elements = modelData->getMesh(0)->getIndices();
	int indexCnt = (int)elements->size();
    
    modelGL= new ModelGL(modelData);
    
	// Do OpenGL setup for single VBO and VAO with vertex positions and color
	//GLuint VBO = 0;
	//GLuint EBO = 0;
	//GLuint VAO = 0;
	
	// Create Vertex Buffer Object (VBO)
	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices->size(), vertices->data(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Vertex Array Object (VAO)
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);

	// Enable the first two vertex attribute arrays
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	
	// Bind the VBO and set up data mappings so that VAO knows how to read it
	// 0 = pos (3 elements)
	// 1 = color (4 elements)
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Attribute, # of components, type, normalized?, stride, array buffer offset
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	
	// Create Element Buffer Object (EBO)
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		//elements->size() * sizeof(GLuint),
		//elements->data(),
		//GL_STATIC_DRAW);

	// Unbind vertex array for now
	//glBindVertexArray(0);

	// Don't need local data anymore, so we can delete it
	delete modelData;

	// Set the background color to a shade of blue
	glClearColor(0.0f, 0.0f, 0.7f, 1.0f);	

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable shader program
	shader->activate();
    shader->setShininess(shiny);
    shader->setMaterialChoice(0);
	
	while (!glfwWindowShouldClose(window)) {
		//cout << "Rendering..." << endl;

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		glViewport(0, 0, framebufferWidth, framebufferHeight);
        camera->updateBufferSize(framebufferWidth,framebufferHeight);
        shader->setViewAndProjection(camera);
        shader->setLight(light);

		// Clear the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader->setViewAndProjection(camera);

		// Draw object
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, indexCnt, GL_UNSIGNED_INT, (void*)0);
		//glBindVertexArray(0);
         modelGL->draw(shader);
		// Swap buffers and poll for window events		
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Sleep for 15 ms
		this_thread::sleep_for(chrono::milliseconds(15));
	}

	// Cleanup		
	delete shader;
    delete modelGL;
    delete camera;

	// Cleanup VBO/VAO stuff
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glDeleteBuffers(1, &VBO);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glDeleteBuffers(1, &EBO);

	//glBindVertexArray(0);
	//glDeleteVertexArrays(1, &VAO);
		
	// Destroy window and stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

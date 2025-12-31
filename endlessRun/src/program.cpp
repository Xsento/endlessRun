// Local headers
#include "program.hpp"
#include "endlessRun/endlessRun.hpp"
#include "endlessRun/EBO.h"
#include "endlessRun/VAO.h"
#include "endlessRun/VBO.h"
#include "endlessRun/shader.h"
#include "endlessRun/texture.h"

// System headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Standard headers
#include <math.h>


void runProgram(GLFWwindow* window)
{
    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // Set up your scene here (create Vertex Array Objects, etc.)
    //------------------------------

    // Vertices coordinates
    GLfloat vertices[] =
    {//     COORDINATES             COLORS           TEXTURE COORDS
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,     0.0f, 0.0f,// Lower left corner
        -0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,     0.0f, 1.0f,// Upper left corner
        0.5f, 0.5f, 0.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,// Upper right corner
        0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f// Lower right corner
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 2, 1, 
        0, 3, 2
    };

    //make a shader program using the shader files
    Shader shaderProgram("../shaders/default.vert","../shaders/default.frag");    

    //make and bind the vertex array object
    VAO VAO1;
    VAO1.Bind();

    //make a vertex buffer object and and index buffer object (EBO) and link them to the vertex and index arrays
    VBO VBO1;
    VBO1.AssignValue(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    //link vbo attriutes like coords and color to the vao
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8*sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8*sizeof(float), (void*)(3*sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8*sizeof(float), (void*)(6*sizeof(float)));
    //take a wild fucking guess
    VAO1.Unbind();
    VBO1.Unbind();  
    EBO1.Unbind();

    //setting up the transformation matrix container, it has to be the identity matrix now
    glm::mat4 trans = glm::mat4(1.0f);

    // Texture
	Texture saul("textures/saul.jpeg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	saul.texUnit(shaderProgram, "tex0", 0);

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Binds texture so that is appears in rendering
		saul.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
    }

    // Delete all the objects
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	saul.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
}


void handleKeyboardInput(GLFWwindow* window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

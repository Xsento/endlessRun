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

class debugObject{
private:
    VAO vao;
    VBO vbo;
    EBO ebo;
    glm::mat4 trans = glm::mat4(1.0f); //transformation matrix set to be an identity matrix (i.e. does nothing)

public:
    glm::vec<2, double> positionPixel = glm::vec<2, double>(0.0,0.0);
    debugObject(Shader& shaderProgram){        
        GLfloat vertices[] =
        {// COORDINATES             COLORS           TEXTURE COORDS
        -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,     0.0f, 0.0f,// Lower left corner
        -0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,// Upper left corner
        0.5f, 0.5f, 0.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,// Upper right corner
        0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f// Lower right corner
        };     // colors dont do anything when theres a texture, might remove them soom
        
        GLuint indices[] =
        {
            0, 2, 1, 
            0, 3, 2
        };

        //size of one vertex
        unsigned int stride = 8*sizeof(float);

        //set up buffers
        vao.Bind();
        vbo.AssignValue(vertices, sizeof(vertices));
        ebo.AssignValue(indices, sizeof(indices));

        
        vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, stride, (void*)0); //position
        vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, stride, (void*)(3*sizeof(float))); //color
        vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, stride, (void*)(6*sizeof(float))); //texture

        //put the identity matrix into the uniform so the object actually renders
        unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();
    }

    void Draw(Texture& _texture, Shader& shaderProgram){
        shaderProgram.Activate();
        unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        
        _texture.texUnit(shaderProgram, "tex0", 0);
        _texture.Bind();

        vao.Bind();
        ebo.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vao.Unbind();
        ebo.Unbind();
        _texture.Unbind();
    }

    void Delete(){
        vao.Delete();
        vbo.Delete();
    }

    void Translate(glm::vec3 transVect){
        trans = glm::translate(trans, transVect); 
        positionPixel = glm::vec2(positionPixel.x+transVect.x,positionPixel.y+transVect.y); 
    }

    void SetPixelPosition(glm::vec2 newPosition){
        //TODO
    }
};


void runProgram(GLFWwindow* window)
{
    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // Set up your scene here (create Vertex Array Objects, etc.)
    //------------------------------

    //make a shader program using the shader files
    Shader shaderProgram("../../endlessRun/shaders/default.vert","../../endlessRun/shaders/default.frag");    

    // Textures
	Texture saul("../../endlessRun/textures/saul.jpeg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	saul.texUnit(shaderProgram, "tex0", 0);

    debugObject saulGoodman(shaderProgram);

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

        saulGoodman.Translate(glm::vec3(sin(glfwGetTime())/100,cos(glfwGetTime())/100,0));
        saulGoodman.Draw(saul,shaderProgram);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
    }

    // Delete all created objects
	saul.Delete();
    saulGoodman.Delete();
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

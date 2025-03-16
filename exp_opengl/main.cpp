#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"
#include "data.h"
#include "camera.h"
#include "texture.h"
using std::cout; using std::endl; using std::sin;
int success; char infoLog[512];
uint scr_w = 1360, scr_h = 768;
float delta_time = 0.0f, lastFrame = 0.0f;
float lastX = 0, lastY = 0;

Camera cam1(scr_w, scr_h);
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam1.action_w(delta_time);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam1.action_s(delta_time);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam1.action_a(delta_time);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam1.action_d(delta_time); 

	if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    cam1.action_on_scroll(yoffset);}
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos; 
    lastY = ypos;
    cam1.action_on_mouse(xoffset, yoffset);}
void framebuffer_size_callback(GLFWwindow* window, int w, int h){glViewport(0,0,w,h);}

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(scr_w, scr_h, "experimental", NULL, NULL);
	if(window == NULL){cout<<"window == NULL"<<endl; glfwTerminate(); return -1;}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout<<"gladLoadGLLoader"<<endl; return -1;}

	glViewport(0,0,scr_w,scr_h);
    
    uint VBO; glGenBuffers(1, &VBO);      glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint VAO; glGenVertexArrays(1, &VAO); glBindVertexArray(VAO); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    uint lightVAO; glGenVertexArrays(1, &lightVAO); glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Texture texture1("/home/ids/pro/exp/rsr/ras0.jpg");
    //Texture texture2("/home/ids/pro/exp/rsr/t9.png");
    Texture diffuseMap("/home/ids/pro/exp/cont2.jpg");
    Texture specularMap("/home/ids/pro/exp/c2s.png");

    Shader shaderProgram("/home/ids/pro/exp/vshader", "/home/ids/pro/exp/multiple_lights_fshader");

    Shader shaderProgramL("/home/ids/pro/exp/vshader", "/home/ids/pro/exp/light_fshader");

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        delta_time = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//(0.5f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture1.ID);
        //glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, texture2.ID);
        
        glm::mat4 projection = glm::perspective(glm::radians(cam1.get_fov()), (float)scr_w/(float)scr_h, 0.1f, 100.0f);
        glm::mat4 view = cam1.get_view();
        shaderProgram.use();
        shaderProgram.setInt("material.diffuse", 0);
        shaderProgram.setInt("material.specular", 1);
        shaderProgram.setFloat("material.shininess", 64.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap.ID);       

        shaderProgram.setVec3f("dir_light.direction", -0.2f, -1.0f, -0.3f);
        shaderProgram.setVec3f("dir_light.ambient", 0.05f, 0.05f, 0.05f);
        shaderProgram.setVec3f("dir_light.diffuse", 0.09f, 0.44f, 0.4f);
        shaderProgram.setVec3f("dir_light.specular", 0.5f, 0.5f, 0.5f);
        
        shaderProgram.setVec3f("pnt_light[0].position", pointLightPositions[0]);
        shaderProgram.setVec3f("pnt_light[0].ambient", 0.05f, 0.05f, 0.05f);
        shaderProgram.setVec3f("pnt_light[0].diffuse", 0.8f, 0.8f, 0.8f);
        shaderProgram.setVec3f("pnt_light[0].specular", 1.0f, 1.0f, 1.0f);
        shaderProgram.setFloat("pnt_light[0].constant", 1.0f);
        shaderProgram.setFloat("pnt_light[0].linear", 0.09f);
        shaderProgram.setFloat("pnt_light[0].quadratic", 0.032f);
        
        shaderProgram.setVec3f("pnt_light[1].position", pointLightPositions[1]);
        shaderProgram.setVec3f("pnt_light[1].ambient", 0.05f, 0.05f, 0.05f);
        shaderProgram.setVec3f("pnt_light[1].diffuse", 0.8f, 0.8f, 0.8f);
        shaderProgram.setVec3f("pnt_light[1].specular", 1.0f, 1.0f, 1.0f);
        shaderProgram.setFloat("pnt_light[1].constant", 1.0f);
        shaderProgram.setFloat("pnt_light[1].linear", 0.09f);
        shaderProgram.setFloat("pnt_light[1].quadratic", 0.032f);
        
        shaderProgram.setVec3f("pnt_light[2].position", pointLightPositions[2]);
        shaderProgram.setVec3f("pnt_light[2].ambient", 0.05f, 0.05f, 0.05f);
        shaderProgram.setVec3f("pnt_light[2].diffuse", 0.8f, 0.8f, 0.8f);
        shaderProgram.setVec3f("pnt_light[2].specular", 1.0f, 1.0f, 1.0f);
        shaderProgram.setFloat("pnt_light[2].constant", 1.0f);
        shaderProgram.setFloat("pnt_light[2].linear", 0.09f);
        shaderProgram.setFloat("pnt_light[2].quadratic", 0.032f);    
    
        shaderProgram.setVec3f("pnt_light[3].position", pointLightPositions[3]);
        shaderProgram.setVec3f("pnt_light[3].ambient", 0.05f, 0.05f, 0.05f);
        shaderProgram.setVec3f("pnt_light[3].diffuse", 0.8f, 0.8f, 0.8f);
        shaderProgram.setVec3f("pnt_light[3].specular", 1.0f, 1.0f, 1.0f);
        shaderProgram.setFloat("pnt_light[3].constant", 1.0f);
        shaderProgram.setFloat("pnt_light[3].linear", 0.09f);
        shaderProgram.setFloat("pnt_light[3].quadratic", 0.032f);        
 
        shaderProgram.setVec3f("spt_light.position", cam1.position);
        shaderProgram.setVec3f("spt_light.direction", cam1.front);
        shaderProgram.setVec3f("spt_light.ambient", 0.0f, 0.0f, 0.0f);
        shaderProgram.setVec3f("spt_light.specular", 1.0f, 10.0f, 1.0f);//check
        shaderProgram.setVec3f("spt_light.diffuse", 1.0f, 0.5f, 0.0f);
        shaderProgram.setFloat("spt_light.constant", 1.0f);
        shaderProgram.setFloat("spt_light.linear", 0.09f);
        shaderProgram.setFloat("spt_light.qudratic", 0.032f);
        shaderProgram.setFloat("spt_light.cutoff", glm::cos(glm::radians(12.5f)));
        shaderProgram.setFloat("spt_light.outer_cutoff", glm::cos(glm::radians(17.5f)));
        //shaderProgram.setVec3f("light.position", 1.2f, 1.0f, 2.0f);
        //shaderProgram.setVec3f("light.direction", -0.2, -1.0, -3.0);
        //shaderProgram.setVec3f("light.position", cam1.position);
        //shaderProgram.setVec3f("light.direction", cam1.front);
        //shaderProgram.setFloat("light.cutoff", (float)glm::cos(glm::radians(12.5f)));
        //shaderProgram.setFloat("light.outercutoff", (float)glm::cos(glm::radians(17.5f)));
        //shaderProgram.setVec3f("viewPos", cam1.front);
        //shaderProgram.setInt("material.diffuse", 0); 
        //shaderProgram.setInt("material.specular", 1);


        //shaderProgram.setFloat("material.shininess", 256.0f);
        //shaderProgram.setVec3f("light.ambient", 0.2f, 0.2f, 0.2f);
        //shaderProgram.setVec3f("light.diffuse", 0.5f, 0.5f, 0.5f);
        //shaderProgram.setVec3f("light.specular", 1.0f, 1.0f, 1.0f);
        //shaderProgram.setFloat("light.constant", 1.0f);
        //shaderProgram.setFloat("light.linear", 0.09f);
        //shaderProgram.setFloat("light.quadratic", 0.032f);

        
        shaderProgram.setMat4f(view);
        shaderProgram.setMat4f(projection);
        glBindVertexArray(VAO);
        for(uint i = 0; i<10; ++i){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime()*glm::radians(-50.0f),
                                glm::vec3(0.5f, 1.0, 0.0f));
            shaderProgram.setMat4f(model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        shaderProgramL.use();
        shaderProgramL.setMat4f(view);
        shaderProgramL.setMat4f(projection);
        for(int i = 0; i<4; ++i){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            shaderProgram.setMat4f(model);
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
	}

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

#ifndef SHADER_H
#define SHADER_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp> 
using std::cout; using std::endl; using std::string;
extern int success; extern char infoLog[512];
struct Shader{ 
    uint vertexShader;
    uint fragmentShader;
    uint shaderProgram;
    Shader(const char* pathv, const char* pathf){
        const char *vertexShaderSource, *fragmentShaderSource;
        std::ifstream vshaderFile, fshaderFile; std::stringstream ssv, ssf; string t1, t2;
        vshaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fshaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try{
            vshaderFile.open(pathv); fshaderFile.open(pathf);
            ssv<<vshaderFile.rdbuf(); t1 = ssv.str(); vertexShaderSource = t1.c_str();
            ssf<<fshaderFile.rdbuf(); t2 = ssf.str(); fragmentShaderSource = t2.c_str();
            vshaderFile.close(); fshaderFile.close();
        }catch(std::ifstream::failure e){
        cout<<"ERROR: SHADER: OPEN VFSHADER"<<endl;}
           
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success){glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); 
        cout<<"ERROR: VERTEX SHADER: "<<infoLog<<endl;}

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success){glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout<<"ERROR: FRAGMENT SHADER: "<<infoLog<<endl;}

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);    
        glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
        if(!success){glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog); 
        cout<<"ERROR: SHADER PROGRAM: "<<infoLog<<endl;}

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    ~Shader(){}
    void use(){glUseProgram(shaderProgram);}
    void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);}
    void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);}
    void setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);}
    void setVec3f(const std::string &name,  float x, float y, float z) const{
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z); }
    void setVec3f(const std::string &name,  const glm::vec3& value) const{
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); }
    void setMat4f(const std::string &name, glm::mat4& val){
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()),1, GL_FALSE, glm::value_ptr(val));}
};

#define setMat4f(val) setMat4f(#val, (val))
#endif 

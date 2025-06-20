#if !defined(CAMERA_H)
#define CAMERA_H 1
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
class Camera{
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = 0.0f;
    float pitch = 0.0f;
    float fov = 42.0f;
    float sens = 0.01f;
    float speed = 2.5f;
public:
    Camera(uint scr_w, uint scr_h){}
    glm::mat4 get_view(){return lookAt(position, position + front, up);}
    float get_fov(){return fov;}
    void action_on_mouse(float& xoffset, float& yoffset){
    yaw   += xoffset*sens;
    pitch += yoffset*sens;
    if(pitch>89.0f)  pitch = 89.0f;
    if(pitch<-89.0f) pitch = -89.0f;
    update_direction();
}
void update_direction(){
    glm::vec3 dir; 
    dir.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw))* cos(glm::radians(pitch));
    front = glm::normalize(dir);    
}

void action_on_scroll(double yoffset){
    fov -=(float)yoffset;
    if(fov<1.0f) fov = 1.0f;
    if(fov>45.0f) fov = 45.0f;   
}

void action_w(float delta_time){position+=delta_time*speed*front;}
void action_s(float delta_time){position-=delta_time*speed*front;}
void action_a(float delta_time){position-=glm::normalize(glm::cross(front, up))*delta_time*speed;}
void action_d(float delta_time){position+=glm::normalize(glm::cross(front, up))*delta_time*speed;}


};
#endif

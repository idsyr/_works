#if !defined(TEXTURE_H)
#define TEXTURE_H 1
struct Texture{
uint ID;
Texture(string path){
    glGenTextures(1, &ID); glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    int width, height, nrChannels; 
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if(data){
        if(path.substr(path.size()-3,3)=="png")
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else    
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {cout<<"ERROR: LOAD TEXTURE: "<<path<<endl;}
    stbi_image_free(data);
}
};
#endif

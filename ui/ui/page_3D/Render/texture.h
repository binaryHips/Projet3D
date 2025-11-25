#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLTexture>
#include <QOpenGLExtraFunctions>

#include <iostream>

#include "external/stb_image.h"

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    Color(unsigned char a, unsigned char b, unsigned char c)
        : r(a),
        g(b),
        b(c)
    {}
};

class Texture {
    // https://learnopengl.com/Getting-started/Textures

protected:

    QOpenGLExtraFunctions *gl_funcs;
    std::vector< unsigned char > data;
    GLuint _texture_id;
    bool __synchronized = false;

    Color getPixel(size_t u, size_t v) const;

    void synchronize(){ // TODO unsynchronize if already synchronized. Otherwise this is a memory leak :)

        if (__synchronized) return; // TODO if we allow modification of the texture, make it desynchronize etc

        // see anisotropic filtering, samplers objects, etc
        gl_funcs->glGenTextures(1, &_texture_id);
        gl_funcs->glBindTexture(GL_TEXTURE_2D, _texture_id);

        gl_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        gl_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        gl_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        gl_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        gl_funcs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,  data.data());
        gl_funcs->glGenerateMipmap(GL_TEXTURE_2D);
    }

    void unsynchronize(){

        if (!__synchronized) return;

        gl_funcs->glDeleteTextures(1, &_texture_id);

        __synchronized = false;
    }

public:

    enum RepeatMode {TILE, EXTEND, MIRROR};

    int width, height;
    int nbChannels;

    Texture(const std::string path, bool cpuOnly = false , QOpenGLExtraFunctions *f = nullptr){
        gl_funcs = f;
        loadTexture(path);
    }

    Texture(const Texture & other , QOpenGLExtraFunctions *f = nullptr)
        : data(other.data),
        width(other.width),
        height(other.height),
        nbChannels(other.nbChannels)
    {
        gl_funcs = f;
        _texture_id = other._texture_id;
        __synchronized = false;
        if (!data.empty())
            synchronize();

    }

    Texture & operator=(const Texture & other){
        data = (other.data);
        width = (other.width);
        height = (other.height);
        nbChannels = (other.nbChannels);
        _texture_id = other._texture_id; // just for the unsafe copies. Remove when not needed
        __synchronized = false;
        if (!data.empty())
            synchronize();

        return *this;
    }

    Texture() = default;
    Texture(Texture &&) = default;
    ~Texture(){if (__synchronized) unsynchronize();}

    void loadTexture(const std::string path, bool cpuOnly = false){
        unsigned char * new_data = stbi_load(path.c_str(), &width, &height, &nbChannels, 0);
        if (!new_data || nbChannels != 3) std::cout << "[Texture] Problem Loading the texture at " << path << "\n\tAre you sure the path is right & the number of channels (" << nbChannels << ") is supported?" << std::endl;

        data.resize(width * height * nbChannels);

        memcpy(data.data(), new_data, width * height * nbChannels); // order seems "inverted" but it's normal

        stbi_image_free(new_data);

        if (!cpuOnly) synchronize();
    }

    Color getPixelSafe(size_t u, size_t v, RepeatMode repeat_mode) const;

    Color operator() (size_t i, size_t j) {

        return getPixelSafe(i, j, RepeatMode::MIRROR);
    }

    static Texture getPixelTexture(uint8_t val){
        Texture t;
        t.data = {val, val, val};
        t.width = t.height = 1;
        t.nbChannels = 3;
        t.synchronize();
        return t;
    }



    GLuint getTextureId(){ return _texture_id;};

    // IN CASE STRANGE THINGS HAPPEN: ALWAYS REMEMBER https://learnopengl.com/Getting-started/Textures
    void bind(GLuint slot = 0) const {
        gl_funcs->glActiveTexture(GL_TEXTURE0+slot); // activate the texture unit first before binding texture
        // this is true here and seems to be the right way, but somehow doesnt work the same on the cubemap

        gl_funcs->glBindTexture(GL_TEXTURE_2D, _texture_id);

        gl_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        gl_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
    }
};
#endif // TEXTURE_H

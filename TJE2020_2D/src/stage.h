#ifndef STAGE_H
#define STAGE_H

#include "image.h"

class Stage {

    public:
        virtual void init() { std::cout<<"Base stage init"<<std::endl; };
        virtual void render(Image* framebuffer) {};
        virtual void update(double dt) {};
        virtual void deinit() {};
};

#endif
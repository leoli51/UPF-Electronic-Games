#ifndef DINO_H
#define DINO_H

#include "framework.h"
#include "game.h"
#include "Sprite.h"

class Dino {

    public:
        AnimatedSprite* sprite;
    
    public:
        Dino(Vector2 position){
            sprite = new AnimatedSprite("data/green_dino.tga", 24, 1, 0, 4, .4f, position);
        }

        void render(Image* framebuffer) {
            sprite->render(framebuffer);
        };

        void update(double dt){
            sprite->update(dt);
        };

        void move_to(Vector2 new_pos, float time /* tween mode*/){
        };


};

#endif
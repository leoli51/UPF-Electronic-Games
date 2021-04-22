#ifndef DINO_H
#define DINO_H

#include "framework.h"
#include "game.h"
#include "Sprite.h"

class Dino {

    public:
        AnimatedSprite* sprite;

        // WOW so many variables just for the jump code??? - my future self
        // Yes... I know... there is an easier and more readable way
        // right now I really dont want to think about it. - my current self
        // oh dont worry i removed some useless variables. - my future self 
        // thx. -my current self
        float jump_velocity = 0;
        float jump_time = -1; 
        float total_jump_time;
    
    public:
        Dino(Vector2 position){
            sprite = new AnimatedSprite("data/green_dino.tga", 24, 1, 0, 4, .4f, position);
        }

        void render(Image* framebuffer) {
            sprite->render(framebuffer);
        };

        void update(double dt){
            sprite->update(dt);

            if (jump_time > 0){
                sprite->position.y += jump_velocity * dt * (jump_time > total_jump_time / 2? 1 : -1);
                jump_time -= dt;
                
            }

        };

        void move(Vector2 dir){
            sprite->position += dir;
        };

        void jump(float velocity, float time){
            if (jump_time <= 0){
                jump_time = time;
                total_jump_time = time;
                jump_velocity = -velocity;//- GRAVITY * time / 2;
            }
        };
};

#endif
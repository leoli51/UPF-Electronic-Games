#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Dino.h"
#include "input.h"
#include "framework.h"

// class that controls the player Party-Rex.
// implementatio could be so much better but there's no need to overengineer it 
// so it will be VERY project specific. Sorry :)

class PlayerController {

    public:
        Dino* controlled_dino = NULL;
        float move_speed;

    public:
        PlayerController(Dino* dino, float speed = 1){
            controlled_dino = dino;
            move_speed = speed;
        };

        void setControlledDino(Dino* controlled){
            controlled_dino = controlled;
        };

        void update(float dt){
            Vector2 move_dir(0,0);

            if (Input::isKeyPressed(SDL_SCANCODE_UP)) {
                move_dir.y -= 1;
            }
            if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
                move_dir.y += 1;
            }
            if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
                move_dir.x += 1;
            }
            if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
                move_dir.x -= 1;
            }

            if (Input::wasKeyPressed(SDL_SCANCODE_Z))
                controlled_dino->jump(50, .2);

            if (move_dir.length() > 0) controlled_dino->move(move_dir.normalize() * dt * move_speed);
        };
};

#endif
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Dino.h"
#include "input.h"
#include "framework.h"
#include "game.h"

#define MAX_INFLUENCE_RADIUS 30
#define MIN_INFLUENCE_RADIUS 2

// class that controls the player Party-Rex.
// implementation could be so much better but there's no need to overengineer it 
// so it will be VERY project specific. Sorry :)

class PlayerController {

    public:
        Dino* player_dino;
        std::vector<Dino*>* controlled_dinos;
        float move_speed;
        float influence_radius;

    public:
        PlayerController(float speed = 1, float i_r = 20){
            controlled_dinos = new std::vector<Dino*>();
            player_dino = new Dino(Vector2(80, 60), "data/blue_dino.tga");
            controlled_dinos->push_back(player_dino);
            move_speed = speed;
            influence_radius = i_r;
        };

        ~PlayerController(){
            for (int i = 0; i < controlled_dinos->size(); i++){
                delete controlled_dinos->at(i);
            }
            delete controlled_dinos;
        };

        void addControlledDino(Dino* controlled){
            controlled->sprite->setImage("data/blue_dino.tga");
            controlled->sprite->setSheetProperties(24, 1);
            controlled->sprite->setAnimation(0,4,.4f);
            controlled_dinos->push_back(controlled);
        };

        void removeControlledDino(Dino* dino){
            for (int i = 0; i < controlled_dinos->size(); i++)
                if (controlled_dinos->at(i) == dino){
                    controlled_dinos->erase(controlled_dinos->begin() + i);
                    return;
                }

        };

        void update(float dt){
            for (int i = 0; i < controlled_dinos->size(); i++)
                controlled_dinos->at(i)->update(dt);
            
            Vector2 move_dir(0,0);

            if (Input::isKeyPressed(SDL_SCANCODE_W)) {
                move_dir.y -= 1;
            }
            if (Input::isKeyPressed(SDL_SCANCODE_S)) {
                move_dir.y += 1;
            }
            if (Input::isKeyPressed(SDL_SCANCODE_D)) {
                move_dir.x += 1;
            }
            if (Input::isKeyPressed(SDL_SCANCODE_A)) {
                move_dir.x -= 1;
            }

            int sound_to_play = 0;
            sound_to_play |= Input::wasKeyPressed(SDL_SCANCODE_K) ? 1 : 0;
            sound_to_play |= Input::wasKeyPressed(SDL_SCANCODE_M) ? 2 : 0;

            if (sound_to_play > 0){
                for (int i = 0; i < controlled_dinos->size(); i++)
                    controlled_dinos->at(i)->jump(50, .2f);
                
                Game::instance->synth.playSample("data/bum.wav", 1, false);
            }

            if (move_dir.length() > 0)
                for (int i = 0; i < controlled_dinos->size(); i++)
                    controlled_dinos->at(i)->move(move_dir.normalize() * dt * move_speed);
        };

        void render(Image* framebuffer){
            std::sort(controlled_dinos->begin(), controlled_dinos->end(), [](const Dino* a, const Dino* b){return a->sprite->position.y < b->sprite->position.y;});

            for (int i = 0; i < controlled_dinos->size(); i++)
                    controlled_dinos->at(i)->render(framebuffer);

            // TODO: visual indication of influence radius 
            Vector2 center = player_dino->sprite->position; 
            framebuffer->drawLine(  center.x - influence_radius, 
                                    center.y - influence_radius, 
                                    center.x + influence_radius, 
                                    center.y - influence_radius, Color::YELLOW);
            framebuffer->drawLine(  center.x - influence_radius, 
                                    center.y - influence_radius, 
                                    center.x - influence_radius, 
                                    center.y + influence_radius, Color::YELLOW);
            framebuffer->drawLine(  center.x + influence_radius, 
                                    center.y - influence_radius, 
                                    center.x + influence_radius, 
                                    center.y + influence_radius, Color::YELLOW);
            framebuffer->drawLine(  center.x - influence_radius, 
                                    center.y + influence_radius, 
                                    center.x + influence_radius, 
                                    center.y + influence_radius, Color::YELLOW);
            
            //framebuffer->drawRectangle(player_dino->sprite->position.x, player_dino->sprite->position.y, 10, 10, Color::WHITE);
            //framebuffer->drawRectangle(player_dino->sprite->position.x, player_dino->sprite->position.y, 4, 4, Color::BLUE);
        };

        bool inDanceRange(Vector2 pos){
            return player_dino->sprite->position.distance(pos) <= influence_radius;
        }

        void increaseInfluenceRadius(){
            influence_radius = clamp(influence_radius * (1 + 0.1f), MIN_INFLUENCE_RADIUS, MAX_INFLUENCE_RADIUS);
        };

        void decreaseInfluenceRadius(){
            influence_radius = clamp(influence_radius * (1 - 0.12f), MIN_INFLUENCE_RADIUS, MAX_INFLUENCE_RADIUS);
        };

        std::vector<Dino*>* getDinosOutOfInfluence(){
            std::vector<Dino*>* out = new std::vector<Dino*>();
            for (int i = 0; i < controlled_dinos->size(); i++){
                Dino* d = controlled_dinos->at(i);
                if (d == player_dino)
                    continue;
                
                if (!inDanceRange(d->sprite->position))
                    out->push_back(d);
            }
            return out;
        }

};

#endif
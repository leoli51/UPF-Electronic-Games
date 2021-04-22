#ifndef GAME_STAGES_H
#define GAME_STAGES_H

#include "stage.h"
#include "Dino.h"
#include "PlayerController.h"
#include "DinoAIController.h"

class IntroStage : public Stage {
    public:
        Image* intro_font;

    public:
        void init(){
            intro_font = Image::Get("data/bitmap-font-white.tga"); //load bitmap-font image

        };

        void render(Image* framebuffer){
            framebuffer->fill(Color::BLACK);
            framebuffer->drawText("Party Dino :)", 0, 0, *intro_font);
        };

        void update(double dt){

        };

        void deinit(){

        };
};

class PlayStage : public Stage {
    public:
        Dino* player_dino;
        PlayerController* player_controller;
        Dino* ai_dino;
        DinoAIController* ai_dino_controller;

    public:
        void init(){
            std::cout<<"Play stage init"<<std::endl;
            player_dino = new Dino(Vector2(20,20)); 
            player_controller = new PlayerController(player_dino, 15);

            ai_dino = new Dino(Vector2(30,30));
            ai_dino_controller = new DinoAIController(ai_dino);
        };

        void render(Image* framebuffer){
            framebuffer->fill(Color::BLACK);
            player_dino->render(framebuffer);

            ai_dino->render(framebuffer);
        };

        void update(double dt){
            player_controller->update(dt);
            player_dino->update(dt);
            
            ai_dino_controller->update(dt);
            ai_dino->update(dt);
        };

        void deinit(){

        };
};



#endif
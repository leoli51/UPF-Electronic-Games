#ifndef GAME_STAGES_H
#define GAME_STAGES_H

#include "stage.h"
#include "Dino.h"
#include "PlayerController.h"
#include "DinoAIController.h"
#include "BeatBar.h"
#include "DiscoTiles.h"
#include "synth.h"

class PlayStage : public Stage {
    public:
        PlayerController* player_controller;
        AiDinoManager* ai_dino_manager;
        BeatBar* beatbar;
        DiscoTiles* tiles;
        Synth::SamplePlayback* music;
        Image* font;
        float victory_countdown;
        bool won;
        char win_message[64] = "Keep partying for 9 s!";

    public:
        void init(){
            std::cout<<"Play stage init"<<std::endl;
            player_controller = new PlayerController(15);
            ai_dino_manager = new AiDinoManager(20); // change to 25


            beatbar = new BeatBar(player_controller);
            tiles = new DiscoTiles(.5f, Color::BRIGHT_SAFFRON, Color::ELECTRIC_FLAMINGO);

            music = Game::instance->synth.playSample("data/funky_mix.wav", 5, true);

            font = Image::Get("data/bitmap-font-white.tga"); //load bitmap-font image
            victory_countdown = 10;
            won = false;
        };

        void render(Image* framebuffer){
            framebuffer->fill(Color::BLACK);

            tiles->render(framebuffer);

            ai_dino_manager->render(framebuffer);
            player_controller->render(framebuffer);

            beatbar->render(framebuffer);

            if (victory_countdown < 10){
                if (won){
                    framebuffer->drawText("WOOO AWESOME PARTY!", 10,0, *font);
                    framebuffer->drawText("Press SPACE to exit!", 2, 8, *font);
                    if (Input::isKeyPressed(SDL_SCANCODE_SPACE)){
                        Game::instance->setStage("menu_stage");
                    }
                }
                else
                    framebuffer->drawText(std::string(win_message), 0,0,*font);
            }
        };

        void update(double dt){
            tiles->update(dt);

            player_controller->update(dt);
            
            ai_dino_manager->update(dt);

            bool beat_hit = false;
            if (!won)
                beat_hit = beatbar->update(dt);

            // hit: lets see if other dinos want to dance
            if (beat_hit){
                Dino* dino = ai_dino_manager->closestDinoTo(player_controller->controlled_dinos->at(0)->sprite->position);
                if (dino != NULL){
                    if (player_controller->inDanceRange(dino->sprite->position)){
                            player_controller->addControlledDino(dino);
                            ai_dino_manager->removeDino(dino);
                    }
                }
            }

            // lets see if some dinos dont want to dance anymore
            if (!won){
                std::vector<Dino*>* out_of_range = player_controller->getDinosOutOfInfluence();
                for (int i = 0; i < out_of_range->size(); i++){
                    ai_dino_manager->addDino(out_of_range->at(i));
                    player_controller->removeControlledDino(out_of_range->at(i));
                }
                delete out_of_range;
            }

            // adjust music volume to number of dinos dancing
            float party_percent = player_controller->controlled_dinos->size() / (float)(ai_dino_manager->controllers.size() + player_controller->controlled_dinos->size());
            tiles->modulate = lerp(.2f, .9f, party_percent);
            music->volume = lerp(5, 30, party_percent);
            
            // victory condition
            if (ai_dino_manager->controllers.empty()){
                victory_countdown -= dt;
                if (victory_countdown <= 0) won = true;
                else {
                    win_message[18] = ((int) victory_countdown) +'0';
                }
            }
            else {
                victory_countdown = 10;
            }
        };

        void deinit(){
            delete ai_dino_manager;
            delete player_controller;
            delete beatbar;
            delete tiles;
            music->stop();
        };
};


class IntroStage : public Stage {
    public:
        Image* white_font;
        Image* white_font_small;
        float period = .5f;
        DiscoTiles* tiles;
        Synth::SamplePlayback* music;

    public:

        void init(){
            white_font = Image::Get("data/bitmap-font-white.tga"); //load bitmap-font image
            white_font_small = Image::Get("data/mini-font-white-4x6.tga");

            tiles = new DiscoTiles(period, Color::ELECTRIC_FLAMINGO, Color::ELECTRIC_CRIMSON);

            music = Game::instance->synth.playSample("data/funky_mix.wav", 3, true);
        };

        void render(Image* framebuffer){
            framebuffer->fill(Color::BLACK);
            tiles->render(framebuffer);

            framebuffer->drawText("Party-Rex", 48, 8, *white_font);
            framebuffer->drawText("Dinosaurs were shy creatures", 8, 24, *white_font_small, 4, 6);
            framebuffer->drawText("Except for one... the Party-Rex", 8, 32, *white_font_small, 4, 6);
            framebuffer->drawText("Not even a meteorite could stop him.", 8, 40, *white_font_small, 4, 6);

            framebuffer->drawText("Use WASD to move", 2, 56, *white_font);
            framebuffer->drawText("Use M and K to dance", 2, 64, *white_font);
            framebuffer->drawText("Dance to the beat!", 2, 72, *white_font);
            framebuffer->drawText("Follow the beat line", 2, 80, *white_font);
            framebuffer->drawText("dance near other dinos", 2, 88, *white_font);
            framebuffer->drawText("you stop they stop", 2, 96, *white_font);
            framebuffer->drawText("Press K+M to party", 2, 104, *white_font);
        };

        void update(double dt){
            tiles->update(dt);
            
            if (Input::isKeyPressed(SDL_SCANCODE_M) && Input::isKeyPressed(SDL_SCANCODE_K)){
                Game::instance->setStage("party_stage");
            }
        };

        void deinit(){
            delete tiles;
            music->stop();
        };
};

#endif
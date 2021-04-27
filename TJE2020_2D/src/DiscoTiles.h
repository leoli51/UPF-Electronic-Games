#ifndef DISCO_TILES_H
#define DISCO_TILES_H

#include "framework.h"
#include "image.h"

class DiscoTiles {
    public:
        float period;
        Color c1;
        Color c2;
        float tile_width;
        float modulate = .2f; 
    
    protected:
        float elapsed_time = 0;
        Color current_color;
        int current_color_index = 0;

    public:
        DiscoTiles(float p, Color a, Color b){
            period = p;
            tile_width = 10;
            c1 = a;
            c2 = b;
        };

        void update(float dt){
            elapsed_time += dt;
            if (elapsed_time >= period){
                current_color_index = (current_color_index + 1) % 2;
                current_color = current_color_index? c1 : c2;
                elapsed_time = 0;
            }
        };

        void render(Image* framebuffer){
            for (int x = 0; x < 16; x++)
                for (int y = 0; y < 11; y++){
                    if ((x+y) % 2 == current_color_index)
                        framebuffer->drawRectangle(x * tile_width, y * tile_width, tile_width, tile_width, current_color * modulate);
                    else 
                        framebuffer->drawRectangle(x * tile_width, y * tile_width, tile_width, tile_width, Color::BLACK);

                }
        };

};

#endif

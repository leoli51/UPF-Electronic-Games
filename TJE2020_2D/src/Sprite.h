#ifndef SPRITE_H
#define SPRITE_H

#include "framework.h"
#include "image.h"
#include "util.h"

class Sprite {

    public:
        Vector2 position;
        Vector2 center;
        Vector2 size;

        Image* image;
    
    public:
        Sprite(){
            image = NULL;
        };

        Sprite(const std::string image_path, Vector2 pos){
            image = Image::Get(image_path);
            position = pos;
            center.set(image->width / 2.0f, image->height / 2.0f);
            size.set(image->width, image->height);
        };

        void setImage(std::string path){
            setImage(Image::Get(path));
        };

        void setImage(Image* new_img){
            image = new_img;
            size.set(image->width, image->height);
            center.set(size.x / 2, size.y / 2);
        };

        void setCenterAbsolute(Vector2 new_center){
            center = new_center;
        };

        void setCenterRelative(Vector2 new_center){
            center.x = new_center.x * size.x;
            center.y = new_center.y * size.y;
        };

        void setSize(Vector2 new_size){
            size = new_size;
        };

        void render(Image* framebuffer){
            framebuffer->drawImage(*image, position.x - center.x, position.y + center.y, size.x, size.y);
            //std::cout<<"drawing sprite at "<<position.x<<" "<<position.y<<" w: "<<size.x<<" h: "<<size.y<<std::endl; 
        };

};

class AnimatedSprite : public Sprite {
    public :
        float animation_time;
        unsigned int w_frames;
        unsigned int h_frames;
        unsigned int total_frames;
        unsigned int start_frame;
        unsigned int end_frame;
        unsigned int current_frame_index;
        Vector2 frame_position;

    private:
        float elapsed_time = 0;

    public:
        AnimatedSprite(const std::string image_path, int w_f, int h_f, int s_f, int e_f, float anim_time, Vector2 pos)
            : Sprite(image_path, pos){            
            setSheetProperties(w_f, h_f);
            setAnimation(s_f, e_f, anim_time);
        };

        void setAnimation(int s_f, int e_f, float anim_time){
            start_frame = s_f;
            end_frame = e_f;
            current_frame_index = start_frame;
            animation_time = anim_time;
            frame_position = frameIndexToPosition(current_frame_index);
        };

        void setSheetProperties(int v_f, int h_f){
            w_frames = v_f;
            h_frames = h_f;
            total_frames = w_frames * h_frames;
            size.set(image->width / w_frames, image->height / h_frames);
            center.set(size.x / 2, size.y / 2);
        };

        void render(Image* framebuffer){
            framebuffer->drawImage(*image, position.x - center.x, position.y - center.y, frame_position.x, frame_position.y, size.x, size.y);
        };

        void update(double dt){
            elapsed_time += dt;
            if (elapsed_time > animation_time) elapsed_time = 0;

            current_frame_index = lerp(start_frame, end_frame, elapsed_time / animation_time);
            frame_position = frameIndexToPosition(current_frame_index);
            //std::cout<<"current frame: "<<current_frame_index<<std::endl;
            //std::cout<<frame_position.x<<" "<<frame_position.y<<" "<<size.x<<" "<<size.y<<std::endl;
        };

    protected:
        Vector2 frameIndexToPosition(unsigned int index){
            float x_pos = (index % w_frames) * size.x;
            float y_pos = (index / w_frames) * size.y;

            return Vector2(x_pos, y_pos);
        };
};

#endif
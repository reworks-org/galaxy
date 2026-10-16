#pragma once

#include "sl.h"

namespace sl 
{

struct AnimationFrame {
    float duration=0.100f;
    float time_left=0.100f;
    int tile_id=0;

};

struct AnimatedSprite {
    sl::Atlas atlas;
    int current_frame;
    int x=0;
    int y=0;
    int width=32;
    int height=32;
    int start_tile_id=0;
    float rotation_angle=0.0f;
    float pivotx=0.0f;
    float pivoty=0.0f;
    bool paused=false;
    bool draw_rotated=false;
    bool one_shot=false;
    int one_shot_counts=0;
    int one_shot_counter=0;
    std::vector<AnimationFrame> frames;
};
AnimatedSprite create_animated_sprite(sl::Atlas atlas,int start_tile_id,
    int numframes,int x, int y, int width, int height, float duration,bool one_shot=false,int oneshot_counts=64);
void draw_animated_sprite(AnimatedSprite &sprite, sl::Bitmap *destination);
void draw_rotated_animated_sprite(AnimatedSprite &sprite, sl::Bitmap *destination);

class AnimationManager {
public:
    AnimationManager();
    ~AnimationManager();
    void update();
    const int  add_animated_sprite(AnimatedSprite &sprite);
    void draw_sprites(sl::Bitmap *bitmap);
    bool get_sprite(int index, AnimatedSprite **sprite);
    bool set_sprite_tiles(int index, std::vector<int> &tiles);
    bool set_sprite_position(int index, int x, int y);
    bool set_sprite_rotation(int index, float angle);
    // not sure we need this...
    bool get_sprite_position(int index, int &x, int &y);
    void toggle_pause(int index);
    // Indices of sprites after the removed one shift down by one.
    bool remove_animated_sprite(int index);
private:
    std::vector<AnimatedSprite> animated_sprites;
};


}
#include "animated_sprite.h"

namespace sl
{

AnimatedSprite create_animated_sprite(sl::Atlas atlas, int start_tile_id, int numframes, int x, int y, int width,
                                      int height, float duration, bool one_shot,  int oneshot_counts)
{
    AnimatedSprite sprite;
    sprite.atlas = atlas;
    sprite.current_frame = 0;
    sprite.width = width;
    sprite.height = height;
    sprite.x = x;
    sprite.y = y;
    sprite.start_tile_id = start_tile_id;
    sprite.one_shot=one_shot;
    sprite.one_shot_counts=oneshot_counts;
    for (int i = 0; i < numframes; i++)
    {
        AnimationFrame frame;
        frame.duration = duration;
        frame.tile_id = i + start_tile_id;
        frame.time_left = duration;
        sprite.frames.emplace_back(frame);
    }
    return sprite;
}

void draw_animated_sprite(AnimatedSprite &sprite, sl::Bitmap *destination)
{
    sl::atlas_stretch_blit(sprite.atlas, destination, sprite.frames[sprite.current_frame].tile_id, sprite.x, sprite.y,
                           sprite.width, sprite.height);
}
void draw_rotated_animated_sprite(AnimatedSprite &sprite, sl::Bitmap *destination)
{
    sl::atlas_rotate_stretch_sprite(sprite.atlas, sprite.frames[sprite.current_frame].tile_id, sprite.x, sprite.y,
                                    sprite.rotation_angle, sprite.width, sprite.height, sprite.pivotx, sprite.pivoty);
}

AnimationManager::AnimationManager()
{
}
AnimationManager::~AnimationManager()
{
}
void AnimationManager::update()
{
    const double delta_t = sl::get_frame_time() / 1000.0;
    std::vector<AnimatedSprite> keep_list;
    for (auto &as : animated_sprites)
    {
        if (as.paused)
        {
            keep_list.emplace_back(as);
            continue;
        }
        double time_left = as.frames[as.current_frame].time_left;
        time_left -= delta_t;
        if (time_left < 0)
        {
            int next_frame = as.current_frame;
            next_frame++;
            if (next_frame >= as.frames.size())
                next_frame = 0;
            as.current_frame = next_frame;
            as.frames[as.current_frame].time_left = as.frames[as.current_frame].duration;
            if(!as.one_shot)
            {
                keep_list.emplace_back(as);
            }
            else
            {
              //  fprintf(stderr,"one_shot_counter=%d one_shot_counts=%d\n",as.one_shot_counter,as.one_shot_counts);
                if(as.one_shot_counter<as.one_shot_counts)
                {
                    
                    as.one_shot_counter++;
                    keep_list.emplace_back(as);
                   // fprintf(stderr,"one_shot_counter=%d one_shot_counts=%d\n",as.one_shot_counter,as.one_shot_counts);
                }
            }
        }
        else
        {
            as.frames[as.current_frame].time_left = time_left;
            keep_list.emplace_back(as);
        }
    }
    animated_sprites.swap(keep_list);
}
const int AnimationManager::add_animated_sprite(AnimatedSprite &sprite)
{
    animated_sprites.emplace_back(sprite);
    return (int)animated_sprites.size() - 1;
}
bool AnimationManager::remove_animated_sprite(int index)
{
    if (index < 0 || index >= static_cast<int>(animated_sprites.size()))
        return false;
    animated_sprites.erase(animated_sprites.begin() + index);
    return true;
}
void AnimationManager::draw_sprites(sl::Bitmap *bitmap)
{
    for (auto &sprite : animated_sprites)
    {
        if (!sprite.draw_rotated)
            draw_animated_sprite(sprite, bitmap);
        else
            draw_rotated_animated_sprite(sprite, bitmap);
    }
}
bool AnimationManager::get_sprite(int index, AnimatedSprite **sprite)
{
    if (index < 0 || index >= static_cast<int>(animated_sprites.size()))
    {
        (*sprite) = nullptr;
        return false;
    }
    (*sprite) = &animated_sprites[index];
    return true;
}
bool AnimationManager::set_sprite_tiles(int index, std::vector<int> &tiles)
{
    AnimatedSprite *sprite;
    bool sprite_valid = get_sprite(index, &sprite);
    if (sprite_valid)
    {
        int i = 0;
        if (sprite->frames.size() != tiles.size())
            return false;
        for (auto &af : sprite->frames)
        {
            af.tile_id = tiles[i];
            i++;
        }
        return true;
    }
    return false;
}
bool AnimationManager::set_sprite_position(int index, int x, int y)
{
    AnimatedSprite *sprite;
    bool sprite_valid = get_sprite(index, &sprite);
    if (sprite_valid)
    {
        sprite->x = x;
        sprite->y = y;
        return true;
    }
    return false;
}
bool AnimationManager::set_sprite_rotation(int index, float angle)
{
    float passed_angle = std::fmod(angle, 360.0f);
    AnimatedSprite *sprite;
    bool sprite_valid = get_sprite(index, &sprite);
    if (sprite_valid)
    {
        sprite->rotation_angle = passed_angle;
        return true;
    }
    return false;
}
// not sure we need this...
bool AnimationManager::get_sprite_position(int index, int &x, int &y)
{
    AnimatedSprite *sprite;
    bool sprite_valid = get_sprite(index, &sprite);
    if (sprite_valid)
    {
        x = sprite->x;
        y = sprite->y;
        return true;
    }
    return false;
}
void AnimationManager::toggle_pause(int index)
{
    AnimatedSprite *sprite;
    bool sprite_valid = get_sprite(index, &sprite);
    if (sprite_valid)
    {
        sprite->paused = !sprite->paused;
    }
}
} // namespace sl
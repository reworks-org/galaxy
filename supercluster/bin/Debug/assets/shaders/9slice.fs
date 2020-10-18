#version 450 core

in vec2 io_texels;
in vec2 io_tex_size;
in float io_opacity;

out vec4 io_frag_colour;

// Thanks to: https://gamedev.stackexchange.com/a/153853
uniform vec2 u_9grid_size;
uniform vec2 u_total_area;
uniform sampler2D u_texture;

float map(float value, float original_min, float original_max, float new_min, float new_max)
{
    return (value - original_min) / (original_max - original_min) * (new_max - new_min) + new_min;
} 

float process_axis(float coord, float texture_border, float window_border)
{
    if (coord < window_border)
    {
        return map(coord, 0, window_border, 0, texture_border);
    }
        
    if (coord < 1 - window_border)
    {
        return map(coord,  window_border, 1 - window_border, texture_border, 1 - texture_border);
    }
        
    return map(coord, 1 - window_border, 1, 1 - texture_border, 1);
} 

void main()
{
    vec2 dim_border = u_9grid_size / u_total_area;
    vec2 tex_border = u_9grid_size / io_tex_size;

    vec2 processed_uv = vec2(
        process_axis(io_texels.x, tex_border.x, dim_border.x),
        process_axis(io_texels.y, tex_border.y, dim_border.y)
    );
    
    // Output the color
    io_frag_colour = texture(u_texture, processed_uv);
	io_frag_colour.a *= io_opacity;
}
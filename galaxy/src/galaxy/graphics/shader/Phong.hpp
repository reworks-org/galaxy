///
/// Phong.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADER_PHONG_HPP_
#define GALAXY_GRAPHICS_SHADER_PHONG_HPP_

#include <string>

namespace galaxy
{
	namespace shaders
	{
		///
		/// Phong fragment shader.
		///
		inline const std::string phong_frag = R"(
			#version 450 core

            #define ATTENUATION_CONSTANT 1.0

            struct DirLight
            {
                vec3 ambient_intensity;
                vec3 diffuse_intensity;
                vec3 specular_intensity;

                vec3 dir;
            };

            struct PointLight
            {
                vec3 ambient_intensity;
                vec3 diffuse_intensity;
                vec3 specular_intensity;

                vec3 pos;
                float linear;
                float quadratic;
            };

            struct SpotLight
            {
                vec3 ambient_intensity;
                vec3 diffuse_intensity;
                vec3 specular_intensity;

                vec3 pos;
                float linear;
                float quadratic;

                vec3 dir;
                float inner_cutoff;
                float outer_cutoff;
            };

            struct Material
            {
                //vec3 ambient;
                //vec3 diffuse;
                //vec3 specular;
                float shininess;
            };

            vec3 calc_directional(DirLight light, vec3 normal, vec3 view_dir, vec4 diff_map, vec3 spec_map);
            vec3 calc_point(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir, vec4 diff_map, vec3 spec_map);
            vec3 calc_spot(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir, vec4 diff_map, vec3 spec_map);

            in vec2 io_texels;
            in vec3 io_normals;
            in vec3 io_frag_pos;
            out vec4 io_frag_colour;

            uniform vec3 u_camera_pos;

            uniform sampler2D tex_DIFFUSE1;
            uniform sampler2D tex_SPECULAR1;
            uniform sampler2D tex_NORMAL1;

            uniform Material material;

            uniform DirLight dir_light;
            uniform PointLight point_light;
            uniform SpotLight spot_light;
            
            void main()
            {
                vec3 normals = normalize(io_normals);
                vec3 view_dir = normalize(u_camera_pos - io_frag_pos);

                vec4 diff_map = texture(tex_DIFFUSE1, io_texels);
                vec3 spec_map = vec3(texture(tex_SPECULAR1, io_texels));

                vec3 result = calc_directional(dir_light, normals, view_dir, diff_map, spec_map);
                result += calc_point(point_light, normals, io_frag_pos, view_dir, diff_map, spec_map);
                result += calc_spot(spot_light, normals, io_frag_pos, view_dir, diff_map, spec_map);

                io_frag_colour = vec4(result, diff_map.a);
            }

            vec3 calc_directional(DirLight light, vec3 normal, vec3 view_dir, vec4 diff_map, vec3 spec_map)
            {
                vec3 light_dir = normalize(-light.dir);

                float diff = max(dot(normal, light_dir), 0.0);
                vec3 reflect_dir = reflect(-light_dir, normal);
                float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

                //vec3 ambient = light.ambient_intensity * (material.ambient * diff_map.rgb);
                //vec3 diffuse = light.diffuse_intensity * diff * (material.diffuse * diff_map.rgb);
                //vec3 specular = light.specular_intensity * spec * (material.specular * spec_map);

                vec3 ambient = light.ambient_intensity * diff_map.rgb;
                vec3 diffuse = light.diffuse_intensity * diff * diff_map.rgb;
                vec3 specular = light.specular_intensity * spec * spec_map;

                return ambient + diffuse + specular;
            }

            vec3 calc_point(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir, vec4 diff_map, vec3 spec_map)
            {
                vec3 light_dir = normalize(light.pos - frag_pos);
    
                float diff = max(dot(normal, light_dir), 0.0);
                vec3 reflect_dir = reflect(-light_dir, normal);
                float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

                float dist    = length(light.pos - frag_pos);
                float attenuation = 1.0 / (ATTENUATION_CONSTANT + light.linear * dist + light.quadratic * (dist * dist));

                //vec3 ambient = light.ambient_intensity * (material.ambient * diff_map.rgb);
                //vec3 diffuse = light.diffuse_intensity * diff * (material.diffuse * diff_map.rgb);
                //vec3 specular = light.specular_intensity * spec * (material.specular * spec_map);

                vec3 ambient = light.ambient_intensity * diff_map.rgb;
                vec3 diffuse = light.diffuse_intensity * diff * diff_map.rgb;
                vec3 specular = light.specular_intensity * spec * spec_map;

                ambient  *= attenuation;
                diffuse  *= attenuation;
                specular *= attenuation;

                return ambient + diffuse + specular;
            }

            vec3 calc_spot(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir, vec4 diff_map, vec3 spec_map)
            {
                vec3 light_dir = normalize(light.pos - frag_pos);
                
                float diff = max(dot(normal, light_dir), 0.0);
                vec3 reflect_dir = reflect(-light_dir, normal);
                float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
                
                float dist = length(light.pos - frag_pos);
                float attenuation = 1.0 / (ATTENUATION_CONSTANT + light.linear * dist + light.quadratic * (dist * dist));    
                
                float theta = dot(light_dir, normalize(-light.dir)); 
                float epsilon = light.inner_cutoff - light.outer_cutoff;
                float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);
                
                //vec3 ambient = light.ambient_intensity * (material.ambient * diff_map.rgb);
                //vec3 diffuse = light.diffuse_intensity * diff * (material.diffuse * diff_map.rgb);
                //vec3 specular = light.specular_intensity * spec * (material.specular * spec_map);

                vec3 ambient = light.ambient_intensity * diff_map.rgb;
                vec3 diffuse = light.diffuse_intensity * diff * diff_map.rgb;
                vec3 specular = light.specular_intensity * spec * spec_map;
                
                ambient *= attenuation * intensity;
                diffuse *= attenuation * intensity;
                specular *= attenuation * intensity;
                
                return ambient + diffuse + specular;
            }
		)";
	} // namespace shaders
} // namespace galaxy

#endif
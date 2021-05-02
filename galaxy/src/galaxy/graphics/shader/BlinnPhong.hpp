///
/// BlinnPhong.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADER_BLINNPHONG_HPP_
#define GALAXY_GRAPHICS_SHADER_BLINNPHONG_HPP_

#include <string>

namespace galaxy
{
	namespace shaders
	{
		///
		/// Blinn-Phong fragment shader.
		///
		inline const std::string blinnphong_frag = R"(
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
            };

            struct SpotLight
            {
                vec3 ambient_intensity;
                vec3 diffuse_intensity;
                vec3 specular_intensity;

                vec3 pos;
                vec3 dir;
                float inner_cutoff;
                float outer_cutoff;
            };

            struct Material
            {
                sampler2D diffuse;
                sampler2D specular;
                sampler2D normal;
                float shininess;
            };

            vec3 calc_directional(DirLight light, vec3 normal, vec3 view_dir, vec3 diff_tex, vec3 spec_map);
            vec3 calc_point(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir, vec3 diff_tex, vec3 spec_map);
            vec3 calc_spot(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir, vec3 diff_tex, vec3 spec_map);
            
            layout (std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_view;
				mat4 u_camera_proj;
				vec3 u_camera_pos;
			};
            
            layout (std430, binding = 1) readonly buffer dir_light_data
            {
                DirLight b_dir_lights[];
            };
            
            layout (std430, binding = 2) readonly buffer point_light_data
            {
                PointLight b_point_lights[];
            };
            
            layout (std430, binding = 3) readonly buffer spot_light_data
            {
                SpotLight b_spot_lights[];
            };
            
            in vec2 io_texels;
            in vec3 io_frag_pos;
            in mat3 io_tbn;
            out vec4 io_frag_colour;

            uniform Material material;

            void main()
            {
                vec3 view_dir = normalize(u_camera_pos - io_frag_pos);
                
                vec4 diff_tex = texture(material.diffuse, io_texels);
                vec3 spec_map = texture(material.specular, io_texels).rgb;

                vec3 normals = texture(material.normal, io_texels).rgb;
                normals = normals * 2.0 - 1.0;
                normals = normalize(io_tbn * normals);
                
                vec3 result = vec3(0);
                for (int i = 0; i < b_dir_lights.length(); i++)
                {
                    result += calc_directional(b_dir_lights[i], normals, view_dir, diff_tex.rgb, spec_map);
                }

                for (int i = 0; i < b_point_lights.length(); i++)
                {
                    result += calc_point(b_point_lights[i], normals, io_frag_pos, view_dir, diff_tex.rgb, spec_map);
                }
                
                for (int i = 0; i < b_spot_lights.length(); i++)
                {
                    result += calc_spot(b_spot_lights[i], normals, io_frag_pos, view_dir, diff_tex.rgb, spec_map);
                }

                io_frag_colour = vec4(result, diff_tex.a);
            }

            vec3 calc_directional(DirLight light, vec3 normal, vec3 view_dir, vec3 diff_tex, vec3 spec_map)
            {
                vec3 light_dir = normalize(-light.dir);
                
                float diff = max(dot(normal, light_dir), 0.0);
                vec3 halfway_dir = normalize(light_dir + view_dir);
                float spec = pow(max(dot(normal, halfway_dir), 0.0), material.shininess);
                
                vec3 ambient = light.ambient_intensity * diff_tex.rgb;
                vec3 diffuse = light.diffuse_intensity * diff * diff_tex.rgb;
                vec3 specular = light.specular_intensity * spec * spec_map;

                return ambient + diffuse + specular;
            }

            vec3 calc_point(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir, vec3 diff_tex, vec3 spec_map)
            {
                vec3 light_dir = normalize(light.pos - frag_pos);
    
                float diff = max(dot(normal, light_dir), 0.0);
                vec3 halfway_dir = normalize(light_dir + view_dir);

                float spec = pow(max(dot(normal, halfway_dir), 0.0), material.shininess);
                float dist    = length(light.pos - frag_pos);
                float attenuation = 1.0 / (dist * dist);                

                vec3 ambient = light.ambient_intensity * diff_tex.rgb;
                vec3 diffuse = light.diffuse_intensity * diff * diff_tex.rgb;
                vec3 specular = light.specular_intensity * spec * spec_map;
                
                ambient  *= attenuation;
                diffuse  *= attenuation;
                specular *= attenuation;
                
                return ambient + diffuse + specular;
            }

            vec3 calc_spot(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir, vec3 diff_tex, vec3 spec_map)
            {
                vec3 light_dir = normalize(light.pos - frag_pos);
                
                float diff = max(dot(normal, light_dir), 0.0);
                vec3 halfway_dir = normalize(light_dir + view_dir);
                float spec = pow(max(dot(normal, halfway_dir), 0.0), material.shininess);
                
                float dist = length(light.pos - frag_pos);
                float attenuation = 1.0 / (dist * dist);
                float theta = dot(light_dir, normalize(-light.dir)); 
                float epsilon = light.inner_cutoff - light.outer_cutoff;
                float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

                vec3 ambient = light.ambient_intensity * diff_tex.rgb;
                vec3 diffuse = light.diffuse_intensity * diff * diff_tex.rgb;
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
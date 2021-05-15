///
/// SMAA.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

/**
    GLSL PORT: https://github.com/Asmodean-/SMAA-OpenGL

    SMAA LICENSE: https://github.com/iryoku/smaa
    Copyright (C) 2011 Jorge Jimenez (jorge@iryoku.com)
    Copyright (C) 2011 Belen Masia (bmasia@unizar.es)
    Copyright (C) 2011 Jose I. Echevarria (joseignacioechevarria@gmail.com)
    Copyright (C) 2011 Fernando Navarro (fernandn@microsoft.com)
    Copyright (C) 2011 Diego Gutierrez (diegog@unizar.es)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

       1. Redistributions of source code must retain the above copyright notice,
          this list of conditions and the following disclaimer.

       2. Redistributions in binary form must reproduce the following disclaimer
          in the documentation and/or other materials provided with the
          distribution:

         "Uses SMAA. Copyright (C) 2011 by Jorge Jimenez, Jose I. Echevarria,
          Belen Masia, Fernando Navarro and Diego Gutierrez."

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
    IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
    THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
    BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are
    those of the authors and should not be interpreted as representing official
    policies, either expressed or implied, of the copyright holders.
*/

#ifndef GALAXY_GRAPHICS_SHADERS_POST_SMAA_HPP_
#define GALAXY_GRAPHICS_SHADERS_POST_SMAA_HPP_

#include "galaxy/graphics/shaders/post/AreaSMAA.hpp"
#include "galaxy/graphics/shaders/post/SearchSMAA.hpp"

namespace galaxy
{
	namespace shaders
	{
		///
		/// SMAA core vertex shader.
		///
		inline constexpr const char* const smaa_header_vert = R"(
            #version 450 core
			
			#ifndef SMAA_PIXEL_SIZE
				#define SMAA_PIXEL_SIZE vec2(1.0 / 800.0, 1.0 / 600.0)
			#endif
			
            #define SMAA_PRESET_ULTRA 1
			#define SMAA_GLSL_4 1
			#define SMAA_ONLY_COMPILE_VS 1
        )";

		///
		/// SMAA core fragment shader.
		///
		inline constexpr const char* const smaa_header_frag = R"(
            #version 450 core

			#ifndef SMAA_PIXEL_SIZE
				#define SMAA_PIXEL_SIZE vec2(1.0 / 800.0, 1.0 / 600.0)
			#endif

			#define SMAA_PRESET_ULTRA 1
			#define SMAA_GLSL_4 1
			#define SMAA_ONLY_COMPILE_PS 1
		)";

		///
		/// SMAA edge vertex shader.
		///
		inline constexpr const char* const smaa_edge_vert = R"(
			layout (location = 0) in vec3 l_pos;
            layout (location = 1) in vec2 l_texels;

            out vec2 texcoord;
			out vec4 offset[3];
			out vec4 dummy2;
			
			void main()
			{
				texcoord = l_texels;
				gl_Position = vec4(l_pos, 1.0);
				
				vec4 dummy1 = vec4(0);
				SMAAEdgeDetectionVS(dummy1, dummy2, texcoord, offset);
			}
		)";

		///
		/// SMAA edge frag shader.
		///
		inline constexpr const char* const smaa_edge_frag = R"(
			in vec2 texcoord;
			in vec4 offset[3];
			in vec4 dummy2;

            out vec4 frag_colour;

			uniform sampler2D albedo_tex;

			void main()
			{
				#if SMAA_PREDICATION == 1
					frag_colour = SMAAColorEdgeDetectionPS(texcoord, offset, albedo_tex, depthTex);
				#else
					frag_colour = SMAAColorEdgeDetectionPS(texcoord, offset, albedo_tex);
				#endif
			}
		)";

		inline constexpr const char* const smaa_blend_vert = R"(
            layout (location = 0) in vec3 l_pos;
            layout (location = 1) in vec2 l_texels;

			out vec2 texcoord;
			out vec2 pixcoord;
			out vec4 offset[3];
			out vec4 dummy2;
			
			void main()
			{
				texcoord = l_texels;
				gl_Position = vec4(l_pos, 1.0);
				
				vec4 dummy1 = vec4(0);
				SMAABlendingWeightCalculationVS(dummy1, dummy2, texcoord, pixcoord, offset);
		    }
		)";

		inline constexpr const char* const smaa_blend_frag = R"(
			in vec2 texcoord;
			in vec2 pixcoord;
			in vec4 offset[3];
			in vec4 dummy2;

            out vec4 frag_colour;
			
            uniform sampler2D edge_tex;
			uniform sampler2D area_tex;
			uniform sampler2D search_tex;

			void main()
			{
				frag_colour = SMAABlendingWeightCalculationPS(texcoord, pixcoord, offset, edge_tex, area_tex, search_tex, ivec4(0));
			}
		)";

		inline constexpr const char* const smaa_neighbour_vert = R"(
            layout (location = 0) in vec3 l_pos;
            layout (location = 1) in vec2 l_texels;

			out vec2 texcoord;
			out vec4 offset[2];
			out vec4 dummy2;
			
			void main()
			{
			    texcoord = l_texels;
			    gl_Position = vec4(l_pos, 1.0);
			
			    vec4 dummy1 = vec4(0);
			    SMAANeighborhoodBlendingVS(dummy1, dummy2, texcoord, offset);
			}
		)";

		inline constexpr const char* const smaa_neighbour_frag = R"(
			in vec2 texcoord;
		    in vec4 offset[2];
			in vec4 dummy2;
			
            out vec4 frag_colour;

            uniform sampler2D albedo_tex;
			uniform sampler2D blend_tex;

			void main()
			{
			    frag_colour = SMAANeighborhoodBlendingPS(texcoord, offset, albedo_tex, blend_tex);
			}
		)";

		inline constexpr const char* const smaa_core_part1 = R"(
            #define API_USED

            #if SMAA_PRESET_LOW == 1
            #define SMAA_THRESHOLD 0.15
            #define SMAA_MAX_SEARCH_STEPS 4
            #define SMAA_MAX_SEARCH_STEPS_DIAG 0
            #define SMAA_CORNER_ROUNDING 100
            #elif SMAA_PRESET_MEDIUM == 1
            #define SMAA_THRESHOLD 0.1
            #define SMAA_MAX_SEARCH_STEPS 8
            #define SMAA_MAX_SEARCH_STEPS_DIAG 0
            #define SMAA_CORNER_ROUNDING 100
            #elif SMAA_PRESET_HIGH == 1
            #define SMAA_THRESHOLD 0.1
            #define SMAA_MAX_SEARCH_STEPS 16
            #define SMAA_MAX_SEARCH_STEPS_DIAG 8
            #define SMAA_CORNER_ROUNDING 25
            #elif SMAA_PRESET_ULTRA == 1
            #define SMAA_THRESHOLD 0.05
            #define SMAA_MAX_SEARCH_STEPS 32
            #define SMAA_MAX_SEARCH_STEPS_DIAG 16
            #define SMAA_CORNER_ROUNDING 25
            #endif

            #ifndef SMAA_THRESHOLD
            #define SMAA_THRESHOLD 0.1
            #endif

            #ifndef SMAA_DEPTH_THRESHOLD
            #define SMAA_DEPTH_THRESHOLD (0.1 * SMAA_THRESHOLD)
            #endif

            #ifndef SMAA_MAX_SEARCH_STEPS
            #define SMAA_MAX_SEARCH_STEPS 16
            #endif

            #ifndef SMAA_MAX_SEARCH_STEPS_DIAG
            #define SMAA_MAX_SEARCH_STEPS_DIAG 8
            #endif

            #ifndef SMAA_CORNER_ROUNDING
            #define SMAA_CORNER_ROUNDING 25
            #endif

            #ifndef SMAA_PREDICATION
            #define SMAA_PREDICATION 0
            #endif

            #ifndef SMAA_PREDICATION_THRESHOLD
            #define SMAA_PREDICATION_THRESHOLD 0.01
            #endif

            #ifndef SMAA_PREDICATION_SCALE
            #define SMAA_PREDICATION_SCALE 2.0
            #endif

            #ifndef SMAA_PREDICATION_STRENGTH
            #define SMAA_PREDICATION_STRENGTH 0.4
            #endif

            #ifndef SMAA_REPROJECTION
            #define SMAA_REPROJECTION 0
            #endif

            #define SMAA_REPROJECTION_WEIGHT_SCALE 30.0

            #ifndef SMAA_DIRECTX9_LINEAR_BLEND
            #define SMAA_DIRECTX9_LINEAR_BLEND 0
            #endif

            #ifndef SMAA_ONLY_COMPILE_VS
            #define SMAA_ONLY_COMPILE_VS 0
            #endif
            #ifndef SMAA_ONLY_COMPILE_PS
            #define SMAA_ONLY_COMPILE_PS 0
            #endif

            #ifndef SMAA_AREATEX_MAX_DISTANCE
            #define SMAA_AREATEX_MAX_DISTANCE 16
            #endif
            #ifndef SMAA_AREATEX_MAX_DISTANCE_DIAG
            #define SMAA_AREATEX_MAX_DISTANCE_DIAG 20
            #endif
            #define SMAA_AREATEX_PIXEL_SIZE (1.0 / float2(160.0, 560.0))
            #define SMAA_AREATEX_SUBTEX_SIZE (1.0 / 7.0)

            #if SMAA_HLSL_3 == 1
            #define SMAATexture2D sampler2D
            #define SMAASampleLevelZero(tex, coord) tex2Dlod(tex, float4(coord, 0.0, 0.0))
            #define SMAASampleLevelZeroPoint(tex, coord) tex2Dlod(tex, float4(coord, 0.0, 0.0))
            #define SMAASample(tex, coord) tex2D(tex, coord)
            #define SMAASamplePoint(tex, coord) tex2D(tex, coord)
            #define SMAASampleLevelZeroOffset(tex, coord, offset) tex2Dlod(tex, float4(coord + offset * SMAA_PIXEL_SIZE, 0.0, 0.0))
            #define SMAASampleOffset(tex, coord, offset) tex2D(tex, coord + offset * SMAA_PIXEL_SIZE)
            #define SMAALerp(a, b, t) lerp(a, b, t)
            #define SMAASaturate(a) saturate(a)
            #define SMAAMad(a, b, c) mad(a, b, c)
            #define SMAA_FLATTEN [flatten]
            #define SMAA_BRANCH [branch]
            #endif
            #if SMAA_HLSL_4 == 1 || SMAA_HLSL_4_1 == 1
            SamplerState LinearSampler { Filter = MIN_MAG_LINEAR_MIP_POINT; AddressU = Clamp; AddressV = Clamp; };
            SamplerState PointSampler { Filter = MIN_MAG_MIP_POINT; AddressU = Clamp; AddressV = Clamp; };
            #define SMAATexture2D Texture2D
            #define SMAASampleLevelZero(tex, coord) tex.SampleLevel(LinearSampler, coord, 0)
            #define SMAASampleLevelZeroPoint(tex, coord) tex.SampleLevel(PointSampler, coord, 0)
            #define SMAASample(tex, coord) SMAASampleLevelZero(tex, coord)
            #define SMAASamplePoint(tex, coord) SMAASampleLevelZeroPoint(tex, coord)
            #define SMAASampleLevelZeroOffset(tex, coord, offset) tex.SampleLevel(LinearSampler, coord, 0, offset)
            #define SMAASampleOffset(tex, coord, offset) SMAASampleLevelZeroOffset(tex, coord, offset)
            #define SMAALerp(a, b, t) lerp(a, b, t)
            #define SMAASaturate(a) saturate(a)
            #define SMAAMad(a, b, c) mad(a, b, c)
            #define SMAA_FLATTEN [flatten]
            #define SMAA_BRANCH [branch]
            #define SMAATexture2DMS2 Texture2DMS<float4, 2>
            #define SMAALoad(tex, pos, sample) tex.Load(pos, sample)
            #endif
            #if SMAA_HLSL_4_1 == 1
            #define SMAAGather(tex, coord) tex.Gather(LinearSampler, coord, 0)
            #endif
            #if SMAA_GLSL_3 == 1 || SMAA_GLSL_4 == 1
            #define SMAATexture2D sampler2D
            #define SMAASampleLevelZero(tex, coord) textureLod(tex, coord, 0.0)
            #define SMAASampleLevelZeroPoint(tex, coord) textureLod(tex, coord, 0.0)
            #define SMAASample(tex, coord) texture(tex, coord)
            #define SMAASamplePoint(tex, coord) texture(tex, coord)
            #define SMAASampleLevelZeroOffset(tex, coord, offset) textureLodOffset(tex, coord, 0.0, offset)
            #define SMAASampleOffset(tex, coord, offset) texture(tex, coord, offset)
            #define SMAALerp(a, b, t) mix(a, b, t)
            #define SMAASaturate(a) clamp(a, 0.0, 1.0)
            #define SMAA_FLATTEN
            #define SMAA_BRANCH
            #define float2 vec2
            #define float3 vec3
            #define float4 vec4
            #define int2 ivec2
            #define int3 ivec3
            #define int4 ivec4
            #endif
            #if SMAA_GLSL_3 == 1
            #define SMAAMad(a, b, c) (a * b + c)
            #endif
            #if SMAA_GLSL_4 == 1
            #define SMAAMad(a, b, c) fma(a, b, c)
            #define SMAAGather(tex, coord) textureGather(tex, coord)
            #endif
        )";

		inline constexpr const char* const smaa_core_part2 = R"(
            float3 SMAAGatherNeighbours(float2 texcoord,
                                        float4 offset[3],
                                        SMAATexture2D tex) {
                #if SMAA_HLSL_4_1 == 1 || SMAA_GLSL_4 == 1
                return SMAAGather(tex, texcoord + SMAA_PIXEL_SIZE * float2(-0.5, -0.5)).grb;
                #else
                float P = SMAASample(tex, texcoord).r;
                float Pleft = SMAASample(tex, offset[0].xy).r;
                float Ptop  = SMAASample(tex, offset[0].zw).r;
                return float3(P, Pleft, Ptop);
                #endif
            }

            float2 SMAACalculatePredicatedThreshold(float2 texcoord,
                                                    float4 offset[3],
                                                    SMAATexture2D colorTex,
                                                    SMAATexture2D predicationTex) {
                float3 neighbours = SMAAGatherNeighbours(texcoord, offset, predicationTex);
                float2 delta = abs(neighbours.xx - neighbours.yz);
                float2 edges = step(SMAA_PREDICATION_THRESHOLD, delta);
                return SMAA_PREDICATION_SCALE * SMAA_THRESHOLD * (1.0 - SMAA_PREDICATION_STRENGTH * edges);
            }

            #if SMAA_ONLY_COMPILE_PS == 0
           
            void SMAAEdgeDetectionVS(float4 position,
                                     out float4 svPosition,
                                     inout float2 texcoord,
                                     out float4 offset[3]) {
                svPosition = position;

                offset[0] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4(-1.0, 0.0, 0.0, -1.0);
                offset[1] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4( 1.0, 0.0, 0.0,  1.0);
                offset[2] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4(-2.0, 0.0, 0.0, -2.0);
            }

            void SMAABlendingWeightCalculationVS(float4 position,
                                                 out float4 svPosition,
                                                 inout float2 texcoord,
                                                 out float2 pixcoord,
                                                 out float4 offset[3]) {
                svPosition = position;

                pixcoord = texcoord / SMAA_PIXEL_SIZE;

                offset[0] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4(-0.25, -0.125,  1.25, -0.125);
                offset[1] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4(-0.125, -0.25, -0.125,  1.25);

                offset[2] = float4(offset[0].xz, offset[1].yw) + 
                            float4(-2.0, 2.0, -2.0, 2.0) *
                            SMAA_PIXEL_SIZE.xxyy * float(SMAA_MAX_SEARCH_STEPS);
            }

            void SMAANeighborhoodBlendingVS(float4 position,
                                            out float4 svPosition,
                                            inout float2 texcoord,
                                            out float4 offset[2]) {
                svPosition = position;

                offset[0] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4(-1.0, 0.0, 0.0, -1.0);
                offset[1] = texcoord.xyxy + SMAA_PIXEL_SIZE.xyxy * float4( 1.0, 0.0, 0.0,  1.0);
            }

            void SMAAResolveVS(float4 position,
                               out float4 svPosition,
                               inout float2 texcoord) {
                svPosition = position;
            }

            void SMAASeparateVS(float4 position,
                                out float4 svPosition,
                                inout float2 texcoord) {
                svPosition = position;
            }
            #endif

            #if SMAA_ONLY_COMPILE_VS == 0

            float4 SMAALumaEdgeDetectionPS(float2 texcoord,
                                           float4 offset[3],
                                           SMAATexture2D colorTex
                                           #if SMAA_PREDICATION == 1
                                           , SMAATexture2D predicationTex
                                           #endif
                                           ) {
                #if SMAA_PREDICATION == 1
                float2 threshold = SMAACalculatePredicatedThreshold(texcoord, offset, colorTex, predicationTex);
                #else
                float2 threshold = float2(SMAA_THRESHOLD, SMAA_THRESHOLD);
                #endif

                float3 weights = float3(0.2126, 0.7152, 0.0722);
                float L = dot(SMAASample(colorTex, texcoord).rgb, weights);
                float Lleft = dot(SMAASample(colorTex, offset[0].xy).rgb, weights);
                float Ltop  = dot(SMAASample(colorTex, offset[0].zw).rgb, weights);

                float4 delta;
                delta.xy = abs(L - float2(Lleft, Ltop));
                float2 edges = step(threshold, delta.xy);

                if (dot(edges, float2(1.0, 1.0)) == 0.0)
                    discard;

                float Lright = dot(SMAASample(colorTex, offset[1].xy).rgb, weights);
                float Lbottom  = dot(SMAASample(colorTex, offset[1].zw).rgb, weights);
                delta.zw = abs(L - float2(Lright, Lbottom));

                float2 maxDelta = max(delta.xy, delta.zw);
                maxDelta = max(maxDelta.xx, maxDelta.yy);

                float Lleftleft = dot(SMAASample(colorTex, offset[2].xy).rgb, weights);
                float Ltoptop = dot(SMAASample(colorTex, offset[2].zw).rgb, weights);
                delta.zw = abs(float2(Lleft, Ltop) - float2(Lleftleft, Ltoptop));

                maxDelta = max(maxDelta.xy, delta.zw);

                edges.xy *= step(0.5 * maxDelta, delta.xy);

                return float4(edges, 0.0, 0.0);
            }

            float4 SMAAColorEdgeDetectionPS(float2 texcoord,
                                            float4 offset[3],
                                            SMAATexture2D colorTex
                                            #if SMAA_PREDICATION == 1
                                            , SMAATexture2D predicationTex
                                            #endif
                                            ) {
                
                #if SMAA_PREDICATION == 1
                float2 threshold = SMAACalculatePredicatedThreshold(texcoord, offset, colorTex, predicationTex);
                #else
                float2 threshold = float2(SMAA_THRESHOLD, SMAA_THRESHOLD);
                #endif

                float4 delta;
                float3 C = SMAASample(colorTex, texcoord).rgb;

                float3 Cleft = SMAASample(colorTex, offset[0].xy).rgb;
                float3 t = abs(C - Cleft);
                delta.x = max(max(t.r, t.g), t.b);

                float3 Ctop  = SMAASample(colorTex, offset[0].zw).rgb;
                t = abs(C - Ctop);
                delta.y = max(max(t.r, t.g), t.b);

                float2 edges = step(threshold, delta.xy);

                if (dot(edges, float2(1.0, 1.0)) == 0.0)
                    discard;

                float3 Cright = SMAASample(colorTex, offset[1].xy).rgb;
                t = abs(C - Cright);
                delta.z = max(max(t.r, t.g), t.b);

                float3 Cbottom  = SMAASample(colorTex, offset[1].zw).rgb;
                t = abs(C - Cbottom);
                delta.w = max(max(t.r, t.g), t.b);

                float maxDelta = max(max(max(delta.x, delta.y), delta.z), delta.w);

                float3 Cleftleft  = SMAASample(colorTex, offset[2].xy).rgb;
                t = abs(C - Cleftleft);
                delta.z = max(max(t.r, t.g), t.b);

                float3 Ctoptop = SMAASample(colorTex, offset[2].zw).rgb;
                t = abs(C - Ctoptop);
                delta.w = max(max(t.r, t.g), t.b);

                maxDelta = max(max(maxDelta, delta.z), delta.w);

                edges.xy *= step(0.5 * maxDelta, delta.xy);

                return float4(edges, 0.0, 0.0);
            }

            float4 SMAADepthEdgeDetectionPS(float2 texcoord,
                                            float4 offset[3],
                                            SMAATexture2D depthTex) {
                float3 neighbours = SMAAGatherNeighbours(texcoord, offset, depthTex);
                float2 delta = abs(neighbours.xx - float2(neighbours.y, neighbours.z));
                float2 edges = step(SMAA_DEPTH_THRESHOLD, delta);

                if (dot(edges, float2(1.0, 1.0)) == 0.0)
                    discard;

                return float4(edges, 0.0, 0.0);
            }
        )";

		inline constexpr const char* const smaa_core_part3 = R"(
            #if SMAA_MAX_SEARCH_STEPS_DIAG > 0 || SMAA_FORCE_DIAGONAL_DETECTION == 1

            float SMAASearchDiag1(SMAATexture2D edgesTex, float2 texcoord, float2 dir, float c) {
                texcoord += dir * SMAA_PIXEL_SIZE;
                float2 e = float2(0.0, 0.0);
                float i;
                for (i = 0.0; i < float(SMAA_MAX_SEARCH_STEPS_DIAG); i++) {
                    e.rg = SMAASampleLevelZero(edgesTex, texcoord).rg;
                    SMAA_FLATTEN if (dot(e, float2(1.0, 1.0)) < 1.9) break;
                    texcoord += dir * SMAA_PIXEL_SIZE;
                }
                return i + float(e.g > 0.9) * c;
            }

            float SMAASearchDiag2(SMAATexture2D edgesTex, float2 texcoord, float2 dir, float c) {
                texcoord += dir * SMAA_PIXEL_SIZE;
                float2 e = float2(0.0, 0.0);
                float i;
                for (i = 0.0; i < float(SMAA_MAX_SEARCH_STEPS_DIAG); i++) {
                    e.g = SMAASampleLevelZero(edgesTex, texcoord).g;
                    e.r = SMAASampleLevelZeroOffset(edgesTex, texcoord, int2(1, 0)).r;
                    SMAA_FLATTEN if (dot(e, float2(1.0, 1.0)) < 1.9) break;
                    texcoord += dir * SMAA_PIXEL_SIZE;
                }
                return i + float(e.g > 0.9) * c;
            }

            float2 SMAAAreaDiag(SMAATexture2D areaTex, float2 dist, float2 e, float offset) {
                float2 texcoord = float(SMAA_AREATEX_MAX_DISTANCE_DIAG) * e + dist;

                texcoord = SMAA_AREATEX_PIXEL_SIZE * texcoord + (0.5 * SMAA_AREATEX_PIXEL_SIZE);

                texcoord.x += 0.5;

                texcoord.y += SMAA_AREATEX_SUBTEX_SIZE * offset;

                #if SMAA_HLSL_3 == 1
                return SMAASampleLevelZero(areaTex, texcoord).ra;
                #else
                return SMAASampleLevelZero(areaTex, texcoord).rg;
                #endif
            }

            float2 SMAACalculateDiagWeights(SMAATexture2D edgesTex, SMAATexture2D areaTex, float2 texcoord, float2 e, int4 subsampleIndices) {
                float2 weights = float2(0.0, 0.0);

                float2 d;
                d.x = e.r > 0.0? SMAASearchDiag1(edgesTex, texcoord, float2(-1.0,  1.0), 1.0) : 0.0;
                d.y = SMAASearchDiag1(edgesTex, texcoord, float2(1.0, -1.0), 0.0);

                SMAA_BRANCH
                if (d.r + d.g > 2.0) {
                    float4 coords = SMAAMad(float4(-d.r, d.r, d.g, -d.g), SMAA_PIXEL_SIZE.xyxy, texcoord.xyxy);

                    float4 c;
                    c.x = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2(-1,  0)).g;
                    c.y = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2( 0,  0)).r;
                    c.z = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1,  0)).g;
                    c.w = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1, -1)).r;
                    float2 e = 2.0 * c.xz + c.yw;
                    float t = float(SMAA_MAX_SEARCH_STEPS_DIAG) - 1.0;
                    e *= step(d.rg, float2(t, t));

                    weights += SMAAAreaDiag(areaTex, d, e, float(subsampleIndices.z));
                }

                d.x = SMAASearchDiag2(edgesTex, texcoord, float2(-1.0, -1.0), 0.0);
                float right = SMAASampleLevelZeroOffset(edgesTex, texcoord, int2(1, 0)).r;
                d.y = right > 0.0? SMAASearchDiag2(edgesTex, texcoord, float2(1.0, 1.0), 1.0) : 0.0;

                SMAA_BRANCH
                if (d.r + d.g > 2.0) {
                    float4 coords = SMAAMad(float4(-d.r, -d.r, d.g, d.g), SMAA_PIXEL_SIZE.xyxy, texcoord.xyxy);

                    float4 c;
                    c.x  = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2(-1,  0)).g;
                    c.y  = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2( 0, -1)).r;
                    c.zw = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1,  0)).gr;
                    float2 e = 2.0 * c.xz + c.yw;
                    float t = float(SMAA_MAX_SEARCH_STEPS_DIAG) - 1.0;
                    e *= step(d.rg, float2(t, t));

                    weights += SMAAAreaDiag(areaTex, d, e, float(subsampleIndices.w)).gr;
                }

                return weights;
            }
            #endif

            float SMAASearchLength(SMAATexture2D searchTex, float2 e, float bias, float scale) {
                e.r = bias + e.r * scale;
	            e.g = -e.g;
                return 255.0 * SMAASampleLevelZeroPoint(searchTex, e).r;
            }

            float SMAASearchXLeft(SMAATexture2D edgesTex, SMAATexture2D searchTex, float2 texcoord, float end) {
                float2 e = float2(0.0, 1.0);
                while (texcoord.x > end && 
                       e.g > 0.8281 &&
                       e.r == 0.0) {
                    e = SMAASampleLevelZero(edgesTex, texcoord).rg;
                    texcoord -= float2(2.0, 0.0) * SMAA_PIXEL_SIZE;
                }

                texcoord.x += 0.25 * SMAA_PIXEL_SIZE.x;

                texcoord.x += SMAA_PIXEL_SIZE.x;

                texcoord.x += 2.0 * SMAA_PIXEL_SIZE.x;
                texcoord.x -= SMAA_PIXEL_SIZE.x * SMAASearchLength(searchTex, e, 0.0, 0.5);

                return texcoord.x;
            }

            float SMAASearchXRight(SMAATexture2D edgesTex, SMAATexture2D searchTex, float2 texcoord, float end) {
                float2 e = float2(0.0, 1.0);
                while (texcoord.x < end && 
                       e.g > 0.8281 &&
                       e.r == 0.0) {
                    e = SMAASampleLevelZero(edgesTex, texcoord).rg;
                    texcoord += float2(2.0, 0.0) * SMAA_PIXEL_SIZE;
                }

                texcoord.x -= 0.25 * SMAA_PIXEL_SIZE.x;
                texcoord.x -= SMAA_PIXEL_SIZE.x;
                texcoord.x -= 2.0 * SMAA_PIXEL_SIZE.x;
                texcoord.x += SMAA_PIXEL_SIZE.x * SMAASearchLength(searchTex, e, 0.5, 0.5);
                return texcoord.x;
            }

            float SMAASearchYUp(SMAATexture2D edgesTex, SMAATexture2D searchTex, float2 texcoord, float end) {
                float2 e = float2(1.0, 0.0);
                while (texcoord.y > end && 
                       e.r > 0.8281 &&
                       e.g == 0.0) {
                    e = SMAASampleLevelZero(edgesTex, texcoord).rg;
                    texcoord -= float2(0.0, 2.0) * SMAA_PIXEL_SIZE;
                }

                texcoord.y += 0.25 * SMAA_PIXEL_SIZE.y;
                texcoord.y += SMAA_PIXEL_SIZE.y;
                texcoord.y += 2.0 * SMAA_PIXEL_SIZE.y;
                texcoord.y -= SMAA_PIXEL_SIZE.y * SMAASearchLength(searchTex, e.gr, 0.0, 0.5);
                return texcoord.y;
            }

            float SMAASearchYDown(SMAATexture2D edgesTex, SMAATexture2D searchTex, float2 texcoord, float end) {
                float2 e = float2(1.0, 0.0);
                while (texcoord.y < end && 
                       e.r > 0.8281 &&
                       e.g == 0.0) {
                    e = SMAASampleLevelZero(edgesTex, texcoord).rg;
                    texcoord += float2(0.0, 2.0) * SMAA_PIXEL_SIZE;
                }
    
                texcoord.y -= 0.25 * SMAA_PIXEL_SIZE.y;
                texcoord.y -= SMAA_PIXEL_SIZE.y;
                texcoord.y -= 2.0 * SMAA_PIXEL_SIZE.y;
                texcoord.y += SMAA_PIXEL_SIZE.y * SMAASearchLength(searchTex, e.gr, 0.5, 0.5);
                return texcoord.y;
            }

            float2 SMAAArea(SMAATexture2D areaTex, float2 dist, float e1, float e2, float offset) {
                float2 texcoord = float(SMAA_AREATEX_MAX_DISTANCE) * round(4.0 * float2(e1, e2)) + dist;
    
                texcoord = SMAA_AREATEX_PIXEL_SIZE * texcoord + (0.5 * SMAA_AREATEX_PIXEL_SIZE);

                texcoord.y += SMAA_AREATEX_SUBTEX_SIZE * offset;

                #if SMAA_HLSL_3 == 1
                return SMAASampleLevelZero(areaTex, texcoord).ra;
                #else
                return SMAASampleLevelZero(areaTex, texcoord).rg;
                #endif
            }

            void SMAADetectHorizontalCornerPattern(SMAATexture2D edgesTex, inout float2 weights, float2 texcoord, float2 d) {
                #if SMAA_CORNER_ROUNDING < 100 || SMAA_FORCE_CORNER_DETECTION == 1
                float4 coords = SMAAMad(float4(d.x, 0.0, d.y, 0.0),
                                        SMAA_PIXEL_SIZE.xyxy, texcoord.xyxy);
                float2 e;
                e.r = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2(0.0,  1.0)).r;
                bool left = abs(d.x) < abs(d.y);
                e.g = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2(0.0, -2.0)).r;
                if (left) weights *= SMAASaturate(float(SMAA_CORNER_ROUNDING) / 100.0 + 1.0 - e);

                e.r = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2(1.0,  1.0)).r;
                e.g = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2(1.0, -2.0)).r;
                if (!left) weights *= SMAASaturate(float(SMAA_CORNER_ROUNDING) / 100.0 + 1.0 - e);
                #endif
            }

            void SMAADetectVerticalCornerPattern(SMAATexture2D edgesTex, inout float2 weights, float2 texcoord, float2 d) {
                #if SMAA_CORNER_ROUNDING < 100 || SMAA_FORCE_CORNER_DETECTION == 1
                float4 coords = SMAAMad(float4(0.0, d.x, 0.0, d.y),
                                        SMAA_PIXEL_SIZE.xyxy, texcoord.xyxy);
                float2 e;
                e.r = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2( 1.0, 0.0)).g;
                bool left = abs(d.x) < abs(d.y);
                e.g = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2(-2.0, 0.0)).g;
                if (left) weights *= SMAASaturate(float(SMAA_CORNER_ROUNDING) / 100.0 + 1.0 - e);

                e.r = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1.0, 1.0)).g;
                e.g = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2(-2.0, 1.0)).g;
                if (!left) weights *= SMAASaturate(float(SMAA_CORNER_ROUNDING) / 100.0 + 1.0 - e);
                #endif
            }
        )";

		inline constexpr const char* const smaa_core_part4 = R"(
            float4 SMAABlendingWeightCalculationPS(float2 texcoord,
                                                   float2 pixcoord,
                                                   float4 offset[3],
                                                   SMAATexture2D edgesTex, 
                                                   SMAATexture2D areaTex, 
                                                   SMAATexture2D searchTex,
                                                   int4 subsampleIndices) {
                float4 weights = float4(0.0, 0.0, 0.0, 0.0);

                float2 e = SMAASample(edgesTex, texcoord).rg;

                SMAA_BRANCH
                if (e.g > 0.0) {
                    #if SMAA_MAX_SEARCH_STEPS_DIAG > 0 || SMAA_FORCE_DIAGONAL_DETECTION == 1
                    weights.rg = SMAACalculateDiagWeights(edgesTex, areaTex, texcoord, e, subsampleIndices);

                    SMAA_BRANCH
                    if (dot(weights.rg, float2(1.0, 1.0)) == 0.0) {
                    #endif

                    float2 d;

                    float2 coords;
                    coords.x = SMAASearchXLeft(edgesTex, searchTex, offset[0].xy, offset[2].x);
                    coords.y = offset[1].y;
                    d.x = coords.x;

                    float e1 = SMAASampleLevelZero(edgesTex, coords).r;

                    coords.x = SMAASearchXRight(edgesTex, searchTex, offset[0].zw, offset[2].y);
                    d.y = coords.x;

                    d = d / SMAA_PIXEL_SIZE.x - pixcoord.x;

                    float2 sqrt_d = sqrt(abs(d));

                    float e2 = SMAASampleLevelZeroOffset(edgesTex, coords, int2(1, 0)).r;

                    weights.rg = SMAAArea(areaTex, sqrt_d, e1, e2, float(subsampleIndices.y));

                    SMAADetectHorizontalCornerPattern(edgesTex, weights.rg, texcoord, d);

                    #if SMAA_MAX_SEARCH_STEPS_DIAG > 0 || SMAA_FORCE_DIAGONAL_DETECTION == 1
                    } else
                        e.r = 0.0;
                    #endif
                }

                SMAA_BRANCH
                if (e.r > 0.0) {
                    float2 d;

                    float2 coords;
                    coords.y = SMAASearchYUp(edgesTex, searchTex, offset[1].xy, offset[2].z);
                    coords.x = offset[0].x;
                    d.x = coords.y;

                    float e1 = SMAASampleLevelZero(edgesTex, coords).g;

                    coords.y = SMAASearchYDown(edgesTex, searchTex, offset[1].zw, offset[2].w);
                    d.y = coords.y;

                    d = d / SMAA_PIXEL_SIZE.y - pixcoord.y;

                    float2 sqrt_d = sqrt(abs(d));

                    float e2 = SMAASampleLevelZeroOffset(edgesTex, coords, int2(0, 1)).g;

                    weights.ba = SMAAArea(areaTex, sqrt_d, e1, e2, float(subsampleIndices.x));

                    SMAADetectVerticalCornerPattern(edgesTex, weights.ba, texcoord, d);
                }

                return weights;
            }

            float4 SMAANeighborhoodBlendingPS(float2 texcoord,
                                              float4 offset[2],
                                              SMAATexture2D colorTex,
                                              SMAATexture2D blendTex) {
                
                float4 a;
                a.xz = SMAASample(blendTex, texcoord).xz;
                a.y = SMAASample(blendTex, offset[1].zw).g;
                a.w = SMAASample(blendTex, offset[1].xy).a;

                SMAA_BRANCH
                if (dot(a, float4(1.0, 1.0, 1.0, 1.0)) < 1e-5)
                    return SMAASampleLevelZero(colorTex, texcoord);
                else {
                    float4 color = float4(0.0, 0.0, 0.0, 0.0);

                    float2 offset;
                    offset.x = a.a > a.b? a.a : -a.b;
                    offset.y = a.g > a.r? a.g : -a.r;

                    if (abs(offset.x) > abs(offset.y))
                        offset.y = 0.0;
                    else
                        offset.x = 0.0;

                    #if SMAA_REPROJECTION == 1
                    
                    float4 C = SMAASampleLevelZero(colorTex, texcoord);
                    texcoord += sign(offset) * SMAA_PIXEL_SIZE;
                    float4 Cop = SMAASampleLevelZero(colorTex, texcoord);
                    float s = abs(offset.x) > abs(offset.y)? abs(offset.x) : abs(offset.y);

                    C.a *= C.a;
                    Cop.a *= Cop.a;

                    float4 Caa = SMAALerp(C, Cop, s);

                    Caa.a = sqrt(Caa.a);
                    return Caa;
                    #elif SMAA_HLSL_4 == 1 || SMAA_DIRECTX9_LINEAR_BLEND == 0
                    texcoord += offset * SMAA_PIXEL_SIZE;
                    return SMAASampleLevelZero(colorTex, texcoord);
                    #else
                    float4 C = SMAASampleLevelZero(colorTex, texcoord);
                    texcoord += sign(offset) * SMAA_PIXEL_SIZE;
                    float4 Cop = SMAASampleLevelZero(colorTex, texcoord);
                    float s = abs(offset.x) > abs(offset.y)? abs(offset.x) : abs(offset.y);
                    return SMAALerp(C, Cop, s);
                    #endif
                }
            }

            float4 SMAAResolvePS(float2 texcoord,
                                 SMAATexture2D colorTexCurr,
                                 SMAATexture2D colorTexPrev
                                 #if SMAA_REPROJECTION == 1
                                 , SMAATexture2D velocityTex
                                 #endif
                                 ) {
                #if SMAA_REPROJECTION == 1
                float2 velocity = -SMAASample(velocityTex, texcoord).rg;

                float4 current = SMAASample(colorTexCurr, texcoord);

                float4 previous = SMAASample(colorTexPrev, texcoord + velocity);

                float delta = abs(current.a * current.a - previous.a * previous.a) / 5.0;
                float weight = 0.5 * SMAASaturate(1.0 - (sqrt(delta) * SMAA_REPROJECTION_WEIGHT_SCALE));

                return SMAALerp(current, previous, weight);
                #else
                float4 current = SMAASample(colorTexCurr, texcoord);
                float4 previous = SMAASample(colorTexPrev, texcoord);
                return SMAALerp(current, previous, 0.5);
                #endif
            }

            #if SMAA_HLSL_4 == 1 || SMAA_HLSL_4_1 == 1
            void SMAASeparatePS(float4 position : SV_POSITION,
                                float2 texcoord : TEXCOORD0,
                                out float4 target0,
                                out float4 target1,
                                uniform SMAATexture2DMS2 colorTexMS) {
                int2 pos = int2(position.xy);
                target0 = SMAALoad(colorTexMS, pos, 0);
                target1 = SMAALoad(colorTexMS, pos, 1);
            }
            #endif
            #endif
		)";
	} // namespace shaders
} // namespace galaxy

#endif
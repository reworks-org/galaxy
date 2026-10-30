#pragma once

#include "surface3d.h"

#include <glm/glm.hpp>

#include <vector>

namespace sl
{
    class Terrain;

    using dvec3 = glm::dvec3;

    /** One Gerstner wave train. */
    struct OceanWave
    {
        /** Crest to crest, in metres. */
        double wavelength = 50.0;
        /** 0 is a sine wave; larger values sharpen the crests (about 0.05 to 0.12 for the default sea). */
        double steepness = 0.1;
        /** Direction the wave travels, in radians from +X toward +Z. */
        double direction = 0.4;
        double phase = 0.0;
    };

    /** Ocean construction parameters. */
    struct OceanConfig
    {
        /** Wave trains; empty uses six trains from 140 m down to 7 m spread over about 2 radians. */
        std::vector<OceanWave> waves;
        /** World Y of the calm surface. */
        double sea_level = 0.0;
        /** Rings and segments of the polar grid around the centre point. More segments smooth the horizon. */
        int rings = 150;
        int segments = 256;
        /** Radius of the first ring and of the last, in metres; rings grow geometrically between them. */
        double inner_radius = 1.5;
        double outer_radius = 14000.0;

        /** Water colour over deep water, and over shallows when a terrain is set (linear 0..1). */
        glm::vec3 deep_colour{0.03f, 0.16f, 0.24f};
        glm::vec3 shallow_colour{0.10f, 0.42f, 0.44f};
        /** Water this deep (metres) or deeper has deep_colour; it blends to shallow_colour toward the shore. */
        double shallow_depth = 10.0;
        /** Water shallower than this (metres) turns see-through, so the sea bed shows (0 turns transparency off). */
        double clear_depth = 3.0;
        /** Foam on the shoreline: it fills water shallower than this (metres) and surges in and out. */
        double foam_depth = 2.5;
        /** Terrain heights are only sampled for the shoreline out to this distance from the centre (metres). */
        double shore_range = 2500.0;
    };

    /**
     * A Gerstner-wave sea. Its surface is a polar grid that follows a centre point (usually the camera or the ship), dense
     * close in and sparse toward the horizon, rebuilt every update() and drawn with draw(). Waves too short for the grid to
     * show fade out with distance so they never alias. The same wave function answers height queries, so ships and
     * floating props can sit on the visible water.
     *
     * With set_terrain() the sea also meets the land: waves flatten as the water shoals, the shallows turn green-blue and
     * transparent, and foam surges along the shore. The terrain's own surface needs no change; the ocean is drawn at
     * `sea_level` and the terrain pokes through it by depth testing, so draw the terrain first.
     *
     * Positions are relative to the eye (as with Terrain::draw), so pass the same double-precision eye to update().
     */
    class Ocean
    {
    public:
        /** Build the grid. Check valid() afterwards (3D must be available). */
        explicit Ocean(const OceanConfig &config = {});
        ~Ocean();
        Ocean(const Ocean &) = delete;
        Ocean &operator=(const Ocean &) = delete;

        bool valid() const;

        /** Sea state scales every wave height: 0.1 is nearly flat, 1 a moderate sea, 2 and more is rough. */
        void set_sea_state(double state);
        double sea_state() const;
        /** The terrain that forms the shore (not owned; may be null). */
        void set_terrain(const Terrain *terrain);
        /** Change the calm-surface height. */
        void set_sea_level(double y);
        double sea_level() const;

        /** The water surface at a world position and time. */
        struct Surface
        {
            /** Position of the displaced point (x and z move a little with the waves). */
            dvec3 position{0.0};
            glm::vec3 normal{0.0f, 1.0f, 0.0f};
            /** 0..1, foam from pinched crests and the shore. */
            float foam = 0.0f;
            /** Water depth over the terrain (metres), or a large value when there is no terrain. */
            double depth = 1.0e9;
        };
        /** The surface above or below the world point (x, z): the rest position is found by inverting the horizontal displacement. */
        Surface surface_at(double x, double z, double time) const;
        /** Shorthand for surface_at(x, z, time).position.y. */
        double height_at(double x, double z, double time) const;
        /** Roughly the crest-to-trough height of the largest swell at the current sea state, in metres. */
        double swell_height() const;

        /** Rebuild the grid around (centre_x, centre_z) for `time` seconds. Call once per frame before draw(). */
        void update(double centre_x, double centre_z, const dvec3 &eye, double time);
        /** A glossy PBR water material, double sided, blended when the shallows are transparent. */
        Material default_material() const;
        /** Draw the grid built by the last update(). */
        bool draw(const Material &material) const;
        bool draw() const { return draw(default_material()); }

    private:
        struct Impl;
        Impl *impl_;
    };
} // namespace sl

#pragma once

#include "flight.h"

#include <string>

namespace sl
{
    /*
     * A head-up display for spacecraft and aircraft, drawn on the screen over the 3D view. It has three modes, cycled
     * with next_mode(): orbit (velocity relative to a reference body, with a ladder and azimuth ribbon referenced to
     * the orbital plane), surface (altitude, speed, heading tape and a horizon pitch ladder) and docking (relative
     * velocity and target markers, plus approach gates leading out of a docking port).
     *
     * The HUD owns no simulation state: the caller fills a HudState every frame with world-space vectors and the
     * readout values, then calls Hud::draw(). Vectors are in any common world frame; the HUD only needs them to agree.
     */

    /** Which set of symbology is shown. */
    enum class HudMode
    {
        off,
        orbit,
        surface,
        docking
    };

    /** The camera the HUD is drawn for, as world-space directions and a focal length in pixels. */
    struct HudCamera
    {
        dvec3 right{1.0, 0.0, 0.0}, up{0.0, 1.0, 0.0}, forward{0.0, 0.0, -1.0};
        /** Distance from the eye to the screen plane in pixels: height / (2 tan(fov_y / 2)). */
        double focal = 800.0;
        int width = 1280, height = 720;
    };

    /** Orbit mode: motion relative to a reference body. */
    struct HudOrbit
    {
        bool valid = false;
        std::string reference;
        /** Vessel velocity and position relative to the reference body, in m/s and m. */
        dvec3 velocity{0.0}, position{0.0};
    };

    /** Surface mode: motion relative to the ground below. */
    struct HudSurface
    {
        bool valid = false;
        std::string reference;
        /** Altitude in metres: above the terrain when radar is true, otherwise above the mean radius. */
        double altitude = 0.0;
        bool radar = false;
        /** Speed in m/s, labelled TAS when airspeed is true and GS (ground speed) otherwise. */
        double speed = 0.0;
        bool airspeed = false;
        /** Velocity over the ground in world coordinates; the velocity marker is hidden when it is almost zero. */
        dvec3 ground_velocity{0.0};
        /** Local vertical (away from the body centre) and the horizontal direction to the north pole. */
        dvec3 up{0.0, 1.0, 0.0}, north{0.0, 0.0, -1.0};
        /** Compass bearing in degrees to a landing target, shown as an index on the heading tape. */
        bool has_target = false;
        double target_bearing = 0.0;
    };

    /** Docking mode: the target vessel and, when known, its docking port. */
    struct HudDocking
    {
        bool valid = false;
        std::string reference, target;
        /** Own velocity relative to the target in m/s, and the target's position relative to own in metres. */
        dvec3 velocity{0.0}, position{0.0};
        /** The target port: position relative to own vessel, outward direction and up direction (world frame). */
        bool has_port = false;
        dvec3 port_position{0.0}, port_direction{0.0, 0.0, 1.0}, port_up{0.0, 1.0, 0.0};
    };

    /** Everything the HUD needs for one frame. */
    struct HudState
    {
        HudCamera camera;
        /** The vessel's nose and roof directions in the world frame. */
        dvec3 nose{0.0, 0.0, -1.0}, roof{0.0, 1.0, 0.0};
        HudOrbit orbit;
        HudSurface surface;
        HudDocking docking;
    };

    /** Name of a mode: "OFF", "ORBIT", "SURFACE" or "DOCKING". */
    const char *hud_mode_name(HudMode mode);
    /** Parse a mode name case-insensitively (also "off"/"none" and "dock"/"srf"); returns false when unknown. */
    bool hud_parse_mode(const std::string &text, HudMode &mode);

    class Hud
    {
    public:
        HudMode mode() const { return mode_; }
        void set_mode(HudMode mode) { mode_ = mode; }
        /** Cycle orbit, surface, docking, off; returns the new mode. */
        HudMode next_mode();

        /** Number of colour schemes and the current one. */
        static int colour_count();
        int colour() const { return colour_; }
        void set_colour(int index);
        /** Step to the next colour scheme. */
        void next_colour();
        /** The current line colour. */
        Colour line_colour() const;

        /** Draw the active mode on the screen; does nothing when the mode is off. */
        void draw(const HudState &state) const;

    private:
        HudMode mode_ = HudMode::off;
        int colour_ = 0;
    };
}

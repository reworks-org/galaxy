#pragma once

#include "clutter.h"

#include <cstdint>

namespace sl
{
    /**
     * A library of low-poly scenery parts, generated in code (no asset files). Every builder returns CPU-side
     * MeshData in the same style as make_conifer_data(): flat-shaded facets with shades in the vertex colours, so
     * parts draw with a plain Material, bake with bake_instances() and cast ordinary shadows.
     *
     * Conventions, shared by every part:
     *   - Units are metres, +Y is up and the part stands on y = 0, centred on x and z.
     *   - The front of a part (house door, car bonnet, ship nose) faces +Z. Long parts (fence, wall, road,
     *     container) run along X, except roads, which run along Z.
     *   - Parameters are sizes and colours; `seed` varies colours, which windows are lit and small details. The same
     *     arguments always give the same geometry.
     *   - Windows are baked into vertex colours as dark or lit quads (no textures).
     *   - Parts are static; moving pieces such as wheels and propellers are not separate.
     */

    // Buildings.
    /** A cottage with a gable roof, chimney, door and windows. The ridge runs along X. */
    MeshData make_house_data(float width = 6.0f, float depth = 5.0f, float height = 3.0f, Colour wall = {214, 196, 160, 255},
                             Colour roof = {150, 64, 48, 255}, std::uint32_t seed = 1);
    /** A flat-roofed office block of `floors` storeys (3.5 m each) with a window grid, entrance and rooftop plant. */
    MeshData make_office_data(float width = 14.0f, float depth = 10.0f, int floors = 4, Colour wall = {170, 172, 176, 255},
                              std::uint32_t seed = 1);
    /** A stepped glass tower of `floors` storeys (3.6 m each) with setbacks and a lit antenna. */
    MeshData make_skyscraper_data(float width = 14.0f, int floors = 30, Colour glass = {66, 96, 128, 255}, std::uint32_t seed = 1);
    /** An aircraft hangar with a vaulted roof and a large dark door on the +Z front. */
    MeshData make_hangar_data(float width = 24.0f, float depth = 30.0f, float height = 9.0f, Colour wall = {170, 175, 180, 255},
                              Colour roof = {118, 128, 140, 255});

    // Structures.
    /** A stone wall of the given length along X, with capstones and slightly raised end piers so runs tile neatly. */
    MeshData make_wall_data(float length = 6.0f, float height = 2.5f, float thickness = 0.4f, Colour colour = {150, 144, 134, 255});
    /** A picket fence section along X: posts, two rails and pointed pickets. */
    MeshData make_fence_data(float length = 6.0f, float height = 1.2f, Colour colour = {176, 140, 96, 255});
    /** A water tower: four braced legs, a tank and a conical lid. */
    MeshData make_water_tower_data(float height = 16.0f, float tank_radius = 3.0f, Colour tank = {120, 140, 160, 255});
    /** A radar: a plinth, a mast and a tilted parabolic dish facing +Z with a feed horn. */
    MeshData make_radar_data(float height = 8.0f, float dish_radius = 3.0f, Colour colour = {205, 208, 212, 255});
    /** A radar dome: a plinth and a faceted white radome. */
    MeshData make_domed_radar_data(float radius = 4.0f, Colour colour = {236, 238, 240, 255});
    /** A pressurised dome for an airless world: a foundation ring, a ribbed shell, an airlock block facing +Z and a vent stack. */
    MeshData make_pressure_dome_data(float radius = 8.0f, Colour colour = {226, 229, 234, 255});
    /** A habitat tube lying along Z on two footings, with ribs, windows along both sides and a hatch at the +Z end. */
    MeshData make_habitat_tube_data(float length = 12.0f, float radius = 2.2f, Colour colour = {222, 224, 226, 255});
    /** A pressurised rover with six wheels, a solar panel and a mast dish; it faces +Z. */
    MeshData make_lunar_rover_data(float length = 3.5f, Colour colour = {214, 214, 208, 255});
    /** A circular landing pad of sintered regolith with a painted ring, an H and amber edge lights. */
    MeshData make_landing_pad_data(float radius = 15.0f);
    /** An airport control tower: a terminal block, a shaft and a flared glazed cab with a roof antenna. */
    MeshData make_control_tower_data(float height = 24.0f, std::uint32_t seed = 1);
    /** A street lamp with a pole and an arm reaching +Z; the lamp head glows underneath at (0, height, arm length). */
    MeshData make_street_lamp_data(float height = 6.0f, Colour colour = {70, 74, 80, 255});
    /** A road segment along Z with kerbs, edge lines and a dashed centre line, 0.06 m thick. */
    MeshData make_road_data(float length = 10.0f, float width = 6.0f);

    // Small props.
    MeshData make_barrel_data(float radius = 0.4f, float height = 1.0f, Colour colour = {60, 90, 150, 255});
    /** A wooden crate with corner posts and edge boards. */
    MeshData make_crate_data(float size = 1.0f, Colour colour = {168, 128, 80, 255});
    /** A shipping container, doors at +Z. Corrugated sides; `seed` shifts the paint. */
    MeshData make_container_data(float length = 6.1f, float width = 2.44f, float height = 2.6f, Colour colour = {160, 52, 42, 255},
                                 std::uint32_t seed = 1);
    /** A vertical storage tank with a domed top, ladder and pipe. */
    MeshData make_tank_data(float radius = 3.0f, float height = 6.0f, Colour colour = {196, 198, 200, 255});

    // Nature.
    /** A low-poly conifer standing on y = 0: a trunk and two stacked foliage cones. Vertex colours carry the shades. */
    MeshData make_conifer_data(float height = 8.0f, float radius = 2.0f, Colour trunk = {96, 64, 40, 255},
                               Colour foliage = {40, 110, 50, 255}, int sides = 8);
    /** A faceted boulder of roughly the given radius, sitting on y = 0; `seed` varies the shape. */
    MeshData make_rock_data(float radius = 1.0f, Colour colour = {120, 118, 112, 255}, std::uint32_t seed = 1);
    /** A broadleaf tree: a short trunk and a canopy of lumpy blobs. */
    MeshData make_tree_data(float height = 6.0f, float radius = 2.0f, Colour trunk = {104, 74, 48, 255}, Colour foliage = {64, 134, 58, 255},
                            std::uint32_t seed = 1);
    MeshData make_bush_data(float radius = 0.8f, Colour colour = {58, 118, 52, 255}, std::uint32_t seed = 1);
    /** Three to five boulders of mixed size gathered together. */
    MeshData make_rock_cluster_data(float radius = 1.5f, Colour colour = {120, 118, 112, 255}, std::uint32_t seed = 1);

    // Vehicles. Their nose points along +Z.
    /** A small car with glazed cabin, wheels and lights. */
    MeshData make_car_data(float length = 4.4f, Colour body = {190, 44, 40, 255}, std::uint32_t seed = 1);
    /** A landed spaceship on three legs: a streamlined hull, canopy, swept wings, a fin and glowing engines. */
    MeshData make_spaceship_data(float length = 8.0f, Colour hull = {170, 176, 190, 255});
    /** A parked twin-engine airliner on its gear, with a window row, swept wings and a tail. */
    MeshData make_aeroplane_data(float wingspan = 12.0f, Colour body = {232, 234, 238, 255}, Colour stripe = {40, 90, 170, 255});

    // Rockets and spacecraft (Apollo era). Rocket and station parts are bodies of revolution about the Y axis, so they
    // stack: each starts on y = 0 and ends at y = its length, and radii match when you give two parts the same radius.
    // Place them with transform_mesh_data(), Builder-style translations or model matrices, and bake or draw as usual.
    // Dimensions default to real Saturn V / Skylab sizes in metres. Umbilical conduits and doors face +Z.

    /** One cylindrical stage (or a short ring such as an instrument unit): accent bands at both ends, tank rings and a +Z umbilical conduit. */
    MeshData make_rocket_stage_data(float radius = 5.05f, float length = 20.0f, Colour body = {236, 236, 230, 255},
                                    Colour accent = {28, 28, 32, 255}, std::uint32_t seed = 1);
    /** An engine cluster hung under a stage of `stage_radius`: 1, 4 or 5 bells (4 and 5 sit on a ring, 5 adds one in the middle). Bell exits are on y = 0, the thrust frame is the top at y = length. */
    MeshData make_rocket_engines_data(float stage_radius = 5.05f, int engines = 5, float length = 5.9f);
    /** A conical interstage or adapter from `bottom_radius` on y = 0 to `top_radius` on y = length. */
    MeshData make_rocket_adapter_data(float bottom_radius = 5.05f, float top_radius = 3.3f, float length = 3.3f,
                                      Colour body = {236, 236, 230, 255}, Colour accent = {28, 28, 32, 255});
    /** `count` swept tail fins around a stage; the root runs from y = 0 to `chord` against a stage of `radius`. The first fin is 45 degrees off +Z. */
    MeshData make_rocket_fins_data(float radius = 5.05f, float span = 3.2f, float chord = 7.0f, int count = 4, Colour colour = {236, 236, 230, 255});
    /** An ogive payload fairing: a short cylinder and a rounded nose, closed at both ends. */
    MeshData make_payload_fairing_data(float radius = 2.0f, float length = 9.0f, Colour body = {236, 236, 230, 255}, Colour accent = {28, 28, 32, 255});
    /** An Apollo launch escape tower: a four-legged truss under a solid motor and nose with a probe. */
    MeshData make_launch_escape_tower_data(float height = 10.0f, Colour motor = {226, 226, 220, 255});
    /** An Apollo command and service module: engine bell, service module with radiator panels and RCS quads, a conical command module with windows and a docking probe. About 7 radii tall; the bell exit is on y = 0. */
    MeshData make_apollo_csm_data(float radius = 1.95f, Colour hull = {220, 220, 212, 255});
    /** A Saturn V on the pad: S-IC with five F-1s and fins, S-II, S-IVB, instrument unit, adapter, boost cover and escape tower, about 115 m tall. */
    MeshData make_saturn_v_data(float scale = 1.0f);
    /** A launch umbilical tower: a braced four-legged lattice `height` tall with swing arms reaching toward +X and a crane on top. */
    MeshData make_launch_tower_data(float height = 120.0f, float width = 8.0f, float arm_length = 14.0f, Colour colour = {196, 70, 52, 255});
    /** A launch pad: a concrete slab with a flame trench opening in the middle and four lightning masts. Top of the slab is at y = 1.5. */
    MeshData make_launch_pad_data(float size = 60.0f);

    /** A cylindrical space-station module (Skylab workshop style): ribbed hull, windows on +Z and a docking port at the top. Axis along Y. */
    MeshData make_station_module_data(float radius = 3.3f, float length = 14.6f, Colour hull = {236, 236, 230, 255}, std::uint32_t seed = 1);
    /** A multiple docking adapter: a short module with an axial port on top and a radial port on +Z. */
    MeshData make_docking_adapter_data(float radius = 1.5f, float length = 5.2f, Colour hull = {200, 200, 196, 255});
    /** A solar array wing: hinged on x = 0, it spreads along +X for `length` metres, `width` wide along Y (centred on y = 0), `panels` hinged panels, cells on both faces. */
    MeshData make_solar_array_data(float length = 10.0f, float width = 3.0f, int panels = 4, Colour cell = {30, 48, 118, 255});
    /** Four solar wings in a cross around a hub on the origin, in the XY plane facing +Z (an Apollo Telescope Mount style windmill). */
    MeshData make_solar_windmill_data(float blade_length = 7.0f, float blade_width = 3.0f);
    /** A dish on a mast: the mast stands on y = 0 and the dish faces +Z, tilted 20 degrees up. */
    MeshData make_antenna_dish_data(float radius = 1.0f);
    /** A satellite body wrapped in gold foil with an instrument tube on +Z, thrusters, a whip antenna and a launch adapter ring at the bottom. About `size` wide. */
    MeshData make_satellite_bus_data(float size = 1.5f, Colour foil = {200, 164, 72, 255}, std::uint32_t seed = 1);
    /** A complete satellite: the bus, two solar wings along X and a dish on top. */
    MeshData make_satellite_data(float size = 1.5f, std::uint32_t seed = 1);
    /** Skylab, standing upright: workshop, airlock shroud and module, docking adapter, two large wings and an Apollo Telescope Mount windmill. About 38 m tall. */
    MeshData make_skylab_data(float scale = 1.0f);

    // Ordnance. Bodies of revolution about the Y axis like the rocket parts: the nose is at y = length, the fins at y = 0.
    /** A general-purpose free-fall bomb: olive casing, ogive nose, a boat-tail and four tail fins joined by a ring. About 3 m long. */
    MeshData make_bomb_data(float radius = 0.25f, float length = 3.0f, Colour casing = {88, 96, 62, 255}, Colour band = {214, 190, 60, 255});
    /** A "Fat Man" implosion bomb: an egg-shaped dark casing with a box-section tail and four fins. About 3.3 m long and 1.5 m across. */
    MeshData make_fatman_data(float radius = 0.75f, float length = 3.3f, Colour casing = {44, 46, 50, 255});

    // Control parts: the pieces of panels, cockpits and cabinets (see controls.h for the interactive layer).
    // Panel parts face +Z with the face of the panel on z = 0, are centred on x and y and are sized in metres.
    // Static parts (panels, dial faces, bases) are baked once; moving parts (needles, knobs, levers, caps) are
    // built around their pivot so they can be rotated or slid with a model matrix. White "lens" and "segment" parts
    // are meant to be tinted and given an emissive colour so they glow.

    /** A flat panel `depth` thick behind the z = 0 face, with a raised frame and four corner screws. */
    MeshData make_panel_data(float width, float height, float depth = 0.03f, Colour colour = {58, 62, 70, 255});
    /** A gauge face with a steel bezel and a scale of `ticks` major marks (with minors) swept over `sweep_degrees`, centred on straight up. */
    MeshData make_dial_data(float radius = 0.1f, int ticks = 11, float sweep_degrees = 270.0f, Colour face = {18, 20, 24, 255});
    /** A needle for make_dial_data(): it points along +Y from the dial centre; rotate about Z (clockwise is negative). */
    MeshData make_needle_data(float radius = 0.1f, Colour colour = {255, 120, 40, 255});
    /** The dark disc and scale marks around a knob. Static. */
    MeshData make_knob_base_data(float radius = 0.04f, int ticks = 11, float sweep_degrees = 270.0f);
    /** A knob with a pointer line along +Y; rotate about Z. */
    MeshData make_knob_data(float radius = 0.04f, float height = 0.04f, Colour colour = {50, 54, 62, 255});
    /** The static nut and collar of a toggle switch. */
    MeshData make_toggle_base_data(float size = 0.04f);
    /** The lever of a toggle switch, pivoting at z = 0.3 * size and pointing along +Z; tilt about X to flip it. */
    MeshData make_toggle_lever_data(float size = 0.04f);
    /** The static bezel of a push button. */
    MeshData make_button_base_data(float radius = 0.03f);
    /** The moving cap of a push button; slide it along -Z when pressed. */
    MeshData make_button_cap_data(float radius = 0.03f, Colour colour = {200, 40, 36, 255});
    /** The static bezel of an indicator lamp or LED. */
    MeshData make_lamp_bezel_data(float radius = 0.015f);
    /** The domed lens of a lamp, white: tint it with the lamp colour and add an emissive colour to make it glow. */
    MeshData make_lamp_lens_data(float radius = 0.015f);
    /** The static track of a slider along Y, with scale marks. */
    MeshData make_slider_track_data(float length = 0.2f, float width = 0.04f);
    /** The cap of a slider, centred on the track. */
    MeshData make_slider_cap_data(float width = 0.04f);
    /** The static dark housing of a level meter. */
    MeshData make_meter_housing_data(float width = 0.03f, float height = 0.2f);
    /** One lit segment of a level meter, white: tint and add emissive to glow. */
    MeshData make_meter_segment_data(float width = 0.026f, float height = 0.014f);

    /** Transform a mesh's positions by `matrix` and its normals by the matrix's rotation (no non-uniform scale). */
    void transform_mesh_data(MeshData &data, const glm::mat4 &matrix);

    // Control assemblies. The panel face is placed on the body; `panel_pose` receives the matrix that puts a panel of
    // `panel_width` x `panel_height` metres (origin at its centre, +Z out) on it.
    /** A small aircraft cockpit: floor, seat, stick, side consoles, canopy frame and a tilted dashboard. The pilot sits at +Z. */
    MeshData make_cockpit_data(float panel_width, float panel_height, glm::mat4 *panel_pose = nullptr);
    /** A freestanding factory control cabinet with a door, handle, vents and hazard stripe; the panel sits on the door. Front faces +Z. */
    MeshData make_control_cabinet_data(float panel_width, float panel_height, glm::mat4 *panel_pose = nullptr);
} // namespace sl

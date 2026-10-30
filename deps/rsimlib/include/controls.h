#pragma once

#include "draw.h"
#include "font.h"
#include "props3d.h"
#include "scene3d.h"
#include "surface3d.h"

#include <functional>
#include <string>
#include <vector>

namespace sl
{
    /**
     * Control panels: dials, knobs, switches, buttons, lamps, sliders and level meters.
     *
     * A ControlPanel holds the layout and state of the controls and handles the mouse. It has no idea how it is drawn, so
     * the same panel works as a flat 2D widget (draw_control_panel()) or as geometry in a 3D scene (ControlPanelView3D).
     *
     * Panel coordinates are panel units with the origin at the top-left corner and y pointing down. Control positions are
     * their centres. Feed the mouse in through a Pointer in the same units, then call update() once per frame.
     */
    enum class ControlKind
    {
        /** A needle gauge. Display only; set its value from your program. */
        dial,
        /** A rotary knob, dragged up/down or left/right. */
        knob,
        /** A two-position toggle switch, clicked to flip. */
        toggle,
        /** A push button, momentary (on while held) or latching (click to flip). */
        button,
        /** An indicator lamp or LED. Display only. */
        lamp,
        /** A linear fader, dragged along its track. */
        slider,
        /** A segmented bar or LED level meter. Display only. */
        meter
    };

    struct Control
    {
        ControlKind kind = ControlKind::dial;
        /** Centre in panel units. */
        float x = 0.0f, y = 0.0f;
        /** Size in panel units. Round controls use `width` as their diameter. */
        float width = 0.0f, height = 0.0f;
        float min = 0.0f, max = 1.0f;
        /** The value, between `min` and `max`. Toggles, buttons and lamps are 0 (off) or 1 (on). */
        float value = 0.0f;
        /** The value as drawn: needles, meters, lamps and switches ease toward `value`. */
        float display = 0.0f;
        /** Buttons: stay on after a click. */
        bool latching = false;
        /** Sliders and meters: run along y (true) or x (false). */
        bool vertical = true;
        /** Lamps: flash while on. */
        bool blink = false;
        /** Lamp and button colour. */
        Colour colour{255, 255, 255, 255};
        /** Shown under the control in 2D and used by ControlPanel::find(). */
        std::string label;
        /** Dials and knobs: the angle covered, centred on straight up. */
        float sweep_degrees = 270.0f;
        /** Meters: the number of segments. */
        int segments = 12;

        /** `value` as 0-1 between `min` and `max`. */
        float fraction() const;
        /** `display` as 0-1 between `min` and `max`. */
        float display_fraction() const;
        /** The needle or pointer angle in radians, clockwise from straight up (uses `display`). */
        float angle() const;
        /** Sliders and meters: the length along their axis. */
        float length() const { return vertical ? height : width; }
        /** Panel y just below the control, where its label belongs. */
        float label_y() const;
    };

    /** The mouse in panel units. */
    struct Pointer
    {
        float x = 0.0f, y = 0.0f;
        bool down = false;
    };

    /** The colour of one meter segment: green for the lower part of the scale, then amber, then red. */
    Colour meter_segment_colour(int segment, int segments);

    class ControlPanel
    {
    public:
        ControlPanel(float width, float height);

        float width() const { return width_; }
        float height() const { return height_; }

        // Each add_ function returns the control's id. Labels should be unique if you want to use find().
        int add_dial(float x, float y, float diameter, float min, float max, const std::string &label = "", float sweep_degrees = 270.0f);
        int add_knob(float x, float y, float diameter, float min, float max, float value, const std::string &label = "");
        int add_toggle(float x, float y, float size, bool on = false, const std::string &label = "");
        int add_button(float x, float y, float diameter, Colour colour, const std::string &label = "", bool latching = false);
        int add_lamp(float x, float y, float diameter, Colour colour, bool on = false, const std::string &label = "", bool blink = false);
        int add_slider(float x, float y, float length, float min, float max, float value, bool vertical = true, const std::string &label = "");
        int add_meter(float x, float y, float width, float height, float min, float max, const std::string &label = "", bool vertical = true,
                      int segments = 12);

        int count() const { return static_cast<int>(controls_.size()); }
        const Control &control(int id) const { return controls_.at(static_cast<std::size_t>(id)); }
        /** The id of the control with this label, or -1. */
        int find(const std::string &label) const;

        float value(int id) const { return control(id).value; }
        /** True when a toggle, button or lamp is on. */
        bool on(int id) const { return control(id).value > 0.5f; }
        /** Set a value from your program, clamped to the control's range. Does not call on_change. */
        void set_value(int id, float value);
        void set_on(int id, bool on) { set_value(id, on ? 1.0f : 0.0f); }
        /** How fast needles and meters ease toward their value, per second (default 10; 0 snaps). */
        void set_smoothing(float rate) { smoothing_ = rate; }

        /** Called when the user changes a control with the pointer. */
        std::function<void(int id, float value)> on_change;

        /** Advance animation by `dt` seconds and process the pointer. Call once per frame. */
        void update(float dt, const Pointer &pointer);
        /** The id of the interactive control under panel point (x, y), or -1. */
        int hit_test(float x, float y) const;
        /** The control under the pointer after the last update(), or -1. */
        int hovered() const { return hovered_; }
        /** The control being dragged or held, or -1. */
        int active() const { return active_; }

        /** The lamp brightness 0-1 after easing and blinking. 1 for any other kind that is on. */
        float glow(int id) const;

    private:
        void change(int id, float value);

        float width_, height_;
        std::vector<Control> controls_;
        float smoothing_ = 10.0f;
        float time_ = 0.0f;
        int hovered_ = -1, active_ = -1;
        bool was_down_ = false;
        Pointer last_;
        float grab_value_ = 0.0f;
        Pointer grab_;
    };

    /**
     * Draw a panel flat into `target` with its top-left corner at (x, y) and `scale` pixels per panel unit. Lamps and lit
     * meter segments get a soft halo; run a Bloom pass over the result to make them glow further. Labels are drawn with
     * `font` (the default monospace font when null) and go to the screen, so draw onto the screen or the current target.
     */
    void draw_control_panel(Bitmap *target, const ControlPanel &panel, float x, float y, float scale = 1.0f, Font *font = nullptr);
    /** Convert a screen position to panel units for a panel drawn with draw_control_panel(). */
    Pointer control_pointer_2d(float panel_x, float panel_y, float scale, float screen_x, float screen_y, bool down);

    /**
     * A ControlPanel drawn as 3D geometry. It builds meshes for the panel's current layout, so create it after adding the
     * controls. One panel unit is `unit` metres. The panel lies in the xy plane of the pose you pass, centred on the
     * origin with +Z toward the viewer.
     *
     * Lamps, lit meter segments and latched buttons are emissive: values above 1 give a bright result that blooms when the
     * scene is drawn through Hdr. lights() returns small local lights for the lit lamps so they colour nearby surfaces.
     */
    class ControlPanelView3D
    {
    public:
        ControlPanelView3D(const ControlPanel &panel, float unit = 0.002f);
        ~ControlPanelView3D();
        ControlPanelView3D(const ControlPanelView3D &) = delete;
        ControlPanelView3D &operator=(const ControlPanelView3D &) = delete;

        bool valid() const { return valid_; }
        float unit() const { return unit_; }

        /** Draw the panel at `pose`. Use between begin_surface_3d() and end_surface_3d(), or in a shadow pass. */
        void draw(const ControlPanel &panel, const glm::mat4 &pose) const;
        /** One point light per lit lamp, `range` metres wide. Add them to your own with set_local_lights_3d(). */
        std::vector<LocalLight3D> lights(const ControlPanel &panel, const glm::mat4 &pose, float intensity = 0.08f, float range = 0.9f) const;
        /**
         * Turn a mouse position over a view `view_width` x `view_height` pixels drawn with `camera` into a Pointer on the
         * panel at `pose`. Returns false when the mouse ray misses the panel's plane. `pointer->down` is left unchanged.
         */
        bool pick(const Camera &camera, int view_width, int view_height, float mouse_x, float mouse_y, const glm::mat4 &pose,
                  Pointer *pointer) const;

    private:
        struct Entry
        {
            Mesh *moving = nullptr;
            Mesh *segment = nullptr;
            float radius = 0.0f;
        };

        Mesh *make(const MeshData &data);

        float unit_;
        float width_, height_;
        bool valid_ = true;
        Mesh *static_mesh_ = nullptr;
        std::vector<Entry> entries_;
        std::vector<Mesh *> owned_;
    };

    /** A panel layout together with the 3D body it sits on. */
    struct ControlAssembly
    {
        ControlAssembly(float panel_width, float panel_height) : panel(panel_width, panel_height) {}
        /** The cockpit or cabinet shell, in the assembly's own frame (floor at y = 0, front toward +Z). Bake or create_mesh() it. */
        MeshData body;
        ControlPanel panel;
        /** Where the panel sits in the assembly frame. */
        glm::mat4 panel_pose{1.0f};
        /** Metres per panel unit for ControlPanelView3D. */
        float unit = 0.002f;
    };

    /**
     * A light-aircraft instrument panel: airspeed, altitude and rpm dials, fuel and temperature meters, a throttle slider,
     * toggles (MASTER, PUMP, NAV, LIGHTS), knobs (TRIM, VOLUME), lamps (WARN, GEAR, READY) and buttons (START, LEVER).
     * The pilot sits at +Z. 700 x 280 panel units.
     */
    ControlAssembly make_cockpit();
    /**
     * A factory process cabinet: lamps (RUN, WARN, FAULT), pressure and temperature dials, tank level meters, valve
     * sliders, a SPEED knob, pump toggles and START, STOP and E-STOP buttons. Faces +Z. 320 x 450 panel units.
     */
    ControlAssembly make_control_cabinet();
    /** The cockpit's panel layout alone, for flat 2D use. */
    ControlPanel make_cockpit_panel();
    /** The cabinet's panel layout alone, for flat 2D use. */
    ControlPanel make_cabinet_panel();
} // namespace sl

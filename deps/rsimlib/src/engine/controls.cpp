#include "controls.h"

#include "font.h"

#include <algorithm>
#include <cmath>

namespace sl
{
    namespace
    {
        constexpr float pi = 3.14159265358979f;

        Colour shaded(Colour colour, float factor, std::uint8_t alpha = 255)
        {
            const auto scale = [&](std::uint8_t channel)
            { return static_cast<std::uint8_t>(std::clamp(static_cast<float>(channel) * factor, 0.0f, 255.0f)); };
            return {scale(colour.red), scale(colour.green), scale(colour.blue), alpha};
        }

        bool interactive(ControlKind kind)
        {
            return kind == ControlKind::knob || kind == ControlKind::toggle || kind == ControlKind::button || kind == ControlKind::slider;
        }

        float ease_rate(const Control &control, float smoothing)
        {
            switch (control.kind)
            {
            case ControlKind::dial:
            case ControlKind::meter:
                return smoothing;
            case ControlKind::lamp:
            case ControlKind::toggle:
            case ControlKind::button:
                return 30.0f;
            default:
                return 0.0f;
            }
        }
    } // namespace

    float Control::fraction() const
    {
        return max > min ? std::clamp((value - min) / (max - min), 0.0f, 1.0f) : 0.0f;
    }

    float Control::display_fraction() const
    {
        return max > min ? std::clamp((display - min) / (max - min), 0.0f, 1.0f) : 0.0f;
    }

    float Control::angle() const
    {
        return (display_fraction() - 0.5f) * sweep_degrees * pi / 180.0f;
    }

    float Control::label_y() const
    {
        switch (kind)
        {
        case ControlKind::dial:
            return y + 0.57f * width + 6.0f;
        case ControlKind::knob:
            return y + 0.75f * width + 6.0f;
        case ControlKind::toggle:
            return y + 0.8f * width + 6.0f;
        case ControlKind::button:
            return y + 0.62f * width + 6.0f;
        case ControlKind::lamp:
            return y + 0.62f * width + 6.0f;
        case ControlKind::slider:
        case ControlKind::meter:
            return y + 0.5f * height + (vertical ? 0.4f * width : 0.3f * height) + 6.0f;
        }
        return y + 6.0f;
    }

    Colour meter_segment_colour(int segment, int segments)
    {
        const float t = (static_cast<float>(segment) + 0.5f) / static_cast<float>(std::max(segments, 1));
        if (t < 0.6f)
            return {60, 220, 90, 255};
        if (t < 0.85f)
            return {250, 190, 40, 255};
        return {240, 60, 50, 255};
    }

    ControlPanel::ControlPanel(float width, float height) : width_(width), height_(height) {}

    int ControlPanel::add_dial(float x, float y, float diameter, float min, float max, const std::string &label, float sweep_degrees)
    {
        Control c;
        c.kind = ControlKind::dial;
        c.x = x;
        c.y = y;
        c.width = c.height = diameter;
        c.min = min;
        c.max = max;
        c.value = c.display = min;
        c.label = label;
        c.sweep_degrees = sweep_degrees;
        controls_.push_back(c);
        return count() - 1;
    }

    int ControlPanel::add_knob(float x, float y, float diameter, float min, float max, float value, const std::string &label)
    {
        Control c;
        c.kind = ControlKind::knob;
        c.x = x;
        c.y = y;
        c.width = c.height = diameter;
        c.min = min;
        c.max = max;
        c.value = c.display = std::clamp(value, min, max);
        c.label = label;
        controls_.push_back(c);
        return count() - 1;
    }

    int ControlPanel::add_toggle(float x, float y, float size, bool on, const std::string &label)
    {
        Control c;
        c.kind = ControlKind::toggle;
        c.x = x;
        c.y = y;
        c.width = c.height = size;
        c.value = c.display = on ? 1.0f : 0.0f;
        c.label = label;
        controls_.push_back(c);
        return count() - 1;
    }

    int ControlPanel::add_button(float x, float y, float diameter, Colour colour, const std::string &label, bool latching)
    {
        Control c;
        c.kind = ControlKind::button;
        c.x = x;
        c.y = y;
        c.width = c.height = diameter;
        c.colour = colour;
        c.latching = latching;
        c.label = label;
        controls_.push_back(c);
        return count() - 1;
    }

    int ControlPanel::add_lamp(float x, float y, float diameter, Colour colour, bool on, const std::string &label, bool blink)
    {
        Control c;
        c.kind = ControlKind::lamp;
        c.x = x;
        c.y = y;
        c.width = c.height = diameter;
        c.colour = colour;
        c.value = c.display = on ? 1.0f : 0.0f;
        c.label = label;
        c.blink = blink;
        controls_.push_back(c);
        return count() - 1;
    }

    int ControlPanel::add_slider(float x, float y, float length, float min, float max, float value, bool vertical, const std::string &label)
    {
        Control c;
        c.kind = ControlKind::slider;
        c.x = x;
        c.y = y;
        const float thickness = 0.2f * length;
        c.width = vertical ? thickness : length;
        c.height = vertical ? length : thickness;
        c.vertical = vertical;
        c.min = min;
        c.max = max;
        c.value = c.display = std::clamp(value, min, max);
        c.label = label;
        controls_.push_back(c);
        return count() - 1;
    }

    int ControlPanel::add_meter(float x, float y, float width, float height, float min, float max, const std::string &label, bool vertical,
                                int segments)
    {
        Control c;
        c.kind = ControlKind::meter;
        c.x = x;
        c.y = y;
        c.width = width;
        c.height = height;
        c.vertical = vertical;
        c.min = min;
        c.max = max;
        c.value = c.display = min;
        c.label = label;
        c.segments = std::max(segments, 1);
        controls_.push_back(c);
        return count() - 1;
    }

    int ControlPanel::find(const std::string &label) const
    {
        for (int i = 0; i < count(); ++i)
            if (controls_[static_cast<std::size_t>(i)].label == label)
                return i;
        return -1;
    }

    void ControlPanel::set_value(int id, float value)
    {
        Control &c = controls_.at(static_cast<std::size_t>(id));
        c.value = std::clamp(value, c.min, c.max);
    }

    void ControlPanel::change(int id, float value)
    {
        Control &c = controls_[static_cast<std::size_t>(id)];
        value = std::clamp(value, c.min, c.max);
        if (value == c.value)
            return;
        c.value = value;
        if (on_change)
            on_change(id, value);
    }

    int ControlPanel::hit_test(float x, float y) const
    {
        for (int i = count() - 1; i >= 0; --i)
        {
            const Control &c = controls_[static_cast<std::size_t>(i)];
            if (!interactive(c.kind))
                continue;
            const float dx = x - c.x, dy = y - c.y;
            bool hit = false;
            switch (c.kind)
            {
            case ControlKind::knob:
                hit = dx * dx + dy * dy <= (0.7f * c.width) * (0.7f * c.width);
                break;
            case ControlKind::button:
                hit = dx * dx + dy * dy <= (0.62f * c.width) * (0.62f * c.width);
                break;
            case ControlKind::toggle:
                hit = std::abs(dx) <= 0.65f * c.width && std::abs(dy) <= 0.7f * c.width;
                break;
            case ControlKind::slider:
                hit = c.vertical ? std::abs(dx) <= 0.8f * c.width && std::abs(dy) <= 0.5f * c.height + 0.35f * c.width
                                 : std::abs(dy) <= 0.8f * c.height && std::abs(dx) <= 0.5f * c.width + 0.35f * c.height;
                break;
            default:
                break;
            }
            if (hit)
                return i;
        }
        return -1;
    }

    void ControlPanel::update(float dt, const Pointer &pointer)
    {
        time_ += dt;
        const bool pressed = pointer.down && !was_down_;
        const bool released = !pointer.down && was_down_;

        if (pressed)
        {
            const int id = hit_test(pointer.x, pointer.y);
            if (id >= 0)
            {
                active_ = id;
                grab_ = pointer;
                grab_value_ = control(id).value;
                const Control &c = control(id);
                if (c.kind == ControlKind::toggle || (c.kind == ControlKind::button && c.latching))
                    change(id, c.value > 0.5f ? 0.0f : 1.0f);
                else if (c.kind == ControlKind::button)
                    change(id, 1.0f);
            }
        }

        if (pointer.down && active_ >= 0)
        {
            const Control &c = control(active_);
            if (c.kind == ControlKind::knob)
            {
                const float per_unit = (c.max - c.min) / (4.0f * c.width);
                change(active_, grab_value_ + ((grab_.y - pointer.y) + (pointer.x - grab_.x)) * per_unit);
            }
            else if (c.kind == ControlKind::slider)
            {
                const float length = c.length();
                const float along = c.vertical ? 1.0f - (pointer.y - (c.y - 0.5f * length)) / length : (pointer.x - (c.x - 0.5f * length)) / length;
                change(active_, c.min + std::clamp(along, 0.0f, 1.0f) * (c.max - c.min));
            }
        }

        if (released && active_ >= 0)
        {
            const Control &c = control(active_);
            if (c.kind == ControlKind::button && !c.latching)
                change(active_, 0.0f);
            active_ = -1;
        }

        hovered_ = active_ >= 0 ? active_ : hit_test(pointer.x, pointer.y);
        was_down_ = pointer.down;
        last_ = pointer;

        for (Control &c : controls_)
        {
            const float rate = ease_rate(c, smoothing_);
            const float range = std::max(c.max - c.min, 1e-6f);
            if (rate <= 0.0f || std::abs(c.value - c.display) < 1e-4f * range)
                c.display = c.value;
            else
                c.display += (c.value - c.display) * (1.0f - std::exp(-rate * dt));
        }
    }

    float ControlPanel::glow(int id) const
    {
        const Control &c = control(id);
        const float level = std::clamp(c.display, 0.0f, 1.0f);
        if (c.kind == ControlKind::lamp && c.blink)
            return level * (std::fmod(time_ * 2.5f, 1.0f) < 0.5f ? 1.0f : 0.12f);
        return level;
    }

    // ---- 2D rendering -----------------------------------------------------------------------------------------------

    Pointer control_pointer_2d(float panel_x, float panel_y, float scale, float screen_x, float screen_y, bool down)
    {
        return {(screen_x - panel_x) / scale, (screen_y - panel_y) / scale, down};
    }

    namespace
    {
        const Colour steel_2d{108, 112, 120, 255};
        const Colour dark_2d{30, 32, 38, 255};

        void halo(Bitmap *target, float x, float y, float radius, Colour colour, float strength)
        {
            for (int i = 4; i >= 1; --i)
            {
                const float k = static_cast<float>(i);
                circlefill(target, x, y, radius * (1.0f + 0.45f * k), shaded(colour, 1.0f, static_cast<std::uint8_t>(std::clamp(20.0f * strength, 0.0f, 255.0f))));
            }
        }

        void scale_ticks_2d(Bitmap *target, float cx, float cy, float outer, float length, float thickness, int count, float sweep_degrees,
                            Colour colour, Colour last)
        {
            for (int i = 0; i < count; ++i)
            {
                const float t = static_cast<float>(i) / static_cast<float>(count - 1);
                const float angle = (t - 0.5f) * sweep_degrees * pi / 180.0f;
                const float dx = std::sin(angle), dy = -std::cos(angle);
                line(target, cx + dx * outer, cy + dy * outer, cx + dx * (outer - length), cy + dy * (outer - length), i == count - 1 ? last : colour,
                     thickness);
                if (i + 1 < count)
                {
                    const float half = angle + 0.5f * sweep_degrees * pi / 180.0f / static_cast<float>(count - 1);
                    const float hx = std::sin(half), hy = -std::cos(half);
                    line(target, cx + hx * outer, cy + hy * outer, cx + hx * (outer - 0.5f * length), cy + hy * (outer - 0.5f * length), colour,
                         std::max(1.0f, 0.6f * thickness));
                }
            }
        }

        void draw_one_2d(Bitmap *target, const ControlPanel &panel, int id, float ox, float oy, float s)
        {
            const Control &c = panel.control(id);
            const float cx = ox + c.x * s, cy = oy + c.y * s;
            const bool lit_outline = panel.hovered() == id || panel.active() == id;
            const Colour ring = lit_outline ? Colour{255, 255, 255, 255} : Colour{0, 0, 0, 0};
            switch (c.kind)
            {
            case ControlKind::dial:
            {
                const float r = 0.5f * c.width * s;
                circlefill(target, cx, cy, 1.14f * r, steel_2d);
                circlefill(target, cx, cy, 1.0f * r, {18, 20, 24, 255});
                scale_ticks_2d(target, cx, cy, 0.9f * r, 0.17f * r, std::max(1.0f, 0.03f * r), 11, c.sweep_degrees, {232, 232, 224, 255},
                               {220, 52, 40, 255});
                const float a = c.angle();
                const float dx = std::sin(a), dy = -std::cos(a);
                line(target, cx - dx * 0.2f * r, cy - dy * 0.2f * r, cx + dx * 0.86f * r, cy + dy * 0.86f * r, {255, 120, 40, 255},
                     std::max(2.0f, 0.07f * r));
                circlefill(target, cx, cy, 0.1f * r, dark_2d);
                break;
            }
            case ControlKind::knob:
            {
                const float r = 0.5f * c.width * s;
                circlefill(target, cx, cy, 1.12f * r, dark_2d);
                scale_ticks_2d(target, cx, cy, 1.5f * r, 0.2f * r, std::max(1.0f, 0.035f * r), 11, c.sweep_degrees, {226, 226, 218, 255},
                               {226, 226, 218, 255});
                circlefill(target, cx, cy, r, {50, 54, 62, 255});
                circlefill(target, cx, cy, 0.8f * r, {68, 72, 82, 255});
                const float a = c.angle();
                const float dx = std::sin(a), dy = -std::cos(a);
                line(target, cx + dx * 0.25f * r, cy + dy * 0.25f * r, cx + dx * 0.9f * r, cy + dy * 0.9f * r, {240, 240, 232, 255},
                     std::max(2.0f, 0.1f * r));
                if (lit_outline)
                    circle(target, cx, cy, 1.1f * r, ring, 1.5f);
                break;
            }
            case ControlKind::toggle:
            {
                const float size = c.width * s;
                circlefill(target, cx, cy, 0.62f * size, steel_2d);
                circlefill(target, cx, cy, 0.34f * size, {150, 154, 162, 255});
                const float tip = cy - (2.0f * c.display - 1.0f) * 0.8f * size;
                line(target, cx, cy, cx, tip, {200, 200, 206, 255}, std::max(2.0f, 0.22f * size));
                circlefill(target, cx, tip, 0.2f * size, {222, 222, 228, 255});
                if (lit_outline)
                    circle(target, cx, cy, 0.8f * size, ring, 1.5f);
                break;
            }
            case ControlKind::button:
            {
                const float r = 0.5f * c.width * s;
                circlefill(target, cx, cy, 1.2f * r, dark_2d);
                circlefill(target, cx, cy, 0.9f * r, {14, 14, 16, 255});
                const float pressed = std::clamp(c.display, 0.0f, 1.0f);
                if (c.latching && pressed > 0.1f)
                    halo(target, cx, cy, 0.84f * r, c.colour, 0.5f * pressed);
                circlefill(target, cx, cy, (0.84f - 0.06f * pressed) * r, shaded(c.colour, 0.7f + 0.3f * (1.0f - pressed)));
                circlefill(target, cx - 0.2f * r, cy - 0.22f * r, 0.22f * r, shaded(c.colour, 1.25f, 140));
                if (lit_outline)
                    circle(target, cx, cy, 1.25f * r, ring, 1.5f);
                break;
            }
            case ControlKind::lamp:
            {
                const float r = 0.5f * c.width * s;
                const float g = panel.glow(id);
                if (g > 0.02f)
                    halo(target, cx, cy, r, c.colour, g);
                circlefill(target, cx, cy, 1.22f * r, steel_2d);
                circlefill(target, cx, cy, 0.98f * r, dark_2d);
                circlefill(target, cx, cy, 0.86f * r, shaded(c.colour, 0.22f + 0.78f * g));
                circlefill(target, cx - 0.25f * r, cy - 0.28f * r, 0.26f * r, {255, 255, 255, static_cast<std::uint8_t>(60 + 120 * g)});
                break;
            }
            case ControlKind::slider:
            {
                const float length = c.length() * s;
                const float thickness = (c.vertical ? c.width : c.height) * s;
                const float hw = c.vertical ? 0.5f * thickness : 0.5f * length;
                const float hh = c.vertical ? 0.5f * length : 0.5f * thickness;
                rectfill(target, cx - hw, cy - hh, cx + hw, cy + hh, dark_2d);
                const float slot = 0.11f * thickness;
                if (c.vertical)
                    rectfill(target, cx - slot, cy - 0.5f * length, cx + slot, cy + 0.5f * length, {8, 8, 10, 255});
                else
                    rectfill(target, cx - 0.5f * length, cy - slot, cx + 0.5f * length, cy + slot, {8, 8, 10, 255});
                for (int i = 0; i <= 10; ++i)
                {
                    const float t = static_cast<float>(i) / 10.0f;
                    const float tick = (i % 5 == 0 ? 0.3f : 0.18f) * thickness;
                    if (c.vertical)
                    {
                        const float ty = cy + 0.5f * length - t * length;
                        line(target, cx - 0.38f * thickness - 0.5f * tick, ty, cx - 0.38f * thickness + 0.5f * tick, ty, {220, 220, 212, 255}, 1.0f);
                        line(target, cx + 0.38f * thickness - 0.5f * tick, ty, cx + 0.38f * thickness + 0.5f * tick, ty, {220, 220, 212, 255}, 1.0f);
                    }
                    else
                    {
                        const float tx = cx - 0.5f * length + t * length;
                        line(target, tx, cy - 0.38f * thickness - 0.5f * tick, tx, cy - 0.38f * thickness + 0.5f * tick, {220, 220, 212, 255}, 1.0f);
                        line(target, tx, cy + 0.38f * thickness - 0.5f * tick, tx, cy + 0.38f * thickness + 0.5f * tick, {220, 220, 212, 255}, 1.0f);
                    }
                }
                const float f = c.fraction();
                const float px = c.vertical ? cx : cx - 0.5f * length + f * length;
                const float py = c.vertical ? cy + 0.5f * length - f * length : cy;
                const float cap_w = 0.42f * thickness * 2.0f, cap_h = 0.3f * thickness * 2.0f;
                const float w2 = c.vertical ? 0.5f * cap_w : 0.5f * cap_h, h2 = c.vertical ? 0.5f * cap_h : 0.5f * cap_w;
                rectfill(target, px - w2, py - h2, px + w2, py + h2, {80, 82, 90, 255});
                rect(target, px - w2, py - h2, px + w2, py + h2, {40, 42, 48, 255}, 1.0f);
                if (c.vertical)
                    line(target, px - 0.7f * w2, py, px + 0.7f * w2, py, {240, 240, 232, 255}, 2.0f);
                else
                    line(target, px, py - 0.7f * h2, px, py + 0.7f * h2, {240, 240, 232, 255}, 2.0f);
                if (lit_outline)
                    rect(target, px - w2 - 2, py - h2 - 2, px + w2 + 2, py + h2 + 2, ring, 1.5f);
                break;
            }
            case ControlKind::meter:
            {
                const float w = c.width * s, h = c.height * s;
                const float pad = 0.12f * std::min(w, h);
                rectfill(target, cx - 0.5f * w - pad, cy - 0.5f * h - pad, cx + 0.5f * w + pad, cy + 0.5f * h + pad, dark_2d);
                rectfill(target, cx - 0.5f * w, cy - 0.5f * h, cx + 0.5f * w, cy + 0.5f * h, {8, 8, 10, 255});
                const int n = c.segments;
                const float lit_count = c.display_fraction() * static_cast<float>(n);
                for (int i = 0; i < n; ++i)
                {
                    const float t0 = static_cast<float>(i) / static_cast<float>(n), t1 = static_cast<float>(i + 1) / static_cast<float>(n);
                    const float gap = 0.14f;
                    float x0, y0, x1, y1;
                    if (c.vertical)
                    {
                        x0 = cx - 0.38f * w;
                        x1 = cx + 0.38f * w;
                        y1 = cy + 0.5f * h - (t0 + gap * (t1 - t0)) * h;
                        y0 = cy + 0.5f * h - (t1 - gap * (t1 - t0)) * h;
                    }
                    else
                    {
                        y0 = cy - 0.38f * h;
                        y1 = cy + 0.38f * h;
                        x0 = cx - 0.5f * w + (t0 + gap * (t1 - t0)) * w;
                        x1 = cx - 0.5f * w + (t1 - gap * (t1 - t0)) * w;
                    }
                    const Colour zone = meter_segment_colour(i, n);
                    const bool lit = lit_count >= static_cast<float>(i) + 0.5f;
                    if (lit)
                    {
                        const float grow = 0.25f * std::min(x1 - x0, y1 - y0);
                        rectfill(target, x0 - grow, y0 - grow, x1 + grow, y1 + grow, shaded(zone, 1.0f, 38));
                    }
                    rectfill(target, x0, y0, x1, y1, lit ? zone : shaded(zone, 0.14f));
                }
                break;
            }
            }
        }
    } // namespace

    void draw_control_panel(Bitmap *target, const ControlPanel &panel, float x, float y, float scale, Font *font)
    {
        const float w = panel.width() * scale, h = panel.height() * scale;
        rectfill(target, x, y, x + w, y + h, {58, 62, 70, 255});
        const float frame = 0.018f * std::min(w, h);
        rect(target, x + 0.5f * frame, y + 0.5f * frame, x + w - 0.5f * frame, y + h - 0.5f * frame, {36, 38, 44, 255}, frame);
        for (const float sx : {1.0f, 0.0f})
            for (const float sy : {1.0f, 0.0f})
            {
                const float px = sx > 0.5f ? x + 2.2f * frame : x + w - 2.2f * frame;
                const float py = sy > 0.5f ? y + 2.2f * frame : y + h - 2.2f * frame;
                circlefill(target, px, py, 0.8f * frame, steel_2d);
                line(target, px - 0.5f * frame, py + 0.2f * frame, px + 0.5f * frame, py - 0.2f * frame, dark_2d, 1.0f);
            }
        for (int i = 0; i < panel.count(); ++i)
            draw_one_2d(target, panel, i, x, y, scale);

        if (!font)
            font = get_default_monospace_font();
        for (int i = 0; i < panel.count(); ++i)
        {
            const Control &c = panel.control(i);
            if (c.label.empty())
                continue;
            const int text_w = text_length(font, c.label);
            textout(font, static_cast<int>(x + c.x * scale) - text_w / 2, static_cast<int>(y + c.label_y() * scale), {206, 210, 218, 255}, c.label);
        }
    }

    // ---- 3D rendering -----------------------------------------------------------------------------------------------

    Mesh *ControlPanelView3D::make(const MeshData &data)
    {
        Mesh *mesh = create_mesh(data.vertices, data.indices);
        if (!mesh)
            valid_ = false;
        else
            owned_.push_back(mesh);
        return mesh;
    }

    ControlPanelView3D::ControlPanelView3D(const ControlPanel &panel, float unit) : unit_(unit), width_(panel.width()), height_(panel.height())
    {
        MeshData stat = make_panel_data(width_ * unit_, height_ * unit_, 0.03f);
        const auto add_static = [&](MeshData part, const glm::mat4 &matrix)
        {
            transform_mesh_data(part, matrix);
            append_mesh_data(stat, part);
        };
        for (int i = 0; i < panel.count(); ++i)
        {
            const Control &c = panel.control(i);
            const glm::mat4 at = glm::translate(glm::mat4(1.0f), glm::vec3((c.x - 0.5f * width_) * unit_, (0.5f * height_ - c.y) * unit_, 0.0f));
            Entry e;
            const float r = 0.5f * c.width * unit_;
            switch (c.kind)
            {
            case ControlKind::dial:
                add_static(make_dial_data(r, 11, c.sweep_degrees), at);
                e.moving = make(make_needle_data(r));
                e.radius = r;
                break;
            case ControlKind::knob:
                add_static(make_knob_base_data(r, 11, c.sweep_degrees), at);
                e.moving = make(make_knob_data(r, 0.9f * r, {50, 54, 62, 255}));
                e.radius = r;
                break;
            case ControlKind::toggle:
                add_static(make_toggle_base_data(c.width * unit_), at);
                e.moving = make(make_toggle_lever_data(c.width * unit_));
                e.radius = c.width * unit_;
                break;
            case ControlKind::button:
                add_static(make_button_base_data(r), at);
                e.moving = make(make_button_cap_data(r, c.colour));
                e.radius = r;
                break;
            case ControlKind::lamp:
                add_static(make_lamp_bezel_data(r), at);
                e.moving = make(make_lamp_lens_data(r));
                e.radius = r;
                break;
            case ControlKind::slider:
            {
                const float length = c.length() * unit_;
                const float thickness = (c.vertical ? c.width : c.height) * unit_;
                const glm::mat4 turn = c.vertical ? glm::mat4(1.0f) : glm::rotate(glm::mat4(1.0f), -0.5f * pi, glm::vec3(0.0f, 0.0f, 1.0f));
                add_static(make_slider_track_data(length, thickness), at * turn);
                e.moving = make(make_slider_cap_data(thickness));
                e.radius = length;
                break;
            }
            case ControlKind::meter:
            {
                add_static(make_meter_housing_data(c.width * unit_, c.height * unit_), at);
                const float n = static_cast<float>(c.segments);
                e.segment = c.vertical ? make(make_meter_segment_data(0.76f * c.width * unit_, 0.72f * c.height * unit_ / n))
                                       : make(make_meter_segment_data(0.72f * c.width * unit_ / n, 0.76f * c.height * unit_));
                break;
            }
            }
            entries_.push_back(e);
        }
        static_mesh_ = make(stat);
    }

    ControlPanelView3D::~ControlPanelView3D()
    {
        for (Mesh *mesh : owned_)
            destroy_mesh(mesh);
    }

    namespace
    {
        glm::vec3 linear(Colour colour, float gain)
        {
            return glm::vec3(colour.red, colour.green, colour.blue) * (gain / 255.0f);
        }

        Material tinted(Colour colour, const glm::vec3 &emissive = glm::vec3(0.0f))
        {
            Material material;
            material.tint = colour;
            material.emissive[0] = emissive.x;
            material.emissive[1] = emissive.y;
            material.emissive[2] = emissive.z;
            return material;
        }
    } // namespace

    void ControlPanelView3D::draw(const ControlPanel &panel, const glm::mat4 &pose) const
    {
        if (!valid_)
            return;
        draw_mesh(static_mesh_, pose);
        const glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
        for (int i = 0; i < panel.count() && i < static_cast<int>(entries_.size()); ++i)
        {
            const Control &c = panel.control(i);
            const Entry &e = entries_[static_cast<std::size_t>(i)];
            const glm::mat4 base =
                pose * glm::translate(glm::mat4(1.0f), glm::vec3((c.x - 0.5f * width_) * unit_, (0.5f * height_ - c.y) * unit_, 0.0f));
            switch (c.kind)
            {
            case ControlKind::dial:
                draw_mesh(e.moving, base * glm::rotate(glm::mat4(1.0f), -c.angle(), z_axis));
                break;
            case ControlKind::knob:
                draw_mesh(e.moving, base * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.006f)) * glm::rotate(glm::mat4(1.0f), -c.angle(), z_axis));
                break;
            case ControlKind::toggle:
            {
                const float pivot = 0.3f * e.radius;
                const float tilt = (0.5f - std::clamp(c.display, 0.0f, 1.0f)) * 70.0f * pi / 180.0f;
                const glm::mat4 flip = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, pivot)) *
                                       glm::rotate(glm::mat4(1.0f), tilt, glm::vec3(1.0f, 0.0f, 0.0f)) *
                                       glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -pivot));
                draw_mesh(e.moving, base * flip);
                break;
            }
            case ControlKind::button:
            {
                const float pressed = std::clamp(c.display, 0.0f, 1.0f);
                const bool glowing = c.latching && pressed > 0.05f;
                draw_mesh(e.moving, base * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.2f * e.radius * pressed)),
                          tinted(glowing ? c.colour : shaded(c.colour, 0.8f), glowing ? linear(c.colour, 1.6f * pressed) : glm::vec3(0.0f)));
                break;
            }
            case ControlKind::lamp:
            {
                const float g = panel.glow(i);
                draw_mesh(e.moving, base, tinted(shaded(c.colour, 0.2f + 0.8f * g), linear(c.colour, 3.5f * g)));
                break;
            }
            case ControlKind::slider:
            {
                const float f = c.fraction();
                const float along = (-0.5f + f) * e.radius;
                const glm::mat4 slide = c.vertical ? glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, along, 0.004f))
                                                   : glm::translate(glm::mat4(1.0f), glm::vec3(along, 0.0f, 0.004f)) *
                                                         glm::rotate(glm::mat4(1.0f), -0.5f * pi, z_axis);
                draw_mesh(e.moving, base * slide);
                break;
            }
            case ControlKind::meter:
            {
                const int n = c.segments;
                const float lit_count = c.display_fraction() * static_cast<float>(n);
                for (int s = 0; s < n; ++s)
                {
                    const float t = (static_cast<float>(s) + 0.5f) / static_cast<float>(n) - 0.5f;
                    const glm::vec3 offset = c.vertical ? glm::vec3(0.0f, t * c.height * unit_, 0.0f) : glm::vec3(t * c.width * unit_, 0.0f, 0.0f);
                    const Colour zone = meter_segment_colour(s, n);
                    const bool lit = lit_count >= static_cast<float>(s) + 0.5f;
                    draw_mesh(e.segment, base * glm::translate(glm::mat4(1.0f), offset), tinted(lit ? zone : shaded(zone, 0.12f), lit ? linear(zone, 3.0f) : glm::vec3(0.0f)));
                }
                break;
            }
            }
        }
    }

    std::vector<LocalLight3D> ControlPanelView3D::lights(const ControlPanel &panel, const glm::mat4 &pose, float intensity, float range) const
    {
        std::vector<LocalLight3D> out;
        for (int i = 0; i < panel.count(); ++i)
        {
            const Control &c = panel.control(i);
            if (c.kind != ControlKind::lamp)
                continue;
            const float g = panel.glow(i);
            if (g < 0.05f)
                continue;
            LocalLight3D light;
            light.position = glm::vec3(pose * glm::vec4((c.x - 0.5f * width_) * unit_, (0.5f * height_ - c.y) * unit_, 0.05f, 1.0f));
            light.colour = linear(c.colour, 1.0f);
            light.intensity = intensity * g;
            light.range = range;
            out.push_back(light);
        }
        return out;
    }

    bool ControlPanelView3D::pick(const Camera &camera, int view_width, int view_height, float mouse_x, float mouse_y, const glm::mat4 &pose,
                                  Pointer *pointer) const
    {
        const Ray3D ray = screen_ray(camera, view_width, view_height, mouse_x, mouse_y);
        const glm::vec3 origin = glm::vec3(pose * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        const glm::vec3 normal = glm::normalize(glm::vec3(pose * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
        const float denominator = glm::dot(ray.direction, normal);
        if (std::abs(denominator) < 1e-6f)
            return false;
        const float t = glm::dot(origin - ray.origin, normal) / denominator;
        if (t <= 0.0f)
            return false;
        const glm::vec3 local = glm::vec3(glm::inverse(pose) * glm::vec4(ray.origin + ray.direction * t, 1.0f));
        if (pointer)
        {
            pointer->x = local.x / unit_ + 0.5f * width_;
            pointer->y = 0.5f * height_ - local.y / unit_;
        }
        return true;
    }

    // ---- Ready-made layouts -----------------------------------------------------------------------------------------

    ControlPanel make_cockpit_panel()
    {
        ControlPanel p(700.0f, 280.0f);
        p.add_dial(100.0f, 95.0f, 150.0f, 0.0f, 400.0f, "AIRSPEED");
        p.add_dial(275.0f, 95.0f, 150.0f, 0.0f, 10000.0f, "ALTITUDE");
        p.add_dial(450.0f, 95.0f, 150.0f, 0.0f, 3000.0f, "RPM");
        p.add_meter(565.0f, 95.0f, 28.0f, 130.0f, 0.0f, 1.0f, "FUEL");
        p.add_meter(625.0f, 95.0f, 28.0f, 130.0f, 0.0f, 1.0f, "TEMP");
        p.add_slider(672.0f, 95.0f, 140.0f, 0.0f, 1.0f, 0.0f, true, "THR");
        p.add_toggle(50.0f, 228.0f, 30.0f, false, "MASTER");
        p.add_toggle(115.0f, 228.0f, 30.0f, false, "PUMP");
        p.add_toggle(180.0f, 228.0f, 30.0f, false, "NAV");
        p.add_toggle(245.0f, 228.0f, 30.0f, false, "LIGHTS");
        p.add_knob(325.0f, 226.0f, 44.0f, -1.0f, 1.0f, 0.0f, "TRIM");
        p.add_knob(395.0f, 226.0f, 44.0f, 0.0f, 1.0f, 0.5f, "VOLUME");
        p.add_lamp(460.0f, 226.0f, 22.0f, {240, 50, 40, 255}, false, "WARN", true);
        p.add_lamp(510.0f, 226.0f, 22.0f, {60, 230, 90, 255}, false, "LOCKED");
        p.add_lamp(560.0f, 226.0f, 22.0f, {255, 190, 50, 255}, false, "READY");
        p.add_button(615.0f, 226.0f, 34.0f, {236, 170, 40, 255}, "START", false);
        p.add_button(668.0f, 226.0f, 34.0f, {70, 200, 100, 255}, "GEAR", true);
        return p;
    }

    ControlPanel make_cabinet_panel()
    {
        ControlPanel p(320.0f, 450.0f);
        p.add_lamp(60.0f, 42.0f, 36.0f, {60, 230, 90, 255}, false, "RUN");
        p.add_lamp(160.0f, 42.0f, 36.0f, {255, 190, 50, 255}, false, "WARN");
        p.add_lamp(260.0f, 42.0f, 36.0f, {240, 50, 40, 255}, false, "FAULT", true);
        p.add_dial(90.0f, 150.0f, 110.0f, 0.0f, 10.0f, "PRESSURE");
        p.add_dial(230.0f, 150.0f, 110.0f, 0.0f, 200.0f, "TEMP");
        p.add_meter(40.0f, 282.0f, 24.0f, 96.0f, 0.0f, 1.0f, "TANK A");
        p.add_meter(95.0f, 282.0f, 24.0f, 96.0f, 0.0f, 1.0f, "TANK B");
        p.add_slider(165.0f, 282.0f, 96.0f, 0.0f, 1.0f, 0.0f, true, "VALVE 1");
        p.add_slider(220.0f, 282.0f, 96.0f, 0.0f, 1.0f, 0.0f, true, "VALVE 2");
        p.add_knob(280.0f, 272.0f, 46.0f, 0.0f, 1.0f, 0.3f, "SPEED");
        p.add_toggle(45.0f, 405.0f, 28.0f, false, "PUMP 1");
        p.add_toggle(105.0f, 405.0f, 28.0f, false, "PUMP 2");
        p.add_button(165.0f, 405.0f, 34.0f, {70, 200, 100, 255}, "START", false);
        p.add_button(220.0f, 405.0f, 34.0f, {210, 50, 44, 255}, "STOP", false);
        p.add_button(285.0f, 400.0f, 52.0f, {230, 40, 34, 255}, "E-STOP", true);
        return p;
    }

    ControlAssembly make_cockpit()
    {
        ControlAssembly a(700.0f, 280.0f);
        a.panel = make_cockpit_panel();
        a.body = make_cockpit_data(700.0f * a.unit, 280.0f * a.unit, &a.panel_pose);
        return a;
    }

    ControlAssembly make_control_cabinet()
    {
        ControlAssembly a(320.0f, 450.0f);
        a.panel = make_cabinet_panel();
        a.body = make_control_cabinet_data(320.0f * a.unit, 450.0f * a.unit, &a.panel_pose);
        return a;
    }
} // namespace sl

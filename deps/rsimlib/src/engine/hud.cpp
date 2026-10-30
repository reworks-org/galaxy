#include "hud.h"

#include "draw.h"
#include "font.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>

namespace sl
{
    namespace
    {
        constexpr double pi = 3.14159265358979323846;
        constexpr double deg = 180.0 / pi;
        constexpr float stroke = 1.5f;

        const Colour palette[] = {{110, 255, 130}, {235, 245, 255}, {255, 200, 90}, {110, 220, 255}, {255, 110, 100}};
        constexpr int palette_size = static_cast<int>(sizeof(palette) / sizeof(palette[0]));

        /** A direction in camera space: x right, y up, z forward. */
        struct View
        {
            double x, y, z;
        };

        /** Metres (or m/s) as a short string such as 842.1, 12.34k or 1.500M. */
        std::string si(double value)
        {
            char text[32];
            const double a = std::fabs(value);
            if (a < 1.0e4)
                std::snprintf(text, sizeof(text), a < 100.0 ? "%.2f" : "%.1f", value);
            else if (a < 1.0e6)
                std::snprintf(text, sizeof(text), "%.2fk", value / 1.0e3);
            else if (a < 1.0e9)
                std::snprintf(text, sizeof(text), "%.3fM", value / 1.0e6);
            else if (a < 1.0e12)
                std::snprintf(text, sizeof(text), "%.3fG", value / 1.0e9);
            else
                return "----";
            return text;
        }

        std::string upper(std::string s)
        {
            for (char &c : s)
                c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            return s;
        }

        /** Draws one frame of HUD symbology; all positions are screen pixels with y down. */
        class Painter
        {
        public:
            Painter(const HudState &state, Colour colour) : s_(state), colour_(colour)
            {
                width_ = s_.camera.width;
                height_ = s_.camera.height;
                font_ = get_default_monospace_font();
                char_w_ = std::max(1, text_length(font_, "0"));
                char_h_ = std::max(1, text_height(font_));
                mark_ = std::max(14.0f, static_cast<float>(height_) / 36.0f);
            }

            void orbit()
            {
                const HudOrbit &o = s_.orbit;
                if (!o.valid)
                    return;
                const double speed = glm::length(o.velocity);
                const dvec3 prograde = speed > 1.0e-9 ? o.velocity / speed : s_.nose;
                direction_marker(prograde, "PG", true);

                readout_boxes(si(speed), "OS", si(glm::length(o.position)), "RAD");

                dvec3 normal = glm::cross(o.position, o.velocity);
                const double n = glm::length(normal);
                if (n < 1.0e-9)
                    return;
                normal /= n;
                ladder(normal, false);
                azimuth_ribbon(normal, prograde);
            }

            void surface()
            {
                const HudSurface &g = s_.surface;
                if (!g.valid)
                    return;
                if (glm::length(g.ground_velocity) > 1.0)
                    direction_marker(glm::normalize(g.ground_velocity), "", false);

                readout_boxes(si(g.speed), g.airspeed ? "TAS" : "GS", si(g.altitude), g.radar ? "R ALT" : "ALT");

                const dvec3 up = glm::normalize(g.up);
                dvec3 north = g.north - up * glm::dot(g.north, up);
                if (glm::length(north) < 1.0e-9)
                    north = glm::cross(up, dvec3(1.0, 0.0, 0.0));
                north = glm::normalize(north);
                const dvec3 east = glm::cross(north, up);
                dvec3 flat = s_.nose - up * glm::dot(s_.nose, up);
                // Looking straight up or down leaves the heading undefined; fall back to the roof direction.
                if (glm::length(flat) < 1.0e-6)
                    flat = s_.roof - up * glm::dot(s_.roof, up);
                const double heading = std::fmod(std::atan2(glm::dot(flat, east), glm::dot(flat, north)) * deg + 360.0, 360.0);
                heading_ribbon(heading, g.has_target, g.target_bearing);
                ladder(up, true);
            }

            void docking()
            {
                const HudDocking &d = s_.docking;
                if (!d.valid)
                    return;

                // Relative velocity marker: a positive marker where the target is approached from, a bare cross behind.
                const double speed = glm::length(d.velocity);
                const dvec3 vdir = speed > 1.0e-9 ? d.velocity / speed : s_.nose;
                const std::string vname = "V[" + d.target + "]";
                float x = 0.0f, y = 0.0f;
                float lx, ly;
                if (project(vdir, x, y))
                {
                    circle(x, y, mark_);
                    cross(x, y, mark_);
                    lx = x;
                    ly = y;
                    label(x, y - mark_ - char_h_ - 2.0f, vname, true);
                    label(x, y + mark_, si(speed), true);
                }
                else if (project(-vdir, x, y))
                {
                    cross(x, y, mark_);
                    label(x, y - mark_ - char_h_ - 2.0f, "-" + vname, true);
                    label(x, y + mark_, si(speed), true);
                }
                else
                {
                    off_screen(vdir, vname, si(speed), lx, ly);
                }

                // Target marker: a square at the target, with its distance.
                const double range = glm::length(d.position);
                const dvec3 tdir = range > 1.0e-9 ? d.position / range : s_.nose;
                if (project(tdir, x, y))
                {
                    box(x - mark_, y - mark_, x + mark_, y + mark_);
                    label(x, y - mark_ - char_h_ - 2.0f, d.target, true);
                    label(x, y + mark_, si(range), true);
                }
                else
                {
                    off_screen(tdir, d.target, si(range), lx, ly);
                }

                if (d.has_port)
                    approach_gates(d);
                aircraft_symbol();
            }

        private:
            const HudState &s_;
            Colour colour_;
            int width_ = 0, height_ = 0, char_w_ = 8, char_h_ = 16;
            float mark_ = 20.0f;
            Font *font_ = nullptr;

            float cx() const { return width_ * 0.5f; }
            float cy() const { return height_ * 0.5f; }

            View to_view(const dvec3 &w) const
            {
                return {glm::dot(w, s_.camera.right), glm::dot(w, s_.camera.up), glm::dot(w, s_.camera.forward)};
            }

            /** Screen position of a world direction; false when it is behind the eye or far outside the screen. */
            bool project(const dvec3 &w, float &x, float &y) const
            {
                const View v = to_view(w);
                if (v.z <= 1.0e-6)
                    return false;
                const double px = cx() + s_.camera.focal * v.x / v.z;
                const double py = cy() - s_.camera.focal * v.y / v.z;
                if (px < 0.0 || px > width_ || py < 0.0 || py > height_)
                    return false;
                x = static_cast<float>(px);
                y = static_cast<float>(py);
                return true;
            }

            /** Screen position of the vessel's nose, which anchors the ladder; false when it is out of sight. */
            bool nose_position(float &x, float &y) const
            {
                const View v = to_view(s_.nose);
                if (v.z <= 1.0e-6)
                    return false;
                x = static_cast<float>(cx() + s_.camera.focal * v.x / v.z);
                y = static_cast<float>(cy() - s_.camera.focal * v.y / v.z);
                return true;
            }

            void line(float x1, float y1, float x2, float y2) const { sl::line(sl::screen, x1, y1, x2, y2, colour_, stroke); }
            void circle(float x, float y, float r) const { sl::circle(sl::screen, x, y, r, colour_, stroke); }
            void box(float l, float t, float r, float b) const { sl::rect(sl::screen, l, t, r, b, colour_, stroke); }
            void cross(float x, float y, float r) const
            {
                line(x - r, y, x + r, y);
                line(x, y - r, x, y + r);
            }

            void text(float x, float y, const std::string &str) const { sl::gprintf(static_cast<int>(x), static_cast<int>(y), font_, colour_, "%s", str.c_str()); }
            void label(float x, float y, const std::string &str, bool centred) const
            {
                const int w = text_length(font_, str);
                text(centred ? x - w * 0.5f : x, y, str);
            }

            /** The fixed marker showing where the vessel's nose points when the camera looks along it. */
            void aircraft_symbol() const
            {
                float x, y;
                if (!nose_position(x, y))
                    return;
                const float d = width_ / 32.0f;
                line(x - 2 * d, y, x - d, y);
                line(x + 2 * d, y, x + d, y);
                line(x - d, y + d, x, y);
                line(x, y, x + d, y + d);
            }

            /** Velocity-style marker: a circle with wings and a fin when seen ahead, a crossed circle when seen behind. */
            void direction_marker(const dvec3 &dir, const std::string &tag, bool allow_retro)
            {
                float x = 0.0f, y = 0.0f;
                const float r = mark_;
                if (project(dir, x, y))
                {
                    circle(x, y, r);
                    line(x - r * 2.0f, y, x - r, y);
                    line(x + r, y, x + r * 2.0f, y);
                    line(x, y - r, x, y - r * 1.6f);
                    return;
                }
                if (allow_retro && project(-dir, x, y))
                {
                    circle(x, y, r);
                    line(x - r * 0.7f, y - r * 0.7f, x + r * 0.7f, y + r * 0.7f);
                    line(x - r * 0.7f, y + r * 0.7f, x + r * 0.7f, y - r * 0.7f);
                    label(x, y - r - char_h_ - 2.0f, "RG", true);
                    return;
                }
                float lx, ly;
                off_screen(dir, tag, "", lx, ly);
            }

            /** A triangle on the screen edge pointing towards a direction that is off screen. */
            void off_screen(const dvec3 &dir, const std::string &name, const std::string &value, float &px, float &py) const
            {
                const View v = to_view(dir);
                double ax = v.x, ay = v.y;
                const double len = std::hypot(ax, ay);
                if (len < 1.0e-9)
                {
                    ax = 0.0;
                    ay = -1.0;
                }
                else
                {
                    ax /= len;
                    ay /= len;
                }
                const double margin = mark_ * 2.5;
                const double hx = width_ * 0.5 - margin, hy = height_ * 0.5 - margin;
                const double scale = std::min(std::fabs(ax) > 1e-9 ? hx / std::fabs(ax) : 1e30, std::fabs(ay) > 1e-9 ? hy / std::fabs(ay) : 1e30);
                px = static_cast<float>(cx() + ax * scale);
                py = static_cast<float>(cy() - ay * scale);
                const float tip = mark_ * 0.9f, back = mark_ * 0.5f;
                const float dx = static_cast<float>(ax), dy = static_cast<float>(-ay);
                const float ox = px + dx * tip, oy = py + dy * tip;
                const float bx = px - dx * back, by = py - dy * back;
                sl::triangle(sl::screen, ox, oy, bx - dy * back, by + dx * back, bx + dy * back, by - dx * back, colour_, stroke);
                if (!name.empty())
                    label(px, py - mark_ - char_h_ - 2.0f, name, true);
                if (!value.empty())
                    label(px, py + mark_, value, true);
            }

            /** Two framed readouts at the sides of the screen with a unit tag beside each. */
            void readout_boxes(const std::string &left, const std::string &left_tag, const std::string &right, const std::string &right_tag) const
            {
                const int box_w = char_w_ * 8;
                const float y = char_h_ * 3.8f;
                const float xl = mark_ * 10.0f;
                const float xr = width_ - mark_ * 10.0f - box_w;
                box(xl, y, xl + box_w, y + char_h_ + 3.0f);
                box(xr, y, xr + box_w, y + char_h_ + 3.0f);
                label(xl + box_w * 0.5f, y + 1.0f, left, true);
                label(xr + box_w * 0.5f, y + 1.0f, right, true);
                text(xl - text_length(font_, left_tag) - 4.0f, y + 1.0f, left_tag);
                text(xr + box_w + 4.0f, y + 1.0f, right_tag);
            }

            /** The pitch ladder: bars every ten degrees of elevation above the plane whose normal is `up`. */
            void ladder(const dvec3 &up, bool mark_negative)
            {
                float nx, ny;
                if (!nose_position(nx, ny))
                    return;
                const View u = to_view(up);
                const double ul = std::hypot(u.x, u.y);
                if (ul < 1.0e-3)
                    return;
                // Screen-plane direction of "up" (y up) and of the bars (perpendicular).
                const double sx = u.x / ul, sy = u.y / ul;
                const double tx = sy, ty = -sx;
                const double pitch = std::asin(std::clamp(glm::dot(s_.nose, up), -1.0, 1.0)) * deg;
                const double per_deg = s_.camera.focal * std::tan(10.0 / deg) / 10.0;
                const double range = 0.43 * height_;
                const double half = 0.12 * width_;

                for (int n = -9; n <= 9; ++n)
                {
                    const double off = (n * 10.0 - pitch) * per_deg;
                    if (std::fabs(off) > range)
                        continue;
                    const double bx = nx + sx * off, by = ny - sy * off;
                    const double w = n == 0 ? half * 1.4 : half;
                    // Bars end in ticks that point towards the horizon.
                    const double tick = n == 0 ? 0.0 : (n > 0 ? -1.0 : 1.0) * half * 0.12;
                    const double gx = tx * w, gy = -ty * w;
                    const double ex = sx * tick, ey = -sy * tick;
                    const double gap = 0.18;
                    // Negative bars are dashed so they read differently from the positive ones.
                    for (int side = -1; side <= 1; side += 2)
                    {
                        const double ax = bx + side * gx, ay = by + side * gy;
                        const double bx2 = bx + side * gx * gap, by2 = by + side * gy * gap;
                        if (mark_negative && n < 0)
                        {
                            const double mx = ax + (bx2 - ax) * 0.5, my = ay + (by2 - ay) * 0.5;
                            line(static_cast<float>(ax), static_cast<float>(ay), static_cast<float>(ax + (mx - ax) * 0.45), static_cast<float>(ay + (my - ay) * 0.45));
                            line(static_cast<float>(mx - (mx - ax) * 0.1), static_cast<float>(my - (my - ay) * 0.1), static_cast<float>(bx2), static_cast<float>(by2));
                        }
                        else
                        {
                            line(static_cast<float>(ax), static_cast<float>(ay), static_cast<float>(bx2), static_cast<float>(by2));
                        }
                        if (n != 0)
                            line(static_cast<float>(ax), static_cast<float>(ay), static_cast<float>(ax + ex), static_cast<float>(ay + ey));
                    }
                    char tag[8];
                    std::snprintf(tag, sizeof(tag), n == 0 ? "0" : (mark_negative ? "%+d" : "%d"), n * 10);
                    label(static_cast<float>(bx), static_cast<float>(by) - char_h_ * 0.5f, tag, true);
                }
            }

            /** The compass tape across the top of the screen with an index for the landing target. */
            void heading_ribbon(double heading, bool has_target, double target) const
            {
                const float x0 = cx();
                const float half = width_ / 5.0f;
                const float per_deg = 8.0f;
                const float top = 6.0f;
                line(x0 - 5, top, x0, top + 6);
                line(x0, top + 6, x0 + 6, top);
                const int first = static_cast<int>(std::ceil((heading - half / per_deg) / 2.0)) * 2;
                for (int d = first; d <= heading + half / per_deg; d += 2)
                {
                    const float x = x0 + static_cast<float>((d - heading) * per_deg);
                    const bool major = ((d % 10) + 10) % 10 == 0;
                    line(x, top + 6, x, top + (major ? 16.0f : 11.0f));
                    if (major)
                    {
                        char tag[8];
                        std::snprintf(tag, sizeof(tag), "%03d", ((d % 360) + 360) % 360);
                        label(x, top + 16.0f, tag, true);
                    }
                }
                if (!has_target)
                    return;
                double delta = std::fmod(target - heading + 540.0, 360.0) - 180.0;
                const double limit = half / per_deg;
                const float y = top + 6.0f;
                if (delta > limit)
                {
                    const float x = x0 + half + 5.0f;
                    line(x, y, x, y + 12.0f);
                    line(x, y + 12.0f, x + 12.0f, y + 6.0f);
                    line(x + 12.0f, y + 6.0f, x, y);
                }
                else if (delta < -limit)
                {
                    const float x = x0 - half - 5.0f;
                    line(x, y, x, y + 12.0f);
                    line(x, y + 12.0f, x - 12.0f, y + 6.0f);
                    line(x - 12.0f, y + 6.0f, x, y);
                }
                else
                {
                    const float x = x0 + static_cast<float>(delta * per_deg);
                    line(x, y, x - 6.0f, y + 12.0f);
                    line(x - 6.0f, y + 12.0f, x + 6.0f, y + 12.0f);
                    line(x + 6.0f, y + 12.0f, x, y);
                }
            }

            /**
             * The azimuth ribbon of orbit mode: a scale along the orbital plane through the nose, in degrees from the
             * prograde direction, tilted with the plane.
             */
            void azimuth_ribbon(const dvec3 &normal, const dvec3 &prograde)
            {
                float nx, ny;
                if (!nose_position(nx, ny))
                    return;
                const dvec3 flat = s_.nose - normal * glm::dot(s_.nose, normal);
                if (glm::length(flat) < 1.0e-6)
                    return;
                const dvec3 fp = glm::normalize(flat);
                const dvec3 tangent = glm::cross(normal, fp);
                const double angle = std::atan2(glm::dot(glm::cross(prograde, fp), normal), glm::dot(prograde, fp)) * deg;
                const View tv = to_view(tangent);
                const double tl = std::hypot(tv.x, tv.y);
                if (tl < 1.0e-3)
                    return;
                // Screen direction in which the angle measured from prograde decreases along the plane.
                const double dx = tv.x / tl, dy = tv.y / tl;
                const double per_deg = s_.camera.focal * std::tan(2.0 / deg) / 2.0;
                const double range = 0.7 * 0.43 * height_;
                const double major = 0.12 * width_ * 0.094;
                const int span = static_cast<int>(range / per_deg) + 2;
                const int base = static_cast<int>(std::floor(angle / 2.0));
                for (int k = base - span; k <= base + span; ++k)
                {
                    const double off = (k * 2.0 - angle) * per_deg;
                    if (std::fabs(off) > range)
                        continue;
                    // The tick for value v sits where the prograde direction is offset by -angle + v... along -tangent.
                    const double x = nx + dx * off, y = ny - dy * off;
                    const int value = ((k * 2) % 360 + 360) % 360;
                    const bool big = value % 10 == 0;
                    const double len = big ? major : major * 0.5;
                    line(static_cast<float>(x - dy * len), static_cast<float>(y - dx * len), static_cast<float>(x + dy * len), static_cast<float>(y + dx * len));
                    if (big)
                    {
                        char tag[8];
                        std::snprintf(tag, sizeof(tag), "%d", value);
                        label(static_cast<float>(x + dy * len * 3.0), static_cast<float>(y + dx * len * 3.0) - char_h_ * 0.5f, tag, true);
                    }
                }
            }

            /** Square gates stepping out from the docking port along its axis, to line up the approach. */
            void approach_gates(const HudDocking &d) const
            {
                const double len = glm::length(d.port_direction);
                if (len < 1.0e-9)
                    return;
                const dvec3 dir = d.port_direction / len;
                const dvec3 up = glm::normalize(d.port_up - dir * glm::dot(d.port_up, dir));
                const dvec3 right = glm::cross(dir, up);
                const double half = 2.5, step = 10.0;
                for (int i = 0; i < 10; ++i)
                {
                    const dvec3 centre = d.port_position + dir * (step * i);
                    const dvec3 corner[4] = {centre + up * half + right * half, centre + up * half - right * half, centre - up * half - right * half,
                                             centre - up * half + right * half};
                    float px[4], py[4];
                    bool all = true;
                    for (int c = 0; c < 4 && all; ++c)
                    {
                        const View v = to_view(corner[c]);
                        if (v.z <= 1.0e-3)
                        {
                            all = false;
                            break;
                        }
                        px[c] = static_cast<float>(cx() + s_.camera.focal * v.x / v.z);
                        py[c] = static_cast<float>(cy() - s_.camera.focal * v.y / v.z);
                        if (std::fabs(px[c] - cx()) > width_ * 4.0f || std::fabs(py[c] - cy()) > height_ * 4.0f)
                            all = false;
                    }
                    if (!all)
                        continue;
                    for (int c = 0; c < 4; ++c)
                        line(px[c], py[c], px[(c + 1) % 4], py[(c + 1) % 4]);
                    // A tick on the top edge shows which way is up for the port.
                    const dvec3 top = centre + up * (half * 1.5);
                    const View tv = to_view(top);
                    if (tv.z > 1.0e-3)
                    {
                        const float tx = static_cast<float>(cx() + s_.camera.focal * tv.x / tv.z), ty = static_cast<float>(cy() - s_.camera.focal * tv.y / tv.z);
                        line((px[0] + px[1]) * 0.5f, (py[0] + py[1]) * 0.5f, tx, ty);
                    }
                }
            }
        };
    }

    const char *hud_mode_name(HudMode mode)
    {
        switch (mode)
        {
        case HudMode::orbit:
            return "ORBIT";
        case HudMode::surface:
            return "SURFACE";
        case HudMode::docking:
            return "DOCKING";
        default:
            return "OFF";
        }
    }

    bool hud_parse_mode(const std::string &text, HudMode &mode)
    {
        const std::string t = upper(text);
        if (t == "OFF" || t == "NONE" || t == "0")
            mode = HudMode::off;
        else if (t == "ORBIT" || t == "1")
            mode = HudMode::orbit;
        else if (t == "SURFACE" || t == "SRF" || t == "SRFCE" || t == "2")
            mode = HudMode::surface;
        else if (t == "DOCKING" || t == "DOCK" || t == "3")
            mode = HudMode::docking;
        else
            return false;
        return true;
    }

    HudMode Hud::next_mode()
    {
        switch (mode_)
        {
        case HudMode::off:
            mode_ = HudMode::orbit;
            break;
        case HudMode::orbit:
            mode_ = HudMode::surface;
            break;
        case HudMode::surface:
            mode_ = HudMode::docking;
            break;
        default:
            mode_ = HudMode::off;
            break;
        }
        return mode_;
    }

    int Hud::colour_count() { return palette_size; }

    void Hud::set_colour(int index) { colour_ = ((index % palette_size) + palette_size) % palette_size; }

    void Hud::next_colour() { set_colour(colour_ + 1); }

    Colour Hud::line_colour() const { return palette[colour_]; }

    void Hud::draw(const HudState &state) const
    {
        if (mode_ == HudMode::off)
            return;
        Painter painter(state, line_colour());
        switch (mode_)
        {
        case HudMode::orbit:
            painter.orbit();
            break;
        case HudMode::surface:
            painter.surface();
            break;
        case HudMode::docking:
            painter.docking();
            break;
        default:
            break;
        }
    }
}

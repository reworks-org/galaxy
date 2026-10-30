#include "flight.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace sl
{
    namespace
    {
        constexpr double pi = 3.14159265358979323846;
        constexpr int max_bodies = 4096;
        constexpr double sea_level_density = 1.225;
        constexpr std::size_t slot_size = 13;

        // The orbit maths works in a Z-up frame; the library is Y-up.
        dvec3 to_std(const dvec3 &v) { return {v.x, -v.z, v.y}; }
        dvec3 from_std(const dvec3 &v) { return {v.x, v.z, -v.y}; }

        double wrap_pi(double a)
        {
            a = std::fmod(a, 2.0 * pi);
            if (a > pi)
                a -= 2.0 * pi;
            else if (a < -pi)
                a += 2.0 * pi;
            return a;
        }

        dquat orbit_rotation(const OrbitElements &el)
        {
            return glm::angleAxis(el.raan, dvec3(0, 0, 1)) * glm::angleAxis(el.inclination, dvec3(1, 0, 0)) *
                   glm::angleAxis(el.arg_periapsis, dvec3(0, 0, 1));
        }

        double smoothstep(double x)
        {
            x = std::clamp(x, 0.0, 1.0);
            return x * x * (3.0 - 2.0 * x);
        }

        void surface_coefficients(const LiftingSurface &s, double alpha, double &cl, double &cd)
        {
            const double a = wrap_pi(alpha + s.control_gain * s.control);
            const double abs_a = std::fabs(a);
            const double limit = std::fabs(s.cl_alpha) * s.stall_angle;
            const double lift_pre = std::clamp(s.cl0 + s.cl_alpha * a, -limit - std::fabs(s.cl0), limit + std::fabs(s.cl0));
            const double drag_pre = s.cd0 + s.induced_drag * lift_pre * lift_pre;
            const double lift_post = 0.9 * std::sin(2.0 * a);
            const double sin_a = std::sin(a);
            const double drag_post = s.cd0 + 2.0 * sin_a * sin_a;
            const double w = smoothstep((abs_a - s.stall_angle) / 0.15);
            cl = (1.0 - w) * lift_pre + w * lift_post;
            cd = (1.0 - w) * drag_pre + w * drag_post;
        }

        double solve_elliptic(double mean, double e)
        {
            double big_e = e < 0.8 ? mean : pi;
            for (int i = 0; i < 60; ++i)
            {
                const double f = big_e - e * std::sin(big_e) - mean;
                const double d = 1.0 - e * std::cos(big_e);
                const double step = f / d;
                big_e -= step;
                if (std::fabs(step) < 1e-14)
                    break;
            }
            return big_e;
        }

        double solve_hyperbolic(double mean, double e)
        {
            double h = std::asinh(mean / e);
            for (int i = 0; i < 80; ++i)
            {
                const double f = e * std::sinh(h) - h - mean;
                const double d = e * std::cosh(h) - 1.0;
                const double step = f / d;
                h -= step;
                if (std::fabs(step) < 1e-14 * std::max(1.0, std::fabs(h)))
                    break;
            }
            return h;
        }

        struct Tableau
        {
            int stages;
            double a[7][7];
            double b[7];
            double c[7];
        };

        const Tableau rk4_tableau = {
            4,
            {{0}, {0.5}, {0, 0.5}, {0, 0, 1}},
            {1.0 / 6, 1.0 / 3, 1.0 / 3, 1.0 / 6},
            {0, 0.5, 0.5, 1}};

        // Butcher's 1964 sixth-order, seven-stage method.
        const Tableau rk6_tableau = {
            7,
            {{0},
             {1.0 / 3},
             {0, 2.0 / 3},
             {1.0 / 12, 1.0 / 3, -1.0 / 12},
             {-1.0 / 16, 9.0 / 8, -3.0 / 16, -3.0 / 8},
             {0, 9.0 / 8, -3.0 / 8, -3.0 / 4, 1.0 / 2},
             {9.0 / 44, -9.0 / 11, 63.0 / 44, 18.0 / 11, 0, -16.0 / 11}},
            {11.0 / 120, 0, 27.0 / 40, 27.0 / 40, -4.0 / 15, -4.0 / 15, 11.0 / 120},
            {0, 1.0 / 3, 2.0 / 3, 1.0 / 3, 1.0 / 2, 1.0 / 2, 1}};
    }

    // ---- Orbits ----

    double OrbitElements::mean_motion(double mu) const
    {
        const double a = std::fabs(semi_major_axis);
        return a > 0.0 ? std::sqrt(mu / (a * a * a)) : 0.0;
    }

    double OrbitElements::period(double mu) const
    {
        if (eccentricity >= 1.0 || semi_major_axis <= 0.0)
            return 0.0;
        return 2.0 * pi / mean_motion(mu);
    }

    double OrbitElements::periapsis() const { return semi_major_axis * (1.0 - eccentricity); }

    double OrbitElements::apoapsis() const
    {
        return eccentricity < 1.0 ? semi_major_axis * (1.0 + eccentricity) : 0.0;
    }

    bool orbit_from_state(double mu, const dvec3 &position, const dvec3 &velocity, OrbitElements &out)
    {
        if (!(mu > 0.0))
            return false;
        const dvec3 r = to_std(position);
        const dvec3 v = to_std(velocity);
        const double rmag = glm::length(r);
        const dvec3 h = glm::cross(r, v);
        const double hmag = glm::length(h);
        if (rmag <= 0.0 || hmag <= 1e-12 * rmag * glm::length(v))
            return false;
        const dvec3 evec = glm::cross(v, h) / mu - r / rmag;
        const double e = glm::length(evec);
        const double energy = 0.5 * glm::dot(v, v) - mu / rmag;
        if (std::fabs(e - 1.0) < 1e-9 || energy == 0.0)
            return false;

        OrbitElements el;
        el.eccentricity = e;
        el.semi_major_axis = -mu / (2.0 * energy);
        el.inclination = std::acos(std::clamp(h.z / hmag, -1.0, 1.0));
        const dvec3 node(-h.y, h.x, 0.0);
        el.raan = glm::length(node) > 1e-12 * hmag ? std::atan2(node.y, node.x) : 0.0;
        const dvec3 node_dir(std::cos(el.raan), std::sin(el.raan), 0.0);
        const dvec3 hhat = h / hmag;
        const dvec3 perp = glm::cross(hhat, node_dir);
        double nu;
        if (e > 1e-9)
        {
            el.arg_periapsis = std::atan2(glm::dot(evec, perp), glm::dot(evec, node_dir));
            const dvec3 ehat = evec / e;
            nu = std::atan2(glm::dot(r, glm::cross(hhat, ehat)), glm::dot(r, ehat));
        }
        else
        {
            el.arg_periapsis = 0.0;
            nu = std::atan2(glm::dot(r, perp), glm::dot(r, node_dir));
        }
        if (e < 1.0)
        {
            const double big_e = 2.0 * std::atan(std::sqrt((1.0 - e) / (1.0 + e)) * std::tan(nu / 2.0));
            el.mean_anomaly = big_e - e * std::sin(big_e);
        }
        else
        {
            const double h_anom = 2.0 * std::atanh(std::sqrt((e - 1.0) / (e + 1.0)) * std::tan(nu / 2.0));
            el.mean_anomaly = e * std::sinh(h_anom) - h_anom;
        }
        out = el;
        return true;
    }

    OrbitState state_from_orbit(double mu, const OrbitElements &el, double dt)
    {
        OrbitState result;
        const double a = el.semi_major_axis;
        const double e = el.eccentricity;
        if (a == 0.0 || !(mu > 0.0) || e < 0.0)
            return result;
        double mean = el.mean_anomaly + el.mean_motion(mu) * dt;
        double nu, r;
        const double p = a * (1.0 - e * e);
        if (e < 1.0)
        {
            mean = wrap_pi(mean);
            const double big_e = solve_elliptic(mean, e);
            nu = 2.0 * std::atan2(std::sqrt(1.0 + e) * std::sin(big_e / 2.0), std::sqrt(1.0 - e) * std::cos(big_e / 2.0));
            r = a * (1.0 - e * std::cos(big_e));
        }
        else
        {
            const double h = solve_hyperbolic(mean, e);
            nu = 2.0 * std::atan(std::sqrt((e + 1.0) / (e - 1.0)) * std::tanh(h / 2.0));
            r = a * (1.0 - e * std::cosh(h));
        }
        const double speed_scale = std::sqrt(mu / p);
        const dvec3 pos_pf(r * std::cos(nu), r * std::sin(nu), 0.0);
        const dvec3 vel_pf(-speed_scale * std::sin(nu), speed_scale * (e + std::cos(nu)), 0.0);
        const dquat rot = orbit_rotation(el);
        result.position = from_std(rot * pos_pf);
        result.velocity = from_std(rot * vel_pf);
        return result;
    }

    std::vector<dvec3> orbit_path(double mu, const OrbitElements &el, int samples, double max_radius)
    {
        std::vector<dvec3> points;
        const double a = el.semi_major_axis;
        const double e = el.eccentricity;
        if (a == 0.0 || !(mu > 0.0) || samples < 2 || e < 0.0)
            return points;
        const double p = a * (1.0 - e * e);
        const dquat rot = orbit_rotation(el);
        double nu_min = -pi;
        double nu_max = pi;
        if (e >= 1.0)
        {
            double limit = std::acos(-1.0 / e) * 0.999;
            if (max_radius > 0.0)
                limit = std::min(limit, std::acos(std::clamp((p / max_radius - 1.0) / e, -1.0, 1.0)));
            nu_min = -limit;
            nu_max = limit;
        }
        points.reserve(static_cast<std::size_t>(samples) + 1);
        for (int i = 0; i <= samples; ++i)
        {
            const double nu = nu_min + (nu_max - nu_min) * static_cast<double>(i) / samples;
            const double r = p / (1.0 + e * std::cos(nu));
            points.push_back(from_std(rot * dvec3(r * std::cos(nu), r * std::sin(nu), 0.0)));
        }
        return points;
    }

    // ---- Small types ----

    double Atmosphere::density(double altitude) const
    {
        if (surface_density <= 0.0 || scale_height <= 0.0)
            return 0.0;
        return surface_density * std::exp(-std::max(altitude, 0.0) / scale_height);
    }

    double FlightBody::mass() const
    {
        double total = dry_mass;
        for (const PropellantTank &tank : tanks)
            total += tank.mass;
        return total;
    }

    dvec3 FlightBody::inertia() const
    {
        return dry_mass > 0.0 ? dry_inertia * (mass() / dry_mass) : dry_inertia;
    }

    dmat3 FlightBody::rotation() const { return glm::mat3_cast(orientation); }
    dvec3 FlightBody::to_world(const dvec3 &p) const { return position + orientation * p; }
    dvec3 FlightBody::to_local(const dvec3 &p) const { return glm::inverse(orientation) * (p - position); }
    dvec3 FlightBody::direction_to_world(const dvec3 &d) const { return orientation * d; }
    dvec3 FlightBody::direction_to_local(const dvec3 &d) const { return glm::inverse(orientation) * d; }

    // Isp at this air density divided by the vacuum Isp; 1 unless the thruster has a sea-level Isp.
    static double isp_ratio(const Thruster &t, double air_density)
    {
        if (t.isp_sea_level <= 0.0 || t.air_breathing || t.isp <= 0.0)
            return 1.0;
        const double p = std::clamp(air_density / sea_level_density, 0.0, 1.0);
        return (t.isp + (t.isp_sea_level - t.isp) * p) / t.isp;
    }

    double thruster_output(const FlightBody &body, const Thruster &t, double air_density)
    {
        const double level = std::clamp(t.level, 0.0, 1.0);
        if (level <= 0.0 || t.max_thrust <= 0.0)
            return 0.0;
        if (t.tank >= 0 && (t.tank >= static_cast<int>(body.tanks.size()) || body.tanks[t.tank].mass <= 0.0))
            return 0.0;
        double force = level * t.max_thrust;
        if (t.air_breathing)
            force *= std::clamp(air_density / sea_level_density, 0.0, 1.5);
        return force * isp_ratio(t, air_density);
    }

    // ---- Bodies ----

    int FlightWorld::create_body(const std::string &name)
    {
        if (static_cast<int>(bodies_.size()) >= max_bodies)
            return -1;
        FlightBody body;
        body.name = name;
        bodies_.push_back(std::move(body));
        return static_cast<int>(bodies_.size()) - 1;
    }

    bool FlightWorld::destroy_body(int id)
    {
        FlightBody *b = body(id);
        if (!b || !b->active)
            return false;
        b->active = false;
        return true;
    }

    FlightBody *FlightWorld::body(int id)
    {
        return id >= 0 && id < static_cast<int>(bodies_.size()) ? &bodies_[id] : nullptr;
    }

    const FlightBody *FlightWorld::body(int id) const
    {
        return id >= 0 && id < static_cast<int>(bodies_.size()) ? &bodies_[id] : nullptr;
    }

    int FlightWorld::find_body(const std::string &name) const
    {
        for (std::size_t i = 0; i < bodies_.size(); ++i)
            if (bodies_[i].active && bodies_[i].name == name)
                return static_cast<int>(i);
        return -1;
    }

    bool FlightWorld::set_rails(int id, int parent)
    {
        FlightBody *b = body(id);
        const FlightBody *p = body(parent);
        if (!b || !p || !b->active || !p->active || parent >= id || p->gm <= 0.0 || b->mode == FlightBodyMode::rails)
            return false;
        OrbitElements el;
        if (!orbit_from_state(p->gm, b->position - p->position, b->velocity - p->velocity, el))
            return false;
        b->rails_orbit = el;
        b->rails_parent = parent;
        b->rails_epoch = time_;
        b->mode = FlightBodyMode::rails;
        return true;
    }

    bool FlightWorld::release_rails(int id)
    {
        FlightBody *b = body(id);
        if (!b || b->mode != FlightBodyMode::rails)
            return false;
        b->mode = FlightBodyMode::dynamic;
        return true;
    }

    // ---- Controls ----

    void FlightWorld::apply_force(int id, const dvec3 &force)
    {
        if (FlightBody *b = body(id))
            b->force_accumulator += force;
    }

    void FlightWorld::apply_force_at(int id, const dvec3 &force, const dvec3 &world_point)
    {
        if (FlightBody *b = body(id))
        {
            b->force_accumulator += force;
            b->torque_accumulator += glm::cross(world_point - b->position, force);
        }
    }

    void FlightWorld::apply_torque(int id, const dvec3 &torque)
    {
        if (FlightBody *b = body(id))
            b->torque_accumulator += torque;
    }

    bool FlightWorld::add_group(int id, const std::string &name, const std::vector<int> &thrusters)
    {
        FlightBody *b = body(id);
        if (!b)
            return false;
        for (int index : thrusters)
            if (index < 0 || index >= static_cast<int>(b->thrusters.size()))
                return false;
        for (ThrusterGroup &g : b->groups)
            if (g.name == name)
            {
                g.thrusters = thrusters;
                return true;
            }
        b->groups.push_back({name, thrusters});
        return true;
    }

    bool FlightWorld::set_group_level(int id, const std::string &name, double level)
    {
        FlightBody *b = body(id);
        if (!b || !std::isfinite(level))
            return false;
        for (const ThrusterGroup &g : b->groups)
            if (g.name == name)
            {
                for (int index : g.thrusters)
                    b->thrusters[index].level = std::clamp(level, 0.0, 1.0);
                return true;
            }
        return false;
    }

    void FlightWorld::build_standard_groups(int id)
    {
        FlightBody *b = body(id);
        if (!b)
            return;
        std::vector<dvec3> dir(b->thrusters.size());
        std::vector<dvec3> tau(b->thrusters.size());
        double max_tau = 0.0;
        for (std::size_t i = 0; i < b->thrusters.size(); ++i)
        {
            const double len = glm::length(b->thrusters[i].direction);
            dir[i] = len > 0.0 ? b->thrusters[i].direction / len : dvec3(0.0);
            tau[i] = glm::cross(b->thrusters[i].position, dir[i]);
            max_tau = std::max(max_tau, glm::length(tau[i]));
        }
        auto build = [&](const char *name, auto predicate)
        {
            std::vector<int> members;
            for (std::size_t i = 0; i < b->thrusters.size(); ++i)
                if (predicate(dir[i], tau[i]))
                    members.push_back(static_cast<int>(i));
            add_group(id, name, members);
        };
        constexpr double axis_dot = 0.7;
        build("main", [&](const dvec3 &d, const dvec3 &) { return -d.z > axis_dot; });
        build("retro", [&](const dvec3 &d, const dvec3 &) { return d.z > axis_dot; });
        build("hover", [&](const dvec3 &d, const dvec3 &) { return d.y > axis_dot; });
        build("x+", [&](const dvec3 &d, const dvec3 &) { return d.x > axis_dot; });
        build("x-", [&](const dvec3 &d, const dvec3 &) { return -d.x > axis_dot; });
        build("y+", [&](const dvec3 &d, const dvec3 &) { return d.y > axis_dot; });
        build("y-", [&](const dvec3 &d, const dvec3 &) { return -d.y > axis_dot; });
        build("z+", [&](const dvec3 &d, const dvec3 &) { return d.z > axis_dot; });
        build("z-", [&](const dvec3 &d, const dvec3 &) { return -d.z > axis_dot; });
        auto rotates = [&](const dvec3 &t, int axis, double sign)
        {
            const double len = glm::length(t);
            return len > 0.1 * max_tau && len > 0.0 && sign * t[axis] > 0.5 * len;
        };
        build("pitch_up", [&](const dvec3 &, const dvec3 &t) { return rotates(t, 0, 1.0); });
        build("pitch_down", [&](const dvec3 &, const dvec3 &t) { return rotates(t, 0, -1.0); });
        build("yaw_left", [&](const dvec3 &, const dvec3 &t) { return rotates(t, 1, 1.0); });
        build("yaw_right", [&](const dvec3 &, const dvec3 &t) { return rotates(t, 1, -1.0); });
        build("roll_left", [&](const dvec3 &, const dvec3 &t) { return rotates(t, 2, 1.0); });
        build("roll_right", [&](const dvec3 &, const dvec3 &t) { return rotates(t, 2, -1.0); });
    }

    void FlightWorld::apply_rcs(FlightBody &b)
    {
        struct Binding
        {
            const char *name;
            int axis;
            double sign;
            bool rotation;
        };
        static const Binding bindings[] = {
            {"pitch_up", 0, 1, true}, {"pitch_down", 0, -1, true}, {"yaw_left", 1, 1, true},
            {"yaw_right", 1, -1, true}, {"roll_left", 2, 1, true}, {"roll_right", 2, -1, true},
            {"x+", 0, 1, false}, {"x-", 0, -1, false}, {"y+", 1, 1, false},
            {"y-", 1, -1, false}, {"z+", 2, 1, false}, {"z-", 2, -1, false}};
        // Main and retro engines are driven by their own throttle, not by RCS commands, even though their
        // direction also places them in the z translation groups.
        std::vector<bool> engine(b.thrusters.size(), false);
        for (const ThrusterGroup &g : b.groups)
            if (g.name == "main" || g.name == "retro")
                for (int index : g.thrusters)
                    engine[index] = true;
        for (const Binding &bind : bindings)
            for (const ThrusterGroup &g : b.groups)
                if (g.name == bind.name)
                    for (int index : g.thrusters)
                        if (!engine[index])
                            b.thrusters[index].level = 0.0;
        for (const Binding &bind : bindings)
        {
            const double command = bind.rotation ? b.rcs_rotation[bind.axis] : b.rcs_translation[bind.axis];
            const double level = std::clamp(bind.sign * command, 0.0, 1.0);
            if (level <= 0.0)
                continue;
            for (const ThrusterGroup &g : b.groups)
                if (g.name == bind.name)
                    for (int index : g.thrusters)
                        if (!engine[index])
                            b.thrusters[index].level = std::max(b.thrusters[index].level, level);
        }
    }

    void FlightWorld::set_rcs_rotation(int id, double pitch, double yaw, double roll)
    {
        if (FlightBody *b = body(id))
        {
            b->rcs_rotation = {std::isfinite(pitch) ? pitch : 0.0, std::isfinite(yaw) ? yaw : 0.0,
                               std::isfinite(roll) ? roll : 0.0};
            apply_rcs(*b);
        }
    }

    void FlightWorld::set_rcs_translation(int id, double x, double y, double z)
    {
        if (FlightBody *b = body(id))
        {
            b->rcs_translation = {std::isfinite(x) ? x : 0.0, std::isfinite(y) ? y : 0.0, std::isfinite(z) ? z : 0.0};
            apply_rcs(*b);
        }
    }

    bool FlightWorld::set_control(int id, const std::string &control_name, double value)
    {
        FlightBody *b = body(id);
        if (!b || !std::isfinite(value))
            return false;
        bool found = false;
        for (LiftingSurface &s : b->surfaces)
            if (s.control_name == control_name)
            {
                s.control = std::clamp(value, -1.0, 1.0);
                found = true;
            }
        return found;
    }

    void FlightWorld::set_brakes(int id, double level)
    {
        if (FlightBody *b = body(id))
            for (GearContact &g : b->gear)
                g.brake = std::clamp(level, 0.0, 1.0);
    }

    // ---- Environment queries ----

    std::vector<FlightWorld::Kinematics> FlightWorld::master_kinematics() const
    {
        std::vector<Kinematics> kin(bodies_.size());
        for (std::size_t i = 0; i < bodies_.size(); ++i)
            kin[i] = {bodies_[i].position, bodies_[i].velocity, bodies_[i].orientation, bodies_[i].angular_velocity};
        return kin;
    }

    dvec3 FlightWorld::gravity_from(const std::vector<Kinematics> &kin, const dvec3 &point, int skip) const
    {
        dvec3 a = uniform_gravity;
        for (std::size_t j = 0; j < bodies_.size(); ++j)
        {
            const FlightBody &src = bodies_[j];
            if (!src.active || src.gm <= 0.0 || static_cast<int>(j) == skip)
                continue;
            const dvec3 d = kin[j].position - point;
            const double r2 = glm::dot(d, d);
            if (r2 < 1e-6)
                continue;
            a += d * (src.gm / (r2 * std::sqrt(r2)));
        }
        return a;
    }

    void FlightWorld::air_from(const std::vector<Kinematics> &kin, const dvec3 &point, double &density,
                               dvec3 &velocity) const
    {
        density = 0.0;
        velocity = wind;
        if (flat_atmosphere.surface_density > 0.0)
            density = flat_atmosphere.density(point.y - (flat_terrain ? flat_terrain->height_at(point.x, point.z) : flat_ground_height));
        for (std::size_t j = 0; j < bodies_.size(); ++j)
        {
            const FlightBody &planet = bodies_[j];
            if (!planet.active || planet.atmosphere.surface_density <= 0.0 || planet.radius <= 0.0)
                continue;
            const dvec3 d = point - kin[j].position;
            const double rho = planet.atmosphere.density(glm::length(d) - planet.radius -
                                                         (planet.terrain ? planet.terrain->height_at_direction(d) : 0.0));
            if (rho > density)
            {
                density = rho;
                velocity = kin[j].velocity + glm::cross(planet.spin_axis * planet.spin_rate, d) +
                           (planet.wind_field ? planet.wind_field(d) : planet.wind);
            }
        }
    }

    double FlightWorld::altitude_from(const std::vector<Kinematics> &kin, const dvec3 &point) const
    {
        double best = std::numeric_limits<double>::infinity();
        if (flat_ground)
            best = point.y - (flat_terrain ? flat_terrain->height_at(point.x, point.z) : flat_ground_height);
        for (std::size_t j = 0; j < bodies_.size(); ++j)
            if (bodies_[j].active && bodies_[j].radius > 0.0)
            {
                const dvec3 d = point - kin[j].position;
                double altitude = glm::length(d) - bodies_[j].radius;
                if (bodies_[j].terrain)
                    altitude -= bodies_[j].terrain->height_at_direction(d);
                best = std::min(best, altitude);
            }
        return best;
    }

    dvec3 FlightWorld::gravity_at(const dvec3 &point) const { return gravity_from(master_kinematics(), point, -1); }

    double FlightWorld::air_density_at(const dvec3 &point) const
    {
        double rho;
        dvec3 v;
        air_from(master_kinematics(), point, rho, v);
        return rho;
    }

    dvec3 FlightWorld::air_velocity_at(const dvec3 &point) const
    {
        double rho;
        dvec3 v;
        air_from(master_kinematics(), point, rho, v);
        return v;
    }

    double FlightWorld::altitude_of(const dvec3 &point) const { return altitude_from(master_kinematics(), point); }

    double FlightWorld::specific_energy(int id) const
    {
        const FlightBody *b = body(id);
        if (!b)
            return 0.0;
        double energy = 0.5 * glm::dot(b->velocity, b->velocity);
        for (std::size_t j = 0; j < bodies_.size(); ++j)
        {
            if (!bodies_[j].active || bodies_[j].gm <= 0.0 || static_cast<int>(j) == id)
                continue;
            const double r = glm::length(bodies_[j].position - b->position);
            if (r > 0.0)
                energy -= bodies_[j].gm / r;
        }
        energy -= glm::dot(uniform_gravity, b->position);
        return energy;
    }

    bool FlightWorld::orbit_of(int id, int parent, OrbitElements &out) const
    {
        const FlightBody *b = body(id);
        const FlightBody *p = body(parent);
        if (!b || !p || id == parent)
            return false;
        return orbit_from_state(p->gm, b->position - p->position, b->velocity - p->velocity, out);
    }

    dvec3 FlightWorld::relative_position(int id, int reference) const
    {
        const FlightBody *b = body(id);
        const FlightBody *r = body(reference);
        if (!b)
            return dvec3(0.0);
        return r ? b->position - r->position : b->position;
    }

    std::vector<dvec3> FlightWorld::predict_path(int id, double duration, int samples) const
    {
        std::vector<dvec3> path;
        if (!body(id) || !(duration > 0.0) || samples < 1)
            return path;
        samples = std::min(samples, 100000);
        FlightWorld copy = *this;
        for (FlightBody &b : copy.bodies_)
        {
            b.force_accumulator = dvec3(0.0);
            b.torque_accumulator = dvec3(0.0);
            for (Thruster &t : b.thrusters)
                t.level = 0.0;
        }
        path.reserve(static_cast<std::size_t>(samples) + 1);
        path.push_back(copy.bodies_[id].position);
        for (int i = 0; i < samples; ++i)
        {
            copy.step(duration / samples);
            path.push_back(copy.bodies_[id].position);
        }
        return path;
    }

    glm::mat4 FlightWorld::render_matrix(int id, const dvec3 &origin) const
    {
        const FlightBody *b = body(id);
        if (!b)
            return glm::mat4(1.0f);
        glm::mat4 m(glm::mat3(glm::mat3_cast(glm::quat(b->orientation))));
        m[3] = glm::vec4(glm::vec3(b->position - origin), 1.0f);
        return m;
    }

    // ---- Dynamics ----

    void FlightWorld::pack(std::vector<double> &y) const
    {
        y.clear();
        for (std::size_t i = 0; i < bodies_.size(); ++i)
        {
            if (slot_[i] < 0)
                continue;
            const FlightBody &b = bodies_[i];
            const double v[slot_size] = {b.position.x, b.position.y, b.position.z,
                                         b.velocity.x, b.velocity.y, b.velocity.z,
                                         b.orientation.w, b.orientation.x, b.orientation.y, b.orientation.z,
                                         b.angular_velocity.x, b.angular_velocity.y, b.angular_velocity.z};
            y.insert(y.end(), v, v + slot_size);
        }
    }

    void FlightWorld::unpack(const std::vector<double> &y)
    {
        for (std::size_t i = 0; i < bodies_.size(); ++i)
        {
            if (slot_[i] < 0)
                continue;
            const double *v = &y[slot_[i]];
            FlightBody &b = bodies_[i];
            b.position = {v[0], v[1], v[2]};
            b.velocity = {v[3], v[4], v[5]};
            const dquat q(v[6], v[7], v[8], v[9]);
            const double len = glm::length(q);
            b.orientation = len > 0.0 ? q / len : dquat(1.0, 0.0, 0.0, 0.0);
            b.angular_velocity = {v[10], v[11], v[12]};
        }
    }

    void FlightWorld::evaluate_kinematics(double t, const std::vector<double> &y)
    {
        kin_.resize(bodies_.size());
        for (std::size_t i = 0; i < bodies_.size(); ++i)
        {
            const FlightBody &b = bodies_[i];
            Kinematics &k = kin_[i];
            if (b.terrain && (b.spin_rate != 0.0 || b.spin_phase != 0.0))
                b.terrain->set_rotation(b.spin_phase + b.spin_rate * t, b.spin_axis);
            if (slot_[i] >= 0)
            {
                const double *v = &y[slot_[i]];
                k.position = {v[0], v[1], v[2]};
                k.velocity = {v[3], v[4], v[5]};
                const dquat q(v[6], v[7], v[8], v[9]);
                const double len = glm::length(q);
                k.orientation = len > 0.0 ? q / len : dquat(1.0, 0.0, 0.0, 0.0);
                k.angular_velocity = {v[10], v[11], v[12]};
            }
            else if (b.active && b.mode == FlightBodyMode::rails && b.rails_parent >= 0 &&
                     b.rails_parent < static_cast<int>(i))
            {
                const Kinematics &parent = kin_[b.rails_parent];
                const OrbitState rel = state_from_orbit(bodies_[b.rails_parent].gm, b.rails_orbit, t - b.rails_epoch);
                k.position = parent.position + rel.position;
                k.velocity = parent.velocity + rel.velocity;
                k.orientation = b.orientation;
                k.angular_velocity = b.angular_velocity;
            }
            else
            {
                k = {b.position, b.velocity, b.orientation, b.angular_velocity};
            }
        }
    }

    void FlightWorld::body_loads(int index, const std::vector<Kinematics> &kin, dvec3 &force, dvec3 &torque_body,
                                 bool record)
    {
        FlightBody &b = bodies_[index];
        const Kinematics &k = kin[index];
        const dmat3 rot = glm::mat3_cast(k.orientation);
        const dmat3 rot_t = glm::transpose(rot);
        const dvec3 &w = k.angular_velocity;

        force = gravity_from(kin, k.position, index) * b.mass();
        torque_body = dvec3(0.0);

        double rho = 0.0;
        dvec3 air_v(0.0);
        air_from(kin, k.position, rho, air_v);

        for (const Thruster &t : b.thrusters)
        {
            const double thrust = thruster_output(b, t, rho);
            if (thrust <= 0.0)
                continue;
            const double len = glm::length(t.direction);
            if (len <= 0.0)
                continue;
            const dvec3 f_body = t.direction * (thrust / len);
            force += rot * f_body;
            torque_body += glm::cross(t.position, f_body);
        }

        if (rho > 0.0)
        {
            const dvec3 v_body = rot_t * (k.velocity - air_v);
            if (b.drag_area > 0.0)
            {
                const dvec3 drag = v_body * (-0.5 * rho * b.drag_area * glm::length(v_body));
                force += rot * drag;
            }
            for (const LiftingSurface &s : b.surfaces)
            {
                const dvec3 vb = v_body + glm::cross(w, s.position);
                const double speed = glm::length(vb);
                const double fwd_len = glm::length(s.forward);
                const double up_len = glm::length(s.up);
                if (speed < 1e-3 || fwd_len <= 0.0 || up_len <= 0.0)
                    continue;
                const dvec3 fwd = s.forward / fwd_len;
                const dvec3 up = s.up / up_len;
                const double vf = glm::dot(vb, fwd);
                const double vu = glm::dot(vb, up);
                const double in_plane = std::hypot(vf, vu);
                if (in_plane < 1e-6)
                    continue;
                double cl, cd;
                surface_coefficients(s, std::atan2(-vu, vf), cl, cd);
                const dvec3 lift_dir = (fwd * -vu + up * vf) / in_plane;
                const dvec3 f_body = (lift_dir * cl - vb / speed * cd) * (0.5 * rho * speed * speed * s.area);
                force += rot * f_body;
                torque_body += glm::cross(s.position, f_body);
            }
        }

        for (GearContact &g : b.gear)
        {
            if (!g.enabled)
            {
                g.compression = 0.0;
                continue;
            }
            const dvec3 point = k.position + rot * g.position;
            const dvec3 point_velocity = k.velocity + rot * glm::cross(w, g.position);
            double compression = 0.0;
            auto contact = [&](const dvec3 &normal, double height, const dvec3 &ground_velocity)
            {
                if (height >= 0.0)
                    return;
                compression = std::max(compression, -height);
                const dvec3 rel = point_velocity - ground_velocity;
                const double vn = glm::dot(rel, normal);
                const double normal_force = std::max(0.0, g.stiffness * -height - g.damping * vn);
                dvec3 f = normal * normal_force;
                const dvec3 vt = rel - normal * vn;
                dvec3 roll_dir = rot * g.forward;
                roll_dir -= normal * glm::dot(roll_dir, normal);
                const double roll_len = glm::length(roll_dir);
                if (roll_len > 1e-9)
                {
                    roll_dir /= roll_len;
                    const dvec3 side_dir = glm::cross(normal, roll_dir);
                    const double v0 = 1.0;
                    const double v_long = glm::dot(vt, roll_dir);
                    const double v_side = glm::dot(vt, side_dir);
                    f -= side_dir * (g.lateral_friction * normal_force * std::clamp(v_side / v0, -1.0, 1.0));
                    f -= roll_dir * ((g.rolling_resistance + g.brake_friction * g.brake) * normal_force *
                                     std::clamp(v_long / v0, -1.0, 1.0));
                    const double traction = g.lateral_friction * normal_force;
                    f += roll_dir * std::clamp(g.drive_force, -traction, traction);
                }
                else
                {
                    f -= vt * (g.lateral_friction * normal_force);
                }
                force += f;
                torque_body += glm::cross(g.position, rot_t * f);
            };
            if (flat_ground)
            {
                if (flat_terrain)
                    contact(flat_terrain->normal_at(point.x, point.z), point.y - flat_terrain->height_at(point.x, point.z),
                            dvec3(0.0));
                else
                    contact(dvec3(0, 1, 0), point.y - flat_ground_height, dvec3(0.0));
            }
            for (std::size_t j = 0; j < bodies_.size(); ++j)
            {
                const FlightBody &planet = bodies_[j];
                if (!planet.active || planet.radius <= 0.0 || static_cast<int>(j) == index)
                    continue;
                const dvec3 d = point - kin[j].position;
                const double dist = glm::length(d);
                const double ground = planet.radius + (planet.terrain ? planet.terrain->height_at_direction(d) : 0.0);
                if (dist <= 0.0 || dist - ground >= 0.0)
                    continue;
                contact(planet.terrain ? planet.terrain->normal_at_direction(d) : d / dist, dist - ground,
                        kin[j].velocity + glm::cross(planet.spin_axis * planet.spin_rate, d));
            }
            if (record)
                g.compression = compression;
        }

        force += b.force_accumulator;
        torque_body += rot_t * b.torque_accumulator - w * b.angular_damping;
    }

    void FlightWorld::derivative(double t, const std::vector<double> &y, std::vector<double> &dy)
    {
        dy.assign(y.size(), 0.0);
        evaluate_kinematics(t, y);
        for (std::size_t i = 0; i < bodies_.size(); ++i)
        {
            if (slot_[i] < 0)
                continue;
            const FlightBody &b = bodies_[i];
            const Kinematics &k = kin_[i];
            double *d = &dy[slot_[i]];
            d[0] = k.velocity.x;
            d[1] = k.velocity.y;
            d[2] = k.velocity.z;
            const dquat dq = dquat(k.orientation * dquat(0.0, k.angular_velocity.x, k.angular_velocity.y,
                                                         k.angular_velocity.z)) * 0.5;
            d[6] = dq.w;
            d[7] = dq.x;
            d[8] = dq.y;
            d[9] = dq.z;
            if (b.mode != FlightBodyMode::dynamic)
                continue;
            dvec3 force, torque;
            body_loads(static_cast<int>(i), kin_, force, torque, false);
            const double mass = b.mass();
            const dvec3 acc = mass > 0.0 ? force / mass : dvec3(0.0);
            const dvec3 inertia = b.inertia();
            const dvec3 &w = k.angular_velocity;
            const dvec3 gyro = glm::cross(w, inertia * w);
            const dvec3 alpha((inertia.x > 0.0 ? (torque.x - gyro.x) / inertia.x : 0.0),
                              (inertia.y > 0.0 ? (torque.y - gyro.y) / inertia.y : 0.0),
                              (inertia.z > 0.0 ? (torque.z - gyro.z) / inertia.z : 0.0));
            d[3] = acc.x;
            d[4] = acc.y;
            d[5] = acc.z;
            d[10] = alpha.x;
            d[11] = alpha.y;
            d[12] = alpha.z;
        }
    }

    void FlightWorld::update_rails(double t)
    {
        for (std::size_t i = 0; i < bodies_.size(); ++i)
        {
            FlightBody &b = bodies_[i];
            if (!b.active || b.mode != FlightBodyMode::rails || b.rails_parent < 0 ||
                b.rails_parent >= static_cast<int>(i))
                continue;
            const FlightBody &parent = bodies_[b.rails_parent];
            const OrbitState rel = state_from_orbit(parent.gm, b.rails_orbit, t - b.rails_epoch);
            b.position = parent.position + rel.position;
            b.velocity = parent.velocity + rel.velocity;
        }
    }

    void FlightWorld::deplete_propellant(double h)
    {
        const std::vector<Kinematics> kin = master_kinematics();
        for (FlightBody &b : bodies_)
        {
            if (!b.active || b.mode != FlightBodyMode::dynamic || b.tanks.empty())
                continue;
            double rho;
            dvec3 v;
            air_from(kin, b.position, rho, v);
            for (const Thruster &t : b.thrusters)
            {
                if (t.tank < 0 || t.tank >= static_cast<int>(b.tanks.size()) || t.isp <= 0.0)
                    continue;
                const double thrust = thruster_output(b, t, rho);
                PropellantTank &tank = b.tanks[t.tank];
                tank.mass = std::max(0.0, tank.mass - thrust / (t.isp * isp_ratio(t, rho) * standard_gravity) * h);
            }
        }
    }

    void FlightWorld::substep(double h)
    {
        slot_.assign(bodies_.size(), -1);
        std::size_t offset = 0;
        for (std::size_t i = 0; i < bodies_.size(); ++i)
            if (bodies_[i].active && bodies_[i].mode != FlightBodyMode::rails)
            {
                slot_[i] = static_cast<int>(offset);
                offset += slot_size;
            }

        // Propellant is burned half before and half after the step so the step sees the mid-step mass.
        deplete_propellant(0.5 * h);
        std::vector<double> y;
        pack(y);
        const double t = time_;
        std::vector<std::vector<double>> &k = stage_;
        k.resize(7);

        switch (integrator)
        {
        case FlightIntegrator::euler:
            derivative(t, y, k[0]);
            for (std::size_t n = 0; n < y.size(); ++n)
                y[n] += h * k[0][n];
            break;
        case FlightIntegrator::semi_implicit:
        {
            derivative(t, y, k[0]);
            for (std::size_t s = 0; s < y.size(); s += slot_size)
            {
                for (int c : {3, 4, 5, 10, 11, 12})
                    y[s + c] += h * k[0][s + c];
                for (int c : {0, 1, 2})
                    y[s + c] += h * y[s + 3 + c];
                const dquat q(y[s + 6], y[s + 7], y[s + 8], y[s + 9]);
                const dquat dq = dquat(q * dquat(0.0, y[s + 10], y[s + 11], y[s + 12])) * 0.5;
                y[s + 6] += h * dq.w;
                y[s + 7] += h * dq.x;
                y[s + 8] += h * dq.y;
                y[s + 9] += h * dq.z;
            }
            break;
        }
        case FlightIntegrator::verlet:
        {
            derivative(t, y, k[0]);
            std::vector<double> y1 = y;
            for (std::size_t s = 0; s < y.size(); s += slot_size)
            {
                for (int c : {0, 1, 2})
                    y1[s + c] = y[s + c] + h * y[s + 3 + c] + 0.5 * h * h * k[0][s + 3 + c];
                for (int c : {3, 4, 5, 10, 11, 12})
                    y1[s + c] = y[s + c] + h * k[0][s + c];
                for (int c : {6, 7, 8, 9})
                    y1[s + c] = y[s + c] + h * k[0][s + c];
            }
            derivative(t + h, y1, k[1]);
            for (std::size_t s = 0; s < y.size(); s += slot_size)
            {
                for (int c : {3, 4, 5, 10, 11, 12})
                    y1[s + c] = y[s + c] + 0.5 * h * (k[0][s + c] + k[1][s + c]);
                for (int c : {6, 7, 8, 9})
                    y1[s + c] = y[s + c] + 0.5 * h * (k[0][s + c] + k[1][s + c]);
            }
            y = y1;
            break;
        }
        case FlightIntegrator::rk4:
        case FlightIntegrator::rk6:
        {
            const Tableau &tab = integrator == FlightIntegrator::rk4 ? rk4_tableau : rk6_tableau;
            std::vector<double> tmp;
            for (int i = 0; i < tab.stages; ++i)
            {
                tmp = y;
                for (int j = 0; j < i; ++j)
                {
                    const double a = tab.a[i][j];
                    if (a == 0.0)
                        continue;
                    for (std::size_t n = 0; n < tmp.size(); ++n)
                        tmp[n] += h * a * k[j][n];
                }
                derivative(t + tab.c[i] * h, tmp, k[i]);
            }
            for (int i = 0; i < tab.stages; ++i)
            {
                if (tab.b[i] == 0.0)
                    continue;
                for (std::size_t n = 0; n < y.size(); ++n)
                    y[n] += h * tab.b[i] * k[i][n];
            }
            break;
        }
        }

        unpack(y);
        deplete_propellant(0.5 * h);
        for (FlightBody &b : bodies_)
            if (b.active && b.mode == FlightBodyMode::rails && glm::length(b.angular_velocity) > 0.0)
            {
                const double angle = glm::length(b.angular_velocity) * h;
                b.orientation = glm::normalize(b.orientation * glm::angleAxis(angle, glm::normalize(b.angular_velocity)));
            }
        time_ += h;
        update_rails(time_);
    }

    void FlightWorld::step(double dt)
    {
        if (!(dt > 0.0) || !std::isfinite(dt))
            return;
        const double limit = max_substep > 0.0 ? max_substep : 0.1;
        int count = static_cast<int>(std::ceil(dt / limit));
        count = std::clamp(count, 1, std::max(1, max_substeps_per_step));
        const double h = dt / count;
        for (int i = 0; i < count; ++i)
            substep(h);

        const std::vector<Kinematics> kin = master_kinematics();
        for (std::size_t i = 0; i < bodies_.size(); ++i)
        {
            FlightBody &b = bodies_[i];
            if (b.active && b.mode == FlightBodyMode::dynamic && !b.gear.empty())
            {
                dvec3 f, t;
                body_loads(static_cast<int>(i), kin, f, t, true);
            }
            b.force_accumulator = dvec3(0.0);
            b.torque_accumulator = dvec3(0.0);
        }
    }
}

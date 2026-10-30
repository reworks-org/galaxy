#pragma once

#include "terrain.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <functional>
#include <vector>

namespace sl
{
    /*
     * 3D flight dynamics for spacecraft and aircraft. Everything is double precision; convert to float only
     * when rendering (see FlightWorld::render_matrix). Coordinates are right-handed with Y up. A vessel's body
     * frame has +X right, +Y up and -Z forward, so pitch-up is a positive rotation about X, yaw-left about Y
     * and roll-left about Z.
     */

    /** Newtonian constant of gravitation in m^3 kg^-1 s^-2. */
    inline constexpr double gravitational_constant = 6.67430e-11;
    /** Standard gravity in m/s^2, used to convert specific impulse to exhaust velocity. */
    inline constexpr double standard_gravity = 9.80665;

    using dvec3 = glm::dvec3;
    using dquat = glm::dquat;
    using dmat3 = glm::dmat3;

    /** Numerical integrator used to advance the world. */
    enum class FlightIntegrator
    {
        /** First order, adds energy to orbits. Included for comparison. */
        euler,
        /** Symplectic Euler: first order but energy stays bounded. Cheap. */
        semi_implicit,
        /** Velocity Verlet: second order and symplectic for conservative forces. */
        verlet,
        /** Classical fourth-order Runge-Kutta. */
        rk4,
        /** Butcher's seven-stage sixth-order Runge-Kutta. */
        rk6
    };

    /** How a body is advanced each step. */
    enum class FlightBodyMode
    {
        /** Integrated under gravity, thrust, aerodynamics, gear and user forces. */
        dynamic,
        /** Moves at constant velocity and angular velocity; no forces act. Use for planets that never move. */
        kinematic,
        /** Follows an analytic Kepler orbit around a parent body: no drift, ideal for planets and time warp. */
        rails
    };

    /**
     * Classical orbital elements about a parent with gravitational parameter mu. The reference plane is XZ with
     * +Y as the pole, and the reference direction (zero longitude) is +X. Hyperbolic orbits have a negative
     * semi-major axis and eccentricity above one.
     */
    struct OrbitElements
    {
        /** Semi-major axis in metres (negative for hyperbolic orbits). */
        double semi_major_axis = 0.0;
        double eccentricity = 0.0;
        /** Inclination in radians. */
        double inclination = 0.0;
        /** Longitude of the ascending node in radians. */
        double raan = 0.0;
        /** Argument of periapsis in radians. */
        double arg_periapsis = 0.0;
        /** Mean anomaly in radians at the reference time. */
        double mean_anomaly = 0.0;

        /** Orbital period in seconds, or 0 for an unbound orbit. */
        double period(double mu) const;
        /** Mean motion in radians per second. */
        double mean_motion(double mu) const;
        /** Distance at closest approach in metres. */
        double periapsis() const;
        /** Distance at furthest point in metres, or 0 for an unbound orbit. */
        double apoapsis() const;
    };

    /** Position and velocity relative to a parent. */
    struct OrbitState
    {
        dvec3 position{0.0};
        dvec3 velocity{0.0};
    };

    /**
     * Convert a state vector to orbital elements. The mean anomaly is the one at the instant of the state.
     * Returns false for a degenerate state (zero angular momentum, or exactly parabolic energy).
     */
    bool orbit_from_state(double mu, const dvec3 &position, const dvec3 &velocity, OrbitElements &out);
    /** Convert elements to a state vector at the elements' own mean anomaly plus mean_motion * dt. */
    OrbitState state_from_orbit(double mu, const OrbitElements &elements, double dt = 0.0);
    /** Sample points along the orbit conic (parent-relative) for drawing. Hyperbolic orbits stop at max_radius. */
    std::vector<dvec3> orbit_path(double mu, const OrbitElements &elements, int samples, double max_radius = 1.0e12);

    /** A fuel or propellant tank. */
    struct PropellantTank
    {
        /** Capacity in kilograms. */
        double capacity = 0.0;
        /** Current contents in kilograms. */
        double mass = 0.0;
    };

    /** A thruster or engine fixed to a vessel. Positions are relative to the centre of mass in the body frame. */
    struct Thruster
    {
        dvec3 position{0.0};
        /** Direction the thruster pushes the vessel in the body frame (opposite the exhaust). */
        dvec3 direction{0.0, 0.0, -1.0};
        /** Thrust at level 1 in newtons. */
        double max_thrust = 0.0;
        /** Specific impulse in seconds; sets propellant use as thrust / (isp * g0). */
        double isp = 300.0;
        /** Index into the body's tanks, or -1 for an engine that uses no propellant. */
        int tank = -1;
        /** Throttle from 0 to 1. */
        double level = 0.0;
        /** If true, thrust scales with air density relative to sea level, so it fades with altitude. */
        bool air_breathing = false;
        /**
         * Specific impulse at sea-level pressure in seconds, or 0 for an engine whose Isp does not change. `isp` is then the
         * vacuum value, propellant flow stays constant and thrust follows the Isp, falling as the air gets denser
         * (the Orbiter model); `max_thrust` is the vacuum thrust.
         */
        double isp_sea_level = 0.0;
    };

    /** A named set of thrusters controlled together (main, retro, hover, RCS axes and so on). */
    struct ThrusterGroup
    {
        std::string name;
        std::vector<int> thrusters;
    };

    /**
     * A lifting or control surface. The surface has its own frame in the body frame: `forward` is the chord
     * direction (zero angle of attack along it) and `up` is the direction of positive lift. A wing has
     * up = +Y, a rudder or fin has up = +X. Coefficients follow a linear lift slope to the stall angle, then
     * blend towards a flat-plate model.
     */
    struct LiftingSurface
    {
        std::string control_name;
        dvec3 position{0.0};
        dvec3 forward{0.0, 0.0, -1.0};
        dvec3 up{0.0, 1.0, 0.0};
        /** Reference area in m^2. */
        double area = 1.0;
        /** Lift coefficient at zero angle of attack (camber). */
        double cl0 = 0.0;
        /** Lift curve slope per radian (about 2*pi for a thin airfoil, lower for finite wings). */
        double cl_alpha = 5.0;
        /** Zero-lift drag coefficient. */
        double cd0 = 0.02;
        /** Induced drag factor: Cd = cd0 + k * Cl^2. */
        double induced_drag = 0.05;
        /** Stall angle in radians. */
        double stall_angle = 0.28;
        /** Change in effective angle of attack in radians per unit control input (-1..1). */
        double control_gain = 0.0;
        /** Current control input from -1 to 1. */
        double control = 0.0;
    };

    /** A wheel or skid that pushes against the ground with a spring and damper. */
    struct GearContact
    {
        /** Contact point relative to the centre of mass in the body frame. */
        dvec3 position{0.0};
        /** Rolling direction in the body frame. */
        dvec3 forward{0.0, 0.0, -1.0};
        /** Spring stiffness in N/m. */
        double stiffness = 2.0e5;
        /** Damper rate in N s/m. */
        double damping = 2.0e4;
        /** Sideways friction coefficient. */
        double lateral_friction = 1.0;
        /** Rolling resistance coefficient. */
        double rolling_resistance = 0.02;
        /** Extra longitudinal friction coefficient applied at brake level 1. */
        double brake_friction = 0.8;
        /** Brake level from 0 to 1. */
        double brake = 0.0;
        /** Driving force in N along the rolling direction (negative reverses). It is limited by traction, lateral_friction times the load on the contact. */
        double drive_force = 0.0;
        /** Compression in metres from the most recent step (0 when airborne). */
        double compression = 0.0;
        /** False while the gear is retracted: the contact produces no force and reports no compression. */
        bool enabled = true;
    };

    /** Atmosphere with exponentially falling density. */
    struct Atmosphere
    {
        /** Density at the surface in kg/m^3 (sea level on Earth is 1.225). 0 disables the atmosphere. */
        double surface_density = 0.0;
        /** Altitude in metres over which density falls by a factor of e (about 8500 for Earth). */
        double scale_height = 8500.0;

        double density(double altitude) const;
    };

    /** A rigid body: vessel, planet or moon. */
    struct FlightBody
    {
        std::string name;
        FlightBodyMode mode = FlightBodyMode::dynamic;
        bool active = true;

        /** Mass without propellant in kilograms. */
        double dry_mass = 1.0;
        /** Principal moments of inertia of the dry vehicle in kg m^2 about the body axes. */
        dvec3 dry_inertia{1.0};
        /** Gravitational parameter G*M in m^3/s^2. Bodies with a value above zero attract everything else. */
        double gm = 0.0;
        /** Surface radius in metres. Above zero makes the body a ground surface and the atmosphere's base. */
        double radius = 0.0;
        /** Spin about +Y in rad/s. Drives the surface velocity used by the atmosphere and gear. */
        double spin_rate = 0.0;
        /** Unit axis the body spins about (the pole, in world axes). */
        dvec3 spin_axis{0.0, 1.0, 0.0};
        /** Turn about spin_axis in radians at time zero; with spin_rate it gives the body-fixed frame's angle at any time. */
        double spin_phase = 0.0;
        /**
         * Optional terrain for the surface of a body with a radius. Gear collides with its height and altitude
         * is measured above it. The terrain's sphere settings (radius, source) are used; its origin is ignored,
         * because the terrain follows the body. It turns with spin_phase + spin_rate * time: the world sets Terrain::rotation() itself, so a spinning planet's ground moves under the gear. Not owned; must outlive the world.
         */
        const Terrain *terrain = nullptr;
        Atmosphere atmosphere;
        /** Wind in m/s (world frame) added to the planet's rotating air, inside its atmosphere. Applies to bodies with a radius and an atmosphere. */
        dvec3 wind{0.0};
        /** When set, gives the wind (world frame, m/s) at a point from its offset from the planet's centre, instead of `wind`; lets each ship feel its own local wind. */
        std::function<dvec3(const dvec3 &offset)> wind_field;

        dvec3 position{0.0};
        dvec3 velocity{0.0};
        dquat orientation{1.0, 0.0, 0.0, 0.0};
        /** Angular velocity in the body frame in rad/s. */
        dvec3 angular_velocity{0.0};
        /** Rotational damping torque coefficient, in N m per rad/s. */
        double angular_damping = 0.0;
        /** Parasitic drag area (Cd * A) in m^2 acting along the velocity relative to the air. */
        double drag_area = 0.0;

        std::vector<PropellantTank> tanks;
        std::vector<Thruster> thrusters;
        std::vector<ThrusterGroup> groups;
        std::vector<LiftingSurface> surfaces;
        std::vector<GearContact> gear;

        /** Parent for rails mode. */
        int rails_parent = -1;
        /** Orbit for rails mode; its mean anomaly applies at rails_epoch. */
        OrbitElements rails_orbit;
        double rails_epoch = 0.0;

        /** Persistent forces and torques (world frame) applied for the next step only; see FlightWorld. */
        dvec3 force_accumulator{0.0};
        dvec3 torque_accumulator{0.0};

        /** RCS commands in -1..1 set by FlightWorld::set_rcs_rotation / set_rcs_translation. */
        dvec3 rcs_rotation{0.0};
        dvec3 rcs_translation{0.0};

        /** Dry mass plus propellant. */
        double mass() const;
        /** Moments of inertia scaled with the current mass. */
        dvec3 inertia() const;
        /** Rotation matrix from body to world. */
        dmat3 rotation() const;
        dvec3 to_world(const dvec3 &local_point) const;
        dvec3 to_local(const dvec3 &world_point) const;
        dvec3 direction_to_world(const dvec3 &local_direction) const;
        dvec3 direction_to_local(const dvec3 &world_direction) const;
    };

    /** The simulation: a set of bodies, gravity, atmosphere, ground and an integrator. */
    class FlightWorld
    {
    public:
        /** Add a body and return its id, or -1 if the limit is reached. Ids are never reused. */
        int create_body(const std::string &name = {});
        /** Deactivate a body. Its id stays valid but it is no longer simulated or attracting. */
        bool destroy_body(int id);
        /** Pointer to a body, or nullptr for an unknown id. Invalidated by create_body. */
        FlightBody *body(int id);
        const FlightBody *body(int id) const;
        int body_count() const { return static_cast<int>(bodies_.size()); }
        /** Find an active body by name, or -1. */
        int find_body(const std::string &name) const;

        /**
         * Put a body on rails around a parent (which must have a smaller id), keeping its current motion.
         * Returns false if the parent is invalid, has no gm, or the orbit is degenerate.
         */
        bool set_rails(int id, int parent);
        /** Return a rails body to dynamic mode with its current state. */
        bool release_rails(int id);

        FlightIntegrator integrator = FlightIntegrator::rk4;
        /** Largest substep in seconds. A step is split into equal substeps no larger than this. */
        double max_substep = 0.1;
        /** Cap on substeps per step() call, so extreme time warp cannot stall a frame. The substep grows instead. */
        int max_substeps_per_step = 2000;
        /** Uniform gravitational acceleration in m/s^2, applied to dynamic bodies in addition to gm sources. */
        dvec3 uniform_gravity{0.0};
        /** Flat ground at the given height with an up normal of +Y. Gear collides with it. */
        bool flat_ground = false;
        double flat_ground_height = 0.0;
        /**
         * Optional terrain used instead of the flat plane when flat_ground is set: gear contacts, altitude and the
         * flat atmosphere follow its height and slope. Not owned; must outlive the world.
         */
        const Terrain *flat_terrain = nullptr;
        /** Atmosphere above the flat ground. */
        Atmosphere flat_atmosphere;
        /** Steady wind in m/s relative to the ground. */
        dvec3 wind{0.0};

        /** Simulation time in seconds. */
        double time() const { return time_; }

        /** Advance by dt seconds (negative or non-finite values are ignored). */
        void step(double dt);

        /** Add a world-frame force at the centre of mass for the next step. */
        void apply_force(int id, const dvec3 &force);
        /** Add a world-frame force at a world-frame point for the next step. */
        void apply_force_at(int id, const dvec3 &force, const dvec3 &world_point);
        /** Add a world-frame torque for the next step. */
        void apply_torque(int id, const dvec3 &torque);

        /** Create a named thruster group. Returns false for bad indices or an unknown body. */
        bool add_group(int id, const std::string &name, const std::vector<int> &thrusters);
        /** Set the throttle of every thruster in a group (clamped to 0..1). Returns false if not found. */
        bool set_group_level(int id, const std::string &name, double level);
        /**
         * Build groups from the thruster layout: "main" (pushing forward), "retro", "hover" (pushing up),
         * translation groups "x+", "x-", "y+", "y-", "z+", "z-" and rotation groups "pitch_up", "pitch_down",
         * "yaw_left", "yaw_right", "roll_left", "roll_right". Existing groups with those names are replaced.
         */
        void build_standard_groups(int id);
        /**
         * Drive the RCS rotation groups with values from -1 to 1 (pitch up, yaw left, roll left positive). RCS
         * commands never change thrusters in the "main" or "retro" groups.
         */
        void set_rcs_rotation(int id, double pitch, double yaw, double roll);
        /** Drive the RCS translation groups with values from -1 to 1 along body X, Y and Z. */
        void set_rcs_translation(int id, double x, double y, double z);
        /** Set the control input of every surface with this control name (-1..1). */
        bool set_control(int id, const std::string &control_name, double value);
        /** Set the brake level of all gear. */
        void set_brakes(int id, double level);

        /** Gravity acceleration at a point from all sources and uniform gravity, in m/s^2. */
        dvec3 gravity_at(const dvec3 &point) const;
        /** Air density at a point in kg/m^3. */
        double air_density_at(const dvec3 &point) const;
        /** Altitude above the nearest ground (flat ground or the planet with the lowest altitude). */
        double altitude_of(const dvec3 &point) const;
        /** Velocity of the air at a point (planet spin plus wind). */
        dvec3 air_velocity_at(const dvec3 &point) const;
        /** Total orbital energy per unit mass of a body about the gm sources: kinetic plus potential. */
        double specific_energy(int id) const;
        /** Elements of a body relative to a parent, using the parent's gm. */
        bool orbit_of(int id, int parent, OrbitElements &out) const;
        /** Position of a body relative to another, double precision. */
        dvec3 relative_position(int id, int reference) const;
        /** Positions of a body over the next `duration` seconds from a copy of the world, for drawing. */
        std::vector<dvec3> predict_path(int id, double duration, int samples) const;
        /** Model matrix (float) for rendering, with the translation taken relative to `origin`. */
        glm::mat4 render_matrix(int id, const dvec3 &origin) const;

    private:
        struct Kinematics
        {
            dvec3 position{0.0};
            dvec3 velocity{0.0};
            dquat orientation{1.0, 0.0, 0.0, 0.0};
            dvec3 angular_velocity{0.0};
        };

        void pack(std::vector<double> &y) const;
        void unpack(const std::vector<double> &y);
        void derivative(double t, const std::vector<double> &y, std::vector<double> &dy);
        void evaluate_kinematics(double t, const std::vector<double> &y);
        void update_rails(double t);
        void substep(double h);
        void deplete_propellant(double h);
        dvec3 gravity_from(const std::vector<Kinematics> &kin, const dvec3 &point, int skip) const;
        void air_from(const std::vector<Kinematics> &kin, const dvec3 &point, double &density, dvec3 &velocity) const;
        double altitude_from(const std::vector<Kinematics> &kin, const dvec3 &point) const;
        void body_loads(int index, const std::vector<Kinematics> &kin, dvec3 &force, dvec3 &torque_body, bool record);
        std::vector<Kinematics> master_kinematics() const;
        void apply_rcs(FlightBody &body);

        std::vector<FlightBody> bodies_;
        std::vector<Kinematics> kin_;
        std::vector<int> slot_;
        std::vector<std::vector<double>> stage_;
        double time_ = 0.0;
    };

    /** Thrust in newtons a thruster currently produces at the given air density (0 when its tank is empty). */
    double thruster_output(const FlightBody &body, const Thruster &thruster, double air_density);
}

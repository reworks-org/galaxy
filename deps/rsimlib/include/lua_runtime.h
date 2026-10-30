#pragma once

#include <functional>
#include <cstddef>
#include <memory>
#include <string>

namespace sol
{
    class state;
}

namespace sl
{
    /** Result of executing a Lua chunk or dispatching a Lua callback. */
    struct LuaScriptResult
    {
        /** True when Lua completed without an error. */
        bool success = false;
        /** Error message; empty on success. */
        std::string error;
    };

    /** Resource limits applied to scripts; a limit of zero disables that check. */
    struct LuaLimits
    {
        /** Maximum Lua VM instructions per execute() or callback call. */
        unsigned long long max_instructions = 100'000'000;
        /** Maximum bytes the Lua state may allocate. */
        std::size_t max_memory_bytes = 256u * 1024u * 1024u;
    };

    /** Owns a sandboxed Lua state and provides script execution and callbacks. */
    class LuaRuntime
    {
    public:
        /** Allocation and instruction accounting for the Lua state (internal). */
        struct Budget;

        /** Receives output emitted by Lua print calls. */
        using OutputHandler = std::function<void(const std::string &)>;

        /** Construct an uninitialised runtime. */
        LuaRuntime();
        /** Destroy the Lua state and its registered resources. */
        ~LuaRuntime();

        LuaRuntime(const LuaRuntime &) = delete;
        LuaRuntime &operator=(const LuaRuntime &) = delete;

        /** Set the resource limits; takes effect for the current state and any later initialise(). */
        void set_limits(const LuaLimits &limits);
        /** Return the active resource limits. */
        const LuaLimits &limits() const;
        /** Create the Lua state and register simlib's script bindings. */
        void initialise(OutputHandler output_handler = {});
        /** Return whether the Lua state has been initialised. */
        bool is_initialised() const;
        /** Execute a Lua source string and capture any runtime error. */
        LuaScriptResult execute(const std::string &script);
        /** Invoke an optional global Lua callback with a string argument. */
        LuaScriptResult call(const std::string &function_name, const std::string &argument);
        /** Emit an optional Lua callback with no arguments. */
        LuaScriptResult emit(const std::string &event_name);
        /** Emit an optional Lua callback with an integer argument. */
        LuaScriptResult emit(const std::string &event_name, int value);
        /** Emit an optional Lua callback with a floating-point argument. */
        LuaScriptResult emit(const std::string &event_name, float value);
        /** Emit an optional Lua callback with a boolean argument. */
        LuaScriptResult emit(const std::string &event_name, bool value);
        /** Emit an optional Lua callback with a string argument. */
        LuaScriptResult emit(const std::string &event_name, const std::string &value);
        /** Emit an optional Lua callback with a named two-dimensional position. */
        LuaScriptResult emit(const std::string &event_name, const std::string &name, float x, float y);
        /** Access the underlying sol2 state for application-defined bindings. */
        sol::state &state();
        /** Destroy the current state and return the runtime to an uninitialised state. */
        void reset();

    private:
        std::unique_ptr<Budget> budget_;
        std::unique_ptr<sol::state> state_;
        OutputHandler output_handler_;
    };
}
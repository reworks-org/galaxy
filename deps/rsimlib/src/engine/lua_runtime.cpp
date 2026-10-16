/** @file
 * @brief Implements the restricted Lua runtime and callback dispatch.
 */

#include "lua_runtime.h"

#include <sol/sol.hpp>

#include <ctime>
#include <utility>

namespace sl
{
    namespace
    {
        LuaScriptResult script_error(sol::protected_function_result result)
        {
            sol::error error = result;
            std::string message = error.what();
            const std::size_t traceback_position = message.find("\nstack traceback:");
            if (traceback_position != std::string::npos)
            {
                message.resize(traceback_position);
            }
            return {false, std::move(message)};
        }

        template <typename... Arguments>
        LuaScriptResult invoke_callback(sol::state &state, const std::string &event_name, Arguments &&...arguments)
        {
            sol::object callback = state[event_name];
            if (!callback.valid() || callback.get_type() == sol::type::nil)
            {
                return {true, {}};
            }
            if (!callback.is<sol::protected_function>())
            {
                return {false, "Lua callback '" + event_name + "' is not a function."};
            }

            sol::protected_function_result result =
                callback.as<sol::protected_function>()(std::forward<Arguments>(arguments)...);
            return result.valid() ? LuaScriptResult{true, {}} : script_error(std::move(result));
        }
    }

    LuaRuntime::LuaRuntime() = default;

    LuaRuntime::~LuaRuntime() = default;

    void LuaRuntime::initialise(OutputHandler output_handler)
    {
        if (state_)
        {
            return;
        }

        output_handler_ = std::move(output_handler);
        state_ = std::make_unique<sol::state>();
        state_->open_libraries(
            sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);
        (*state_)["dofile"] = sol::nil;
        (*state_)["loadfile"] = sol::nil;
        (*state_)["load"] = sol::nil;

        sol::table os_table = state_->create_named_table("os");
        os_table.set_function("time", []()
                              { return static_cast<lua_Integer>(std::time(nullptr)); });
        os_table.set_function("clock", []()
                              { return static_cast<double>(std::clock()) / CLOCKS_PER_SEC; });
        state_->create_named_table("app");

        state_->set_function("print", [this](sol::this_state this_state, sol::variadic_args args)
                             {
            if (!output_handler_)
            {
                return;
            }

            lua_State *lua = this_state;
            std::string line;
            for (auto argument : args)
            {
                if (!line.empty())
                {
                    line += '\t';
                }
                std::size_t length = 0;
                const char *text = luaL_tolstring(lua, argument.stack_index(), &length);
                line.append(text, length);
                lua_pop(lua, 1);
            }
            output_handler_(line); });
    }

    bool LuaRuntime::is_initialised() const
    {
        return state_ != nullptr;
    }

    LuaScriptResult LuaRuntime::execute(const std::string &script)
    {
        if (!state_)
        {
            return {false, "Lua runtime has not been initialised."};
        }

        sol::protected_function_result result =
            state_->safe_script(script, sol::script_pass_on_error);
        if (result.valid())
        {
            return {true, {}};
        }

        return script_error(std::move(result));
    }

    LuaScriptResult LuaRuntime::call(const std::string &function_name, const std::string &argument)
    {
        return emit(function_name, argument);
    }

    LuaScriptResult LuaRuntime::emit(const std::string &event_name)
    {
        if (!state_)
        {
            return {false, "Lua runtime has not been initialised."};
        }
        return invoke_callback(*state_, event_name);
    }

    LuaScriptResult LuaRuntime::emit(const std::string &event_name, int value)
    {
        if (!state_)
        {
            return {false, "Lua runtime has not been initialised."};
        }
        return invoke_callback(*state_, event_name, value);
    }

    LuaScriptResult LuaRuntime::emit(const std::string &event_name, float value)
    {
        if (!state_)
        {
            return {false, "Lua runtime has not been initialised."};
        }
        return invoke_callback(*state_, event_name, value);
    }

    LuaScriptResult LuaRuntime::emit(const std::string &event_name, bool value)
    {
        if (!state_)
        {
            return {false, "Lua runtime has not been initialised."};
        }
        return invoke_callback(*state_, event_name, value);
    }

    LuaScriptResult LuaRuntime::emit(const std::string &event_name, const std::string &value)
    {
        if (!state_)
        {
            return {false, "Lua runtime has not been initialised."};
        }
        return invoke_callback(*state_, event_name, value);
    }

    LuaScriptResult LuaRuntime::emit(const std::string &event_name, const std::string &name, float x, float y)
    {
        if (!state_)
        {
            return {false, "Lua runtime has not been initialised."};
        }
        return invoke_callback(*state_, event_name, name, x, y);
    }

    sol::state &LuaRuntime::state()
    {
        return *state_;
    }

    void LuaRuntime::reset()
    {
        state_.reset();
        output_handler_ = {};
    }
}
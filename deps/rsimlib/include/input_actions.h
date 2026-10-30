#pragma once

#include "display.h"
#include "gamepad.h"
#include "input.h"

#include <SDL2/SDL.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace sl
{
    /**
     * Optional action mapping. Give game actions names ("jump", "move_left") and bind any mix of keys, mouse buttons,
     * gamepad buttons, gamepad axes and touch to each. Nothing else in simlib depends on it: raw key_down(),
     * mouse_buttons() and gamepad_*() keep working, and you can use an InputMap for only some of your input.
     *
     * It polls, so no event hookup is needed. Call update() once per frame, then query value(), down(), pressed(),
     * released(), axis() or vector(). Bindings can be saved to and loaded from text for a remapping screen.
     */
    struct InputBinding
    {
        enum class Kind
        {
            key,
            mouse_button,
            gamepad_button,
            gamepad_axis,
            touch
        };

        Kind kind = Kind::key;

        /** True when this is a keyboard binding for `key`. */
        bool is_key(Key key) const { return kind == Kind::key && code == key_scancode(key); }
        /** Backend key code, MouseButton, GamepadButton or GamepadAxis value, depending on kind. */
        int code = 0;
        /** For gamepad axes: +1 or -1, the half of the axis that triggers the action. */
        int direction = 1;
        /** Gamepad index, or -1 for any connected gamepad. */
        int gamepad = -1;
    };

    class InputMap
    {
    public:
        /** Fraction of full travel at which an analogue action counts as down. */
        float press_threshold = 0.5f;

        void bind_key(const std::string &action, Key key)
        {
            add(action, {InputBinding::Kind::key, key_scancode(key), 1, -1});
        }
        void bind_mouse(const std::string &action, MouseButton button)
        {
            add(action, {InputBinding::Kind::mouse_button, static_cast<int>(button), 1, -1});
        }
        void bind_gamepad_button(const std::string &action, GamepadButton button, int gamepad = -1)
        {
            add(action, {InputBinding::Kind::gamepad_button, static_cast<int>(button), 1, gamepad});
        }
        /** Bind one direction of an axis, e.g. (left_x, -1) for "move_left". Triggers use direction +1. */
        void bind_gamepad_axis(const std::string &action, GamepadAxis axis, int direction, int gamepad = -1)
        {
            add(action, {InputBinding::Kind::gamepad_axis, static_cast<int>(axis), direction < 0 ? -1 : 1, gamepad});
        }
        /** Any finger touching the screen. */
        void bind_touch(const std::string &action)
        {
            add(action, {InputBinding::Kind::touch, 0, 1, -1});
        }
        void bind(const std::string &action, const InputBinding &binding)
        {
            add(action, binding);
        }

        /** Remove every binding of one action (its name stays registered). */
        void clear_bindings(const std::string &action)
        {
            const auto found = actions_.find(action);
            if (found != actions_.end())
                found->second.bindings.clear();
        }
        /** Forget an action completely. */
        void remove_action(const std::string &action) { actions_.erase(action); }
        void clear() { actions_.clear(); }

        bool has_action(const std::string &action) const { return actions_.count(action) != 0; }
        const std::vector<InputBinding> &bindings(const std::string &action) const
        {
            static const std::vector<InputBinding> none;
            const auto found = actions_.find(action);
            return found == actions_.end() ? none : found->second.bindings;
        }
        std::vector<std::string> actions() const
        {
            std::vector<std::string> names;
            for (const auto &[name, state] : actions_)
                names.push_back(name);
            return names;
        }

        /**
         * Virtual input for this frame's update(): touch buttons drawn on screen, AI, replays or tests. The strongest of
         * the real and injected values wins. Injections are cleared by update().
         */
        void inject(const std::string &action, float value = 1.0f)
        {
            const auto found = actions_.find(action);
            if (found != actions_.end())
                found->second.injected = std::max(found->second.injected, std::clamp(value, 0.0f, 1.0f));
        }

        /** Sample all devices. Call once per frame, before querying actions. */
        void update()
        {
            for (auto &[name, state] : actions_)
            {
                float strongest = state.injected;
                for (const InputBinding &binding : state.bindings)
                    strongest = std::max(strongest, sample(binding));
                state.injected = 0.0f;
                state.was_down = state.is_down;
                state.value = strongest;
                // Hysteresis keeps a stick resting near the threshold from chattering between down and up.
                const float release_level = press_threshold * 0.8f;
                state.is_down = state.is_down ? strongest > release_level : strongest >= press_threshold;
            }
        }

        /** Current strength, 0 to 1. Digital inputs give 0 or 1. */
        float value(const std::string &action) const { return state_of(action).value; }
        bool down(const std::string &action) const { return state_of(action).is_down; }
        /** True only on the frame the action went down. */
        bool pressed(const std::string &action) const
        {
            const State &state = state_of(action);
            return state.is_down && !state.was_down;
        }
        /** True only on the frame the action went up. */
        bool released(const std::string &action) const
        {
            const State &state = state_of(action);
            return !state.is_down && state.was_down;
        }

        /** value(positive) - value(negative), from -1 to 1. */
        float axis(const std::string &negative, const std::string &positive) const
        {
            return value(positive) - value(negative);
        }
        /** Two axes as a vector whose length never exceeds 1, so diagonals are not faster. */
        void vector(const std::string &left, const std::string &right, const std::string &up, const std::string &down_action,
                    float &x, float &y) const
        {
            x = axis(left, right);
            y = axis(up, down_action);
            const float length = std::sqrt(x * x + y * y);
            if (length > 1.0f)
            {
                x /= length;
                y /= length;
            }
        }

        /**
         * For a remapping screen: returns the first input currently held (keyboard, mouse, gamepad button, or a stick
         * pushed past half way), or nothing. Call each frame while waiting, then bind() the result.
         */
        static std::optional<InputBinding> capture()
        {
            int key_count = 0;
            const Uint8 *keys = SDL_GetKeyboardState(&key_count);
            for (int scancode = 0; keys && scancode < key_count; ++scancode)
            {
                if (keys[scancode])
                    return InputBinding{InputBinding::Kind::key, scancode, 1, -1};
            }
            for (int button = 1; button <= 5; ++button)
            {
                if (mouse_button_down(button))
                    return InputBinding{InputBinding::Kind::mouse_button, button, 1, -1};
            }
            for (int pad = 0; pad < gamepad_count(); ++pad)
            {
                for (int button = 0; button <= static_cast<int>(GamepadButton::dpad_right); ++button)
                {
                    if (gamepad_button(pad, static_cast<GamepadButton>(button)))
                        return InputBinding{InputBinding::Kind::gamepad_button, button, 1, pad};
                }
                for (int axis_index = 0; axis_index <= static_cast<int>(GamepadAxis::right_trigger); ++axis_index)
                {
                    const float amount = gamepad_axis(pad, static_cast<GamepadAxis>(axis_index));
                    if (std::fabs(amount) > 0.5f)
                        return InputBinding{InputBinding::Kind::gamepad_axis, axis_index, amount < 0.0f ? -1 : 1, pad};
                }
            }
            return std::nullopt;
        }

        /** A short label such as "Space", "Mouse 1", "Pad South" or "Pad Left X-" for showing in a UI. */
        static std::string describe(const InputBinding &binding)
        {
            switch (binding.kind)
            {
            case InputBinding::Kind::key:
            {
                const char *name = SDL_GetScancodeName(static_cast<SDL_Scancode>(binding.code));
                return name && *name ? name : "Key " + std::to_string(binding.code);
            }
            case InputBinding::Kind::mouse_button:
                return "Mouse " + std::to_string(binding.code);
            case InputBinding::Kind::gamepad_button:
                return std::string("Pad ") + button_names()[std::clamp(binding.code, 0, button_count - 1)];
            case InputBinding::Kind::gamepad_axis:
                return std::string("Pad ") + axis_names()[std::clamp(binding.code, 0, axis_count - 1)] +
                       (binding.direction < 0 ? "-" : "+");
            case InputBinding::Kind::touch:
                return "Touch";
            }
            return {};
        }

        /**
         * Text form for settings files: one line per action, "name<TAB>binding;binding". Bindings read key:Space,
         * mouse:1, pad:south, axis:left_x- or touch, with an optional @N suffix to pin a gamepad.
         */
        std::string save() const
        {
            std::ostringstream out;
            for (const auto &[name, state] : actions_)
            {
                out << name << '\t';
                for (std::size_t i = 0; i < state.bindings.size(); ++i)
                    out << (i ? ";" : "") << encode(state.bindings[i]);
                out << '\n';
            }
            return out.str();
        }

        /** Replace all actions with those in `text`. Returns false (leaving the map unchanged) on a malformed line. */
        bool load(const std::string &text)
        {
            std::map<std::string, State> loaded;
            std::istringstream in(text);
            std::string line;
            while (std::getline(in, line))
            {
                if (!line.empty() && line.back() == '\r')
                    line.pop_back();
                if (line.empty())
                    continue;
                const std::size_t tab = line.find('\t');
                if (tab == std::string::npos || tab == 0)
                    return false;
                State &state = loaded[line.substr(0, tab)];
                std::istringstream list(line.substr(tab + 1));
                std::string item;
                while (std::getline(list, item, ';'))
                {
                    if (item.empty())
                        continue;
                    InputBinding binding;
                    if (!decode(item, binding))
                        return false;
                    state.bindings.push_back(binding);
                }
            }
            actions_ = std::move(loaded);
            return true;
        }

    private:
        struct State
        {
            std::vector<InputBinding> bindings;
            float value = 0.0f;
            float injected = 0.0f;
            bool is_down = false;
            bool was_down = false;
        };

        static constexpr int button_count = static_cast<int>(GamepadButton::dpad_right) + 1;
        static constexpr int axis_count = static_cast<int>(GamepadAxis::right_trigger) + 1;

        static const char *const *button_names()
        {
            static const char *const names[button_count] = {"south", "east", "west", "north", "back", "guide", "start",
                                                            "left_stick", "right_stick", "left_shoulder", "right_shoulder",
                                                            "dpad_up", "dpad_down", "dpad_left", "dpad_right"};
            return names;
        }
        static const char *const *axis_names()
        {
            static const char *const names[axis_count] = {"left_x", "left_y", "right_x", "right_y", "left_trigger",
                                                          "right_trigger"};
            return names;
        }

        std::map<std::string, State> actions_;

        void add(const std::string &action, const InputBinding &binding)
        {
            if (!action.empty())
                actions_[action].bindings.push_back(binding);
        }

        const State &state_of(const std::string &action) const
        {
            static const State none;
            const auto found = actions_.find(action);
            return found == actions_.end() ? none : found->second;
        }

        static float sample(const InputBinding &binding)
        {
            switch (binding.kind)
            {
            case InputBinding::Kind::key:
                return key_down(static_cast<SDL_Scancode>(binding.code)) ? 1.0f : 0.0f;
            case InputBinding::Kind::mouse_button:
                return mouse_button_down(binding.code) ? 1.0f : 0.0f;
            case InputBinding::Kind::gamepad_button:
                return any_pad(binding, [&](int pad)
                               { return gamepad_button(pad, static_cast<GamepadButton>(binding.code)) ? 1.0f : 0.0f; });
            case InputBinding::Kind::gamepad_axis:
                return any_pad(binding, [&](int pad)
                               { return std::max(0.0f, gamepad_axis(pad, static_cast<GamepadAxis>(binding.code)) *
                                                           static_cast<float>(binding.direction)); });
            case InputBinding::Kind::touch:
            {
                for (int device = 0; device < SDL_GetNumTouchDevices(); ++device)
                {
                    if (SDL_GetNumTouchFingers(SDL_GetTouchDevice(device)) > 0)
                        return 1.0f;
                }
                return 0.0f;
            }
            }
            return 0.0f;
        }

        template <typename Reader>
        static float any_pad(const InputBinding &binding, Reader reader)
        {
            if (binding.gamepad >= 0)
                return reader(binding.gamepad);
            float strongest = 0.0f;
            for (int pad = 0; pad < gamepad_count(); ++pad)
                strongest = std::max(strongest, reader(pad));
            return strongest;
        }

        static std::string encode(const InputBinding &binding)
        {
            std::string text;
            switch (binding.kind)
            {
            case InputBinding::Kind::key:
            {
                const char *name = SDL_GetScancodeName(static_cast<SDL_Scancode>(binding.code));
                text = "key:" + std::string(name ? name : "");
                break;
            }
            case InputBinding::Kind::mouse_button:
                text = "mouse:" + std::to_string(binding.code);
                break;
            case InputBinding::Kind::gamepad_button:
                text = std::string("pad:") + button_names()[std::clamp(binding.code, 0, button_count - 1)];
                break;
            case InputBinding::Kind::gamepad_axis:
                text = std::string("axis:") + axis_names()[std::clamp(binding.code, 0, axis_count - 1)] +
                       (binding.direction < 0 ? "-" : "+");
                break;
            case InputBinding::Kind::touch:
                text = "touch";
                break;
            }
            if (binding.gamepad >= 0 && (binding.kind == InputBinding::Kind::gamepad_button ||
                                         binding.kind == InputBinding::Kind::gamepad_axis))
                text += "@" + std::to_string(binding.gamepad);
            return text;
        }

        static bool decode(std::string item, InputBinding &out)
        {
            out = InputBinding{};
            const std::size_t at = item.rfind('@');
            if (at != std::string::npos)
            {
                char *end = nullptr;
                const long pad = std::strtol(item.c_str() + at + 1, &end, 10);
                if (end == item.c_str() + at + 1 || *end != '\0' || pad < 0)
                    return false;
                out.gamepad = static_cast<int>(pad);
                item.resize(at);
            }
            if (item == "touch")
            {
                out.kind = InputBinding::Kind::touch;
                return true;
            }
            const std::size_t colon = item.find(':');
            if (colon == std::string::npos)
                return false;
            const std::string type = item.substr(0, colon);
            std::string name = item.substr(colon + 1);
            if (type == "key")
            {
                const SDL_Scancode scancode = SDL_GetScancodeFromName(name.c_str());
                if (scancode == SDL_SCANCODE_UNKNOWN)
                    return false;
                out.kind = InputBinding::Kind::key;
                out.code = static_cast<int>(scancode);
                return true;
            }
            if (type == "mouse")
            {
                const int button = std::atoi(name.c_str());
                if (button < 1 || button > 5)
                    return false;
                out.kind = InputBinding::Kind::mouse_button;
                out.code = button;
                return true;
            }
            if (type == "pad")
            {
                for (int i = 0; i < button_count; ++i)
                {
                    if (name == button_names()[i])
                    {
                        out.kind = InputBinding::Kind::gamepad_button;
                        out.code = i;
                        return true;
                    }
                }
                return false;
            }
            if (type == "axis" && !name.empty() && (name.back() == '+' || name.back() == '-'))
            {
                out.direction = name.back() == '-' ? -1 : 1;
                name.pop_back();
                for (int i = 0; i < axis_count; ++i)
                {
                    if (name == axis_names()[i])
                    {
                        out.kind = InputBinding::Kind::gamepad_axis;
                        out.code = i;
                        return true;
                    }
                }
            }
            return false;
        }
    };

    /** Convert a touch finger position (0-1 in each axis, as SDL reports it) to screen coordinates. */
    inline void touch_to_screen(float normalized_x, float normalized_y, float &screen_x, float &screen_y)
    {
        screen_x = normalized_x * static_cast<float>(virtual_screen_width());
        screen_y = normalized_y * static_cast<float>(virtual_screen_height());
    }

    /** Screen positions of every finger currently touching, for on-screen controls. */
    inline std::vector<std::pair<float, float>> touch_points()
    {
        std::vector<std::pair<float, float>> points;
        for (int device = 0; device < SDL_GetNumTouchDevices(); ++device)
        {
            const SDL_TouchID id = SDL_GetTouchDevice(device);
            for (int finger = 0; finger < SDL_GetNumTouchFingers(id); ++finger)
            {
                if (const SDL_Finger *state = SDL_GetTouchFinger(id, finger))
                {
                    float x = 0.0f, y = 0.0f;
                    touch_to_screen(state->x, state->y, x, y);
                    points.emplace_back(x, y);
                }
            }
        }
        return points;
    }
}

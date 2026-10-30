/** @file
 * @brief Lua bindings for the property tree.
 */

#include "lua_runtime.h"
#include "proptree.h"

#include <sol/sol.hpp>

#include <map>
#include <string>

namespace sl
{
    namespace
    {
        sol::object read_value(sol::state_view lua, const PropertyNode &node)
        {
            switch (node.type())
            {
            case PropertyNode::Type::boolean: return sol::make_object(lua, node.get_bool());
            case PropertyNode::Type::integer: return sol::make_object(lua, node.get_int());
            case PropertyNode::Type::real: return sol::make_object(lua, node.get_double());
            case PropertyNode::Type::string: return sol::make_object(lua, node.get_string());
            default: return sol::nil;
            }
        }

        void write_value(PropertyNode &node, const sol::object &value)
        {
            switch (value.get_type())
            {
            case sol::type::boolean: node.set_bool(value.as<bool>()); break;
            case sol::type::number:
                if (value.is<int>() && value.as<double>() == static_cast<double>(value.as<int>()) && node.type() != PropertyNode::Type::real)
                    node.set_int(value.as<int>());
                else
                    node.set_double(value.as<double>());
                break;
            case sol::type::string: node.set_string(value.as<std::string>()); break;
            default: break;
            }
        }

        // Owns the listeners Lua registered. Its destructor runs when the Lua state closes, so listeners never
        // outlive the functions they call.
        struct Binding
        {
            PropertyTree *tree = nullptr;
            sol::state *state = nullptr;
            std::map<PropertyNode::ListenerId, std::string> listeners;

            ~Binding()
            {
                for (const auto &[id, path] : listeners)
                    if (PropertyNode *node = tree->find(path))
                        node->remove_listener(id);
            }

            PropertyNode::ListenerId listen(PropertyNode &node, sol::protected_function callback)
            {
                sol::table store = (*state)["__props_listeners"];
                sol::state *lua = state;
                std::uint64_t *serial = &next_key;
                const std::uint64_t key = (*serial)++;
                store[key] = callback;
                const PropertyNode::ListenerId id = node.add_listener([lua, key](PropertyNode &changed) {
                    sol::table functions = (*lua)["__props_listeners"];
                    sol::protected_function function = functions[key];
                    if (function.valid())
                        function(changed.path(), read_value(*lua, changed));
                });
                listeners[id] = node.path();
                keys[id] = key;
                return id;
            }

            void unlisten(PropertyNode::ListenerId id)
            {
                const auto found = listeners.find(id);
                if (found == listeners.end())
                    return;
                if (PropertyNode *node = tree->find(found->second))
                    node->remove_listener(id);
                sol::table store = (*state)["__props_listeners"];
                store[keys[id]] = sol::nil;
                keys.erase(id);
                listeners.erase(found);
            }

            std::uint64_t next_key = 1;
            std::map<PropertyNode::ListenerId, std::uint64_t> keys;
        };
    } // namespace

    void bind_property_tree(LuaRuntime &runtime, PropertyTree &tree)
    {
        sol::state &lua = runtime.state();
        lua["__props_listeners"] = lua.create_table();
        lua.new_usertype<Binding>("PropertyBinding", sol::no_constructor);
        lua["__props_binding"] = std::make_unique<Binding>();
        Binding &binding = *lua["__props_binding"].get<Binding *>();
        binding.tree = &tree;
        binding.state = &lua;

        lua.new_usertype<PropertyNode>(
            "PropertyNode", sol::no_constructor,
            "value", [](sol::this_state s, PropertyNode &node) { return read_value(s, node); },
            "set", [](PropertyNode &node, sol::object value) { write_value(node, value); },
            "path", &PropertyNode::path,
            "name", [](const PropertyNode &node) { return node.name(); },
            "index", &PropertyNode::index,
            "child", [](PropertyNode &node, const std::string &name, sol::optional<int> index) { return node.child(name, index.value_or(0)); },
            "children", [](sol::this_state s, PropertyNode &node) {
                sol::table list = sol::state_view(s).create_table();
                for (PropertyNode *child : node.children())
                    list.add(child->index() == 0 ? child->name() : child->name() + "[" + std::to_string(child->index()) + "]");
                return list;
            },
            "on_change", [&binding](PropertyNode &node, sol::protected_function callback) { return binding.listen(node, callback); });

        sol::table props = lua.create_named_table("props");
        props.set_function("get", [&tree](sol::this_state s, const std::string &path) -> sol::object {
            const PropertyNode *node = tree.find(path);
            return node ? read_value(s, *node) : sol::object(sol::nil);
        });
        props.set_function("set", [&tree](const std::string &path, sol::object value) {
            if (PropertyNode *node = tree.node(path))
                write_value(*node, value);
        });
        props.set_function("exists", [&tree](const std::string &path) { return tree.exists(path); });
        props.set_function("node", [&tree](const std::string &path) { return tree.node(path); });
        props.set_function("children", [&tree](sol::this_state s, const std::string &path) {
            sol::table list = sol::state_view(s).create_table();
            if (const PropertyNode *node = tree.find(path))
                for (const PropertyNode *child : node->children())
                    list.add(child->index() == 0 ? child->name() : child->name() + "[" + std::to_string(child->index()) + "]");
            return list;
        });
        props.set_function("listen", [&tree, &binding](const std::string &path, sol::protected_function callback) -> sol::optional<std::uint64_t> {
            PropertyNode *node = tree.node(path);
            if (!node)
                return sol::nullopt;
            return binding.listen(*node, callback);
        });
        props.set_function("unlisten", [&binding](std::uint64_t id) { binding.unlisten(id); });
    }

} // namespace sl

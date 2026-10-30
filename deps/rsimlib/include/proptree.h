#pragma once

#include <nlohmann/json.hpp>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace sl
{
    class PropertyTree;
    class LuaRuntime;

    /**
     * A node in a PropertyTree, modelled on FlightGear's property tree. A node has a name and an index (so several
     * siblings can share a name, as in `/engines/engine[1]/thrust`), any number of children, and optionally a value.
     * Values are bool, int, double or string and convert between each other on read. A node can instead be tied to
     * C++ state through getter and setter callbacks, or be an alias for another node. Listeners are called when a
     * value changes. Pointers to nodes stay valid until remove_child() or clear() destroys them.
     */
    class PropertyNode
    {
    public:
        enum class Type
        {
            /** No value (a pure branch, or never written). */
            none,
            boolean,
            integer,
            real,
            string
        };

        /** Called after the node's value has changed, with the node that changed (the alias target for aliases). */
        using Listener = std::function<void(PropertyNode &)>;
        using ListenerId = std::uint64_t;

        PropertyNode(const PropertyNode &) = delete;
        PropertyNode &operator=(const PropertyNode &) = delete;

        const std::string &name() const { return name_; }
        int index() const { return index_; }
        PropertyNode *parent() const { return parent_; }
        /** Absolute path such as `/ship/engines/engine[1]/throttle`. */
        std::string path() const;

        /** Find or create the child `name[index]`. Returns nullptr only if the name is invalid. */
        PropertyNode *child(std::string_view name, int index = 0);
        /** Find a child without creating it. */
        PropertyNode *find_child(std::string_view name, int index = 0) const;
        /** Find or create a descendant by relative path, for example `engines/engine[1]/throttle`. */
        PropertyNode *node(std::string_view path);
        /** Find a descendant by relative path without creating anything. */
        PropertyNode *find(std::string_view path) const;
        /** All children in creation order. */
        std::vector<PropertyNode *> children() const;
        /** The children called `name`, ordered by index. */
        std::vector<PropertyNode *> children(std::string_view name) const;
        /** How many children are called `name`. */
        int child_count(std::string_view name) const;
        /** Remove a child and everything below it. Listeners on removed nodes are dropped. */
        bool remove_child(std::string_view name, int index = 0);
        bool has_children() const { return !children_.empty(); }

        // The value API follows the target of an alias, and the tied callbacks of a tied node.
        Type type() const;
        bool has_value() const { return type() != Type::none; }
        bool get_bool(bool fallback = false) const;
        int get_int(int fallback = 0) const;
        double get_double(double fallback = 0.0) const;
        std::string get_string(const std::string &fallback = {}) const;
        /** Set the value, converting to the node's existing type if it has one; fires listeners if it changed. */
        void set_bool(bool value);
        void set_int(int value);
        void set_double(double value);
        void set_string(const std::string &value);
        /** Set a value from text, parsing it as the node's existing type (or guessing bool/int/double/string if none). */
        void set_from_string(const std::string &text);

        /** Tie the node to C++ state. The setter may be empty for a read-only property. Replaces any stored value. */
        void tie_bool(std::function<bool()> getter, std::function<void(bool)> setter = {});
        void tie_int(std::function<int()> getter, std::function<void(int)> setter = {});
        void tie_double(std::function<double()> getter, std::function<void(double)> setter = {});
        void tie_string(std::function<std::string()> getter, std::function<void(const std::string &)> setter = {});
        /** Tie to a variable owned by the caller, which must outlive the node. */
        void tie(double *variable);
        void tie(int *variable);
        void tie(bool *variable);
        /** Remove a tie, keeping the current value as a plain stored value. */
        void untie();
        bool is_tied() const;
        bool is_read_only() const;

        /** Make this node an alias for `target` (which must not be this node or an alias leading back to it). */
        bool set_alias(PropertyNode *target);
        void clear_alias() { alias_ = nullptr; }
        PropertyNode *alias() const { return alias_; }

        /** Call `listener` whenever this node's value changes. */
        ListenerId add_listener(Listener listener);
        /** Like add_listener(), but also called for every change below this node. */
        ListenerId add_subtree_listener(Listener listener);
        void remove_listener(ListenerId id);

        /** A tied node cannot detect changes made behind its back; call this after one to notify listeners. */
        void notify();

    private:
        friend class PropertyTree;
        PropertyNode(std::string name, int index, PropertyNode *parent);

        struct Tie
        {
            Type type = Type::none;
            std::function<bool()> get_bool;
            std::function<int()> get_int;
            std::function<double()> get_double;
            std::function<std::string()> get_string;
            std::function<void(bool)> set_bool;
            std::function<void(int)> set_int;
            std::function<void(double)> set_double;
            std::function<void(const std::string &)> set_string;
            bool writable = false;
        };
        struct ListenerEntry
        {
            ListenerId id;
            Listener callback;
            bool subtree;
        };

        PropertyNode *target();
        const PropertyNode *target() const;
        void fire(PropertyNode &changed);
        void assign(const std::variant<std::monostate, bool, int, double, std::string> &value);
        std::variant<std::monostate, bool, int, double, std::string> read() const;
        void set_tie(Tie tie);

        std::string name_;
        int index_ = 0;
        PropertyNode *parent_ = nullptr;
        PropertyTree *tree_ = nullptr;
        std::vector<std::unique_ptr<PropertyNode>> children_;
        std::variant<std::monostate, bool, int, double, std::string> value_;
        std::unique_ptr<Tie> tie_;
        PropertyNode *alias_ = nullptr;
        std::vector<ListenerEntry> listeners_;
    };

    /**
     * The root of a property tree. This is the shared state interface between C++ systems and Lua scripts: C++ code
     * publishes state by tying nodes, scripts read and write the same paths (see bind_property_tree()). Single
     * threaded: use it from the simulation thread only.
     */
    class PropertyTree
    {
    public:
        PropertyTree();
        ~PropertyTree();
        PropertyTree(const PropertyTree &) = delete;
        PropertyTree &operator=(const PropertyTree &) = delete;

        PropertyNode &root() { return *root_; }
        const PropertyNode &root() const { return *root_; }

        /** Find or create the node at an absolute path (leading '/' optional). Returns nullptr for an invalid path. */
        PropertyNode *node(std::string_view path);
        /** Find a node without creating anything. */
        PropertyNode *find(std::string_view path) const;
        bool exists(std::string_view path) const { return find(path) != nullptr; }

        bool get_bool(std::string_view path, bool fallback = false) const;
        int get_int(std::string_view path, int fallback = 0) const;
        double get_double(std::string_view path, double fallback = 0.0) const;
        std::string get_string(std::string_view path, const std::string &fallback = {}) const;
        void set_bool(std::string_view path, bool value);
        void set_int(std::string_view path, int value);
        void set_double(std::string_view path, double value);
        void set_string(std::string_view path, const std::string &value);

        /** Remove everything below the root. */
        void clear();

        /**
         * Convert the subtree at `path` (default the whole tree) to JSON. Leaves become values, branches objects, and
         * siblings sharing a name (or with a non-zero index) become arrays. Aliases and read-only ties are skipped
         * unless `include_ties` is true.
         */
        nlohmann::json to_json(std::string_view path = "/", bool include_ties = false) const;
        /** Merge JSON into the tree below `path`, creating nodes and setting values (arrays become indexed children). */
        bool from_json(const nlohmann::json &json, std::string_view path = "/");
        bool load_json(const std::string &file, std::string_view path = "/");
        bool save_json(const std::string &file, std::string_view path = "/", int indentation = 2) const;

    private:
        friend class PropertyNode;
        std::unique_ptr<PropertyNode> root_;
        std::uint64_t next_listener_ = 1;
    };

    /**
     * Expose a property tree to Lua as the global table `props`: `props.get(path)`, `props.set(path, value)`,
     * `props.exists(path)`, `props.children(path)` (a list of child names), `props.listen(path, fn)` (returns an id;
     * `fn(path, value)` is called on change) and `props.unlisten(id)`, plus `props.node(path)` returning a node object
     * with `value`, `set`, `path`, `name`, `child`, `children` and `on_change`. Property values are bool, number or
     * string. The tree must outlive the runtime, and listeners registered from Lua are removed by `props.unlisten()`.
     */
    void bind_property_tree(LuaRuntime &runtime, PropertyTree &tree);

} // namespace sl

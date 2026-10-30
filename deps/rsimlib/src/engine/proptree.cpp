/** @file
 * @brief Hierarchical property tree shared between C++ and Lua.
 */

#include "proptree.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <fstream>

namespace sl
{
    namespace
    {
        using Value = std::variant<std::monostate, bool, int, double, std::string>;

        bool valid_name(std::string_view name)
        {
            if (name.empty() || !(std::isalpha(static_cast<unsigned char>(name[0])) || name[0] == '_'))
                return false;
            return std::all_of(name.begin(), name.end(), [](char c) {
                return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-' || c == '.';
            });
        }

        // Splits "name[3]" into its parts; false if malformed.
        bool parse_segment(std::string_view segment, std::string_view &name, int &index)
        {
            index = 0;
            const std::size_t bracket = segment.find('[');
            if (bracket == std::string_view::npos)
            {
                name = segment;
                return valid_name(name);
            }
            if (segment.back() != ']' || segment.size() < bracket + 3)
                return false;
            name = segment.substr(0, bracket);
            const std::string number(segment.substr(bracket + 1, segment.size() - bracket - 2));
            if (number.empty() || !std::all_of(number.begin(), number.end(), [](char c) { return std::isdigit(static_cast<unsigned char>(c)); }))
                return false;
            index = std::atoi(number.c_str());
            return valid_name(name);
        }

        std::string segment_text(const PropertyNode &node)
        {
            return node.index() == 0 ? node.name() : node.name() + "[" + std::to_string(node.index()) + "]";
        }

        bool to_bool(const Value &v, bool fallback)
        {
            struct
            {
                bool fallback;
                bool operator()(std::monostate) const { return fallback; }
                bool operator()(bool b) const { return b; }
                bool operator()(int i) const { return i != 0; }
                bool operator()(double d) const { return d != 0.0; }
                bool operator()(const std::string &s) const
                {
                    return s == "true" || s == "TRUE" || s == "True" || (!s.empty() && s != "false" && s != "FALSE" && s != "False" && std::atof(s.c_str()) != 0.0);
                }
            } visitor{fallback};
            return std::visit(visitor, v);
        }

        double to_double(const Value &v, double fallback)
        {
            struct
            {
                double fallback;
                double operator()(std::monostate) const { return fallback; }
                double operator()(bool b) const { return b ? 1.0 : 0.0; }
                double operator()(int i) const { return i; }
                double operator()(double d) const { return d; }
                double operator()(const std::string &s) const
                {
                    char *end = nullptr;
                    const double d = std::strtod(s.c_str(), &end);
                    return end == s.c_str() ? fallback : d;
                }
            } visitor{fallback};
            return std::visit(visitor, v);
        }

        int to_int(const Value &v, int fallback)
        {
            if (std::holds_alternative<int>(v))
                return std::get<int>(v);
            if (std::holds_alternative<bool>(v))
                return std::get<bool>(v) ? 1 : 0;
            const double d = to_double(v, fallback);
            return static_cast<int>(std::lround(std::clamp(d, -2147483648.0, 2147483647.0)));
        }

        std::string to_string(const Value &v, const std::string &fallback)
        {
            struct
            {
                const std::string &fallback;
                std::string operator()(std::monostate) const { return fallback; }
                std::string operator()(bool b) const { return b ? "true" : "false"; }
                std::string operator()(int i) const { return std::to_string(i); }
                std::string operator()(double d) const
                {
                    nlohmann::json j = d;
                    return j.dump();
                }
                std::string operator()(const std::string &s) const { return s; }
            } visitor{fallback};
            return std::visit(visitor, v);
        }

        PropertyNode::Type type_of(const Value &v)
        {
            switch (v.index())
            {
            case 1: return PropertyNode::Type::boolean;
            case 2: return PropertyNode::Type::integer;
            case 3: return PropertyNode::Type::real;
            case 4: return PropertyNode::Type::string;
            default: return PropertyNode::Type::none;
            }
        }

        Value convert(const Value &v, PropertyNode::Type type)
        {
            switch (type)
            {
            case PropertyNode::Type::boolean: return to_bool(v, false);
            case PropertyNode::Type::integer: return to_int(v, 0);
            case PropertyNode::Type::real: return to_double(v, 0.0);
            case PropertyNode::Type::string: return to_string(v, {});
            default: return v;
            }
        }
    } // namespace

    PropertyNode::PropertyNode(std::string name, int index, PropertyNode *parent)
        : name_(std::move(name)), index_(index), parent_(parent), tree_(parent ? parent->tree_ : nullptr)
    {
    }

    std::string PropertyNode::path() const
    {
        if (!parent_)
            return "/";
        std::string result = parent_->path();
        if (result.back() != '/')
            result += '/';
        return result + segment_text(*this);
    }

    PropertyNode *PropertyNode::find_child(std::string_view name, int index) const
    {
        for (const auto &child : children_)
            if (child->index_ == index && child->name_ == name)
                return child.get();
        return nullptr;
    }

    PropertyNode *PropertyNode::child(std::string_view name, int index)
    {
        if (index < 0 || !valid_name(name))
            return nullptr;
        if (PropertyNode *existing = find_child(name, index))
            return existing;
        children_.push_back(std::unique_ptr<PropertyNode>(new PropertyNode(std::string(name), index, this)));
        return children_.back().get();
    }

    PropertyNode *PropertyNode::node(std::string_view path)
    {
        PropertyNode *current = this;
        std::size_t start = 0;
        while (start <= path.size())
        {
            std::size_t end = path.find('/', start);
            if (end == std::string_view::npos)
                end = path.size();
            const std::string_view segment = path.substr(start, end - start);
            start = end + 1;
            if (segment.empty() || segment == ".")
                continue;
            std::string_view name;
            int index;
            if (!parse_segment(segment, name, index))
                return nullptr;
            current = current->child(name, index);
            if (!current)
                return nullptr;
        }
        return current;
    }

    PropertyNode *PropertyNode::find(std::string_view path) const
    {
        const PropertyNode *current = this;
        std::size_t start = 0;
        while (start <= path.size())
        {
            std::size_t end = path.find('/', start);
            if (end == std::string_view::npos)
                end = path.size();
            const std::string_view segment = path.substr(start, end - start);
            start = end + 1;
            if (segment.empty() || segment == ".")
                continue;
            std::string_view name;
            int index;
            if (!parse_segment(segment, name, index))
                return nullptr;
            current = current->find_child(name, index);
            if (!current)
                return nullptr;
        }
        return const_cast<PropertyNode *>(current);
    }

    std::vector<PropertyNode *> PropertyNode::children() const
    {
        std::vector<PropertyNode *> result;
        result.reserve(children_.size());
        for (const auto &child : children_)
            result.push_back(child.get());
        return result;
    }

    std::vector<PropertyNode *> PropertyNode::children(std::string_view name) const
    {
        std::vector<PropertyNode *> result;
        for (const auto &child : children_)
            if (child->name_ == name)
                result.push_back(child.get());
        std::sort(result.begin(), result.end(), [](const PropertyNode *a, const PropertyNode *b) { return a->index_ < b->index_; });
        return result;
    }

    int PropertyNode::child_count(std::string_view name) const
    {
        int count = 0;
        for (const auto &child : children_)
            count += child->name_ == name ? 1 : 0;
        return count;
    }

    bool PropertyNode::remove_child(std::string_view name, int index)
    {
        for (auto it = children_.begin(); it != children_.end(); ++it)
            if ((*it)->index_ == index && (*it)->name_ == name)
            {
                children_.erase(it);
                return true;
            }
        return false;
    }

    PropertyNode *PropertyNode::target()
    {
        PropertyNode *node = this;
        for (int hops = 0; node->alias_ && hops < 16; ++hops)
            node = node->alias_;
        return node;
    }

    const PropertyNode *PropertyNode::target() const
    {
        const PropertyNode *node = this;
        for (int hops = 0; node->alias_ && hops < 16; ++hops)
            node = node->alias_;
        return node;
    }

    Value PropertyNode::read() const
    {
        const PropertyNode *node = target();
        if (node->tie_)
        {
            const Tie &tie = *node->tie_;
            switch (tie.type)
            {
            case Type::boolean: return tie.get_bool();
            case Type::integer: return tie.get_int();
            case Type::real: return tie.get_double();
            case Type::string: return tie.get_string();
            default: return {};
            }
        }
        return node->value_;
    }

    PropertyNode::Type PropertyNode::type() const
    {
        const PropertyNode *node = target();
        return node->tie_ ? node->tie_->type : type_of(node->value_);
    }

    bool PropertyNode::get_bool(bool fallback) const { return to_bool(read(), fallback); }
    int PropertyNode::get_int(int fallback) const { return to_int(read(), fallback); }
    double PropertyNode::get_double(double fallback) const { return to_double(read(), fallback); }
    std::string PropertyNode::get_string(const std::string &fallback) const { return to_string(read(), fallback); }

    void PropertyNode::assign(const Value &value)
    {
        PropertyNode *node = target();
        if (node->tie_)
        {
            Tie &tie = *node->tie_;
            if (!tie.writable)
                return;
            switch (tie.type)
            {
            case Type::boolean: tie.set_bool(to_bool(value, false)); break;
            case Type::integer: tie.set_int(to_int(value, 0)); break;
            case Type::real: tie.set_double(to_double(value, 0.0)); break;
            case Type::string: tie.set_string(to_string(value, {})); break;
            default: return;
            }
            node->fire(*node);
            return;
        }
        const Type existing = type_of(node->value_);
        const Value converted = existing == Type::none ? value : convert(value, existing);
        if (converted == node->value_)
            return;
        node->value_ = converted;
        node->fire(*node);
    }

    void PropertyNode::set_bool(bool value) { assign(value); }
    void PropertyNode::set_int(int value) { assign(value); }
    void PropertyNode::set_double(double value) { assign(value); }
    void PropertyNode::set_string(const std::string &value) { assign(value); }

    void PropertyNode::set_from_string(const std::string &text)
    {
        switch (type())
        {
        case Type::boolean: assign(to_bool(text, false)); return;
        case Type::integer: assign(to_int(text, 0)); return;
        case Type::real: assign(to_double(text, 0.0)); return;
        case Type::string: assign(text); return;
        default: break;
        }
        if (text == "true" || text == "false")
        {
            assign(text == "true");
            return;
        }
        char *end = nullptr;
        const long integer = std::strtol(text.c_str(), &end, 10);
        if (!text.empty() && *end == '\0' && integer >= INT32_MIN && integer <= INT32_MAX)
        {
            assign(static_cast<int>(integer));
            return;
        }
        end = nullptr;
        const double real = std::strtod(text.c_str(), &end);
        if (!text.empty() && *end == '\0')
        {
            assign(real);
            return;
        }
        assign(text);
    }

    void PropertyNode::set_tie(Tie tie)
    {
        tie_ = std::make_unique<Tie>(std::move(tie));
        value_ = {};
    }

    void PropertyNode::tie_bool(std::function<bool()> getter, std::function<void(bool)> setter)
    {
        Tie tie;
        tie.type = Type::boolean;
        tie.get_bool = std::move(getter);
        tie.writable = static_cast<bool>(setter);
        tie.set_bool = std::move(setter);
        set_tie(std::move(tie));
    }

    void PropertyNode::tie_int(std::function<int()> getter, std::function<void(int)> setter)
    {
        Tie tie;
        tie.type = Type::integer;
        tie.get_int = std::move(getter);
        tie.writable = static_cast<bool>(setter);
        tie.set_int = std::move(setter);
        set_tie(std::move(tie));
    }

    void PropertyNode::tie_double(std::function<double()> getter, std::function<void(double)> setter)
    {
        Tie tie;
        tie.type = Type::real;
        tie.get_double = std::move(getter);
        tie.writable = static_cast<bool>(setter);
        tie.set_double = std::move(setter);
        set_tie(std::move(tie));
    }

    void PropertyNode::tie_string(std::function<std::string()> getter, std::function<void(const std::string &)> setter)
    {
        Tie tie;
        tie.type = Type::string;
        tie.get_string = std::move(getter);
        tie.writable = static_cast<bool>(setter);
        tie.set_string = std::move(setter);
        set_tie(std::move(tie));
    }

    void PropertyNode::tie(double *variable)
    {
        tie_double([variable] { return *variable; }, [variable](double v) { *variable = v; });
    }

    void PropertyNode::tie(int *variable)
    {
        tie_int([variable] { return *variable; }, [variable](int v) { *variable = v; });
    }

    void PropertyNode::tie(bool *variable)
    {
        tie_bool([variable] { return *variable; }, [variable](bool v) { *variable = v; });
    }

    void PropertyNode::untie()
    {
        if (!tie_)
            return;
        value_ = read();
        tie_.reset();
    }

    bool PropertyNode::is_tied() const { return target()->tie_ != nullptr; }
    bool PropertyNode::is_read_only() const
    {
        const PropertyNode *node = target();
        return node->tie_ && !node->tie_->writable;
    }

    bool PropertyNode::set_alias(PropertyNode *target_node)
    {
        if (!target_node)
            return false;
        const PropertyNode *walk = target_node;
        for (int hops = 0; walk && hops < 16; ++hops, walk = walk->alias_)
            if (walk == this)
                return false;
        alias_ = target_node;
        return true;
    }

    PropertyNode::ListenerId PropertyNode::add_listener(Listener listener)
    {
        const ListenerId id = tree_ ? tree_->next_listener_++ : 0;
        listeners_.push_back({id, std::move(listener), false});
        return id;
    }

    PropertyNode::ListenerId PropertyNode::add_subtree_listener(Listener listener)
    {
        const ListenerId id = tree_ ? tree_->next_listener_++ : 0;
        listeners_.push_back({id, std::move(listener), true});
        return id;
    }

    void PropertyNode::remove_listener(ListenerId id)
    {
        listeners_.erase(std::remove_if(listeners_.begin(), listeners_.end(), [id](const ListenerEntry &e) { return e.id == id; }),
                         listeners_.end());
    }

    void PropertyNode::notify() { target()->fire(*target()); }

    void PropertyNode::fire(PropertyNode &changed)
    {
        for (PropertyNode *node = this; node; node = node->parent_)
        {
            if (node->listeners_.empty())
                continue;
            // Copy first: a callback may add or remove listeners, or change properties.
            const std::vector<ListenerEntry> snapshot = node->listeners_;
            for (const ListenerEntry &entry : snapshot)
                if (node == this || entry.subtree)
                    entry.callback(changed);
        }
    }

    PropertyTree::PropertyTree() : root_(new PropertyNode("", 0, nullptr)) { root_->tree_ = this; }
    PropertyTree::~PropertyTree() = default;

    PropertyNode *PropertyTree::node(std::string_view path) { return root_->node(path); }
    PropertyNode *PropertyTree::find(std::string_view path) const { return root_->find(path); }

    bool PropertyTree::get_bool(std::string_view path, bool fallback) const
    {
        const PropertyNode *n = find(path);
        return n ? n->get_bool(fallback) : fallback;
    }
    int PropertyTree::get_int(std::string_view path, int fallback) const
    {
        const PropertyNode *n = find(path);
        return n ? n->get_int(fallback) : fallback;
    }
    double PropertyTree::get_double(std::string_view path, double fallback) const
    {
        const PropertyNode *n = find(path);
        return n ? n->get_double(fallback) : fallback;
    }
    std::string PropertyTree::get_string(std::string_view path, const std::string &fallback) const
    {
        const PropertyNode *n = find(path);
        return n ? n->get_string(fallback) : fallback;
    }
    void PropertyTree::set_bool(std::string_view path, bool value)
    {
        if (PropertyNode *n = node(path))
            n->set_bool(value);
    }
    void PropertyTree::set_int(std::string_view path, int value)
    {
        if (PropertyNode *n = node(path))
            n->set_int(value);
    }
    void PropertyTree::set_double(std::string_view path, double value)
    {
        if (PropertyNode *n = node(path))
            n->set_double(value);
    }
    void PropertyTree::set_string(std::string_view path, const std::string &value)
    {
        if (PropertyNode *n = node(path))
            n->set_string(value);
    }

    void PropertyTree::clear() { root_->children_.clear(); }

    namespace
    {
        nlohmann::json value_to_json(const PropertyNode &node)
        {
            switch (node.type())
            {
            case PropertyNode::Type::boolean: return node.get_bool();
            case PropertyNode::Type::integer: return node.get_int();
            case PropertyNode::Type::real: return node.get_double();
            case PropertyNode::Type::string: return node.get_string();
            default: return nullptr;
            }
        }

        nlohmann::json node_to_json(const PropertyNode &node, bool include_ties)
        {
            if (!node.has_children())
            {
                nlohmann::json value = value_to_json(node);
                return value.is_null() ? nlohmann::json::object() : value;
            }
            nlohmann::json object = nlohmann::json::object();
            if (node.has_value())
                object["_value"] = value_to_json(node);
            std::vector<std::string> done;
            for (const PropertyNode *child : node.children())
            {
                if (std::find(done.begin(), done.end(), child->name()) != done.end())
                    continue;
                done.push_back(child->name());
                const std::vector<PropertyNode *> group = node.children(child->name());
                std::vector<nlohmann::json> items;
                for (const PropertyNode *member : group)
                {
                    if (!include_ties && (member->alias() || member->is_read_only()))
                        continue;
                    items.push_back(node_to_json(*member, include_ties));
                }
                if (items.empty())
                    continue;
                if (group.size() > 1 || group.front()->index() != 0)
                    object[child->name()] = items;
                else
                    object[child->name()] = items.front();
            }
            return object;
        }

        bool apply_json(PropertyNode &node, const nlohmann::json &json)
        {
            bool ok = true;
            if (json.is_object())
            {
                for (auto it = json.begin(); it != json.end(); ++it)
                {
                    if (it.key() == "_value")
                    {
                        ok &= apply_json(node, it.value());
                        continue;
                    }
                    if (it.value().is_array())
                    {
                        int index = 0;
                        for (const nlohmann::json &element : it.value())
                        {
                            PropertyNode *child = node.child(it.key(), index++);
                            ok &= child && apply_json(*child, element);
                        }
                    }
                    else
                    {
                        PropertyNode *child = node.child(it.key(), 0);
                        ok &= child && apply_json(*child, it.value());
                    }
                }
            }
            else if (json.is_boolean())
                node.set_bool(json.get<bool>());
            else if (json.is_number_integer())
            {
                const long long v = json.get<long long>();
                if (v >= INT32_MIN && v <= INT32_MAX)
                    node.set_int(static_cast<int>(v));
                else
                    node.set_double(static_cast<double>(v));
            }
            else if (json.is_number())
                node.set_double(json.get<double>());
            else if (json.is_string())
                node.set_string(json.get<std::string>());
            return ok;
        }
    } // namespace

    nlohmann::json PropertyTree::to_json(std::string_view path, bool include_ties) const
    {
        const PropertyNode *n = find(path);
        return n ? node_to_json(*n, include_ties) : nlohmann::json::object();
    }

    bool PropertyTree::from_json(const nlohmann::json &json, std::string_view path)
    {
        PropertyNode *n = node(path);
        return n && apply_json(*n, json);
    }

    bool PropertyTree::load_json(const std::string &file, std::string_view path)
    {
        std::ifstream stream(file);
        if (!stream)
            return false;
        const nlohmann::json json = nlohmann::json::parse(stream, nullptr, false);
        return !json.is_discarded() && from_json(json, path);
    }

    bool PropertyTree::save_json(const std::string &file, std::string_view path, int indentation) const
    {
        std::ofstream stream(file);
        if (!stream)
            return false;
        stream << to_json(path).dump(indentation) << '\n';
        return static_cast<bool>(stream);
    }

} // namespace sl

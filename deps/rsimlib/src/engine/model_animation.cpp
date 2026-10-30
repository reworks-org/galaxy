#include "model3d.h"

#include "error.h"
#include <nlohmann/json.hpp>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string_view>

#include <glm/gtc/matrix_transform.hpp>

namespace sl
{
    namespace
    {
        using json = nlohmann::json;

        struct XmlNode
        {
            std::string name;
            std::string text;
            std::map<std::string, std::string> attributes;
            std::vector<std::unique_ptr<XmlNode>> children;
        };

        void convert_xml(const pugi::xml_node &source, XmlNode &target)
        {
            for (const pugi::xml_attribute &attribute : source.attributes())
                target.attributes.emplace(attribute.name(), attribute.value());
            for (const pugi::xml_node &entry : source.children())
            {
                switch (entry.type())
                {
                case pugi::node_element:
                {
                    auto node = std::make_unique<XmlNode>();
                    node->name = entry.name();
                    convert_xml(entry, *node);
                    target.children.push_back(std::move(node));
                    break;
                }
                case pugi::node_pcdata:
                case pugi::node_cdata:
                    target.text += entry.value();
                    break;
                default:
                    break;
                }
            }
        }

        bool parse_xml(const std::string &source, XmlNode &document, std::string &error)
        {
            pugi::xml_document parsed;
            const pugi::xml_parse_result result = parsed.load_buffer(source.data(), source.size());
            if (!result)
            {
                const std::size_t line = 1 + static_cast<std::size_t>(std::count(
                                                 source.begin(), source.begin() + std::min<std::size_t>(result.offset, source.size()), '\n'));
                error = std::string(result.description()) + " (line " + std::to_string(line) + ")";
                return false;
            }
            convert_xml(parsed, document);
            return true;
        }

        std::string trim(std::string value)
        {
            const auto first = value.find_first_not_of(" \t\r\n");
            if (first == std::string::npos)
                return {};
            const auto last = value.find_last_not_of(" \t\r\n");
            return value.substr(first, last - first + 1);
        }

        const XmlNode *child(const XmlNode &node, const std::string &name)
        {
            for (const auto &entry : node.children)
                if (entry->name == name)
                    return entry.get();
            return nullptr;
        }

        std::string text(const XmlNode &node, const std::string &name)
        {
            const XmlNode *entry = child(node, name);
            return entry ? trim(entry->text) : std::string{};
        }

        bool parse_float(const std::string &value, float &result)
        {
            if (value.empty())
                return false;
            std::istringstream stream(value);
            stream >> result;
            return static_cast<bool>(stream) && std::isfinite(result);
        }

        bool parse_vec3_json(const json &value, glm::vec3 &result)
        {
            if (!value.is_array() || value.size() != 3)
                return false;
            for (std::size_t i = 0; i < 3; ++i)
            {
                if (!value[i].is_number())
                    return false;
                result[static_cast<int>(i)] = value[i].get<float>();
                if (!std::isfinite(result[static_cast<int>(i)]))
                    return false;
            }
            return true;
        }

        bool parse_animation_json(const json &entry, ModelAnimation &animation)
        {
            if (!entry.is_object() || !entry.contains("type") || !entry["type"].is_string() ||
                !entry.contains("property") || !entry["property"].is_string() ||
                !entry.contains("objects") || !entry["objects"].is_array())
                return false;
            const std::string type = entry["type"].get<std::string>();
            if (type == "rotate" || type == "rotation")
                animation.type = ModelAnimationType::rotation;
            else if (type == "translate" || type == "translation")
                animation.type = ModelAnimationType::translation;
            else if (type == "spin")
                animation.type = ModelAnimationType::spin;
            else if (type == "blend")
                animation.type = ModelAnimationType::blend;
            else
                return false;
            animation.property = entry["property"].get<std::string>();
            if (animation.property.empty() && animation.type != ModelAnimationType::translation)
                return false;
            for (const json &object : entry["objects"])
            {
                if (!object.is_string() || object.get<std::string>().empty())
                    return false;
                animation.objects.push_back(object.get<std::string>());
            }
            if (animation.objects.empty())
                return false;
            if (animation.type != ModelAnimationType::blend &&
                (!entry.contains("axis") || !parse_vec3_json(entry["axis"], animation.axis) ||
                 glm::dot(animation.axis, animation.axis) <= 1.0e-20f))
                return false;
            if (entry.contains("center") && !parse_vec3_json(entry["center"], animation.center))
                return false;
            if (entry.contains("factor"))
            {
                if (!entry["factor"].is_number())
                    return false;
                animation.factor = entry["factor"].get<float>();
            }
            if (entry.contains("offset"))
            {
                if (!entry["offset"].is_number())
                    return false;
                animation.offset = entry["offset"].get<float>();
            }
            if (entry.contains("minimum"))
            {
                if (!entry["minimum"].is_number())
                    return false;
                animation.minimum = entry["minimum"].get<float>();
            }
            if (entry.contains("maximum"))
            {
                if (!entry["maximum"].is_number())
                    return false;
                animation.maximum = entry["maximum"].get<float>();
            }
            if (!std::isfinite(animation.factor) || !std::isfinite(animation.offset))
                return false;
            if (!std::isfinite(animation.minimum) || !std::isfinite(animation.maximum) ||
                animation.minimum > animation.maximum)
                return false;
            if (entry.contains("interpolation"))
            {
                if (!entry["interpolation"].is_array())
                    return false;
                for (const json &pair : entry["interpolation"])
                {
                    if (!pair.is_array() || pair.size() != 2 || !pair[0].is_number() || !pair[1].is_number())
                        return false;
                    const float input = pair[0].get<float>();
                    const float output = pair[1].get<float>();
                    if (!std::isfinite(input) || !std::isfinite(output))
                        return false;
                    animation.interpolation.emplace_back(input, output);
                }
                std::sort(animation.interpolation.begin(), animation.interpolation.end(),
                          [](const glm::vec2 &a, const glm::vec2 &b) { return a.x < b.x; });
                for (std::size_t i = 1; i < animation.interpolation.size(); ++i)
                    if (animation.interpolation[i - 1].x == animation.interpolation[i].x)
                        return false;
            }
            return true;
        }

        bool is_comparison(ModelConditionType type)
        {
            return type == ModelConditionType::less_than || type == ModelConditionType::greater_than ||
                   type == ModelConditionType::equal || type == ModelConditionType::not_equal ||
                   type == ModelConditionType::less_equal || type == ModelConditionType::greater_equal;
        }

        bool parse_condition_json(const json &entry, ModelCondition &condition)
        {
            if (!entry.is_object() || !entry.contains("op") || !entry["op"].is_string())
                return false;
            const std::string op = entry["op"].get<std::string>();
            if (op == "property")
                condition.type = ModelConditionType::property;
            else if (op == "less_than")
                condition.type = ModelConditionType::less_than;
            else if (op == "greater_than")
                condition.type = ModelConditionType::greater_than;
            else if (op == "equal")
                condition.type = ModelConditionType::equal;
            else if (op == "not_equal")
                condition.type = ModelConditionType::not_equal;
            else if (op == "less_equal")
                condition.type = ModelConditionType::less_equal;
            else if (op == "greater_equal")
                condition.type = ModelConditionType::greater_equal;
            else if (op == "and")
                condition.type = ModelConditionType::logical_and;
            else if (op == "or")
                condition.type = ModelConditionType::logical_or;
            else if (op == "not")
                condition.type = ModelConditionType::logical_not;
            else
                return false;

            if (condition.type == ModelConditionType::property || is_comparison(condition.type))
            {
                if (!entry.contains("property") || !entry["property"].is_string())
                    return false;
                condition.property = entry["property"].get<std::string>();
                if (condition.property.empty())
                    return false;
                if (condition.type != ModelConditionType::property)
                {
                    if (!entry.contains("value") || !entry["value"].is_number())
                        return false;
                    condition.value = entry["value"].get<float>();
                    if (!std::isfinite(condition.value))
                        return false;
                }
                return true;
            }
            if (!entry.contains("conditions") || !entry["conditions"].is_array() ||
                entry["conditions"].empty() ||
                (condition.type == ModelConditionType::logical_not && entry["conditions"].size() != 1))
                return false;
            for (const json &child_entry : entry["conditions"])
            {
                ModelCondition child;
                if (!parse_condition_json(child_entry, child))
                    return false;
                condition.children.push_back(std::move(child));
            }
            return true;
        }

        json serialize_condition(const ModelCondition &condition)
        {
            json result;
            switch (condition.type)
            {
            case ModelConditionType::property: result["op"] = "property"; break;
            case ModelConditionType::less_than: result["op"] = "less_than"; break;
            case ModelConditionType::greater_than: result["op"] = "greater_than"; break;
            case ModelConditionType::equal: result["op"] = "equal"; break;
            case ModelConditionType::not_equal: result["op"] = "not_equal"; break;
            case ModelConditionType::less_equal: result["op"] = "less_equal"; break;
            case ModelConditionType::greater_equal: result["op"] = "greater_equal"; break;
            case ModelConditionType::logical_and: result["op"] = "and"; break;
            case ModelConditionType::logical_or: result["op"] = "or"; break;
            case ModelConditionType::logical_not: result["op"] = "not"; break;
            }
            if (condition.type == ModelConditionType::property || is_comparison(condition.type))
            {
                result["property"] = condition.property;
                if (condition.type != ModelConditionType::property)
                    result["value"] = condition.value;
            }
            else
            {
                result["conditions"] = json::array();
                for (const ModelCondition &child : condition.children)
                    result["conditions"].push_back(serialize_condition(child));
            }
            return result;
        }

        bool read_xml_condition(const XmlNode &node, ModelCondition &condition, std::string &error)
        {
            if (node.name == "property")
                condition.type = ModelConditionType::property;
            else if (node.name == "less-than")
                condition.type = ModelConditionType::less_than;
            else if (node.name == "greater-than")
                condition.type = ModelConditionType::greater_than;
            else if (node.name == "equals")
                condition.type = ModelConditionType::equal;
            else if (node.name == "not-equals")
                condition.type = ModelConditionType::not_equal;
            else if (node.name == "less-than-equals")
                condition.type = ModelConditionType::less_equal;
            else if (node.name == "greater-than-equals")
                condition.type = ModelConditionType::greater_equal;
            else if (node.name == "and")
                condition.type = ModelConditionType::logical_and;
            else if (node.name == "or")
                condition.type = ModelConditionType::logical_or;
            else if (node.name == "not")
                condition.type = ModelConditionType::logical_not;
            else
            {
                error = "unsupported condition <" + node.name + ">";
                return false;
            }

            if (condition.type == ModelConditionType::property || is_comparison(condition.type))
            {
                const XmlNode *property_node = condition.type == ModelConditionType::property
                                                   ? &node
                                                   : child(node, "property");
                if (!property_node)
                {
                    error = "condition has no property";
                    return false;
                }
                const auto alias = property_node->attributes.find("alias");
                condition.property = alias == property_node->attributes.end()
                                         ? trim(property_node->text)
                                         : alias->second;
                if (condition.property.empty())
                {
                    error = "condition property is empty";
                    return false;
                }
                if (condition.type != ModelConditionType::property &&
                    !parse_float(text(node, "value"), condition.value))
                {
                    error = "comparison condition has an invalid value";
                    return false;
                }
                return true;
            }

            for (const auto &entry : node.children)
            {
                ModelCondition child_condition;
                if (!read_xml_condition(*entry, child_condition, error))
                    return false;
                condition.children.push_back(std::move(child_condition));
            }
            if (condition.children.empty() ||
                (condition.type == ModelConditionType::logical_not && condition.children.size() != 1))
            {
                error = "logical condition has invalid child count";
                return false;
            }
            return true;
        }

        bool read_xml_visibility(const XmlNode &node, ModelVisibility &visibility, std::string &unsupported)
        {
            const XmlNode *condition_node = child(node, "condition");
            if (!condition_node)
            {
                unsupported = "select (missing condition)";
                return false;
            }
            for (const auto &entry : node.children)
                if (entry->name == "object-name")
                {
                    const std::string name = trim(entry->text);
                    if (!name.empty())
                        visibility.objects.push_back(name);
                }
            std::string error;
            if (visibility.objects.empty() || condition_node->children.size() != 1 ||
                !read_xml_condition(*condition_node->children.front(), visibility.condition, error))
            {
                unsupported = "select (" + (error.empty() ? std::string("invalid condition") : error) + ")";
                return false;
            }
            return true;
        }

        bool read_xml_animation(const XmlNode &node, ModelAnimation &animation, std::string &unsupported)
        {
            const std::string type = text(node, "type");
            if (type != "rotate" && type != "translate" && type != "spin" && type != "blend")
            {
                unsupported = type.empty() ? "unknown" : type;
                return false;
            }
            if (type == "rotate")
                animation.type = ModelAnimationType::rotation;
            else if (type == "translate")
                animation.type = ModelAnimationType::translation;
            else if (type == "spin")
                animation.type = ModelAnimationType::spin;
            else
                animation.type = ModelAnimationType::blend;
            const XmlNode *property_node = child(node, "property");
            if (property_node)
            {
                auto alias = property_node->attributes.find("alias");
                animation.property = alias == property_node->attributes.end() ? trim(property_node->text) : alias->second;
            }
            for (const auto &entry : node.children)
                if (entry->name == "object-name")
                {
                    const std::string object_name = trim(entry->text);
                    if (!object_name.empty())
                        animation.objects.push_back(object_name);
                }
            if (animation.objects.empty() ||
                (animation.property.empty() &&
                 (animation.type != ModelAnimationType::translation || text(node, "offset-m").empty())))
            {
                unsupported = type + " (missing property or object-name)";
                return false;
            }
            float value = 0.0f;
            const std::string factor_text = text(node, "factor");
            if (!factor_text.empty() && !parse_float(factor_text, value))
            {
                unsupported = type + " (invalid factor)";
                return false;
            }
            if (!factor_text.empty())
                animation.factor = value;
            const char *offset_names[] = {"offset-deg", "offset-m"};
            for (const char *offset_name : offset_names)
            {
                const std::string offset_text = text(node, offset_name);
                if (!offset_text.empty() && !parse_float(offset_text, value))
                {
                    unsupported = type + " (invalid offset)";
                    return false;
                }
                if (!offset_text.empty())
                {
                    animation.offset = value;
                    break;
                }
            }
            const XmlNode *axis = child(node, "axis");
            if (axis)
            {
                float x1, y1, z1, x2, y2, z2;
                const bool endpoints = parse_float(text(*axis, "x1-m"), x1) &&
                                       parse_float(text(*axis, "y1-m"), y1) &&
                                       parse_float(text(*axis, "z1-m"), z1) &&
                                       parse_float(text(*axis, "x2-m"), x2) &&
                                       parse_float(text(*axis, "y2-m"), y2) &&
                                       parse_float(text(*axis, "z2-m"), z2);
                if (endpoints)
                {
                    animation.center = {x1, y1, z1};
                    animation.axis = glm::vec3(x2 - x1, y2 - y1, z2 - z1);
                }
                else
                {
                    float x, y, z;
                    if (!parse_float(text(*axis, "x"), x) || !parse_float(text(*axis, "y"), y) ||
                        !parse_float(text(*axis, "z"), z))
                    {
                        unsupported = type + " (unsupported axis)";
                        return false;
                    }
                    animation.axis = {x, y, z};
                }
            }
            if (const XmlNode *center = child(node, "center"))
            {
                float x, y, z;
                const std::string x_value = text(*center, "x-m").empty() ? text(*center, "x") : text(*center, "x-m");
                const std::string y_value = text(*center, "y-m").empty() ? text(*center, "y") : text(*center, "y-m");
                const std::string z_value = text(*center, "z-m").empty() ? text(*center, "z") : text(*center, "z-m");
                if (!parse_float(x_value, x) || !parse_float(y_value, y) || !parse_float(z_value, z))
                {
                    unsupported = type + " (unsupported center)";
                    return false;
                }
                animation.center = {x, y, z};
            }
            if (animation.type != ModelAnimationType::blend &&
                glm::dot(animation.axis, animation.axis) <= 1.0e-20f)
            {
                unsupported = type + " (zero axis)";
                return false;
            }
            if (animation.type == ModelAnimationType::translation)
                animation.axis = glm::normalize(animation.axis);
            if (animation.type == ModelAnimationType::blend)
            {
                const std::string minimum = text(node, "min");
                const std::string maximum = text(node, "max");
                if ((!minimum.empty() && !parse_float(minimum, animation.minimum)) ||
                    (!maximum.empty() && !parse_float(maximum, animation.maximum)) ||
                    animation.minimum > animation.maximum)
                {
                    unsupported = type + " (invalid range)";
                    return false;
                }
            }
            if (const XmlNode *interpolation = child(node, "interpolation"))
                for (const auto &entry : interpolation->children)
                    if (entry->name == "entry")
                    {
                        float input, output;
                        if (!parse_float(text(*entry, "ind"), input) || !parse_float(text(*entry, "dep"), output))
                        {
                            unsupported = type + " (invalid interpolation)";
                            return false;
                        }
                        animation.interpolation.emplace_back(input, output);
                    }
            std::sort(animation.interpolation.begin(), animation.interpolation.end(),
                      [](const glm::vec2 &a, const glm::vec2 &b) { return a.x < b.x; });
            for (std::size_t i = 1; i < animation.interpolation.size(); ++i)
                if (animation.interpolation[i - 1].x == animation.interpolation[i].x)
                {
                    unsupported = type + " (duplicate interpolation input)";
                    return false;
                }
            return true;
        }

        void find_animations(const XmlNode &node, Model &model)
        {
            if (node.name == "animation")
            {
                if (text(node, "type") == "select")
                {
                    ModelVisibility visibility;
                    std::string unsupported;
                    if (read_xml_visibility(node, visibility, unsupported))
                        model.visibility.push_back(std::move(visibility));
                    else
                        model.unsupported_animations.push_back(std::move(unsupported));
                    return;
                }
                ModelAnimation animation;
                std::string unsupported;
                if (read_xml_animation(node, animation, unsupported))
                    model.animations.push_back(std::move(animation));
                else
                    model.unsupported_animations.push_back(std::move(unsupported));
                return;
            }
            for (const auto &entry : node.children)
                find_animations(*entry, model);
        }

        bool evaluate_condition(const ModelCondition &condition, const Model &model)
        {
            const auto found = model.animation_properties.find(condition.property);
            const float value = found == model.animation_properties.end() ? 0.0f : found->second;
            switch (condition.type)
            {
            case ModelConditionType::property:
                return value != 0.0f;
            case ModelConditionType::less_than:
                return value < condition.value;
            case ModelConditionType::greater_than:
                return value > condition.value;
            case ModelConditionType::equal:
                return value == condition.value;
            case ModelConditionType::not_equal:
                return value != condition.value;
            case ModelConditionType::less_equal:
                return value <= condition.value;
            case ModelConditionType::greater_equal:
                return value >= condition.value;
            case ModelConditionType::logical_and:
                return std::all_of(condition.children.begin(), condition.children.end(),
                                   [&](const ModelCondition &child) { return evaluate_condition(child, model); });
            case ModelConditionType::logical_or:
                return std::any_of(condition.children.begin(), condition.children.end(),
                                   [&](const ModelCondition &child) { return evaluate_condition(child, model); });
            case ModelConditionType::logical_not:
                return !evaluate_condition(condition.children.front(), model);
            }
            return false;
        }

        bool part_is_visible(const Model &model, const ModelPart &part)
        {
            if (!part.visible)
                return false;
            for (const ModelVisibility &visibility : model.visibility)
                if (std::find(visibility.objects.begin(), visibility.objects.end(), part.name) != visibility.objects.end() &&
                    !evaluate_condition(visibility.condition, model))
                    return false;
            return true;
        }

        float animation_value(const ModelAnimation &animation, float input)
        {
            if (animation.interpolation.empty())
                return input * animation.factor + animation.offset;
            if (input <= animation.interpolation.front().x)
                return animation.interpolation.front().y;
            if (input >= animation.interpolation.back().x)
                return animation.interpolation.back().y;
            for (std::size_t i = 1; i < animation.interpolation.size(); ++i)
                if (input <= animation.interpolation[i].x)
                {
                    const glm::vec2 &a = animation.interpolation[i - 1];
                    const glm::vec2 &b = animation.interpolation[i];
                    const float t = (input - a.x) / (b.x - a.x);
                    return a.y + (b.y - a.y) * t;
                }
            return animation.interpolation.back().y;
        }

        float animation_value(const ModelAnimation &animation, float input, double elapsed_seconds)
        {
            if (animation.type == ModelAnimationType::spin)
            {
                const double degrees = static_cast<double>(input) * animation.factor * 6.0 * elapsed_seconds;
                return static_cast<float>(std::remainder(degrees, 360.0)) + animation.offset;
            }
            return animation_value(animation, input);
        }

        glm::mat4 animation_transform(const ModelAnimation &animation, float value)
        {
            if (animation.type == ModelAnimationType::translation)
                return glm::translate(glm::mat4(1.0f), animation.axis * value);
            const glm::vec3 axis = glm::normalize(animation.axis);
            const glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(value), axis);
            return glm::translate(glm::mat4(1.0f), animation.center) * rotation *
                   glm::translate(glm::mat4(1.0f), -animation.center);
        }
    }

    bool load_model_animations_json(Model *model, const std::string &path)
    {
        if (!model)
        {
            detail::set_error("load_model_animations_json requires a model");
            return false;
        }
        std::ifstream file(path);
        if (!file)
        {
            detail::set_error("Unable to open model animation JSON: " + path);
            return false;
        }
        try
        {
            const json document = json::parse(file);
            if (!document.is_object() || !document.contains("version") ||
                document["version"].get<int>() != 1 || !document.contains("animations") ||
                !document["animations"].is_array())
            {
                detail::set_error("Invalid simlib animation JSON structure");
                return false;
            }
            std::vector<ModelAnimation> parsed;
            for (const json &entry : document["animations"])
            {
                ModelAnimation animation;
                if (!parse_animation_json(entry, animation))
                {
                    detail::set_error("Invalid simlib animation entry");
                    return false;
                }
                parsed.push_back(std::move(animation));
            }
            std::vector<ModelVisibility> parsed_visibility;
            if (document.contains("visibility"))
            {
                if (!document["visibility"].is_array())
                {
                    detail::set_error("Invalid simlib animation JSON visibility array");
                    return false;
                }
                for (const json &entry : document["visibility"])
                {
                    if (!entry.is_object() || !entry.contains("objects") || !entry["objects"].is_array() ||
                        !entry.contains("condition"))
                    {
                        detail::set_error("Invalid simlib animation JSON visibility entry");
                        return false;
                    }
                    ModelVisibility visibility;
                    for (const json &object : entry["objects"])
                    {
                        if (!object.is_string() || object.get<std::string>().empty())
                        {
                            detail::set_error("Invalid simlib animation JSON visibility object");
                            return false;
                        }
                        visibility.objects.push_back(object.get<std::string>());
                    }
                    if (visibility.objects.empty() ||
                        !parse_condition_json(entry["condition"], visibility.condition))
                    {
                        detail::set_error("Invalid simlib animation JSON visibility condition");
                        return false;
                    }
                    parsed_visibility.push_back(std::move(visibility));
                }
            }
            model->animations.insert(model->animations.end(), parsed.begin(), parsed.end());
            model->visibility.insert(model->visibility.end(), parsed_visibility.begin(), parsed_visibility.end());
        }
        catch (const json::exception &exception)
        {
            detail::set_error(std::string("Invalid simlib animation JSON: ") + exception.what());
            return false;
        }
        return true;
    }

    bool save_model_animations_json(const Model *model, const std::string &path)
    {
        if (!model)
        {
            detail::set_error("save_model_animations_json requires a model");
            return false;
        }
        json document;
        document["version"] = 1;
        document["animations"] = json::array();
        document["visibility"] = json::array();
        for (const ModelAnimation &animation : model->animations)
        {
            json entry;
            switch (animation.type)
            {
            case ModelAnimationType::rotation: entry["type"] = "rotate"; break;
            case ModelAnimationType::translation: entry["type"] = "translate"; break;
            case ModelAnimationType::spin: entry["type"] = "spin"; break;
            case ModelAnimationType::blend: entry["type"] = "blend"; break;
            }
            entry["property"] = animation.property;
            entry["objects"] = animation.objects;
            if (animation.type != ModelAnimationType::blend)
            {
                entry["axis"] = {animation.axis.x, animation.axis.y, animation.axis.z};
                entry["center"] = {animation.center.x, animation.center.y, animation.center.z};
            }
            entry["factor"] = animation.factor;
            entry["offset"] = animation.offset;
            if (animation.type == ModelAnimationType::blend)
            {
                entry["minimum"] = animation.minimum;
                entry["maximum"] = animation.maximum;
            }
            entry["interpolation"] = json::array();
            for (const glm::vec2 &point : animation.interpolation)
                entry["interpolation"].push_back({point.x, point.y});
            document["animations"].push_back(std::move(entry));
        }
        for (const ModelVisibility &visibility : model->visibility)
        {
            json entry;
            entry["objects"] = visibility.objects;
            entry["condition"] = serialize_condition(visibility.condition);
            document["visibility"].push_back(std::move(entry));
        }
        std::ofstream file(path, std::ios::binary);
        if (!file)
        {
            detail::set_error("Unable to write model animation JSON: " + path);
            return false;
        }
        file << document.dump(2) << '\n';
        if (!file)
        {
            detail::set_error("Failed while writing model animation JSON: " + path);
            return false;
        }
        return true;
    }

    Model *load_flightgear_model(const std::string &path)
    {
        std::ifstream file(path);
        if (!file)
        {
            detail::set_error("Unable to open FlightGear model XML: " + path);
            return nullptr;
        }
        std::ostringstream contents;
        contents << file.rdbuf();
        XmlNode document;
        std::string error;
        if (!parse_xml(contents.str(), document, error))
        {
            detail::set_error("Invalid FlightGear model XML: " + error);
            return nullptr;
        }
        const XmlNode *model_root = nullptr;
        for (const auto &entry : document.children)
            if (entry->name == "PropertyList")
            {
                model_root = entry.get();
                break;
            }
        const std::string model_path = model_root ? text(*model_root, "path") : std::string{};
        if (model_path.empty())
        {
            detail::set_error("FlightGear model XML has no direct <path> entry");
            return nullptr;
        }
        std::string normalized_path = model_path;
        std::replace(normalized_path.begin(), normalized_path.end(), '\\', '/');
        const std::filesystem::path asset_path =
            (std::filesystem::path(path).parent_path() / normalized_path).lexically_normal();
        Model *model = load_model(asset_path.string());
        if (!model)
            return nullptr;
        find_animations(*model_root, *model);
        return model;
    }

    bool set_model_property(Model *model, const std::string &property, float value)
    {
        if (!model || property.empty() || !std::isfinite(value))
        {
            detail::set_error("set_model_property requires a model, property name and finite value");
            return false;
        }
        const auto condition_uses_property = [&](const auto &self, const ModelCondition &condition) -> bool {
            return condition.property == property ||
                   std::any_of(condition.children.begin(), condition.children.end(),
                               [&](const ModelCondition &child) { return self(self, child); });
        };
        const bool found = std::any_of(model->animations.begin(), model->animations.end(),
                                       [&](const ModelAnimation &animation) { return animation.property == property; }) ||
                           std::any_of(model->visibility.begin(), model->visibility.end(),
                                       [&](const ModelVisibility &visibility) {
                                           return condition_uses_property(condition_uses_property, visibility.condition);
                                       });
        if (!found)
        {
            detail::set_error("Model has no animation using property: " + property);
            return false;
        }
        model->animation_properties[property] = value;
        return true;
    }

    namespace detail
    {
        glm::mat4 model_animation_transform(const Model &model, const ModelPart &part)
        {
            glm::mat4 result(1.0f);
            const double elapsed_seconds = std::chrono::duration<double>(
                                               std::chrono::steady_clock::now().time_since_epoch())
                                               .count();
            for (const ModelAnimation &animation : model.animations)
            {
                if (animation.type == ModelAnimationType::blend)
                    continue;
                if (std::find(animation.objects.begin(), animation.objects.end(), part.name) == animation.objects.end())
                    continue;
                const auto property = model.animation_properties.find(animation.property);
                const float input = property == model.animation_properties.end() ? 0.0f : property->second;
                result = animation_transform(animation, animation_value(animation, input, elapsed_seconds)) * result;
            }
            return result;
        }

        float model_animation_alpha(const Model &model, const ModelPart &part)
        {
            float alpha = part.material.tint.alpha / 255.0f;
            for (const ModelAnimation &animation : model.animations)
            {
                if (animation.type != ModelAnimationType::blend ||
                    std::find(animation.objects.begin(), animation.objects.end(), part.name) == animation.objects.end())
                    continue;
                const auto property = model.animation_properties.find(animation.property);
                const float input = property == model.animation_properties.end() ? 0.0f : property->second;
                alpha = std::clamp(animation_value(animation, input), animation.minimum, animation.maximum);
            }
            return alpha;
        }

        bool model_part_visible(const Model &model, const ModelPart &part)
        {
            return part_is_visible(model, part);
        }
    }
} // namespace sl

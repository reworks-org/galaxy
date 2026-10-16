#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sl
{

    /** A perspective camera using right-handed GLM coordinates. */
    struct Camera
    {
        /** Camera position in right-handed world coordinates. */
        glm::vec3 position{0.0f, 0.0f, 3.0f};
        /** Point the camera looks toward. */
        glm::vec3 target{0.0f, 0.0f, 0.0f};
        /** Camera's upward direction. */
        glm::vec3 up{0.0f, 1.0f, 0.0f};
        /** Vertical field of view in degrees. */
        float field_of_view = 60.0f;
        /** Near clipping distance. */
        float near_plane = 0.1f;
        /** Far clipping distance. */
        float far_plane = 1000.0f;

        /** Build the camera view matrix. */
        glm::mat4 view_matrix() const
        {
            return glm::lookAt(position, target, up);
        }

        /** Build a perspective projection matrix. */
        glm::mat4 projection_matrix(float aspect_ratio) const
        {
            return glm::perspective(glm::radians(field_of_view), aspect_ratio, near_plane, far_plane);
        }
    };

    /** Build a model matrix from translation, Euler rotation, and scale. */
    inline glm::mat4 model_matrix(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale = glm::vec3(1.0f))
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        return glm::scale(model, scale);
    }

} // namespace sl

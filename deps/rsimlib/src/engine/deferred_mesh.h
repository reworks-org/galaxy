#pragma once

#include "surface3d.h"

#include <vector>

namespace sl::detail
{
    /** A mesh built on a worker thread whose GPU upload is still to be done. */
    struct DeferredMesh
    {
        Mesh *mesh = nullptr;
        bool retain_cpu = false; ///< Keep Mesh::vertices / indices after the upload, as create_mesh() would have.
    };

    /**
     * While alive, create_mesh() on this thread validates and prepares the geometry but does not touch the graphics
     * backend: it returns a Mesh with handle 0 and records it in `out`. Used by the async loaders.
     */
    class DeferredMeshScope
    {
    public:
        explicit DeferredMeshScope(std::vector<DeferredMesh> &out);
        ~DeferredMeshScope();
        DeferredMeshScope(const DeferredMeshScope &) = delete;
        DeferredMeshScope &operator=(const DeferredMeshScope &) = delete;

    private:
        std::vector<DeferredMesh> *previous_;
    };

    /** Create the GPU buffers for a deferred mesh. Main thread only. */
    bool upload_deferred_mesh(const DeferredMesh &deferred);
} // namespace sl::detail

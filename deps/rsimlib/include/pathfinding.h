// pathfinding.h - single-file tile-grid flow fields (stb style)
//
// In exactly one source file:
//     #define PATHFINDING_IMPLEMENTATION
//     #include "pathfinding.h"
//
// Optionally define PATHFINDING_RANDOM() before the implementation include. It must return a float in
// [0, 1) and is used to break ties between equally good steps. Defaults to std::rand().
//
// Enemies never search; they read the neighbouring cells of a layer and step to the best one:
// downhill on the chase distance field, uphill on the scent and sound intensities.

#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <vector>

enum class PathLayer { chase, scent, sound };

struct TileStep
{
    int dx{0};
    int dy{0};
};

// Returns true if the rectangle at (x, y) with the given size is blocked.
using BlockedFn = bool (*)(float x, float y, float width, float height);

class Pathfinder
{
  public:
    void resize(int columns, int rows, float tile_width, float tile_height, BlockedFn blocked);
    // Call once per frame; rebuilds the walkable grid, the chase field and updates scent and sound.
    void update(int player_col, int player_row, bool player_moving);
    // Returns false when the layer offers no usable direction from this tile.
    bool next_step(PathLayer layer, int col, int row, TileStep &step) const;
    bool open(int col, int row) const;
    float value(PathLayer layer, int col, int row) const;
    int columns() const { return column_count; }
    int rows() const { return row_count; }
    float tile_width() const { return tile_w; }
    float tile_height() const { return tile_h; }

  private:
    int index(int col, int row) const { return row * column_count + col; }
    bool in_bounds(int col, int row) const { return col >= 0 && row >= 0 && col < column_count && row < row_count; }
    // Breadth-first distance from a tile through open tiles; unreachable cells stay at -1.
    void flood(int col, int row, std::vector<int> &distance) const;

    int column_count{0};
    int row_count{0};
    float tile_w{0.0f};
    float tile_h{0.0f};
    BlockedFn blocked_fn{nullptr};
    std::vector<unsigned char> walkable;
    std::vector<int> chase;
    std::vector<float> scent;
    std::vector<float> sound;
};

#endif // PATHFINDING_H

#ifdef PATHFINDING_IMPLEMENTATION
#ifndef PATHFINDING_IMPLEMENTATION_DONE
#define PATHFINDING_IMPLEMENTATION_DONE

#include <algorithm>
#include <cstdlib>
#include <queue>

#ifndef PATHFINDING_RANDOM
#define PATHFINDING_RANDOM() ((float)std::rand() / ((float)RAND_MAX + 1.0f))
#endif

namespace pathfinding_detail
{
const int kDirs[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
const float kScentDecay = 0.997f;
const float kScentThreshold = 0.05f;
const float kSoundDecay = 0.95f;
const float kSoundThreshold = 0.05f;
const int kSoundRadius = 6;
} // namespace pathfinding_detail

void Pathfinder::resize(int columns, int rows, float tile_width, float tile_height, BlockedFn blocked)
{
    column_count = columns;
    row_count = rows;
    tile_w = tile_width;
    tile_h = tile_height;
    blocked_fn = blocked;
    const size_t count = (size_t)columns * rows;
    walkable.assign(count, 1);
    chase.assign(count, -1);
    scent.assign(count, 0.0f);
    sound.assign(count, 0.0f);
}

bool Pathfinder::open(int col, int row) const
{
    return in_bounds(col, row) && walkable[index(col, row)];
}

void Pathfinder::flood(int col, int row, std::vector<int> &distance) const
{
    std::fill(distance.begin(), distance.end(), -1);
    if (!in_bounds(col, row))
        return;
    std::queue<std::pair<int, int>> frontier;
    distance[index(col, row)] = 0;
    frontier.push({col, row});
    while (!frontier.empty())
    {
        const auto [c, r] = frontier.front();
        frontier.pop();
        const int d = distance[index(c, r)];
        for (const auto &dir : pathfinding_detail::kDirs)
        {
            const int nc = c + dir[0];
            const int nr = r + dir[1];
            if (!open(nc, nr) || distance[index(nc, nr)] != -1)
                continue;
            distance[index(nc, nr)] = d + 1;
            frontier.push({nc, nr});
        }
    }
}

void Pathfinder::update(int player_col, int player_row, bool player_moving)
{
    using namespace pathfinding_detail;
    if (column_count <= 0 || row_count <= 0 || !blocked_fn)
        return;
    for (int r = 0; r < row_count; ++r)
        for (int c = 0; c < column_count; ++c)
            walkable[index(c, r)] = blocked_fn(c * tile_w, r * tile_h, tile_w, tile_h) ? 0 : 1;

    // the player's own tile is always a valid source, even if a wall was placed on it
    if (in_bounds(player_col, player_row))
        walkable[index(player_col, player_row)] = 1;

    flood(player_col, player_row, chase);

    for (auto &v : scent)
        v *= kScentDecay;
    if (in_bounds(player_col, player_row))
        scent[index(player_col, player_row)] = 1.0f;

    for (auto &v : sound)
        v *= kSoundDecay;
    if (player_moving)
    {
        // a pulse spreads through open tiles only, so walls block it
        for (int i = 0; i < (int)chase.size(); ++i)
        {
            const int d = chase[i];
            if (d < 0 || d > kSoundRadius)
                continue;
            const float pulse = 1.0f - (float)d / (float)(kSoundRadius + 1);
            sound[i] = std::max(sound[i], pulse);
        }
    }
}

float Pathfinder::value(PathLayer layer, int col, int row) const
{
    if (!in_bounds(col, row))
        return 0.0f;
    switch (layer)
    {
    case PathLayer::chase:
        return (float)chase[index(col, row)];
    case PathLayer::scent:
        return scent[index(col, row)];
    case PathLayer::sound:
        return sound[index(col, row)];
    }
    return 0.0f;
}

bool Pathfinder::next_step(PathLayer layer, int col, int row, TileStep &step) const
{
    using namespace pathfinding_detail;
    if (!in_bounds(col, row))
        return false;
    const bool downhill = layer == PathLayer::chase;
    const float threshold = layer == PathLayer::scent ? kScentThreshold : kSoundThreshold;
    const float here = value(layer, col, row);
    if (downhill && here < 0.0f)
        return false;

    float best_value = here;
    std::vector<TileStep> best;
    for (const auto &dir : kDirs)
    {
        const int nc = col + dir[0];
        const int nr = row + dir[1];
        if (!open(nc, nr))
            continue;
        const float v = value(layer, nc, nr);
        if (downhill ? v < 0.0f : v < threshold)
            continue;
        const bool improves = downhill ? v < best_value : v > best_value;
        if (improves)
        {
            best_value = v;
            best.clear();
        }
        if (v == best_value && (improves || !best.empty()))
            best.push_back({dir[0], dir[1]});
    }
    if (best.empty())
        return false;
    // random tie-break so enemies don't all take the same route
    const size_t pick = best.size() == 1 ? 0 : (size_t)(PATHFINDING_RANDOM() * best.size()) % best.size();
    step = best[pick];
    return true;
}

#endif // PATHFINDING_IMPLEMENTATION_DONE
#endif // PATHFINDING_IMPLEMENTATION

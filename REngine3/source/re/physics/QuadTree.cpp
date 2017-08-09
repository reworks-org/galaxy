#include "QuadTree.hpp"

QuadTree::QuadTree(int newLevel, Rectangle rect)
: level(newLevel), boundingBox(rect) {
}

QuadTree::~QuadTree() {
    clear();
}

/*
 * clear the entities in the current quadtree
 * loop through the children
 *   clear them
 * clear the children
 */
void QuadTree::clear() {
    entities.clear();
    for (auto& i : nodes) {
        i->clear();
    }
    nodes.clear();
}

/*
 * if this quadtree has children
 *   get the quadrant
 *   insert it in the correct quadtree
 * put the entity into the list
 * if there is to many entities and current level is less than the max level
 *   if this quadtree does not have children
 *     split into 4 different quadrants
 *   loop through the entities and insert them into the correct quadtree
 *
 */
void QuadTree::insert(const std::shared_ptr<Entity>& entity) {
    if (hasChildren()) {
        int index = getIndex(entity->getRect());
        if (index != NO_QUADRANT) {
            nodes[index]->insert(entity);
            return;
        }
    }
    entities.push_back(std::weak_ptr<Entity>(entity));
    if (entities.size() > MAX_ENTITIES && level < MAX_LEVELS) {
        if (!hasChildren()) {
            split();
        }
        int index;
        std::weak_ptr<Entity> currentEntity;
        for (unsigned i = 0; i < entities.size(); ) {
            index = getIndex(entities.at(i).lock()->getRect());
            if (index != NO_QUADRANT) {
                currentEntity = entities.at(i);
                entities.erase(entities.begin() + i);
                nodes[index]->insert(currentEntity.lock());
            } else {
                i++;
            }
        }
    }
}

/*
 * Sets up for retrieving the possible collisions
 */
std::vector<std::weak_ptr<Entity>> QuadTree::retrievePossibleCollisions(std::shared_ptr<Entity>& entity) {
    std::vector<std::weak_ptr<Entity>> possibleCollisions;
    return retrieve(possibleCollisions, std::weak_ptr<Entity>(entity));
}

/*
 * gets the quadrant the entity is in
 * if it is in a quadrant and the node you are in has children
 *   recurse on the nodes index retrieve
 * else
 *   populate the returning list and return it
 */
std::vector<std::weak_ptr<Entity>>& QuadTree::retrieve(std::vector<std::weak_ptr<Entity>>& returnEntities,
                                                       const std::weak_ptr<Entity>& entity) {
    int index = getIndex(entity.lock()->getRect());
    if (index != NO_QUADRANT && hasChildren()) {
        nodes[index]->retrieve(returnEntities, entity);
    }
    for (auto& i : entities) {
        returnEntities.push_back(i);
    }
    return returnEntities;
}

bool QuadTree::hasChildren() {
    return nodes.size() > 0;
}

/*
 *    II  |  I
 *    ---------
 *    III | IV
 *
 * Splits into 4 quadrants with half the size
 * level = 0
 * rect: x = 0, y = 0, width = 600, height = 600
 * splits into
 * quadrant I level = 1
 * quadrant I rect: x = 300, y = 0, width = 300, height = 300
 * quadrant II level = 1
 * quadrant II rect: x = 0, y = 0, width = 300, height = 300
 * quadrant III level = 1
 * quadrant III rect: x = 0, y = 300, width = 300, height = 300
 * quadrant IV level = 1
 * quadrant IV rect: x = 300, y = 300, width = 300, height = 300
 * and so on
 */
void QuadTree::split() {
    Vec2f subSize = {boundingBox.getWidth() / 2, boundingBox.getHeight() / 2};
    Vec2f position = boundingBox.getPosition();
    nodes.push_back(std::shared_ptr<QuadTree>(new QuadTree(level+1, {position.x + subSize.x, position.y, subSize})));
    nodes.push_back(std::shared_ptr<QuadTree>(new QuadTree(level+1, {position, subSize})));
    nodes.push_back(std::shared_ptr<QuadTree>(new QuadTree(level+1, {position.x, position.y + subSize.y, subSize})));
    nodes.push_back(std::shared_ptr<QuadTree>(new QuadTree(level+1, {position.x + subSize.x, position.y + subSize.y, subSize})));
}

/*
 * Determines which quadrant the rectangle is in
 * Checks the bounds of each sub quadrant
 */
int QuadTree::getIndex(Rectangle& rect) {
    int index = NO_QUADRANT;
    Vec2f midPoint = boundingBox.getCenter();
    // Top quadrants
    bool topQuadrant = rect.getPosition().y + rect.getHeight() < midPoint.y;
    // Bottom quadrants
    bool bottomQuadrant = rect.getPosition().y > midPoint.y;
    
    // Left quadrants
    if (rect.getPosition().x + rect.getWidth() < midPoint.x) {
        if (topQuadrant) {
            index = QUADRANT_2;
        } else if (bottomQuadrant) {
            index = QUADRANT_3;
        }
    }
    // Right quadrants
    else if (rect.getPosition().x > midPoint.x) {
        if (topQuadrant) {
            index = QUADRANT_1;
        } else if (bottomQuadrant) {
            index = QUADRANT_4;
        }
    }
    return index;
}

/*
 * For debugging
 * Visit the node first and then visit its children
 * Uses tabs to show which level you are on
 */
void QuadTree::preOrderTraversal() {
    for (int i = 0; i < level; i++) {
        std::cout << "\t";
    }
    std::cout << "Entity count: " << entities.size() << "\n";
    for (int i = 0; i < level; i++) {
        std::cout << "\t";
    }
    std::cout << "Bounds: " << boundingBox << "\n";
    for (auto& i : nodes) {
        i->preOrderTraversal();
    }
}

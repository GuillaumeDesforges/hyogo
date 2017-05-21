#include "Entity.h"

Entity::Entity(AssetId assetId, const std::string &name, EntityType type, int maxHp, int viewRange)
        : assetId(assetId),
          name(name),
          type(type),
          maxHp(maxHp),
          hp(maxHp),
          viewRange(viewRange),
          position(Vect2D(0, 0)) {}

void Entity::draw(const ResourceManager &resourceManager, const Vect2D &viewOffset) const {
    Vect2D renderPosition = position * MAP_SQUARE_PIXEL_SIZE + viewOffset;
    resourceManager.getResource(assetId).draw(renderPosition);
    AlphaColor c = (owner == PLAYER1) ? ACYAN : AMAGENTA;
    Imagine::drawRect(renderPosition, MAP_SQUARE_PIXEL_SIZE - 1, MAP_SQUARE_PIXEL_SIZE - 1, c);
}

void Entity::drawSelectionBox(const Vect2D &viewOffset) const {
    AlphaColor c(0, 0, 255, 100);
    Vect2D renderPosition = position * MAP_SQUARE_PIXEL_SIZE + viewOffset;
    Imagine::fillRect(renderPosition.x(), renderPosition.y(), MAP_SQUARE_PIXEL_SIZE, MAP_SQUARE_PIXEL_SIZE, c);
}

void Entity::drawIsFinishedTurnBox(const Vect2D &viewOffset) const {
    AlphaColor c(255, 0, 0, 100);
    Vect2D renderPosition = position * MAP_SQUARE_PIXEL_SIZE + viewOffset;
    Imagine::fillRect(renderPosition.x(), renderPosition.y(), MAP_SQUARE_PIXEL_SIZE, MAP_SQUARE_PIXEL_SIZE, c);
}

void Entity::setPosition(const Vect2D &position) {
    this->position = position;
}

const Vect2D &Entity::getPosition() const {
    return position;
}

EntityType Entity::getType() const {
    return type;
}

void Entity::setFinishedTurn(bool finishedTurn) {
    this->finishedTurn = finishedTurn;
}

const bool Entity::isFinishedTurn() const {
    return finishedTurn;
}

void Entity::setOwner(Player player) {
    owner = player;
}

const Player Entity::getOwner() const {
    return owner;
}

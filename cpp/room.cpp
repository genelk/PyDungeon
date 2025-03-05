#include "room.h"

Room::Room(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

bool Room::overlaps(const Room& other, int padding) const {
    // Check if this room overlaps with another room
    // We add padding to create a buffer zone around rooms
    return (
        x < other.x + other.width + padding &&
        x + width + padding > other.x &&
        y < other.y + other.height + padding &&
        y + height + padding > other.y
    );
}

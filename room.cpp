#include "room.h"
#include <algorithm>

namespace pydungeon {

Room::Room(int x, int y, int width, int height)
    : m_x(x), m_y(y), m_width(width), m_height(height) {}

bool Room::overlaps(const Room& other, int padding) const {
    // Check if this room overlaps with another room
    return (m_x < other.m_x + other.m_width + padding &&
            m_x + m_width + padding > other.m_x &&
            m_y < other.m_y + other.m_height + padding &&
            m_y + m_height + padding > other.m_y);
}

} // namespace pydungeon

#include "corridor.h"

namespace pydungeon {

Corridor::Corridor(int startX, int startY, int endX, int endY, CorridorType type)
    : m_startX(startX), m_startY(startY), m_endX(endX), m_endY(endY), m_type(type) {}

std::pair<int, int> Corridor::getCornerPoint() const {
    if (m_type == HORIZONTAL_THEN_VERTICAL) {
        return std::make_pair(m_endX, m_startY);
    } else {
        return std::make_pair(m_startX, m_endY);
    }
}

} // namespace pydungeon

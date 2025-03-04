#pragma once

#include <utility>

namespace pydungeon {

/**
 * @brief The Corridor class represents a corridor connecting two points in the dungeon.
 */
class Corridor {
public:
    enum CorridorType {
        HORIZONTAL_THEN_VERTICAL,
        VERTICAL_THEN_HORIZONTAL
    };

    /**
     * @brief Construct a new Corridor object
     * 
     * @param startX Starting X-coordinate
     * @param startY Starting Y-coordinate
     * @param endX Ending X-coordinate
     * @param endY Ending Y-coordinate
     * @param type The type of L-shaped corridor
     */
    Corridor(int startX, int startY, int endX, int endY, CorridorType type);

    /**
     * @brief Get the starting X-coordinate
     * 
     * @return int 
     */
    int getStartX() const { return m_startX; }

    /**
     * @brief Get the starting Y-coordinate
     * 
     * @return int 
     */
    int getStartY() const { return m_startY; }

    /**
     * @brief Get the ending X-coordinate
     * 
     * @return int 
     */
    int getEndX() const { return m_endX; }

    /**
     * @brief Get the ending Y-coordinate
     * 
     * @return int 
     */
    int getEndY() const { return m_endY; }

    /**
     * @brief Get the corridor type
     * 
     * @return CorridorType 
     */
    CorridorType getType() const { return m_type; }

    /**
     * @brief Get the corner point where the corridor bends
     * 
     * @return std::pair<int, int> The x,y coordinates of the bend
     */
    std::pair<int, int> getCornerPoint() const;

private:
    int m_startX;
    int m_startY;
    int m_endX;
    int m_endY;
    CorridorType m_type;
};

} // namespace pydungeon

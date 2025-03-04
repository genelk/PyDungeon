#pragma once

namespace pydungeon {

/**
 * @brief The Room class represents a rectangular room in the dungeon.
 */
class Room {
public:
    /**
     * @brief Construct a new Room object
     * 
     * @param x X-coordinate of the top-left corner
     * @param y Y-coordinate of the top-left corner
     * @param width Width of the room
     * @param height Height of the room
     */
    Room(int x, int y, int width, int height);

    /**
     * @brief Get the x-coordinate of the room
     * 
     * @return int The x-coordinate
     */
    int getX() const { return m_x; }
    
    /**
     * @brief Get the y-coordinate of the room
     * 
     * @return int The y-coordinate
     */
    int getY() const { return m_y; }
    
    /**
     * @brief Get the width of the room
     * 
     * @return int The width
     */
    int getWidth() const { return m_width; }
    
    /**
     * @brief Get the height of the room
     * 
     * @return int The height
     */
    int getHeight() const { return m_height; }
    
    /**
     * @brief Get the center x-coordinate of the room
     * 
     * @return int The center x-coordinate
     */
    int getCenterX() const { return m_x + m_width / 2; }
    
    /**
     * @brief Get the center y-coordinate of the room
     * 
     * @return int The center y-coordinate
     */
    int getCenterY() const { return m_y + m_height / 2; }
    
    /**
     * @brief Check if this room overlaps with another room
     * 
     * @param other The other room to check against
     * @param padding Optional padding around rooms (default: 1)
     * @return true If the rooms overlap
     * @return false If the rooms do not overlap
     */
    bool overlaps(const Room& other, int padding = 1) const;

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
};

} // namespace pydungeon

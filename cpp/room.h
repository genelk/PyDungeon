#pragma once

class Room {
public:
    /**
     * @brief Construct a new Room
     * 
     * @param x X-coordinate of the top-left corner
     * @param y Y-coordinate of the top-left corner
     * @param width Width of the room
     * @param height Height of the room
     */
    Room(int x, int y, int width, int height);
    
    /**
     * @brief Get the x-coordinate
     */
    int getX() const { return x; }
    
    /**
     * @brief Get the y-coordinate
     */
    int getY() const { return y; }
    
    /**
     * @brief Get the width of the room
     */
    int getWidth() const { return width; }
    
    /**
     * @brief Get the height of the room
     */
    int getHeight() const { return height; }
    
    /**
     * @brief Get the center x-coordinate
     */
    int getCenterX() const { return x + width / 2; }
    
    /**
     * @brief Get the center y-coordinate
     */
    int getCenterY() const { return y + height / 2; }
    
    /**
     * @brief Check if this room overlaps with another room
     * 
     * @param other Another room to check for overlap
     * @param padding Additional padding around rooms (default: 1)
     * @return true if rooms overlap, false otherwise
     */
    bool overlaps(const Room& other, int padding = 1) const;

private:
    int x;
    int y;
    int width;
    int height;
};

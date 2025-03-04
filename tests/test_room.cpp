#include <gtest/gtest.h>
#include "room.h"

using namespace pydungeon;

TEST(RoomTest, Construction) {
    Room room(10, 20, 30, 40);
    
    EXPECT_EQ(room.getX(), 10);
    EXPECT_EQ(room.getY(), 20);
    EXPECT_EQ(room.getWidth(), 30);
    EXPECT_EQ(room.getHeight(), 40);
}

TEST(RoomTest, CenterCalculation) {
    Room room(10, 20, 30, 40);
    
    EXPECT_EQ(room.getCenterX(), 25); // 10 + 30/2
    EXPECT_EQ(room.getCenterY(), 40); // 20 + 40/2
}

TEST(RoomTest, Overlapping) {
    // Test cases for room overlap detection
    
    // Case 1: Completely separate rooms
    Room room1(0, 0, 10, 10);
    Room room2(20, 20, 10, 10);
    EXPECT_FALSE(room1.overlaps(room2));
    EXPECT_FALSE(room2.overlaps(room1));
    
    // Case 2: Rooms touching at edges (should not be considered overlapping)
    Room room3(0, 0, 10, 10);
    Room room4(10, 0, 10, 10);
    EXPECT_FALSE(room3.overlaps(room4, 0)); // With no padding
    EXPECT_TRUE(room3.overlaps(room4, 1));  // With default padding of 1
    
    // Case 3: Rooms overlapping
    Room room5(5, 5, 10, 10);
    Room room6(10, 10, 10, 10);
    EXPECT_TRUE(room5.overlaps(room6));
    EXPECT_TRUE(room6.overlaps(room5));
    
    // Case 4: One room completely inside another
    Room room7(0, 0, 20, 20);
    Room room8(5, 5, 5, 5);
    EXPECT_TRUE(room7.overlaps(room8));
    EXPECT_TRUE(room8.overlaps(room7));
    
    // Case 5: Rooms separated by the padding distance
    Room room9(0, 0, 10, 10);
    Room room10(12, 0, 10, 10);
    EXPECT_FALSE(room9.overlaps(room10, 1));
    EXPECT_TRUE(room9.overlaps(room10, 3));
}

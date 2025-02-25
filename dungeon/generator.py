import numpy as np
import random

class DungeonGenerator:
    """
    Generates procedural dungeons inspired by Diablo's gothic aesthetic.
    
    The generator uses a room-based approach, placing rooms and connecting
    them with corridors to create a dungeon layout.
    """
    
    # Tile types
    EMPTY = 0
    WALL = 1
    FLOOR = 2
    CORRIDOR = 3
    DOOR = 4
    
    def __init__(self, width=50, height=50, min_rooms=5, max_rooms=15, 
                 min_room_size=5, max_room_size=15):
        """
        Initialize the dungeon generator.
        
        Args:
            width (int): Width of the dungeon grid
            height (int): Height of the dungeon grid
            min_rooms (int): Minimum number of rooms to generate
            max_rooms (int): Maximum number of rooms to generate
            min_room_size (int): Minimum size (width/height) of rooms
            max_room_size (int): Maximum size (width/height) of rooms
        """
        self.width = width
        self.height = height
        self.min_rooms = min_rooms
        self.max_rooms = max_rooms
        self.min_room_size = min_room_size
        self.max_room_size = max_room_size
    
    def generate(self):
        """
        Generate a new dungeon layout.
        
        Returns:
            numpy.ndarray: 2D array representing the dungeon layout
        """
        # Initialize dungeon with walls
        dungeon = np.ones((self.height, self.width), dtype=int) * self.WALL
        
        # List to store room locations
        rooms = []
        
        # Generate rooms
        num_rooms = random.randint(self.min_rooms, self.max_rooms)
        for _ in range(num_rooms):
            # Generate random room dimensions
            room_width = random.randint(self.min_room_size, self.max_room_size)
            room_height = random.randint(self.min_room_size, self.max_room_size)
            
            # Generate random room position (ensuring it's within dungeon bounds)
            x = random.randint(1, self.width - room_width - 1)
            y = random.randint(1, self.height - room_height - 1)
            
            # Check if room overlaps with existing rooms
            overlaps = False
            for other_room in rooms:
                ox, oy, ow, oh = other_room
                if (x < ox + ow + 1 and x + room_width + 1 > ox and
                    y < oy + oh + 1 and y + room_height + 1 > oy):
                    overlaps = True
                    break
            
            # If room doesn't overlap, add it to the dungeon
            if not overlaps:
                self._create_room(dungeon, x, y, room_width, room_height)
                rooms.append((x, y, room_width, room_height))
        
        # Connect rooms with corridors
        for i in range(len(rooms) - 1):
            # Get center of current room
            x1 = rooms[i][0] + rooms[i][2] // 2
            y1 = rooms[i][1] + rooms[i][3] // 2
            
            # Get center of next room
            x2 = rooms[i+1][0] + rooms[i+1][2] // 2
            y2 = rooms[i+1][1] + rooms[i+1][3] // 2
            
            # Create L-shaped corridor
            self._create_corridor(dungeon, x1, y1, x2, y2)
        
        return dungeon
    
    def _create_room(self, dungeon, x, y, width, height):
        """
        Create a rectangular room in the dungeon.
        
        Args:
            dungeon (numpy.ndarray): The dungeon grid
            x (int): X-coordinate of the top-left corner
            y (int): Y-coordinate of the top-left corner
            width (int): Width of the room
            height (int): Height of the room
        """
        for i in range(y, y + height):
            for j in range(x, x + width):
                dungeon[i, j] = self.FLOOR
    
    def _create_corridor(self, dungeon, x1, y1, x2, y2):
        """
        Create an L-shaped corridor between two points.
        
        Args:
            dungeon (numpy.ndarray): The dungeon grid
            x1 (int): X-coordinate of the first point
            y1 (int): Y-coordinate of the first point
            x2 (int): X-coordinate of the second point
            y2 (int): Y-coordinate of the second point
        """
        # Randomly decide whether to go horizontal then vertical, or vertical then horizontal
        if random.random() < 0.5:
            # Horizontal then vertical
            self._create_h_corridor(dungeon, x1, x2, y1)
            self._create_v_corridor(dungeon, y1, y2, x2)
        else:
            # Vertical then horizontal
            self._create_v_corridor(dungeon, y1, y2, x1)
            self._create_h_corridor(dungeon, x1, x2, y2)
    
    def _create_h_corridor(self, dungeon, x1, x2, y):
        """Create a horizontal corridor."""
        for x in range(min(x1, x2), max(x1, x2) + 1):
            if 0 <= y < self.height and 0 <= x < self.width:
                if dungeon[y, x] == self.WALL:
                    dungeon[y, x] = self.CORRIDOR
    
    def _create_v_corridor(self, dungeon, y1, y2, x):
        """Create a vertical corridor."""
        for y in range(min(y1, y2), max(y1, y2) + 1):
            if 0 <= y < self.height and 0 <= x < self.width:
                if dungeon[y, x] == self.WALL:
                    dungeon[y, x] = self.CORRIDOR

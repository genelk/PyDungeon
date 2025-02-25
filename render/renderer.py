import pygame
import numpy as np
from dungeon.generator import DungeonGenerator

class DungeonRenderer:
    """
    Handles rendering of the dungeon on screen.
    """
    
    def __init__(self, screen, tile_size=16):
        """
        Initialize the renderer.
        
        Args:
            screen (pygame.Surface): Pygame screen to render on
            tile_size (int): Size of each tile in pixels
        """
        self.screen = screen
        self.tile_size = tile_size
        
        # Define colors for different tile types
        self.colors = {
            DungeonGenerator.EMPTY: (0, 0, 0),        # Black
            DungeonGenerator.WALL: (80, 80, 80),      # Dark gray
            DungeonGenerator.FLOOR: (150, 120, 90),   # Brown (floor)
            DungeonGenerator.CORRIDOR: (120, 100, 80),# Darker brown (corridor)
            DungeonGenerator.DOOR: (120, 60, 30)      # Reddish brown (doors)
        }
    
    def render(self, dungeon):
        """
        Render the dungeon to the screen.
        
        Args:
            dungeon (numpy.ndarray): 2D array representing the dungeon
        """
        # Get screen dimensions
        screen_width, screen_height = self.screen.get_size()
        
        # Calculate the visible portion of the dungeon
        visible_width = screen_width // self.tile_size
        visible_height = screen_height // self.tile_size
        
        # Center the dungeon
        offset_x = (screen_width - min(dungeon.shape[1], visible_width) * self.tile_size) // 2
        offset_y = (screen_height - min(dungeon.shape[0], visible_height) * self.tile_size) // 2
        
        # Draw each tile
        for y in range(min(dungeon.shape[0], visible_height)):
            for x in range(min(dungeon.shape[1], visible_width)):
                tile_type = dungeon[y, x]
                color = self.colors.get(tile_type, (255, 0, 255))  # Default to magenta for unknown types
                
                # Draw rectangle for the tile
                rect = pygame.Rect(
                    offset_x + x * self.tile_size,
                    offset_y + y * self.tile_size,
                    self.tile_size,
                    self.tile_size
                )
                pygame.draw.rect(self.screen, color, rect)
                
                # Draw outline for walls
                if tile_type == DungeonGenerator.WALL:
                    pygame.draw.rect(self.screen, (50, 50, 50), rect, 1)

import pygame
import numpy as np
from typing import Tuple, Dict, Any, Optional

class DungeonRenderer:
    """
    Handles rendering of the dungeon on screen.
    """
    
    def __init__(self, screen: pygame.Surface, tile_size: int = 16):
        """
        Initialize the renderer.
        
        Args:
            screen: Pygame screen to render on
            tile_size: Size of each tile in pixels
        """
        self.screen = screen
        self.tile_size = tile_size
        
        # Define colors for different tile types
        self.colors = {
            0: (0, 0, 0),        # Black (EMPTY)
            1: (80, 80, 80),      # Dark gray (WALL)
            2: (150, 120, 90),   # Brown (FLOOR)
            3: (120, 100, 80),   # Darker brown (CORRIDOR)
            4: (120, 60, 30)     # Reddish brown (DOOR)
        }
        
        # Performance stats for display
        self.stats = {}
    
    def render(self, dungeon: np.ndarray, stats: Optional[Dict[str, Any]] = None) -> None:
        """
        Render the dungeon to the screen.
        
        Args:
            dungeon: 2D array representing the dungeon
            stats: Optional dictionary of stats to display (e.g., generation time)
        """
        if stats:
            self.stats = stats
            
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
                if tile_type == 1:  # WALL
                    pygame.draw.rect(self.screen, (50, 50, 50), rect, 1)
        
        # Render stats if available
        self._render_stats()
        
    def _render_stats(self) -> None:
        """Render performance statistics on the screen."""
        if not self.stats:
            return
            
        # Initialize font if not already done
        if not hasattr(self, 'font'):
            pygame.font.init()
            self.font = pygame.font.SysFont('Arial', 16)
            
        # Prepare stats text
        y_offset = 10
        
        # Implementation type
        impl_type = "C++ Implementation" if self.stats.get('use_cpp', False) else "Python Implementation"
        impl_surface = self.font.render(impl_type, True, (255, 255, 255))
        self.screen.blit(impl_surface, (10, y_offset))
        y_offset += 20
        
        # Generation time
        if 'generation_time' in self.stats:
            time_text = f"Generation time: {self.stats['generation_time']:.4f} seconds"
            time_surface = self.font.render(time_text, True, (255, 255, 255))
            self.screen.blit(time_surface, (10, y_offset))
            y_offset += 20
            
        # Dungeon size
        if 'width' in self.stats and 'height' in self.stats:
            size_text = f"Dungeon size: {self.stats['width']}x{self.stats['height']}"
            size_surface = self.font.render(size_text, True, (255, 255, 255))
            self.screen.blit(size_surface, (10, y_offset))
            y_offset += 20
            
        # Room count if available
        if 'room_count' in self.stats:
            room_text = f"Rooms: {self.stats['room_count']}"
            room_surface = self.font.render(room_text, True, (255, 255, 255))
            self.screen.blit(room_surface, (10, y_offset))

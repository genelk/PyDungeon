import pygame
import sys
import numpy as np
import os

def load_dungeon(filename):
    """Load a dungeon from a file."""
    try:
        with open(filename, 'r') as f:
            lines = f.readlines()
            
        # First line contains width and height
        width, height = map(int, lines[0].strip().split())
        
        # Create numpy array for the dungeon
        dungeon = np.zeros((height, width), dtype=int)
        
        # Read the dungeon data
        for y in range(height):
            row = lines[y + 1].strip().split()
            for x in range(width):
                dungeon[y, x] = int(row[x])
                
        return dungeon
    except Exception as e:
        print(f"Error loading dungeon: {e}")
        return None

def main():
    # Initialize pygame
    pygame.init()
    screen = pygame.display.set_mode((800, 600))
    pygame.display.set_caption("Dungeon Visualizer")
    clock = pygame.time.Clock()
    
    # File path - check parent directory if not in current directory
    filename = "dungeon.txt"
    if not os.path.exists(filename):
        parent_filename = os.path.join("..", filename)
        if os.path.exists(parent_filename):
            filename = parent_filename
    
    # Load the dungeon
    dungeon = load_dungeon(filename)
    if dungeon is None:
        print(f"Could not load dungeon file. Make sure '{filename}' exists.")
        pygame.quit()
        sys.exit(1)
        
    print(f"Loaded dungeon with size {dungeon.shape[1]}x{dungeon.shape[0]}")
    
    # Set up colors for different tile types
    colors = {
        0: (0, 0, 0),        # Black (EMPTY)
        1: (80, 80, 80),     # Dark gray (WALL)
        2: (150, 120, 90),   # Brown (FLOOR)
        3: (120, 100, 80),   # Darker brown (CORRIDOR)
        4: (120, 60, 30)     # Reddish brown (DOOR)
    }
    
    # Tile size - dynamically calculated based on dungeon size and screen size
    screen_width, screen_height = screen.get_size()
    dungeon_width, dungeon_height = dungeon.shape[1], dungeon.shape[0]
    
    # Calculate tile size to fit the dungeon on screen with some padding
    max_tile_size_x = (screen_width - 40) // dungeon_width
    max_tile_size_y = (screen_height - 80) // dungeon_height
    tile_size = min(max_tile_size_x, max_tile_size_y)
    
    # Ensure minimum tile size
    tile_size = max(tile_size, 4)
    
    # Font for text rendering
    font = pygame.font.SysFont('Arial', 16)
    title_text = f"Dungeon ({dungeon_width}x{dungeon_height})"
    title_surface = font.render(title_text, True, (255, 255, 255))
    help_text = "Press R to reload | ESC to exit"
    help_surface = font.render(help_text, True, (200, 200, 200))
    
    # Main loop
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False
                elif event.key == pygame.K_r:
                    # Reload the dungeon
                    new_dungeon = load_dungeon(filename)
                    if new_dungeon is not None:
                        dungeon = new_dungeon
                        print(f"Reloaded dungeon with size {dungeon.shape[1]}x{dungeon.shape[0]}")
                        
                        # Recalculate tile size
                        dungeon_width, dungeon_height = dungeon.shape[1], dungeon.shape[0]
                        max_tile_size_x = (screen_width - 40) // dungeon_width
                        max_tile_size_y = (screen_height - 80) // dungeon_height
                        tile_size = min(max_tile_size_x, max_tile_size_y)
                        tile_size = max(tile_size, 4)
                        
                        # Update title
                        title_text = f"Dungeon ({dungeon_width}x{dungeon_height})"
                        title_surface = font.render(title_text, True, (255, 255, 255))
        
        # Clear screen
        screen.fill((30, 30, 30))
        
        # Calculate the position to center the dungeon
        offset_x = (screen_width - dungeon_width * tile_size) // 2
        offset_y = (screen_height - dungeon_height * tile_size) // 2 + 20  # Add space for title
        
        # Draw each tile
        for y in range(dungeon_height):
            for x in range(dungeon_width):
                tile_type = dungeon[y, x]
                color = colors.get(tile_type, (255, 0, 255))  # Default to magenta for unknown types
                
                rect = pygame.Rect(
                    offset_x + x * tile_size,
                    offset_y + y * tile_size,
                    tile_size,
                    tile_size
                )
                pygame.draw.rect(screen, color, rect)
                
                # Draw outline for walls (only if tile size is big enough)
                if tile_type == 1 and tile_size >= 6:  # WALL
                    pygame.draw.rect(screen, (50, 50, 50), rect, 1)
        
        # Display title and help text
        screen.blit(title_surface, (screen_width // 2 - title_surface.get_width() // 2, 10))
        screen.blit(help_surface, (screen_width // 2 - help_surface.get_width() // 2, 
                                  screen_height - help_surface.get_height() - 10))
        
        # Update display
        pygame.display.flip()
        clock.tick(60)
    
    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()

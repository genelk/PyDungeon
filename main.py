import pygame
import sys
from dungeon.generator import DungeonGenerator
from render.renderer import DungeonRenderer

def main():
    # Initialize pygame
    pygame.init()
    screen = pygame.display.set_mode((800, 600))
    pygame.display.set_caption("PyDungeon - Diablo-inspired Dungeon Generator")
    clock = pygame.time.Clock()
    
    # Generate a dungeon
    generator = DungeonGenerator(width=50, height=50)
    dungeon = generator.generate()
    
    # Set up renderer
    renderer = DungeonRenderer(screen, tile_size=16)
    
    # Main game loop
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    dungeon = generator.generate()  # Generate new dungeon on space
                if event.key == pygame.K_ESCAPE:
                    running = False
        
        # Clear screen
        screen.fill((0, 0, 0))
        
        # Render dungeon
        renderer.render(dungeon)
        
        # Update display
        pygame.display.flip()
        clock.tick(60)
    
    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()

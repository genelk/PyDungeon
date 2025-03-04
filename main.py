import pygame
import sys
import argparse
import numpy as np
from pydungeon.dungeon_wrapper import DungeonGenerator, benchmark_comparison
from pydungeon.visualization import DungeonRenderer

def main():
    # Parse command line arguments
    parser = argparse.ArgumentParser(description="PyDungeon - Procedural Dungeon Generator")
    parser.add_argument("--width", type=int, default=50, help="Width of the dungeon")
    parser.add_argument("--height", type=int, default=50, help="Height of the dungeon")
    parser.add_argument("--python", action="store_true", help="Force Python implementation")
    parser.add_argument("--benchmark", action="store_true", help="Run benchmarks and display results")
    parser.add_argument("--seed", type=int, default=None, help="Random seed for dungeon generation")
    args = parser.parse_args()
    
    # Initialize pygame
    pygame.init()
    screen = pygame.display.set_mode((800, 600))
    pygame.display.set_caption("PyDungeon - Diablo-inspired Dungeon Generator")
    clock = pygame.time.Clock()
    
    # Set up renderer
    renderer = DungeonRenderer(screen, tile_size=16)
    
    # Run benchmark if requested
    if args.benchmark:
        print("Running benchmark...")
        results = benchmark_comparison(args.width, args.height, iterations=5)
        
        print(f"\nBenchmark Results (Dungeon size: {args.width}x{args.height}):")
        print("-" * 50)
        
        if "python" in results:
            print(f"Python implementation: {results['python']['avg_time']:.6f} seconds")
        
        if "cpp" in results:
            print(f"C++ implementation:    {results['cpp']['avg_time']:.6f} seconds")
        
        if "speedup" in results:
            print(f"Speedup:               {results['speedup']:.2f}x")
        
        print("-" * 50)
    
    # Generate a dungeon
    use_cpp = not args.python
    generator = DungeonGenerator(
        width=args.width, 
        height=args.height,
        seed=args.seed,
        use_cpp=use_cpp
    )
    
    # First generation
    dungeon, generation_time = generator.generate()
    
    # Store stats for renderer
    stats = {
        'use_cpp': use_cpp,
        'generation_time': generation_time,
        'width': args.width,
        'height': args.height,
    }
    
    # Font for text rendering
    font = pygame.font.SysFont('Arial', 16)
    help_text = "Press SPACE to generate new dungeon | ESC to exit | B to toggle benchmark | P to toggle Python/C++"
    help_surface = font.render(help_text, True, (200, 200, 200))
    
    # Main game loop
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    # Generate new dungeon
                    dungeon, generation_time = generator.generate()
                    stats['generation_time'] = generation_time
                elif event.key == pygame.K_b:
                    # Run quick benchmark
                    print("Running quick benchmark...")
                    results = benchmark_comparison(args.width, args.height, iterations=3)
                    if "speedup" in results:
                        print(f"C++ is {results['speedup']:.2f}x faster than Python")
                elif event.key == pygame.K_p:
                    # Toggle between Python and C++ implementation
                    use_cpp = not use_cpp
                    generator = DungeonGenerator(
                        width=args.width, 
                        height=args.height,
                        seed=args.seed,
                        use_cpp=use_cpp
                    )
                    dungeon, generation_time = generator.generate()
                    stats['use_cpp'] = use_cpp
                    stats['generation_time'] = generation_time
                    print(f"Using {'C++' if use_cpp else 'Python'} implementation")
                elif event.key == pygame.K_ESCAPE:
                    running = False
        
        # Clear screen
        screen.fill((0, 0, 0))
        
        # Render dungeon
        renderer.render(dungeon, stats)
        
        # Display help text at the bottom
        screen.blit(help_surface, (10, screen.get_height() - 30))
        
        # Update display
        pygame.display.flip()
        clock.tick(60)
    
    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()

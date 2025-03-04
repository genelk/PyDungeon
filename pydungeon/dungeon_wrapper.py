import time
import numpy as np
from typing import Dict, Any, Optional, Tuple

try:
    # Import the C++ extension
    from pydungeon import pydungeon_cpp
    USE_CPP = True
except ImportError:
    print("C++ extension not available, falling back to Python implementation")
    USE_CPP = False
    from dungeon.generator import DungeonGenerator as PyDungeonGenerator


class DungeonGenerator:
    """
    Wrapper class for the dungeon generator that can use either the C++ or Python implementation.
    
    This class provides a consistent interface regardless of which implementation is used,
    allowing for seamless fallback to the Python implementation if the C++ extension is not available.
    """
    
    # Tile type constants
    EMPTY = 0
    WALL = 1
    FLOOR = 2
    CORRIDOR = 3
    DOOR = 4
    
    def __init__(
        self,
        width: int = 50,
        height: int = 50,
        min_rooms: int = 5,
        max_rooms: int = 15,
        min_room_size: int = 5,
        max_room_size: int = 15,
        seed: Optional[int] = None,
        use_cpp: Optional[bool] = None
    ):
        """
        Initialize the dungeon generator.
        
        Args:
            width: Width of the dungeon grid
            height: Height of the dungeon grid
            min_rooms: Minimum number of rooms to generate
            max_rooms: Maximum number of rooms to generate
            min_room_size: Minimum size (width/height) of rooms
            max_room_size: Maximum size (width/height) of rooms
            seed: Random seed (default: None, which uses a random seed)
            use_cpp: Whether to use the C++ implementation (default: None, which uses it if available)
        """
        # Determine whether to use C++ or Python implementation
        self.use_cpp = USE_CPP if use_cpp is None else use_cpp
        
        # Convert None seed to 0 for C++ (it will generate a random seed internally)
        cpp_seed = 0 if seed is None else seed
        
        if self.use_cpp and USE_CPP:
            self.generator = pydungeon_cpp.DungeonGenerator(
                width, height, min_rooms, max_rooms, min_room_size, max_room_size, cpp_seed
            )
        else:
            self.generator = PyDungeonGenerator(
                width=width, height=height, min_rooms=min_rooms, max_rooms=max_rooms,
                min_room_size=min_room_size, max_room_size=max_room_size
            )
            
            # Python implementation doesn't accept a seed directly, so we'd need to set the
            # random seed if it's provided
            if seed is not None:
                import random
                random.seed(seed)
                
    def generate(self) -> Tuple[np.ndarray, float]:
        """
        Generate a new dungeon layout.
        
        Returns:
            Tuple containing:
                - 2D numpy array representing the dungeon layout
                - Time taken to generate the dungeon (in seconds)
        """
        if self.use_cpp and USE_CPP:
            result = self.generator.generate()
            return result["dungeon"], result["time"]
        else:
            # For Python implementation, we need to time it ourselves
            start_time = time.time()
            dungeon = self.generator.generate()
            end_time = time.time()
            return dungeon, end_time - start_time
        
    def get_width(self) -> int:
        """Get the width of the dungeon."""
        if self.use_cpp and USE_CPP:
            return self.generator.get_width()
        else:
            return self.generator.width
        
    def get_height(self) -> int:
        """Get the height of the dungeon."""
        if self.use_cpp and USE_CPP:
            return self.generator.get_height()
        else:
            return self.generator.height


def benchmark_comparison(
    width: int = 100,
    height: int = 100,
    iterations: int = 5
) -> Dict[str, Any]:
    """
    Benchmark the C++ and Python implementations for comparison.
    
    Args:
        width: Width of the dungeon
        height: Height of the dungeon
        iterations: Number of iterations to run
        
    Returns:
        Dictionary with benchmark results
    """
    results = {}
    
    # If C++ extension is available, benchmark it
    if USE_CPP:
        cpp_result = pydungeon_cpp.benchmark(width, height, iterations)
        results["cpp"] = {
            "avg_time": cpp_result["avg_time"],
            "iterations": cpp_result["iterations"]
        }
    
    # Benchmark Python implementation
    py_generator = PyDungeonGenerator(width=width, height=height)
    
    total_time = 0
    for _ in range(iterations):
        start_time = time.time()
        py_generator.generate()
        end_time = time.time()
        total_time += (end_time - start_time)
    
    py_avg_time = total_time / iterations
    
    results["python"] = {
        "avg_time": py_avg_time,
        "iterations": iterations
    }
    
    # Calculate speedup if both implementations are available
    if "cpp" in results:
        results["speedup"] = py_avg_time / results["cpp"]["avg_time"]
    
    results["width"] = width
    results["height"] = height
    
    return results

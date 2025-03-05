
#!/usr/bin/env python3
import os
import subprocess
import sys
import platform

def main():
    print("PyDungeon - Dungeon Generator and Visualizer")
    print("-------------------------------------------")
    
    # Determine if we need to build the C++ component
    cpp_dir = "cpp"
    py_dir = "python"
    
    # Determine executable name based on platform
    exe_extension = ".exe" if platform.system() == "Windows" else ""
    executable = os.path.join(cpp_dir, f"dungeon_generator{exe_extension}")
    
    # Check if executable exists
    if not os.path.exists(executable):
        print(f"C++ executable not found at {executable}")
        print("You may need to build the C++ component first.")
        print("See cpp/VISUAL_STUDIO.md or README.md for build instructions.")
        return 1
    
    # Get dungeon parameters from user
    try:
        width = int(input("Dungeon width (default 50): ") or "50")
        height = int(input("Dungeon height (default 50): ") or "50")
        min_rooms = int(input("Minimum rooms (default 5): ") or "5")
        max_rooms = int(input("Maximum rooms (default 15): ") or "15")
        seed = input("Random seed (leave empty for random): ")
    except ValueError:
        print("Invalid input. Please enter numeric values.")
        return 1
    
    # Build the command
    cmd = [executable, str(width), str(height), str(min_rooms), str(max_rooms)]
    if seed:
        cmd.append(seed)
    
    # Generate the dungeon
    print("\nGenerating dungeon...")
    try:
        result = subprocess.run(cmd, cwd=os.path.dirname(executable), check=True)
        if result.returncode != 0:
            print("Dungeon generation failed.")
            return 1
    except subprocess.CalledProcessError:
        print("Dungeon generation failed.")
        return 1
    except FileNotFoundError:
        print(f"Could not run {executable}. Make sure the C++ component is built.")
        return 1
    
    # Run the visualizer
    print("\nLaunching visualizer...")
    try:
        subprocess.run(["python", "visualize.py"], cwd=py_dir)
    except FileNotFoundError:
        print("Could not run Python visualizer. Make sure Python and required packages are installed.")
        print("Try running: pip install -r python/requirements.txt")
        return 1
    
    return 0

if __name__ == "__main__":
    sys.exit(main())

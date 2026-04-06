AVL Tree Spell Checker
A high-performance C++ spell-checking utility utilizing a template-based AVL Tree (Self-Balancing Binary Search Tree). This project demonstrates efficient data lookups, recursive balancing algorithms, and real-time performance benchmarking.

Features
  - Self-Balancing Architecture: Implements a custom AVL Tree that maintains a strict $O(\log n)$ search, insert, and delete time complexity.
  - Recursive Rotations: Features automated Left, Right, Left-Right, and Right-Left rotations to maintain tree height equilibrium.
  - Automated Sanitization: Processes input text to remove non-alphabetic characters and normalize casing for case-insensitive dictionary matching.
  - Performance Benchmarking: Uses std::chrono to provide sub-millisecond high-resolution timing for dictionary loading and individual word lookups.
  
Technical Deep Dive
1. Balancing LogicThe tree utilizes a recursive height-tracking system. Upon every insertion or removal, the "Balance Factor" is calculated (height_left - height_right). If the factor exceeds 1 or falls below -1, the tree performs specific rotations to restore balance.

2. Efficiency vs. Hash Tables
While Hash Tables provide O(1) average time, this AVL implementation offers:
  - Ordered Traversal: Allows for alphabetical sorting of the dictionary without extra overhead.
  - Predictable Performance: Guarantees O(log n) even in worst-case scenarios where a Hash Table might suffer from excessive collisions.
  - Memory Safety: Manual memory management via recursive destructor calls to prevent leaks during tree teardown.

3. Modern C++ Practices
  - Templates: The AVLTree class is templated to support any comparable data type.
  - High-Resolution Clock: Utilizes modern std::chrono for precise performance metrics.
  - File Stream Optimization: Efficiently handles large-scale dictionary loading (e.g., 100,000+ words) using std::ifstream.

Getting Started
  - PrerequisitesA C++11 (or higher) compliant compiler.
  - A dictionary.txt file placed in the same directory as the executable.

Execution
  1. Compile the source:
    g++ -std=c++11 src/main.cpp -o SpellChecker
  2. Run the application:
    ./SpellChecker

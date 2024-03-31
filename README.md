# Self-Balancing-Threaded-Binary-Search-Tree-Map-mymap-
This project introduces mymap, a custom templated map class implemented in C++ that emulates the functionality of the standard library map. Unlike conventional maps, mymap is backed by a self-balancing, threaded binary search tree (BST). This sophisticated data structure enables efficient data storage, retrieval, and iteration by maintaining a balanced tree and allowing in-order traversal without recursion.

# Unique Features
Self-Balancing BST: Ensures logarithmic time complexity for insertions, deletions, and lookups by maintaining balance through the Seesaw-Balance property.
Threaded Traversal: Utilizes threads within the BST to facilitate efficient in-order traversal, leveraging null pointers to denote the "next" node in sequence, thus eliminating the need for recursion or a stack during iteration.
Templated Implementation: Supports a wide range of data types for keys and values, allowing for flexible usage across various applications.
Efficient Memory Use: Implements a smart memory management strategy to optimize space usage, especially in the context of null pointer reutilization for threading.

# Potential Applications
mymap finds utility in scenarios where data dynamics are critically analyzed over time, such as:

Data Analytics: For tracking and visualizing trends in large datasets.
Simulation Systems: Where entities and their attributes need to be rapidly accessed and updated.
Educational Tools: As a teaching aid for data structures, illustrating the balance and traversal of binary search trees.

# Challenges and Learning Outcomes
Developing mymap was a journey filled with learning opportunities:

Balancing the BST: Implementing the Seesaw-Balance property presented a unique challenge, requiring a deep understanding of tree structures and balancing algorithms.

Threading for Traversal: The addition of threads to facilitate in-order traversal without recursion tested our problem-solving skills and understanding of pointer manipulation.

Memory Management: Ensuring efficient and leak-free memory usage while implementing the map's dynamic operations honed our skills in resource management.

# Getting Started

## Prerequisites
Ensure you have a modern C++ compiler (supporting C++11 or later) installed.

## Compilation
Compile the project using the provided makefile for convenience: make all

## Running Tests
Execute the test suite to verify the functionality: ./tests

# License
This project is licensed under the MIT License - see the LICENSE file for details.

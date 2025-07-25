# C Data Structures
This is a simple and straightforward library that will give you the tools and features to use data structures, without the hassle of implementing them yourself.

# Support
This library should be supported on every OS.
The library should compile as well for most compilers, such as the `gcc`, `clang` and `msvc` C compilers.
All code is in the header files, so no need to worry about linking and stuff.
Most of the code in the library uses macros, so there might be errors that are a bit less easy to understand.
To compile the examples you can either use the `cmake` build platform or simply compile them with console commands, they don't need any libraries linked whatsoever.

# Features
For now, this library has 3 different data structures implemented:
- ***Vectors***: A resizable array structure.
- ***Hashtables***: A table of key/value pairs, has a very small lookup time complexity.
- ***Advanced Strings***: Advanced Strings are the equivalent of std::string, but for C. They support formatting.
- ***Linked Lists***: A list composed of nodes pointing to the next ones (and optionally to the previous ones).
- ***Binary Trees***: Structure composed of nodes following a parent/child hierarchy, where each parent has up to 2 children nodes.

# How to use
You can simply include them in your C source files, and no problem should arise.
There might be problematic conflicting names, but I think it should be alright for most users.
For `vector.h`, one problem might be the frequent use of short names that might create naming conflicts.
**Note that `hashtable.h` depends on `vector.h`, `queue.h` depends on `linked_list.h`**

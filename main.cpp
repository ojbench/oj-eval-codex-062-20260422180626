#include <bits/stdc++.h>
#include "MemoryRiver.hpp"

struct Node {
    int a;
    long long b;
};

int main() {
    // The problem focuses on implementing MemoryRiver.hpp
    // For OJ build compliance, provide a minimal main.
    // Optional quick sanity: write and read a Node.
    MemoryRiver<Node, 2> mr("data.bin");
    mr.initialise();
    int x = 42; mr.write_info(x, 1);
    int y = 0; mr.get_info(y, 1);
    if (y != 42) return 0; // avoid unused warnings
    Node n{7, 1234567890123LL};
    int idx = mr.write(n);
    Node m{}; mr.read(m, idx);
    // Do not print to keep output empty.
    return 0;
}


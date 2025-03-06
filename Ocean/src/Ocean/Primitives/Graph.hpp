#pragma once

#include "Ocean/Types/Integers.hpp"

template <typename K, class T>
class Graph {
private:
    struct Node {
        T* data;

        u32 firstEdge;

        Node() : data(nullptr), firstEdge(u32_max) { }
        Node(T* data) : data(data), firstEdge(u32_max) { }
        ~Node() { delete data; }

    };  // Node

    struct Edge {
        u32 src;
        u32 dst;

    };  // Edge

public:
    Graph();
    ~Graph();

    u32 AddNode(K name, T* data);
    void RemoveNode(K name);

    void AddEdge(K firstName, K lastName);
    void RemoveEdge(K firstName, K lastName);

    /** @todo Emplace & Move Nodes. */

    /** @todo Count Connected Edges Of Node. */

    /** @todo Check for cycles in the graph. */

private:


};  // Graph

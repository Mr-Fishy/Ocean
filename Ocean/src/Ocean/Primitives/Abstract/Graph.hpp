#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/DynamicArray.hpp"

/**
 * @brief The abstract Graph container.
 * 
 * @tparam T The data type.
 */
template <class T>
class Graph {
public:
    virtual ~Graph() = default;

    /**
     * @brief Adds an edge from vertex to vertex.
     * 
     * @param from A unique index of the starting vertex.
     * @param to A unique index of the ending vertex.
     */
    virtual void AddEdge(u16 from, u16 to) = 0;
    /**
     * @brief Removes an edge from vertex to vertex.
     * 
     * @param from A unique index of the starting vertex.
     * @param to A unique index of the ending vertex.
     */
    virtual void RemoveEdge(u16 from, u16 to) = 0;

    /**
     * @brief Determines if the given vertex's share an edge.
     * 
     * @param from A unique index of the starting vertex.
     * @param to A unique index of the ending vertex.
     * @return b8
     */
    virtual b8 IsAdjacent(u16 from, u16 to) = 0;

    

};  // Graph

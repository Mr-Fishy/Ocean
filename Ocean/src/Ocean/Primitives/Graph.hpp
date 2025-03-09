#pragma once

#include "Ocean/Types/Bool.hpp"

#include "Ocean/Primitives/Array.hpp"

/**
 * @brief The base Graph class for all implementations.
 * 
 * @tparam K The key type.
 * @tparam T The data type.
 */
template <typename K, class T>
class Graph {
public:
    virtual ~Graph();

    /**
     * @brief Adds a vertex with the given data.
     * 
     * @param key A uniquely identifying key of the vertex.
     * @param data The data that the vertex holds.
     */
    virtual void AddVertex(K key, T* data);
    /**
     * @brief Removes the vertex if it exists.
     * 
     * @param key A uniquely identifying key of the vertex.
     * @return T*
     */
    virtual T* RemoveVertex(K key);

    /**
     * @brief Get the vertex data if it exists.
     * 
     * @param key A uniquely identifying key of the vertex.
     * @return T*
     */
    virtual T* GetVertex(K key);
    /**
     * @brief Set the vertex data.
     * 
     * @param key A uniquely identifying key of the vertex.
     * @param data The new data to set.
     * @return T*
     */
    virtual T* SetVertex(K key, T* data);

    /**
     * @brief Adds an edge from vertex to vertex.
     * 
     * @param from A uniquely identifying key of the starting vertex.
     * @param to A uniquely identifying key of the ending vertex.
     */
    virtual void AddEdge(K from, K to);
    /**
     * @brief Removes an edge from vertex to vertex.
     * 
     * @param from A uniquely identifying key of the starting vertex.
     * @param to A uniquely identifying key of the ending vertex.
     */
    virtual void RemoveEdge(K from, K to);

    /**
     * @brief Determines if the given vertex's share an edge.
     * 
     * @param from A uniquely identifying key of the starting vertex.
     * @param to A uniquely identifying key of the ending vertex.
     * @return b8
     */
    virtual b8 IsAdjacent(K from, K to);

    /**
     * @brief Gets the neighboring vertices as a list.
     * 
     * @param start A uniquely identifying key of the starting vertex.
     * @return DynamicArray<T*> 
     */
    virtual DynamicArray<T*> GetNeighbors(K start);

};  // Graph

template <typename K, class T>
class DirectedGraph : public Graph<K, T> {
public:


};  // DirectedGraph
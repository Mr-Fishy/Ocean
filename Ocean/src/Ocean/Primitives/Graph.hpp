#pragma once

#include "Exceptions.hpp"
#include "Ocean/Types/Strings.hpp"
#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/HashMap.hpp"
#include "Ocean/Primitives/Macros.hpp"

template <class T>
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

    u32 AddNode(cstring name, T* data);
    void RemoveNode(cstring name);

    void AddEdge(cstring firstName, cstring lastName);
    void RemoveEdge(cstring firstName, cstring lastName);

    /** @todo Emplace & Move Nodes. */

    /** @todo Count Connected Edges Of Node. */

    /** @todo Check for cycles in the graph. */

private:
    UnorderedMap<cstring, Node> m_Nodes;

    DynamicArray<Edge> m_Edges;

};  // Graph



template <class T>
OC_INLINE Graph<T>::Graph() :
    m_Nodes(0),
    m_Edges(0)
{
    
}

template <class T>
OC_INLINE Graph<T>::~Graph()
{
    
}

template <class T>
OC_INLINE u32 Graph<T>::AddNode(cstring name, T* data) {
    if (this->m_Nodes[name] != this->m_Nodes.end())
        throw Ocean::Exception(Ocean::Error::INVALID_ARGUMENT, "Given new node that already exists! Graph<T>::AddNode Error.");

    this->m_Nodes.try_emplace(name, data);
}

template <class T>
OC_INLINE void Graph<T>::RemoveNode(cstring name) {
    
}

template <class T>
OC_INLINE void Graph<T>::AddEdge(cstring firstName, cstring lastName) {
    if (this->m_Nodes[firstName] == this->m_Nodes.end() && this->m_Nodes[lastName] == this->m_Nodes.end())
        throw Ocean::Exception(Ocean::Error::INVALID_ARGUMENT, "");
}

template <class T>
OC_INLINE void Graph<T>::RemoveEdge(cstring firstName, cstring lastName) {
    
}

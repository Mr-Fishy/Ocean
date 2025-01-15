#pragma once

#include <map>
#include <unordered_map>

template <typename K, class T>
using OrderedMap = std::map<K, T>;

template <typename K, class T>
using UnorderedMap = std::unordered_map<K, T>;

// TODO: Replace OrderedMap & UnorderedMap with local implementations.

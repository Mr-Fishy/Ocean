#pragma once

#include "Ocean/Types/Strings.hpp"

/**
	* @brief A service class to allow services to be made for the Service Manager.
	*/
class Service {
public:
	virtual ~Service() = default;

	virtual cstring GetName() const = 0;

};	// Service

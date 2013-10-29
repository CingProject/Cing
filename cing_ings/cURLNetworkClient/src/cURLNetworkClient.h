#pragma once

#include "Cing-Precompiled.h"
#include "Cing.h"

#include <iostream>
#include <curl/curl.h>

namespace Cing
{

class cURLNetworkClient
{
public:
	cURLNetworkClient(void);
	~cURLNetworkClient(void);

	void setup();
	void end();

	std::string fetchJSON(std::string URL);

};

};
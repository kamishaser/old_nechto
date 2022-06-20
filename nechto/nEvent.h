#pragma once
#include "node.h"

namespace nechto
{
	nodeEvent newNodeEvent					= nullptr;
	nodeEvent deleteNodeEvent				= nullptr;
	nodeEvent addHubEvent					= nullptr;
	nodeConnectionEvent HubHubConnectEvent  = nullptr;
	nodeConnectionEvent NumHubConnectEvent  = nullptr;
	nodeConnectionEvent disconnectEvent		= nullptr;
}
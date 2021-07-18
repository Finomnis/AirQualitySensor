#pragma once
#include <string>

namespace homie {
	struct client_event_handler {
		virtual void on_broadcast(const std::string& level, const std::string& payload) = 0;
	};
}
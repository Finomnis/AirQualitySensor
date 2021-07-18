#pragma once
#include <string>

namespace homie {
	struct mqtt_event_handler {
		// Called after connection is initiated
		virtual void on_connect(bool session_present, bool reconnected) = 0;
		// Clean disconnect started
		virtual void on_closing() = 0;
		// Clean disconnect
		virtual void on_closed() = 0;
		// Unexpected connection loss
		virtual void on_offline() = 0;
		virtual void on_message(const std::string& topic, const std::string& payload) = 0;
	};
}
#pragma once
#include <string>

namespace homie {
	template<typename T>
	inline T enum_from_string(const std::string& s);

	enum class device_state {
		init,
		ready,
		disconnected,
		sleeping,
		lost,
		alert
	};

	inline std::string enum_to_string(device_state s) {
		switch (s)
		{
		case device_state::init: return "init";
		case device_state::ready: return "ready";
		case device_state::disconnected: return "disconnected";
		case device_state::sleeping: return "sleeping";
		case device_state::lost: return "lost";
		case device_state::alert: return "alert";
		default:
			throw std::invalid_argument("invalid enum value");
		}
	}

	template<>
	inline device_state enum_from_string<device_state>(const std::string& s) {
		if (s == "init") return device_state::init;
		if (s == "ready") return device_state::ready;
		if (s == "disconnected") return device_state::disconnected;
		if (s == "sleeping") return device_state::sleeping;
		if (s == "lost") return device_state::lost;
		if (s == "alert") return device_state::alert;
		throw std::invalid_argument("not a enum member");
	}
}
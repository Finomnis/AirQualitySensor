#pragma once
#include <string>
#include "device.h"

namespace homie {
	struct master_event_handler {
		virtual void on_broadcast(const std::string& level, const std::string& payload) = 0;
		// Called when device state changes from init to something else
		virtual void on_device_discovered(device_ptr dev) = 0;
		virtual void on_device_changed(device_ptr dev, const std::string& attribute) = 0;
		virtual void on_node_changed(node_ptr node, const std::string& attribute) = 0;
		virtual void on_node_changed(node_ptr node, int64_t idx, const std::string& attribute) = 0;
		virtual void on_property_changed(property_ptr prop, const std::string& attribute) = 0;
		virtual void on_property_changed(property_ptr prop, int64_t idx, const std::string& attribute) = 0;
		virtual void on_property_value_changed(property_ptr prop, const std::string& value) = 0;
		virtual void on_property_value_changed(property_ptr prop, int64_t idx, const std::string& value) = 0;
	};
}
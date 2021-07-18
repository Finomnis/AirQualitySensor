#pragma once
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include "device_state.h"
#include "node.h"
#include "utils.h"

namespace homie {
	struct device {
		virtual std::string get_id() const = 0;
		virtual std::string get_name() const = 0;
		virtual device_state get_state() const = 0;
		virtual std::string get_localip() const = 0;
		virtual std::string get_mac() const = 0;
		virtual std::string get_firmware_name() const = 0;
		virtual std::string get_firmware_version() const = 0;
		virtual std::set<std::string> get_nodes() const = 0;
		virtual node_ptr get_node(const std::string& id) = 0;
		virtual const_node_ptr get_node(const std::string& id) const = 0;
		virtual std::string get_implementation() const = 0;
		virtual std::set<std::string> get_stats() const = 0;
		virtual std::string get_stat(const std::string& id) const = 0;
		virtual std::chrono::seconds get_stats_interval() const = 0;

		virtual std::set<std::string> get_attributes() const = 0;
		virtual std::string get_attribute(const std::string& id) const = 0;
		virtual void set_attribute(const std::string& id, const std::string& value) = 0;
	};

	struct basic_device : public device {
		// Geerbt über device
		virtual std::string get_name() const override { return get_attribute("name"); }
		virtual device_state get_state() const override { try { return enum_from_string<device_state>(get_attribute("state")); } catch (const std::exception&) { return device_state::init; } }
		virtual std::string get_localip() const override { return get_attribute("localip"); }
		virtual std::string get_mac() const override { return get_attribute("mac"); }
		virtual std::string get_firmware_name() const override { return get_attribute("fw/name"); }
		virtual std::string get_firmware_version() const override { return get_attribute("fw/version"); }
		virtual std::string get_implementation() const override { return get_attribute("implementation"); }
		virtual std::set<std::string> get_stats() const override {
			auto parts = utils::split<std::string>(get_attribute("stats"), ",");
			return std::set<std::string>(parts.begin(), parts.end());
		}
		virtual std::string get_stat(const std::string& id) const { return get_attribute("stats/" + id); }
		virtual std::chrono::seconds get_stats_interval() const override { return std::chrono::seconds(std::stoull(get_attribute("stats/interval"))); }
	};
	typedef std::shared_ptr<device> device_ptr;
	typedef std::shared_ptr<const device> const_device_ptr;
}
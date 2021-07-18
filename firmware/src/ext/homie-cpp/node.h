#pragma once
#include <string>
#include <memory>
#include <set>
#include "property.h"

namespace homie {
	struct device;
	typedef std::shared_ptr<device> device_ptr;
	typedef std::shared_ptr<const device> const_device_ptr;

	struct node {
		virtual device_ptr get_device() = 0;
		virtual const_device_ptr get_device() const = 0;

		virtual std::string get_id() const = 0;
		virtual std::string get_name() const = 0;
		virtual std::string get_name(int64_t node_idx) const = 0;
		virtual std::string get_type() const = 0;
		virtual bool is_array() const = 0;
		virtual std::pair<int64_t, int64_t> array_range() const = 0;
		virtual std::set<std::string> get_properties() const = 0;
		virtual const_property_ptr get_property(const std::string& id) const = 0;
		virtual property_ptr get_property(const std::string& id) = 0;

		virtual std::set<std::string> get_attributes() const = 0;
		virtual std::set<std::string> get_attributes(int64_t idx) const = 0;
		virtual std::string get_attribute(const std::string& id) const = 0;
		virtual void set_attribute(const std::string& id, const std::string& value) = 0;
		virtual std::string get_attribute(const std::string& id, int64_t idx) const = 0;
		virtual void set_attribute(const std::string& id, const std::string& value, int64_t idx) = 0;
	};
	struct basic_node : public node {
		virtual std::string get_name() const override { return get_attribute("name"); }
		virtual std::string get_name(int64_t node_idx) const { return get_attribute("name", node_idx); }
		virtual std::string get_type() const { return get_attribute("type"); }
		virtual bool is_array() const { return get_attribute("array") != ""; }
		virtual std::pair<int64_t, int64_t> array_range() const {
			auto att = get_attribute("array");
			auto pos = att.find('-');
			if (pos == std::string::npos || pos == 0 || pos == att.size() - 1) throw std::logic_error("invalid attribute");
			return{ std::stoll(att.substr(0, pos)), std::stoll(att.substr(pos + 1)) };
		}
	};
	typedef std::shared_ptr<node> node_ptr;
	typedef std::shared_ptr<const node> const_node_ptr;
}
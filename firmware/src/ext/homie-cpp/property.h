#pragma once
#include <string>
#include <memory>
#include "datatype.h"

namespace homie {
	struct node;
	typedef std::shared_ptr<node> node_ptr;
	typedef std::shared_ptr<const node> const_node_ptr;

	struct property {
		virtual node_ptr get_node() = 0;
		virtual const_node_ptr get_node() const = 0;

		virtual std::string get_id() const = 0;
		virtual std::string get_name() const = 0;
		virtual bool is_settable() const = 0;
		virtual std::string get_unit() const = 0;
		virtual datatype get_datatype() const = 0;
		virtual std::string get_format() const = 0;

		virtual std::string get_value(int64_t node_idx) const = 0;
		virtual void set_value(int64_t node_idx, const std::string& value) = 0;
		virtual std::string get_value() const = 0;
		virtual void set_value(const std::string& value) = 0;

		virtual std::set<std::string> get_attributes() const = 0;
		virtual std::string get_attribute(const std::string& id) const = 0;
		virtual void set_attribute(const std::string& id, const std::string& value) = 0;
	};
	struct basic_property : public property {
		virtual std::string get_name() const override { return get_attribute("name"); }
		virtual bool is_settable() const { return get_attribute("settable") == "true"; }
		virtual std::string get_unit() const { return get_attribute("unit"); }
		virtual datatype get_datatype() const {
			auto att = get_attribute("datatype");
			return enum_from_string<datatype>(att.empty() ? "string" : att);
		}
		virtual std::string get_format() const { return get_attribute("format"); }
	};
	typedef std::shared_ptr<property> property_ptr;
	typedef std::shared_ptr<const property> const_property_ptr;
}
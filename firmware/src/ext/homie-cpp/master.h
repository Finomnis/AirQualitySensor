#pragma once
#include "mqtt_client.h"
#include "device.h"
#include "utils.h"
#include "master_event_handler.h"
#include <set>
#include <map>

namespace homie {
	class master : private mqtt_event_handler {
		struct remote_property : public homie::basic_property, public std::enable_shared_from_this<remote_property> {
			master* parent;
			std::string value;
			std::map<int64_t, std::string> value_array;
			std::string id;
			std::map<std::string, std::string> attributes;
			std::weak_ptr<homie::node> node;

			remote_property(master* p, std::weak_ptr<homie::node> ptr, const std::string& mid)
				: parent(p), node(ptr), id(mid)
			{ }

			virtual node_ptr get_node() { return node.lock(); }
			virtual const_node_ptr get_node() const { return node.lock(); }

			virtual std::string get_id() const {
				return id;
			}

			virtual std::string get_value(int64_t node_idx) const { return value_array.count(node_idx) ? value_array.at(node_idx) : ""; }
			virtual void set_value(int64_t node_idx, const std::string& value) { parent->publish_set_property(this, value, node_idx); }
			virtual std::string get_value() const { return value; }
			virtual void set_value(const std::string& value) { parent->publish_set_property(this, value); }

			virtual std::set<std::string> get_attributes() const override {
				std::set<std::string> res;
				for (auto& e : attributes) res.insert(e.first);
				return res;
			}
			virtual std::string get_attribute(const std::string& id) const override {
				auto it = attributes.find(id);
				if (it != attributes.cend()) return it->second;
				return "";
			}
			virtual void set_attribute(const std::string& id, const std::string& value) override {
				attributes[id] = value;
			}
		};
		struct remote_node : public homie::basic_node, public std::enable_shared_from_this<remote_node> {
			master* parent;
			std::string id;
			std::map<std::string, std::shared_ptr<remote_property>> properties;
			std::map<std::string, std::string> attributes;
			std::map<std::pair<int64_t, std::string>, std::string> attributes_array;
			std::weak_ptr<homie::device> device;

			remote_node(master* p, std::weak_ptr<homie::device> dev, const std::string& mid)
				: parent(p), id(mid), device(dev)
			{}

			std::shared_ptr<remote_property> get_add_property(const std::string& id) {
				if (properties.count(id)) return properties.at(id);
				auto prop = std::make_shared<remote_property>(parent, this->shared_from_this(), id);
				properties.insert({ id, prop });
				return prop;
			}

			// Geerbt über node
			virtual device_ptr get_device() override {
				return device.lock();
			}
			virtual const_device_ptr get_device() const override {
				return device.lock();
			}
			virtual std::string get_id() const override
			{
				return id;
			}
			virtual std::set<std::string> get_properties() const override
			{
				std::set<std::string> res;
				for (auto& e : properties) res.insert(e.first);
				return res;
			}
			virtual property_ptr get_property(const std::string& id) override
			{
				return properties.count(id) ? properties.at(id) : nullptr;
			}
			virtual const_property_ptr get_property(const std::string& id) const override
			{
				return properties.count(id) ? properties.at(id) : nullptr;
			}

			virtual std::set<std::string> get_attributes() const override {
				std::set<std::string> res;
				for (auto& e : attributes) res.insert(e.first);
				return res;
			}
			virtual std::set<std::string> get_attributes(int64_t idx) const override {
				std::set<std::string> res;
				for (auto& e : attributes_array)
					if(e.first.first == idx)
						res.insert(e.first.second);
				return res;
			}
			virtual std::string get_attribute(const std::string& id) const override {
				auto it = attributes.find(id);
				if (it != attributes.cend()) return it->second;
				return "";
			}
			virtual void set_attribute(const std::string& id, const std::string& value) override {
				attributes[id] = value;
			}
			virtual std::string get_attribute(const std::string& id, int64_t idx) const override {
				auto it = attributes_array.find({ idx, id });
				if (it != attributes_array.cend()) return it->second;
				return "";
			}
			virtual void set_attribute(const std::string& id, const std::string& value, int64_t idx) override {
				attributes_array[{idx, id}] = value;
			}
		};
		struct remote_device : public homie::basic_device, public std::enable_shared_from_this<remote_device> {
			master* parent;
			std::string id;
			std::map<std::string, std::shared_ptr<remote_node>> nodes;
			std::map<std::string, std::string> attributes;

			remote_device(master* p, const std::string& mid)
				: parent(p), id(mid)
			{}

			std::shared_ptr<remote_node> get_add_node(const std::string& id) {
				if (nodes.count(id)) return nodes.at(id);
				auto node = std::make_shared<remote_node>(parent, this->shared_from_this(), id);
				nodes.insert({ id, node });
				return node;
			}

			// Geerbt über device
			virtual std::string get_id() const override { return id; }
			virtual std::set<std::string> get_nodes() const override
			{
				std::set<std::string> res;
				for (auto& e : nodes) res.insert(e.first);
				return res;
			}
			virtual node_ptr get_node(const std::string& id) override
			{
				return nodes.count(id) ? nodes.at(id) : nullptr;
			}
			virtual const_node_ptr get_node(const std::string& id) const override
			{
				return nodes.count(id) ? nodes.at(id) : nullptr;
			}

			virtual std::set<std::string> get_attributes() const override {
				std::set<std::string> res;
				for (auto& e : attributes) res.insert(e.first);
				return res;
			}
			virtual std::string get_attribute(const std::string& id) const {
				auto it = attributes.find(id);
				if (it != attributes.cend()) return it->second;
				return "";
			}
			virtual void set_attribute(const std::string& id, const std::string& value) {
				attributes[id] = value;
			}
		};

		mqtt_client& mqtt;
		master_event_handler* handler;
		std::string base_topic;
		std::map<std::string, std::shared_ptr<remote_device>> devices;

		// Inherited by mqtt_event_handler
		virtual void on_connect(bool session_present, bool reconnected) override {
			if (!session_present) {
				mqtt.subscribe(base_topic + "#", 1);
			}
		}
		virtual void on_closing() override {
			mqtt.unsubscribe(base_topic + "#");
		}
		virtual void on_closed() override {}
		virtual void on_offline() override {}
		virtual void on_message(const std::string & topic, const std::string & payload) override {
			// Check basetopic
			if (topic.size() < base_topic.size())
				return;
			if (topic.compare(0, base_topic.size(), base_topic) != 0)
				return;

			auto parts = utils::split<std::string>(topic, "/", base_topic.size());
			if (parts.size() < 2)
				return;
			for (auto& e : parts) if (e.empty()) return;
			if (parts[0][0] == '$') {
				if (parts[0] == "$broadcast") {
					this->handle_broadcast(parts[1], payload);
				}
			}
			else {
				this->handle_device_message(parts, payload);
			}
		}

		void handle_broadcast(const std::string& level, const std::string& payload) {
			if (handler)
				handler->on_broadcast(level, payload);
		}

		void handle_device_message(const std::vector<std::string>& parts, const std::string& payload) {
			auto dev = get_add_device(parts[0]);
			if (parts[1][0] == '$') {
				std::string id = parts[1].substr(1);
				for (size_t i = 2; i < parts.size(); i++) {
					id += "/" + parts[i];
				}
				if (id == "state" && payload != "init" && (dev->get_attribute("state") == "" || dev->get_state() == device_state::init)) {
					dev->set_attribute(id, payload);
					if (handler)
						handler->on_device_discovered(dev);
				}
				else {
					dev->set_attribute(id, payload);
					if (handler && dev->get_state() != device_state::init) {
						handler->on_device_changed(dev, id);
					}
				}
			}
			else if (parts.size() >= 3) {
				bool is_array = false;
				int64_t idx = 0;
				std::shared_ptr<remote_node> node;
				{
					auto pos = parts[1].find('_');
					if (pos != std::string::npos) {
						node = dev->get_add_node(parts[1].substr(0, pos));
						is_array = true;
						idx = std::stoll(parts[1].substr(pos + 1));
					}
					else
						node = dev->get_add_node(parts[1]);
				}

				if (parts[2][0] == '$') {
					std::string id = parts[2].substr(1);
					for (size_t i = 3; i < parts.size(); i++) {
						id += "/" + parts[i];
					}
					if (is_array) node->set_attribute(id, payload, idx);
					else node->set_attribute(id, payload);
					if (handler && dev->get_state() != device_state::init) {
						if (is_array) handler->on_node_changed(node, idx, id);
						else handler->on_node_changed(node, id);
					}
				}
				else {
					auto prop = node->get_add_property(parts[2]);
					if (parts.size() == 3) {
						if (is_array) prop->value_array[idx] = payload;
						else prop->value = payload;

						if (handler && dev->get_state() != device_state::init) {
							if (is_array) handler->on_property_value_changed(prop, idx, payload);
							else handler->on_property_value_changed(prop, payload);
						}
					}
					else {
						std::string id = parts[3].substr(1);
						for (size_t i = 4; i < parts.size(); i++) {
							id += "/" + parts[i];
						}
						prop->set_attribute(id, payload);
						if (handler && dev->get_state() != device_state::init) {
							if (is_array) handler->on_property_changed(prop, idx, id);
							else handler->on_property_changed(prop, id);
						}
					}
				}
			}
		}

		std::shared_ptr<remote_device> get_add_device(const std::string& id) {
			if (devices.count(id)) return devices.at(id);
			auto dev = std::make_shared<remote_device>(this, id);
			devices.insert({ id, dev });
			return dev;
		}

		void publish_set_property(const homie::property* prop, const std::string& value) {
			auto node = prop->get_node();
			auto dev = node->get_device();
			mqtt.publish(base_topic + dev->get_id() + "/" + node->get_id() + "/" + prop->get_id() + "/set", value, 1, true);
		}

		void publish_set_property(const homie::property* prop, const std::string& value, int64_t idx) {
			auto node = prop->get_node();
			auto dev = node->get_device();
			mqtt.publish(base_topic + dev->get_id() + "/" + node->get_id() + "_" + std::to_string(idx) + "/" + prop->get_id() + "/set", value, 1, true);
		}
	public:
		master(mqtt_client& con, std::string basetopic = "homie/")
			: mqtt(con), handler(nullptr), base_topic(basetopic)
		{
			mqtt.set_event_handler(this);
			mqtt.open();
		}

		~master() {
			this->mqtt.unsubscribe(base_topic + "#");
			mqtt.set_event_handler(nullptr);
		}

		std::set<device_ptr> get_discovered_devices() {
			std::set<device_ptr> res;
			for (auto& e : devices) res.insert(e.second);
			return res;
		}

		std::set<const_device_ptr> get_discovered_devices() const {
			std::set<const_device_ptr> res;
			for (auto& e : devices) res.insert(e.second);
			return res;
		}

		device_ptr get_discovered_device(const std::string& id) {
			return devices.count(id) ? devices.at(id) : nullptr;
		}

		const_device_ptr get_discovered_device(const std::string& id) const {
			return devices.count(id) ? devices.at(id) : nullptr;
		}

		void publish_broadcast(const std::string& level, const std::string& payload) {
			mqtt.publish(base_topic + "$broadcast/" + level, payload, 1, false);
		}

		void set_event_handler(master_event_handler* hdl) {
			handler = hdl;
		}
	};
}
#pragma once
#include "mqtt_client.h"
#include "device.h"
#include "utils.h"
#include "client_event_handler.h"
#include <set>

namespace homie {
	class client : private mqtt_event_handler {
		mqtt_client& mqtt;
		std::string base_topic;
		device_ptr dev;
		client_event_handler* handler;

		// Inherited by mqtt_event_handler
		virtual void on_connect(bool session_present, bool reconnected) override {
			if (reconnected) {
				mqtt.publish(base_topic + dev->get_id() + "/$state", enum_to_string(dev->get_state()), 1, true);
			}
			else {
				publish_device_info();
			}
		}
		virtual void on_closing() override {
			mqtt.publish(base_topic + dev->get_id() + "/$state", enum_to_string(device_state::disconnected), 1, true);
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
			else if(parts[0] == dev->get_id()) {
				if (parts.size() != 4
					|| parts[3] != "set"
					|| parts[2][0] == '$')
					return;
				this->handle_property_set(parts[1], parts[2], payload);
			}
		}

		void handle_property_set(const std::string& snode, const std::string& sproperty, const std::string& payload) {
			if (snode.empty() || sproperty.empty())
				return;

			int64_t id = 0;
			bool is_array_node = false;
			std::string rnode = snode;
			auto pos = rnode.find('_');
			if (pos != std::string::npos) {
				id = std::stoll(rnode.substr(pos + 1));
				is_array_node = true;
				rnode.resize(pos);
			}

			auto node = dev->get_node(rnode);
			if (node == nullptr || node->is_array() != is_array_node) return;
			auto prop = node->get_property(sproperty);
			if (prop == nullptr) return;

			if (is_array_node)
				prop->set_value(id, payload);
			else prop->set_value(payload);
		}

		void handle_broadcast(const std::string& level, const std::string& payload) {
			if(handler)
				handler->on_broadcast(level, payload);
		}

		void publish_device_info() {
			// Signal initialisation phase
			this->publish_device_attribute("$state", enum_to_string(device_state::init));

			// Public device properties
			this->publish_device_attribute("$homie", "3.0.0");
			this->publish_device_attribute("$name", dev->get_name());
			this->publish_device_attribute("$localip", dev->get_localip());
			this->publish_device_attribute("$mac", dev->get_mac());
			this->publish_device_attribute("$fw/name", dev->get_firmware_name());
			this->publish_device_attribute("$fw/version", dev->get_firmware_version());
			this->publish_device_attribute("$implementation", dev->get_implementation());
			this->publish_device_attribute("$stats/interval", std::to_string(dev->get_stats_interval().count()));

			// Publish nodes
			std::string nodes = "";
			for (auto& nodename : dev->get_nodes()) {
				auto node = dev->get_node(nodename);
				if (node->is_array()) {
					nodes += node->get_id() + "[],";
					this->publish_node_attribute(node, "$array", std::to_string(node->array_range().first) + "-" + std::to_string(node->array_range().second));
					for (int64_t i = node->array_range().first; i <= node->array_range().second; i++) {
						auto n = node->get_name(i);
						if(n != "")
						this->publish_device_attribute(node->get_id() + "_" + std::to_string(i) + "/$name", n);
					}
				}
				else {
					nodes += node->get_id() + ",";
				}
				this->publish_node_attribute(node, "$name", node->get_name());
				this->publish_node_attribute(node, "$type", node->get_type());

				// Publish node properties
				std::string properties = "";
				for (auto& propertyname : node->get_properties()) {
					auto property = node->get_property(propertyname);
					properties += property->get_id() + ",";
					this->publish_property_attribute(node, property, "$name", property->get_name());
					this->publish_property_attribute(node, property, "$settable", property->is_settable() ? "true" : "false");
					this->publish_property_attribute(node, property, "$unit", property->get_unit());
					this->publish_property_attribute(node, property, "$datatype", enum_to_string(property->get_datatype()));
					this->publish_device_attribute(node->get_id() + "/" + property->get_id() + "/$format", property->get_format());
					if (!node->is_array()) {
						auto val = property->get_value();
						if (!val.empty())
							this->publish_node_attribute(node, property->get_id(), val);
					}
					else {
						for (int64_t i = node->array_range().first; i <= node->array_range().second; i++) {
							auto val = property->get_value(i);
							if(!val.empty())
								this->publish_device_attribute(node->get_id() + "_" + std::to_string(i) + "/" + property->get_id(), val);
						}
					}
				}
				if (!properties.empty())
					properties.resize(properties.size() - 1);
				this->publish_node_attribute(node, "$properties", properties);
			}
			if (!nodes.empty())
				nodes.resize(nodes.size() - 1);
			this->publish_device_attribute("$nodes", nodes);

			// Publish stats
			std::string stats = "";
			for (auto& stat : dev->get_stats()) {
				stats += stat + ",";
				this->publish_device_attribute("$stats/" + stat, dev->get_stat(stat));
			}
			if (!stats.empty())
				stats.resize(stats.size() - 1);
			this->publish_device_attribute("$stats", stats);

			// Subscribe to set topics
			this->mqtt.subscribe(base_topic + dev->get_id() + "/+/+/set", 1);

			// Everything done, set device to real state
			this->publish_device_attribute("$state", enum_to_string(dev->get_state()));
		}

		void publish_device_attribute(const std::string& attribute, const std::string& value) {
			mqtt.publish(base_topic + dev->get_id() + "/" + attribute, value, 1, true);
		}

		void publish_node_attribute(const_node_ptr node, const std::string& attribute, const std::string& value) {
			publish_device_attribute(node->get_id() + "/" + attribute, value);
		}

		void publish_property_attribute(const_node_ptr node, const_property_ptr prop, const std::string& attribute, const std::string& value) {
			publish_node_attribute(node, prop->get_id() + "/" + attribute, value);
		}

		void notify_property_changed_impl(const std::string& snode, const std::string& sproperty, const int64_t* idx) {
			if (snode.empty() || sproperty.empty())
				return;

			auto node = dev->get_node(snode);
			if (!node) return;
			auto prop = node->get_property(sproperty);
			if (!prop) return;
			if (node->is_array()) {
				if (idx != nullptr) {
					this->publish_device_attribute(node->get_id() + "_" + std::to_string(*idx) + "/" + prop->get_id(), prop->get_value(*idx));
				}
				else {
					auto range = node->array_range();
					for (auto i = range.first; i <= range.second; i++) {
						this->publish_device_attribute(node->get_id() + "_" + std::to_string(i) + "/" + prop->get_id(), prop->get_value(i));
					}
				}
			}
			else {
				this->publish_device_attribute(node->get_id() + "/" + prop->get_id(), prop->get_value());
			}
		}
	public:
		client(mqtt_client& con, device_ptr pdev, std::string basetopic = "homie/")
			: mqtt(con), base_topic(basetopic), dev(pdev), handler(nullptr)
		{
			if (!pdev) throw std::invalid_argument("device is null");
			mqtt.set_event_handler(this);

			mqtt.open(base_topic + dev->get_id() + "/$state", enum_to_string(device_state::lost), 1, true);
		}

		~client() {
			this->publish_device_attribute("$state", enum_to_string(device_state::disconnected));
			this->mqtt.unsubscribe(base_topic + dev->get_id() + "/+/+/set");
			mqtt.set_event_handler(nullptr);
		}

		void notify_property_changed(const std::string& snode, const std::string& sproperty) {
			notify_property_changed_impl(snode, sproperty, nullptr);
		}

		void notify_property_changed(const std::string& snode, const std::string& sproperty, int64_t idx) {
			notify_property_changed_impl(snode, sproperty, &idx);
		}

		void set_event_handler(client_event_handler* hdl) {
			handler = hdl;
		}
	};
}
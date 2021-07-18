#pragma once
#include <string>

namespace homie {
	template<typename T>
	inline T enum_from_string(const std::string& s);

	enum class datatype {
		integer,
		number,
		boolean,
		string,
		enumeration,
		color
	};

	inline std::string enum_to_string(datatype s) {
		switch (s)
		{
		case datatype::integer: return "integer";
		case datatype::number: return "float";
		case datatype::boolean: return "boolean";
		case datatype::string: return "string";
		case datatype::enumeration: return "enum";
		case datatype::color: return "color";
		default:
			throw std::invalid_argument("invalid enum value");
		}
	}

	template<>
	inline datatype enum_from_string<datatype>(const std::string& s) {
		if (s == "integer") return datatype::integer;
		if (s == "float") return datatype::number;
		if (s == "boolean") return datatype::boolean;
		if (s == "string") return datatype::string;
		if (s == "enum") return datatype::enumeration;
		if (s == "color") return datatype::color;
		throw std::invalid_argument("not a enum member");
	}
}
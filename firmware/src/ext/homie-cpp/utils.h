#pragma once
#include <vector>

namespace homie {
	namespace utils {
		// split string
		template<typename StringType = std::string>
		inline std::vector<StringType> split(const StringType& s, const StringType& delim, size_t offset = 0, size_t max = std::numeric_limits<size_t>::max()) {
			std::vector<StringType> res;
			do {
				auto pos = s.find(delim, offset);
				if (res.size() < max - 1 && pos != std::string::npos)
					res.push_back(s.substr(offset, pos - offset));
				else {
					res.push_back(s.substr(offset));
					break;
				}
				offset = pos + delim.size();
			} while (true);
			return res;
		}
	}
}
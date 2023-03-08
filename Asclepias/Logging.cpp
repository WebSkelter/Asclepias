/*
* File:		Logging.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#include "Asclepias.h"

namespace ASC {
	// Define LogMgr functions.

	void LogMgr::init(bool console, const std::vector<std::string>& fileNames,
		const std::string& timestampFmt) {
		console_ = console;
		for (const std::string& fileName : fileNames) {
			files_.push_back(std::ofstream(fileName));
		}
		timestampFmt_ = timestampFmt;
	}

	const std::string LogMgr::getTimestamp() const {
		struct tm t;
		__time64_t lt;
		char buffer[64];
		errno_t err;
		_time64(&lt);
		err = _localtime64_s(&t, &lt);
		if (err) {
			return "";
		}
		strftime(buffer, sizeof(buffer), timestampFmt_.c_str(), &t);
		return std::string(buffer);
	}

	void LogMgr::destroy() {
#ifdef _DEBUG
		console_ = true;
#else
		console_ = false;
#endif
		for (std::ofstream& of : files_) {
			of.close();
		}
		files_.clear();
		timestampFmt_ = "";
	}
}
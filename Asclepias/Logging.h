/*
* File:		Logging.h
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#ifndef ASC_LOGGING_H
#define ASC_LOGGING_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#define ASCLOG(SRC, LVL, ...) App::log(), App::log().getTimestamp(), \
	": [", #SRC, "] [", #LVL, "] ", __VA_ARGS__, "\n"

namespace ASC {
	// Class for writing application logs to the console and multiple files.
	class LogMgr {
	public:
		/*
		* Initialize this log manager's memory.
		* @param bool console: Whether to print this log's messages to the console.
		* @param const std::vector<std::string>& fileNames: The file paths to write this log's
		* messages to.
		* @param const std::string timestamptFmt: The format to display this log's timestamps in.
		*/
		void init(bool, const std::vector<std::string>&, const std::string&);
		/*
		* Write data to this log.
		* @param const T& t: The data to write.
		*/
		template <typename T>
		void write(const T& t) {
			if (console_) {
				std::cout << t;
			}
			for (std::ofstream& of : files_) {
				if (of.is_open()) {
					of << t;
				}
			}
		}
		/*
		* @return A string containing the current date and time in the timestampFmt.
		*/
		const std::string getTimestamp() const;
		/*
		* Free this log manager's memory and close its files.
		*/
		void destroy();

	private:
		// Whether this log prints messages to the console.
		bool console_
#ifdef _DEBUG
			= true;
#else
			= false;
#endif
		// The set of file paths to write messages to.
		std::vector<std::ofstream> files_;
		// The format to write timestamps in.
		std::string timestampFmt_ = "";
	};

	/*
	* Overload the , operator to write data to a LogMgr.
	* @param LogMgr& log: The log to write to.
	* @param const T& t: The data to write.
	* @return The log.
	*/
	template<typename T>
	LogMgr& operator , (LogMgr& log, const T& t) {
		log.write(t);
		return log;
	}
}

#endif
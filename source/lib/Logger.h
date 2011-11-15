#ifndef BLITZORTUNG_LOGGER_H_
#define BLITZORTUNG_LOGGER_H_

#include <log4cpp/Category.hh>

namespace blitzortung {

  //! class for Logging for all classes with selectable and inherited priorities
  class Logger {

    private:

      //! log4cpp logger category
      log4cpp::Category& category_;

    public:

      //! construct logger (optional with a class name)
      Logger(const std::string& loggerName="");

      //! destruct logger
      virtual ~Logger();

      //! set logging priority
      void setPriority(const log4cpp::Priority::Value priority);

      //! set log output to file
      void setLogFile(const std::string& logFileName);

      //! write INFO string
      void info(const std::string& message);

      //! write MESSAGE string
      void message(const std::string& message);

      //! write NOTICE string
      void notice(const std::string& message);

      //! write DEBUG string
      void debug(const std::string& message);

      //! write WARN string
      void warn(const std::string& message);

      //! write ERROR string
      void error(const std::string& message);

      //! returns whether debug is enabled.
      inline bool isInfoEnabled() const {
	return category_.isInfoEnabled();
      }

      //! returns whether debug is enabled.
      inline bool isNoticeEnabled() const {
	return category_.isNoticeEnabled();
      }

      //! returns whether debug is enabled.
      inline bool isDebugEnabled() const {
	return category_.isDebugEnabled();
      }

      //! returns whether debug is enabled.
      inline bool isWarnEnabled() const {
	return category_.isWarnEnabled();
      }

      //! returns a category stream for info
      inline log4cpp::CategoryStream infoStream() {
	return category_.infoStream();
      }

      //! returns a category stream for info
      inline log4cpp::CategoryStream noticeStream() {
	return category_.noticeStream();
      }

      //! returns a category stream for info
      inline log4cpp::CategoryStream debugStream() {
	return category_.debugStream();
      }

      //! returns a category stream for info
      inline log4cpp::CategoryStream warnStream() {
	return category_.warnStream();
      }

      //! returns a category stream for info
      inline log4cpp::CategoryStream errorStream() {
	return category_.errorStream();
      }

  };
}

#endif

#ifndef BLITZORTUNG_LOGGER_H_
#define BLITZORTUNG_LOGGER_H_

#include <log4cpp/Category.hh>
#include <boost/shared_ptr.hpp>

namespace blitzortung {

  //! class for Logging in different categories
  class Logger {
    private:
      log4cpp::Category& category_;

    public:
      //! construct logger with a name
      Logger(const std::string& loggerName);

      //! destruct logger
      virtual ~Logger();

      //! set logging priority
      void setPriority(const log4cpp::Priority::Value priority);

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

      //! Returns whether debug is enabled.
      inline bool isInfoEnabled() const {
	return category_.isInfoEnabled();
      }

      //! Returns whether debug is enabled.
      inline bool isNoticeEnabled() const {
	return category_.isNoticeEnabled();
      }

      //! Returns whether debug is enabled.
      inline bool isDebugEnabled() const {
	return category_.isDebugEnabled();
      }

      //! Returns whether debug is enabled.
      inline bool isWarnEnabled() const {
	return category_.isWarnEnabled();
      }

      //! returns a cateogry stream for info
      inline log4cpp::CategoryStream infoStream() {
	return category_.infoStream();
      }

      //! returns a cateogry stream for info
      inline log4cpp::CategoryStream noticeStream() {
	return category_.noticeStream();
      }

      //! returns a cateogry stream for info
      inline log4cpp::CategoryStream debugStream() {
	return category_.debugStream();
      }

      //! returns a cateogry stream for info
      inline log4cpp::CategoryStream warnStream() {
	return category_.warnStream();
      }

      //! returns a cateogry stream for info
      inline log4cpp::CategoryStream errorStream() {
	return category_.errorStream();
      }

      //! globel function to get reference to logger
      static Logger& get();
  };

}

#endif

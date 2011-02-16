#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/BasicLayout.hh>

#include "Logger.h"

namespace blitzortung {

  Logger::Logger(const std::string& loggerName) :
    category_(log4cpp::Category::getInstance(loggerName))
  {
    // only create and set an appender when the category instance is without one
    if (! category_.ownsAppender()) {

      // create and set appender to standard output
      log4cpp::Appender* app = new log4cpp::OstreamAppender("Appender", &std::cout);
      category_.setAppender(app);

      // create and set basic layout
      log4cpp::Layout* layout = new log4cpp::BasicLayout();
      app->setLayout(layout);

      // set further logging  parameters
      category_.setAdditivity(false);
      //category_.setPriority(log4cpp::Priority::NOTICE);
    }
  }

  Logger::~Logger() {
  }

  void Logger::setPriority(const log4cpp::Priority::Value priority) {
    category_.setPriority(priority);
  }

  void Logger::setLogFile(const std::string& logFileName) {

    category_.setAdditivity(false);

    log4cpp::Appender* app = new log4cpp::FileAppender("FileAppender", logFileName);

    // create and set basic layout
    log4cpp::Layout* layout = new log4cpp::BasicLayout();
    app->setLayout(layout);

    category_.setAppender(app);
  }

  void Logger::info(const std::string& message) {
    category_.info(message);
  }

  void Logger::notice(const std::string& message) {
    category_.notice(message);
  }

  void Logger::debug(const std::string& message) {
    category_.debug(message);
  }

  void Logger::warn(const std::string& message) {
    category_.warn(message);
  }

  void Logger::error(const std::string& message) {
    category_.error(message);
  }

}

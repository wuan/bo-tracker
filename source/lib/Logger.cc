#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>

#include "Logger.h"

#define LOGGER_LAYOUT "%d{%d.%m.%Y %H:%M:%S.%l} %p %c %x: %m%n"

namespace blitzortung {

  Logger::Logger(const std::string& loggerName) :
    category_(log4cpp::Category::getInstance(loggerName))
  {
    // only create and set an appender when the category instance is without one
    if (category_.getParent() == 0 && ! category_.ownsAppender()) {

      // create and set appender to standard output
      log4cpp::Appender* app = new log4cpp::OstreamAppender("Appender", &std::cout);
      category_.setAppender(app);

      // create and set layout
      log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
      layout->setConversionPattern(LOGGER_LAYOUT);
      app->setLayout((log4cpp::Layout*)layout);

      //category_.setPriority(log4cpp::Priority::NOTICE);
    }
  }

  Logger::~Logger() = default;

  void Logger::setPriority(const log4cpp::Priority::Value priority) {
    category_.setPriority(priority);
  }

  void Logger::setLogFile(const std::string& logFileName) {

    log4cpp::Category& rootCategory = category_.getRoot();

    log4cpp::Appender* app = new log4cpp::FileAppender("FileAppender", logFileName);

    // create and set layout
      log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
      layout->setConversionPattern(LOGGER_LAYOUT);
      app->setLayout((log4cpp::Layout*)layout);

    rootCategory.setAppender(app);
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

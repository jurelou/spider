#include      <iostream>
#include      "Logger.hpp"

Logger::Logger() {}

Logger::~Logger() {}

boost::shared_ptr<Logger> Logger::create() {
  return boost::shared_ptr<Logger>(new Logger());
}

void          Logger::logMessage(const std::string& ip, const boost::array<char, 2048>& data) {
  this->currentDate = boost::posix_time::second_clock::universal_time();
  std::cout << "\033[1m[\033[0m\033[1;34mMessage from " << ip << " at " <<  this->currentDate.date() << " " << this->currentDate.time_of_day() << "\033[0m\033[1m]\033[0m : " << data.data();
}

void          Logger::logMessage(const std::string& ip, const std::string& message) {
  this->currentDate = boost::posix_time::second_clock::universal_time();
  std::cout << "\033[1m[\033[0m\033[1;34mMessage from " << ip << " at " <<  this->currentDate.date() << " " << this->currentDate.time_of_day() << "\033[0m\033[1m]\033[0m : " << message << std::endl;
}

void          Logger::logEvent(const std::string& ip, const std::string& info) {
  this->currentDate = boost::posix_time::second_clock::universal_time();
  std::cout << "\033[1m[\033[0m\033[1;32mEvent from " << ip << " at " << this->currentDate.date() << " " << this->currentDate.time_of_day() << "\033[0m\033[1m]\033[0m : " << info;
}

void          Logger::logError(const std::string& ip, const std::string& error) {
  this->currentDate = boost::posix_time::second_clock::universal_time();
  std::cout << "\033[1m[\033[0m\033[1;31mError from " << ip << " at " << this->currentDate.date() << " " << this->currentDate.time_of_day() << "\033[0m\033[1m]\033[0m : " << error << std::endl;
}

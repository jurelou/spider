#ifndef                                 LOGGER_HPP_
# define                                LOGGER_HPP_

#include                                <boost/enable_shared_from_this.hpp>
#include                                <boost/date_time/gregorian/gregorian.hpp>
#include                                <boost/date_time/posix_time/posix_time.hpp>
#include                                <boost/array.hpp>

class Logger : public boost::enable_shared_from_this<Logger> {
  public:
    Logger();
    ~Logger();
    static boost::shared_ptr<Logger>    create();
    void                                logMessage(const std::string& ip, const boost::array<char, 2048>& data);
    void                                logMessage(const std::string& ip, const std::string& message);
    void                                logEvent(const std::string& ip, const std::string& info);
    void                                logError(const std::string& ip, const std::string& error);
  private:
    boost::posix_time::ptime            currentDate;
};

#endif                                  /* !LOGGER_HPP_ */
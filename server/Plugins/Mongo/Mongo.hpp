#ifndef       MONGO_HPP_
# define      MONGO_HPP_

#include      "../IPlugin.hpp"

class Mongo  : public IPlugin {
  public:
    Mongo();
    ~Mongo();
    void        onLoad();
    void        onReceiveData(const std::string& hostname, const std::string& ip, const std::string& action);
    void        onSendData(const std::string& hostname, const std::string& ip, const std::string& action);
};

#endif        /* !MONGO_HPP */
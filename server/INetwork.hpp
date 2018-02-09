#ifndef             INETWORK_HPP_
# define            INETWORK_HPP_

#include            <string>

class               INetwork {
  public:
    virtual         ~INetwork() {};
    virtual void    initData() = 0;
    virtual void    receive() = 0;
    virtual void    send(const std::string& str) = 0;
};

#endif              /* !INETWORK_HPP_ */
#ifndef           IPLUGIN_HPP_
# define          IPLUGIN_HPP_

#include          <vector>
#include          <string>

class             IPlugin {
  public:
    virtual       ~IPlugin() {};
    /* Appelé lorsque le plugin est chargé */
    virtual void  onLoad() = 0;
    /* Appelé lorsque des données sont lues depuis la socket */
    virtual void  onReceiveData(const std::string& hostname, const std::string& ip, const std::string& action) = 0;
    /* Appelé lorsque des données vont être écrites sur la socket */
    virtual void  onSendData(const std::string& hostname, const std::string& ip, const std::string& action) = 0;
};

#endif            /* !IPLUGIN_HPP_ */
#ifndef                                 PLUGINS_HPP_
# define                                PLUGINS_HPP_

#include                                <boost/enable_shared_from_this.hpp>
#include                                "Plugins/IPlugin.hpp"

class                                   Plugins : public boost::enable_shared_from_this<Plugins> {
  public:
    Plugins();
    ~Plugins();
    static boost::shared_ptr<Plugins>   create();
    void                                onSendMiddleware(const std::string& hostname, const std::string& ip, const std::string& actio);
    void                                onReceiveMiddleware(const std::string& hostname, const std::string& ip, const std::string& actio);
    void                                checkAndLoadPlugin(const std::string& file);
    void                                loadPlugins();
  private:
    std::vector<IPlugin *>              plugins;
};

#endif                                  /* !PLUGINS_HPP_ */

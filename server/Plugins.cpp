#include            <iostream>
#include            <regex>
#include            <dirent.h>
#include            <dlfcn.h>
#include            "Plugins.hpp"

Plugins::Plugins() {}
Plugins::~Plugins() {}

boost::shared_ptr<Plugins> Plugins::create() {
  return boost::shared_ptr<Plugins>(new Plugins());
}

void                Plugins::onSendMiddleware(const std::string& hostname, const std::string& ip, const std::string& action) {
  for (auto it: this->plugins)
    it->onSendData(hostname, ip, action);
}

void                Plugins::onReceiveMiddleware(const std::string& hostname, const std::string& ip, const std::string& action) {
  for (auto it: this->plugins)
    it->onReceiveData(hostname, ip, action);
}

void                Plugins::checkAndLoadPlugin(const std::string& file) {
  void              *handler;
  void              *(*funcPtr)();

  if (!(handler = dlopen(file.c_str(), RTLD_LAZY)) || !(funcPtr = reinterpret_cast<void *(*)()>(dlsym(handler, "createPlugin")))) {
    std::cerr << "Error while loading plugin " << file << std::endl;
    return;
  }
  this->plugins.push_back(reinterpret_cast<IPlugin *>(funcPtr()));
  this->plugins.back()->onLoad();
}

void                Plugins::loadPlugins() {
  DIR     *dir;
  struct  dirent *sdir;

  dir = opendir("./Plugins");
  if (dir != NULL) {
    while ((sdir = readdir(dir))) {
      if (std::regex_match(std::string(sdir->d_name), std::regex("(splugin_)(.*)(.so)")))
        this->checkAndLoadPlugin("./Plugins/" + std::string(sdir->d_name));
    }
  }
  closedir(dir);
}

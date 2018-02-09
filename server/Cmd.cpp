#include                 "Cmd.hpp"

Cmd::Cmd() {
  this->actions.push_back(std::make_tuple("PAUSE", &Cmd::commandPause));
  this->actions.push_back(std::make_tuple("STOP", &Cmd::commandStop));
  this->actions.push_back(std::make_tuple("RESUME", &Cmd::commandResume));
  this->actions.push_back(std::make_tuple("SCREENSHOT", &Cmd::commandScreenshot));
  this->actions.push_back(std::make_tuple("STATUS", &Cmd::commandStatus));
}

Cmd::~Cmd() {}

boost::shared_ptr<Cmd>   Cmd::create() {
  return boost::shared_ptr<Cmd>(new Cmd());
}

void                     Cmd::commandPause(int id) {
  int                    i = 0;

  for (auto it: this->clientPool) {
    if (i == id) {
      it->send("PAUSE\r\n");
      return;
    }
    i += 1;
  }
}

void                     Cmd::commandStop(int id) {
  int                    i = 0;

  for (auto it: this->clientPool) {
    if (i == id) {
      it->send("STOP\r\n");
      return;
    }
    i += 1;
  }
}

void                     Cmd::commandResume(int id) {
  int                    i = 0;

  for (auto it: this->clientPool) {
    if (i == id) {
      it->send("RESUME\r\n");
      return;
    }
    i += 1;
  }
}

void                     Cmd::commandScreenshot(int id) {
  int                    i = 0;

  for (auto it: this->clientPool) {
    if (i == id) {
      it->send("SCREENSHOT\r\n");
      return;
    }
    i += 1;
  }
}

void                     Cmd::commandStatus(int id) {
  int                    i = 0;

  for (auto it: this->clientPool) {
    if (i == id) {
      it->send("STATUS\r\n");
      return;
    }
    i += 1;
  }
}

void                     Cmd::updatePool(const std::vector<boost::shared_ptr<SocketIO> >& clientPool) {
  this->clientPool = clientPool;
}

void                     Cmd::manageCmd(const std::vector<std::string>& cmd) {
  if (cmd[0] == "LIST") {
    this->list();
    return;
  } 

  for (auto it: this->actions) {
    if (std::get<0>(it) == cmd[1]) {
      (this->*(std::get<1>(it)))(std::stoi(cmd[0]));
      return;
    }
  }

  std::cout << "Error command not found" << std::endl;
}

void                     Cmd::list() {
  int                    i = 0;

  for (auto it: this->clientPool) {
    if (it->isActive)
      std::cout << "$ " << i << " " << it->getIp() << " " << it->getHostname() << std::endl;
    i += 1;
  }
}
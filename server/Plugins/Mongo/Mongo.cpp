#include        <iostream>
#include        <bsoncxx/builder/stream/document.hpp>
#include        <bsoncxx/json.hpp>
#include        <mongocxx/client.hpp>
#include        <mongocxx/instance.hpp>
#include        "Mongo.hpp"

Mongo::Mongo() {}
Mongo::~Mongo() {}

void            Mongo::onLoad() {
  std::cout << "🍃 MongoDB Plugin loaded" << std::endl;
}

void                    Mongo::onReceiveData(const std::string& hostname, const std::string& ip, const std::string& action) {
  mongocxx::instance    inst{};
  mongocxx::client      conn{mongocxx::uri{}};
  mongocxx::database    db = conn["Spider"];
  mongocxx::collection  log = db[ip];

  if (action.length() == 0)
    return;
  auto builder1 = bsoncxx::builder::stream::document{};
  bsoncxx::document::value doc_value1 = builder1
          << "Hostname" << hostname
          << "Data" << action
          << bsoncxx::builder::stream::finalize;

  bsoncxx::stdx::optional<mongocxx::result::insert_one> result1 = log.insert_one(doc_value1.view());
}

void            Mongo::onSendData(const std::string& hostname, const std::string& ip, const std::string& action) {
}

extern "C" {
  IPlugin       *createPlugin() {
    return (new Mongo());
  }
}

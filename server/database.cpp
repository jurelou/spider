#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

int main(int, char**) {
  mongocxx::instance inst{};
  mongocxx::client conn{mongocxx::uri{}};

  mongocxx::database db = conn["KeylDB"];
  mongocxx::collection log = db["log"];

  auto builder1 = bsoncxx::builder::stream::document{};
  bsoncxx::document::value doc_value1 = builder1
    << "_id" << 1
    << "IP" << "127.0.0.0.1"
    << "Hostname" << "Spring_r"
    << "Date" << "2.10.17 - 11h40"
    << "Data" << "Je pense qu'on m'espionne"
    << bsoncxx::builder::stream::finalize;

  bsoncxx::stdx::optional<mongocxx::result::insert_one> result1 =
    log.insert_one(doc_value1.view());

  auto builder2 = bsoncxx::builder::stream::document{};
  bsoncxx::document::value doc_value2 = builder2
    << "_id" << 2
    << "IP" << "127.0.0.0.1"
    << "Hostname" << "Spring_r"
    << "Date" << "2.10.17 - 11h45"
    << "Data" << "Don't search happiness"
    << bsoncxx::builder::stream::finalize;

  bsoncxx::stdx::optional<mongocxx::result::insert_one> result2 =
  log.insert_one(doc_value2.view());
}

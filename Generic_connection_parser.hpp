#pragma once

#include "Generic_station_parser.hpp"
#include <unordered_map>

namespace travel{
  class Generic_connection_parser: public Generic_station_parser{
  public:

    const std::unordered_map<uint64_t,std::unordered_map<uint64_t,uint64_t> >& get_connections_hashmap() const{
      return this->connections_hashmap;
    }

  protected:
    virtual void read_connections(const std::string& _filename) = 0;

  public:
    std::unordered_map< uint64_t ,std::unordered_map<uint64_t,uint64_t> > connections_hashmap;
  };
}

#ifndef JSONABLE_H
#define JSONABLE_H

#include <nlohmann/json.hpp>

using nlohmann::json;

class Jsonable {
public:
    virtual ~Jsonable() = default;
    [[nodiscard]] virtual json to_json() const = 0;
};

#endif //JSONABLE_H

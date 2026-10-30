#ifndef ROLE_H
#define ROLE_H

#include <string>

class Role {
private:
    std::string name_;
public:
    explicit Role(const std::string& name);
    const std::string& getName() const;
};

#endif // ROLE_H

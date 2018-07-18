#pragma once
#include <string>
#include <list>
#include <vector>
#include "Command.h"

class CommandList
{
    std::list<Command> m_CommandList;
public:
    friend std::ostream& operator<<(std::ostream &os, const CommandList &list);

    Command &push_back(const std::string &comm_str);
    Command &push_front(const std::string &comm_str);
    Command &push_to_index(const std::string &comm_str, size_t index);
    void assign(const std::vector<std::string> &commandList);

    void reset_by_index(const std::string &comm_str, size_t index);

    void remove(size_t index);
    void remove(const std::string &comm_str);

    Command &operator[](size_t index);
    Command &front();
    Command &back();

    void run() const;
};

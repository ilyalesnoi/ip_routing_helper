#include <iostream>
#include <algorithm>
#include "CommandList.h"

Command &CommandList::push_back(const std::string &comm_str)
{
    m_CommandList.push_back(comm_str);
    return m_CommandList.back();
}

Command &CommandList::push_front(const std::string &comm_str)
{
    m_CommandList.push_front(comm_str);
    return m_CommandList.front();
}

Command &CommandList::push_to_index(const std::string &comm_str, size_t index)
{
    auto it = std::begin(m_CommandList);
    std::advance(it, index);
    m_CommandList.insert(it, comm_str);
    std::cout << ">>> " << (*it).to_string() << "\n";
    return *it;
}

void CommandList::assign(const std::vector<std::string> &commandList)
{
    m_CommandList.clear();
    for(auto &com : commandList) {
        m_CommandList.push_back(com);
    }
}

void CommandList::reset_by_index(const std::string &comm_str, size_t index)
{
    auto it = std::begin(m_CommandList);
    std::advance(it, index);
    *it = comm_str;
}

void CommandList::remove(size_t index)
{
    auto it = std::begin(m_CommandList);
    std::advance(it, index);
    m_CommandList.erase(it);
}

void CommandList::remove(const std::string &comm_str)
{
    auto it =
    std::find_if
    (
        std::begin(m_CommandList),
        std::end(m_CommandList),
        [comm_str](const Command &cmd)
        {return !comm_str.compare(cmd.to_string());}
    );
    m_CommandList.erase(it);
}

Command& CommandList::operator[](size_t index)
{
    auto it = std::begin(m_CommandList);
    std::advance(it, index);
    return *it;
}

Command &CommandList::front()
{
    return m_CommandList.front();
}

Command &CommandList::back()
{
    return m_CommandList.back();
}

void CommandList::run() const
{
    for(auto &cmd : m_CommandList)
        cmd.run();
}

std::ostream& operator<<(std::ostream &os, const CommandList &list)
{
    size_t i = 0;
    for(auto elem : list.m_CommandList)
        os << i++ << " [" << elem.to_string() << "]\n";
    return os;
}

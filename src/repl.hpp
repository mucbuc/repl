#pragma once

#include <memory>
#include <string>
#include <thread>

namespace om636 {
struct repl : public std::enable_shared_from_this<repl> {

private:

    struct PrivateKey {};

public:

    repl(PrivateKey, std::istream& is, std::ostream& os)
        : m_exit(false)
        , m_is(is)
        , m_os(os)
    {
    }

    std::thread eval_loop(std::function<void(std::string, std::ostream&, std::function<void()>)> eval)
    {
        return std::thread([eval, shared_this = shared_from_this()] {
            while (true) {
                eval(read(shared_this->m_is), shared_this->m_os, [shared_this] {
                    shared_this->m_exit = true;
                });
                if (shared_this->m_exit) {
                    break;
                }
            }
        });
    }

    void exit()
    {
        m_exit = true;
    }

    bool should_exit()
    {
        return m_exit;
    }

    static std::shared_ptr<repl> make_repl(std::istream& is, std::ostream& os)
    {
        return std::make_shared<repl>(PrivateKey{}, is, os);
    }

private:

    static std::string read(std::istream& is)
    {
        std::string line;
        std::getline(is, line);
        return line;
    }

    std::atomic<bool> m_exit;
    std::istream& m_is;
    std::ostream& m_os;
};

}

#pragma once

#include <memory>
#include <string>
#include <thread>
#include <istream>
#include <functional>

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

    std::thread eval_loop(std::function<void(std::string, std::ostream&, std::function<void()>)> && eval)
    {
        return std::thread([eval = std::move(eval), shared_this = shared_from_this()] {
            while (!shared_this->should_exit()) {
                auto line = read(shared_this->m_is);

                if (shared_this->m_is.fail() || shared_this->m_is.eof())
                {
                    shared_this->exit();
                }
                else if (!line.empty())
                {
                    eval(std::move(line), shared_this->m_os, [shared_this] {
                        shared_this->exit();
                    });
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

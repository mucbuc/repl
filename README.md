# om636::repl

![Tests](https://github.com/mucbuc/repl/workflows/Test/badge.svg)

A lightweight, thread-safe C++ REPL (Read-Eval-Print Loop) library for building interactive command-line interfaces.

## Features

- **Header-only** - Single include, no linking required
- **Thread-safe** - Uses `std::atomic` for safe concurrent access
- **Flexible** - Custom evaluation function with exit callback
- **Modern C++** - Leverages smart pointers and move semantics
- **Stream-based** - Works with any `std::istream`/`std::ostream`

## Requirements

- C++14 or later
- Standard library with `<thread>`, `<functional>`, `<memory>`

## Basic Usage

Include the header:
```cpp
#include <lib/repl/src/repl.hpp>
```

Create a REPL instance and provide an evaluation function:
```cpp
auto repl = om636::repl::make_repl(std::cin, std::cout);
```

## API

### `make_repl(std::istream& is, std::ostream& os)`
Factory function to create a REPL instance.

### `eval_loop(std::function<void(std::string, std::ostream&, std::function<void()>)> eval)`
Starts the REPL loop in a new thread. Returns a `std::thread` that must be joined or detached.

**Callback parameters:**
- `std::string` - The input line read from the stream
- `std::ostream&` - Output stream for writing responses
- `std::function<void()>` - Exit callback to terminate the loop

### `should_exit()`
Returns `true` if the REPL has been signaled to exit.

### `exit()`
Signals the REPL to exit (thread-safe).

## Behavior

- Empty lines are automatically ignored
- EOF or stream failure triggers automatic exit
- The evaluation function is called for each non-empty line
- Thread ownership belongs to the caller (must join or detach)

## Examples

### Example 1: Blocking REPL (Join)

Simple echo REPL that blocks until "exit" is typed:

```cpp
#include <lib/repl/src/repl.hpp>
#include <iostream>

int main()
{
    auto repl = om636::repl::make_repl(std::cin, std::cout);

    repl->eval_loop([](auto message, auto & out, auto exit){
        out << message << std::endl;
        if (message == "exit") 
        {
            exit();
        }
    }).join();

    return 0; 
}
```

**Use case:** Simple command-line tools where the REPL is the main program activity.

### Example 2: Non-blocking REPL (Detach)

REPL runs in background while main thread does other work:

```cpp
#include <lib/repl/src/repl.hpp>
#include <iostream>

int main()
{
    auto repl = om636::repl::make_repl(std::cin, std::cout);

    repl->eval_loop([](auto message, auto & out, auto exit){
        out << message << std::endl;
        if (message == "exit") 
        {
            exit();
        }
    }).detach();

    while (!repl->should_exit())
    {
        // Main thread can do other work here
    }

    return 0; 
}
```

**Use case:** Applications that need a REPL interface alongside other concurrent operations (e.g., servers, games, monitoring tools).

## Thread Safety

- `exit()` and `should_exit()` are thread-safe
- Multiple threads can safely check exit status
- The input stream should only be read by the REPL thread
- The output stream may need external synchronization if accessed from multiple threads


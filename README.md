# <p align="center"><img src="https://repository-images.githubusercontent.com/233715891/ac176580-3666-11ea-9cb6-d12850ec2d74" width="80px" height="auto"></p> Volta

Simplified Header-only volatile read and write in C++


## What is `volatile` ?
A C and C++ keyword commonly used to suppress compiler optimizations.
Commonly used when performing memory mapped I/O.
Volatile can also be thought of as "Hey compiler, I do not want you to optimize whatever I do here"


### Quick Example:
        
```cpp
        #include <cinttypes>

        uint16_t volatile tick_count = 0U;

        auto TickISR() -> void {
            // increment tick count
            tick_count++;
        }

        auto PerformTask() -> void {
            // do something
        }

        int main(){
            while(true){
                while(tick_count > 512U) {
                    PerformTask();
                };
            };
        }
```


In this example TickISR might appear not to be called at all and thus, without `volatile` the compiler might assume `tick_count` never changes before and after main. Since `TickISR` can actually be called by an interrupt request, the `volatile` keyword is needed to prevent the assumption that `tick_count` might not change (remain at 0U). Else, the `PerformTask()` might never execute in this example.



## What is the problem with `volatile`? 
- The `volatile` keyword is easily misused and often hinders performance and compiler optimizations in the process.
- It is difficult to tell by just looking at a code whether we are dealing with volatile read and write or not, as the code base grows larger.
- Declaring variables as `volatile` is not the initial intention for `volatile`.
        intended use (by C standard) is for pointers only, just as in:
            
```cpp
            #include <cinttypes>

            uint16_t tick_count = 0U;

            auto TickISR() -> void {
                // get tick count address
                auto tick_count_ptr = &tick_count;

                // make volatile so whatever we do on the variable is not optimized / elided by the compiler
                auto volatile_tick_count_ptr = (volatile uint16_t *)tick_count_ptr;

                // here, we perform a volatile write (increment)
                (*volatile_tick_count_ptr)++;
            }
```
            


## Example with Volta
        
```cpp
        #include <cinttypes>

        #include "volta/volta.h"

        uint16_t tick_count = 0U;

        auto TickISR() -> void {
            // perform a volatile read
            auto tick_count_capture = volta::volatile_read(&tick_count);

            // increment tick count
            tick_count_capture++;

            // perform a volatile write
            volta::volatile_write(&tick_count, tick_count_capture);
        }

        auto PerformTask() -> void {
            // do something
        }

        int main(){
            while(true){
                while(volta::volatile_read(&tick_count) > 512U) {
                    PerformTask();
                };
            };
        }
```
        
## My Tip
- Never use the keyword `volatile`
- Only use `volta::volatile_read` and `volta::volatile_read` on integral types or intergal member types


## Inspiration
- This is inspired by <a href="https://twitter.com/jfbastien">JF Bastein's</a> lightning talk at CppCon 2019

<a href="https://www.youtube.com/watch?v=KJW_DLaVXIY"><img src="https://img.youtube.com/vi/KJW_DLaVXIY/maxresdefault.jpg" width="100%" height="auto"/> </a>

- Also inspired by Rust's `core::ptr::read_volatile` and `core::ptr::write_volatile`

### Installation
- This is an header-only library and can be used directly in your projects.
- For CMake, use the command:
    `add_subdirectory(parent_dir/volta volta_dependency)`
    where `parent_dir` is the parent directory of volta as downloaded to your workspace.


### C++ Support
- C++ 11
- C++ 14
- C++ 17
- C++ 20


### Build Configurations
- None

### CMAKE options:
- None

### Include Paths:
- volta

### Compiler Flags:
- None
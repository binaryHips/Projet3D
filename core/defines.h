#pragma once
#include <iostream>


// Definitions

#ifndef NDEBUG
#define DEBUG false
#else
#define DEBUG true
#endif

#if DEBUG
    #define DEBUG_EXCEPT 
#else
    #define DEBUG_EXCEPT noexcept
#endif

// assertions (https://www.foonathan.net/2016/09/assertions/)
namespace {
    struct source_location{
        const char* file_name;
        unsigned line_number;
        const char* function_name;
    };


    class BaseHandler{
        static void handle(const source_location& loc, const char* expr) noexcept {
            std::cout << "ASSERTION FAILED\n" <<
            "   File " << loc.file_name << "\n" <<
            "   Function " << loc.function_name << "\n" <<
            "   Line " << std::to_string(loc.line_number) << "\n" <<
            "On expression ( " << expr << " )" << 
            std::endl;
        }
    };

    template <class Handler, class Expr>
    void do_assert(std::true_type, const Expr& e, const source_location& loc, const char* expression) noexcept
    {
        if (!e())
        {
            Handler::handle(loc, expression);
            std::abort();
        }
    }

    template <class Handler, class Expr>
    void do_assert(std::false_type, const Expr&, const source_location&, const char*) noexcept {}

    template <class Handler, class Expr>
    void do_assert(const Expr& e, const source_location& loc, const char* expression){
        do_assert<Handler>(Handler{}, e, loc, expression);
    }

    #define ASSERT(Expr, Handler) \
        do_assert<Handler>([&] { return Expr; }, CUR_SOURCE_LOCATION, #Expr)
}



// Type aliases

using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;

using ParticleId = u32;
using Pixel = u8;


#pragma once
#include <Urho3D/IO/Log.h>
#include <tuple>
#include <ostream>

template<bool Statement, typename Return>
using EnableIf = typename std::enable_if<Statement, Return>::type;



struct Log {
public:
    template<typename ... T>
    static void Info(const T& ... args) {
        URHO3D_LOGINFO(tupleToString(std::tie(args...)));
    }

private:
    template<size_t Current = 0, typename... T>
    static auto tupleToString(const std::tuple<T...>&) ->
        EnableIf<(Current >= sizeof...(T)), Urho3D::String> {

        return Urho3D::String::EMPTY;
    }

    template<size_t Current = 0, typename... T>
    static auto tupleToString(const std::tuple<T...>& t) ->
        EnableIf<(Current < sizeof...(T)), Urho3D::String> {

        return Urho3D::String {std::get<Current>(t)} + ", " +
                               tupleToString<Current+1>(t);
    }
};


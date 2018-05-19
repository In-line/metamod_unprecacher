//
// Created by alik on 5/18/18.
//
#pragma once
#ifndef UNPRECACHER_FAST_UNORDERED_STRING_MAP
#define UNPRECACHER_FAST_UNORDERED_STRING_MAP

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <string>
#include <string_view>

namespace up {

    namespace detail {
        struct universal_string_hash {
            inline std::size_t operator()(const std::string &v) const {
                return std::hash<std::string>()(v);
            }

            inline std::size_t operator()(const std::string_view &v) const {
                return std::hash<std::string_view>()(v);
            }
        };

        struct universal_equal_to {
            template<typename T, typename U>
            inline bool operator()(const T &v1, const U &v2) const {
                return v1 == v2;
            }
        };

        using namespace boost::multi_index;
        template<typename T>
        using unordered_string_map = multi_index_container<
        std::pair<std::string, T>,
        indexed_by<
                hashed_unique<
                        member<
                                std::pair<std::string, T>,
                                std::string,
                                &std::pair<std::string, T>::first
                                >,
                        universal_string_hash,
                        universal_equal_to
                        >
                >
        >;
    }

    template<typename T>
    using fast_unordered_string_map = detail::unordered_string_map<T>;
}
#endif //UNPRECACHER_FAST_UNORDERED_STRING_MAP

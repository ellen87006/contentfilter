cmake_minimum_required (VERSION 2.6)

project (contentfilter)

add_executable (contentfilter filter.cpp)
add_executable (Bcontentfilter Brute.cpp)
add_executable (Trie Trie.cpp)
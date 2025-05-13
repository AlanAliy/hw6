#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <unordered_map>
#include <vector>
typedef std::size_t HASH_INDEX_T;
 

std::unordered_map<char, uint> build_chars_to_ints() {
    std::unordered_map<char, uint> map;
    for (char c = 'a'; c <= 'z'; ++c) {
        map[c] = c - 'a';  // 'a'->0, 'b'->1, ..., 'z'->25
    }
    for (char c = '0'; c <= '9'; ++c) {
        map[c] = 26 + (c - '0'); // '0'->26, '1'->27, ..., '9'->35
    }

    return map;
};

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    std::unordered_map<char,uint> chars_to_ints = build_chars_to_ints();

    
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        std::vector<unsigned long long> numbers(5,0);
        std::string clean;

        for (char c : k) {
            char character = std::tolower(c);
            if(chars_to_ints.count(character)) {
                clean += character;
            }
        }
        
        int len = clean.size();
        int group = 4;
        
        while (len > 0 && group >= 0) {
            int chunk_len = std::min(6, len);
            int start = len - chunk_len;

            unsigned long long value = 0;
            for (int i = start; i < len; ++i) {
                value = value * 36 + chars_to_ints.at(clean[i]);
            }

            numbers[group] = value;
            len -= chunk_len;
            --group;
        }

        unsigned long long hash = 0;
        for (int i = 0; i < 5; i++) 
        {
            hash += rValues[i] * numbers[i];
        }
        return hash;
    }


    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif

#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here  
        unsigned long long w[5];

        // Make a string of size 30 (adding a's to the beggining because they have a value of 0)
        std::string full_string = fillString(k);
        // Find the hashes and add them to w
        fillHashes(full_string, w, 0);
        // "salt" each hash and add them together
        HASH_INDEX_T value = addHashes(rValues, w);
        return value;
    }

    void fillHashes(const std::string& k, unsigned long long *w, size_t depth) const
    {
        if (depth == 5) return;

        size_t substr = k.size() / 6;
        if (k.size() % 6 != 0) substr++;

        unsigned long long hash = findHash(k, depth);
        w[depth] = hash;
        fillHashes(k, w, depth + 1);
    }

    // Creates a string of a length of size 30, so that it divides evenly to 6 substrings
    std::string fillString(const std::string& k) const
    {
        std::string returnee("");
        size_t i = 30 - k.size();
        if (i != 0)
        {
            for(; i > 0; i--)
            {
                returnee += 'a';
            }
            returnee += k;
        }
        else returnee = k;

        return returnee;
    }

    // Finds the hash of every group of 6 characters
    long long findHash(const std::string& k, size_t depth) const
    {
        if (k.empty() || (!k.empty() && depth * 6 >= k.size())) return 0; // Fix this param
        std::string substr("");
        for (size_t i=0; i < 6; ++i)
        {
            size_t d = depth * 6 + i;
            if (d < k.size()) substr += k[depth*6 + i];
        }

        return (((((( letterDigitToNumber(substr[0]) ) * 36 + letterDigitToNumber(substr[1]) ) * 36 + letterDigitToNumber(substr[2]) ) * 36 + letterDigitToNumber(substr[3]) ) * 36 + letterDigitToNumber(substr[4]) ) * 36 + letterDigitToNumber(substr[5]));
    }   

    // Multiplies the rValues with each hash and adds them together
    HASH_INDEX_T addHashes(const HASH_INDEX_T *rValues, unsigned long long *w) const
    {
        HASH_INDEX_T value = (rValues[0] * w[0]) + (rValues[1] * w[1]) + (rValues[2] * w[2])  + (rValues[3] * w[3]) + (rValues[4] * w[4]);
        return value;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if (97 <= letter) return letter-97; // lowercase letters
        else if (65 <= letter) return letter-65; // capital letters
        else return letter-22; // numbers
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

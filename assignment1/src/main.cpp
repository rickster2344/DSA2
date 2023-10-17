#include <iostream>
#include <fstream>
#include "hash.h"
#include "spellcheck.h"

int main()
{
    std::string dictLoc;
    std::cout << "Enter name of dictionary:";
    std:: cin >> dictLoc;
    hashTable dictionary(50000); //initialize an average 50000 word dictionary 
    // dictLoc = "./include/wordlist_small.txt";
    loadDictionary(dictLoc, dictionary);   

    std::string inputLoc;
    std::cout <<"Enter name of input file:";
    std::cin >> inputLoc;

    std::string outName;
    std::cout <<"Enter name of output file:";
    std::cin >>outName;

    // inputLoc = "./include/lyrics.txt";
    // outName = "checkedLyrics.txt";

    spellCheck(inputLoc, outName, dictionary);
}

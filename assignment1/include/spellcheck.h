#pragma once
#include "hash.h"
#include <fstream>
#include <time.h>
#include <iostream>



enum wordType{valid, tooLong, invalidCh};
wordType checkWord(std::string word);

void loadDictionary(std::string dictLocation, hashTable &dictionary); //pass address of dictionary
void spellCheck(std::string inputLocation, std::string outName, hashTable &dictionary);
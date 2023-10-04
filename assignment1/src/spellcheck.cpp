#include <iostream>
#include "spellcheck.h"
#include "hash.h"

//code that will go through the lyrics and find if the words exist in the dictionary
void loadDictionary(std::string dictLocation, hashTable &dictionary){
    std::ifstream infile; 
    infile.open(dictLocation);
    clock_t t = clock();

    if(infile.is_open())
    {
        std::string word;
        int numInvalid =0;
        while (getline(infile, word))
        {
            if(checkWord(word)==valid){
                for(int i=0; i< word.length(); i++){
                    word[i] = tolower(word[i]);
                }       
                dictionary.insert(word);
            }
            else
            {//not used but useful for debugging
                // std::cout<<word<<std::endl;
                numInvalid++;
            }
        }
        infile.close();
    }
    else
    {
        std::cout << "Unable to open dictionary input file"<<std::endl;
        return;
    }
    t = clock()-t;
    float cpuTime = (float)t/CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to load dictionary: " << cpuTime <<std::endl;
}

wordType checkWord(std::string word){
    int count = 0;
    for(char ch : word){
        if(isalnum(ch) ==0 && ch != '-' && ch != '\''){
            return invalidCh;
        }
        count++;
        if(count>20){
            return tooLong;
        }
    }
    //note there is no point in checking if there are digits because nothing happens when there is a digit anyways
    return valid;
}


void spellCheck(std::string inputLoc, std::string outName, hashTable &dictionary)
{
    std::ifstream infile; 
    infile.open(inputLoc);

    std::ofstream output(outName);
    clock_t t = clock();
    int lineNum = 1;

    if(infile.is_open())
    {
        std::string line;
        std::string word;
        int linelength = 0;
        while (getline(infile, line))
        {
            linelength = line.length();
            if (linelength != 0)
            {
                for(int i=0; i<=linelength; i++)
                {
                    if(isalnum(line[i])!=0 || line[i] == '-' || line[i]=='\'')
                    {
                        word += tolower(line[i]);
                    }   
                    else
                    {
                        if(word.length() != 0)
                        {
                            if(checkWord(word)==tooLong)
                            {
                                output << "Long word at line " <<lineNum<< ", starts: " << word.substr(0,20)<< std::endl;
                            }
                            else if(!dictionary.contains(word))
                            {
                                output << "Unknown word at line "<<lineNum<< ": " << word<<std::endl;
                            }
                            word = "";
                        }
                    }
                }   
            }
            lineNum++;
        }
        infile.close();
        output.close();
    }
    else
    {
        std::cout << "Unable to open input file" << std::endl;
        return;
    }
    t = clock() - t;
    float cpuTime = (float)t/CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to check document: " << cpuTime <<std::endl;
}

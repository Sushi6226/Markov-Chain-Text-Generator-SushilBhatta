#include "markov.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

int main ()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    const int MAX_WORDS = 5000;

    std::string filename;
    std::string inputLine;
    int order = 0;
    int requestedWords = 0;

    while (true)
    {
        std::cout<<"Enter the name of the file: ";

        if(!std::getline(std::cin, filename))
        {
            std::cout <<"\nSorry, text could't be generated. good luck next time :) ";
            return 1;
        }
        if (filename.empty())
        {
            std::cout << "The filename cannot be empty. Please try again :) ";
            continue;
        }
        break;
    }

    while (true) 
    {
        std::cout << "Enter order (1, 2, or 3): ";

        if (!std::getline(std::cin, inputLine)) 
        {
            std::cout << "\nSorry, text could't be generated. good luck next time :) \n";
            return 1;
        }

        std::istringstream input(inputLine);
        char extra;

        if (!(input >> order) || (input >> extra)) 
        {
            std::cout << "Invalid input. Please enter a whole number: 1, 2, or 3.\n";
            continue;
        }

        if (order < 1 || order > 3) 
        {

            std::cout << "The order must be between 1 and 3. Please try again:) \n";
            continue;
        }

        break;
    }

     while (true) 
     {
        std::cout << "Enter maximum number of words to generate (4000 is max): ";

        if (!std::getline(std::cin, inputLine)) 
        {
            std::cout << "\n Sorry, No text was generated :( \n";
            return 1;
        }

        std::istringstream input(inputLine);
        char extra;

        if (!(input >> requestedWords) || (input >> extra)) 
        {
            std::cout << "Invalid input. Enter a whole number within the "
                         "range of an int.\n";
            continue;
        }

        if (requestedWords < order) 
        {
            std::cout << "The requested count must be at least " << order
                      << " because the initial prefix counts toward the total:)\n";
            continue;
        }

        break;
    }

    std::string words[MAX_WORDS];
    std::string prefixes[MAX_WORDS];
    std::string suffixes[MAX_WORDS];

    int numWords = readWordsFromFile(filename, words, MAX_WORDS);

    if (numWords == -1) {
        std::cout << "Could not open :( \"" << filename
                  << "\". sadly, no text was generated.\n";
        return 1;
    }

    if (numWords == 0) {
        std::cout << "The file is empty or contains only whitespace. "
                     "wuhh, no text was generated.\n";
        return 1;
    }

    if (numWords <= order) {
        std::cout << "The file contains only " << numWords
                  << " training word(s). Order " << order
                  << " requires at least " << order + 1
                  << ". No text was generated.\n";
        return 1;
    }

    int chainSize = buildMarkovChain(words, numWords, order,
                                     prefixes, suffixes, MAX_WORDS);

    if (chainSize <= 0) {
        std::cout << "No prefix-suffix pairs were available. "
                     "No text was generated.\n";
        return 1;
    }

    std::string output = generateText(prefixes, suffixes, chainSize, order, requestedWords);

    std::istringstream generatedWords(output);
    std::string word;
    int actualWordCount = 0;
    while (generatedWords >> word)
    {
        ++actualWordCount;
    }

    std::cout << "\nGenerated Text:\n"<< output << "\n\n";
    std::cout<< "Generated " << actualWordCount << "of at most " << requestedWords << "words.\n";

    if (actualWordCount < requestedWords)
    {
        std::cout << "Stopped early: there is no record for current prefix :(" 
                     "successor (a dead end).\n";
    }

    return 0;


}
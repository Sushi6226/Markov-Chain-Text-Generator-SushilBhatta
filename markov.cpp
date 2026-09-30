#include "markov.h"

#include <cstdlib>
#include <fstream>
#include <string>

std::string joinWords(const std::string words[], int startIndex, int count)
{
    if (startIndex < 0 || count <= 0) {
        return "";
    }

    // The caller must ensure the entire requested range is in bounds.
    std::string result;

    for (int i = 0; i < count; ++i) {
        if (i > 0) {
            result += ' ';
        }
        result += words[startIndex + i];
    }

    return result;
}

int readWordsFromFile(std::string filename, std::string words[], int maxWords)
{
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        return -1;
    }

    if (maxWords <= 0) {
        return 0;
    }

    int count = 0;

    // Check capacity before accessing the next array element.
    while (count < maxWords && inputFile >> words[count]) {
        ++count;
    }

    inputFile.close();
    return count;
}

int buildMarkovChain(const std::string words[], int numWords, int order,
                     std::string prefixes[], std::string suffixes[],
                     int maxChainSize)
{
    if (order < 1 || order > 3 ||
        numWords <= order || maxChainSize <= 0) {
        return 0;
    }

    int count = 0;

    for (int i = 0;
         i < numWords - order && count < maxChainSize;
         ++i) {
        // Keep every occurrence, including duplicate pairs.
        prefixes[count] = joinWords(words, i, order);
        suffixes[count] = words[i + order];
        ++count;
    }

    return count;
}

std::string getRandomSuffix(const std::string prefixes[],
                            const std::string suffixes[],
                            int chainSize, std::string currentPrefix)
{
    if (chainSize <= 0) {
        return "";
    }

    int matchCount = 0;

    for (int i = 0; i < chainSize; ++i) {
        if (prefixes[i] == currentPrefix) {
            ++matchCount;
        }
    }

    if (matchCount == 0) {
        return "";
    }

    int pick = std::rand() % matchCount;
    int matchIndex = 0;

    for (int i = 0; i < chainSize; ++i) {
        if (prefixes[i] == currentPrefix) {
            if (matchIndex == pick) {
                return suffixes[i];
            }
            ++matchIndex;
        }
    }

    return "";
}

std::string getRandomPrefix(const std::string prefixes[], int chainSize)
{
    if (chainSize <= 0) {
        return "";
    }

    int index = std::rand() % chainSize;
    return prefixes[index];
}

std::string generateText(const std::string prefixes[],
                         const std::string suffixes[],
                         int chainSize, int order, int numWords)
{
    if (chainSize <= 0 || order < 1 || order > 3 || numWords < order) {
        return "";
    }

    std::string currentPrefix = getRandomPrefix(prefixes, chainSize);
    std::string currentWords[3];
    std::string temp;
    int wordIndex = 0;

    // Split the starting prefix, checking bounds before each array write.
    for (std::string::size_type i = 0; i < currentPrefix.length(); ++i) {
        if (currentPrefix[i] == ' ') {
            if (wordIndex >= order || temp.empty()) {
                return "";
            }

            currentWords[wordIndex] = temp;
            ++wordIndex;
            temp = "";
        } else {
            temp += currentPrefix[i];
        }
    }

    if (wordIndex != order - 1 || temp.empty()) {
        return "";
    }
    currentWords[wordIndex] = temp;

    std::string result = currentPrefix;

       for (int wordCount = order; wordCount < numWords; ++wordCount) {
        std::string newWord =
            getRandomSuffix(prefixes, suffixes, chainSize, currentPrefix);

        if (newWord.empty()) {
            break;
        }

        result += ' ';
        result += newWord;

        for (int j = 0; j < order - 1; ++j) {
            currentWords[j] = currentWords[j + 1];
        }
        currentWords[order - 1] = newWord;

        currentPrefix = joinWords(currentWords, 0, order);
    }

    return result;
}
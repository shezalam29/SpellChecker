// Shezan Alam

// References used
// - Data Structures and Algorithm Analysis in C++ by Mark A. Weiss
// - Sven Dietrich CSCI 335 Slides
// In this file, I implemented a spell-checker. The program takes in two files. 
// One file contains words to be spell-checked and corrected. The other file is a 
// dictionary file that contains words of the English language. The program 
// first adds all the words into a hashtable that uses 
// double hashing. Then it will check to see if each word in the table can be 
// found in the dictionary file. If it is not found (if it's incorrectly spelled)
// the program will add a letter to any spot in the word, remove a letter, or keep swapping 
// adjacent letters until it find the correct word. The program
// prints if the word is correct or incorrect and prints out the correction
// when needed. 

#include "double_hashing.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/**
 * @brief Makes the dictionary HashTable
 * @tparam HashTableType 
 * @param hash_table 
 * @param dictionary_filename 
 */
template <typename HashTableType>
void MakeDictionary(HashTableType &hash_table,
                    const string &dictionary_filename) {
  string Word;
  ifstream inFile(dictionary_filename);
  if (inFile.is_open()) {
    while (inFile >> Word) {
      hash_table.Insert(Word);
    }
  }
}

/**
 * @brief This helper function first converts all letters to lowercase
 * Then it removes punctuation from the end of the word
 * @param old : the string with punctuation to be removed 
 * @return string : the lowercase version string of the word without punctuation 
 */
string puncCheck(string old) 
{
  std::transform(old.begin(), old.end(), old.begin(), ::tolower);
  
  if (old[old.length() - 1] == '.')
  {
    old = old.substr(0, old.length() - 1);
  }
  if (old[old.length() - 1] == ',') 
  {
    old = old.substr(0, old.length() - 1);
  }
  if (old[old.length() - 1] == '!') 
  {
    old = old.substr(0, old.length() - 1);
  }
  if (old[old.length() - 1] == '?') 
  {
    old = old.substr(0, old.length() - 1);
  }
  
  return old;
}


/**
 * @brief The actual spell checker that compares the word to see if its 
 * is in the dictionary and then corrects it by adding a letter
 * removing a letter or swapping adjacent characters 
 * @tparam HashTableType 
 * @param hash_table 
 * @param document_filename 
 */
template <typename HashTableType>
void SpellChecker(HashTableType &hash_table, const string &document_filename) {
  string Word;

  ifstream inFile(document_filename);
  if (inFile.is_open()) {
    while (inFile >> Word) {

      Word = puncCheck(Word);

      if (hash_table.Contains(Word)) {
        cout << Word << " is CORRECT\n";
      } else {
        cout << Word << " is INCORRECT";
// Case A 
        for (unsigned int i = 0; i < Word.length(); i++) 
        {
          for (unsigned int j = 97; j < 122; j++) 
          {
            string temp = Word.insert(i,1,j);
            if (hash_table.Contains(temp)) 
            {
              cout << "\n" << "*** " << Word << " -> " << temp << " *** case A"; 
            }

            Word = Word.erase(i,1);

          }
        }
// Case B
        for (size_t i = 0; i < Word.length(); i++) {
          string temp = Word.substr(0, i) + Word.substr(i+1);
          if (hash_table.Contains(temp)) {
              cout << "\n" << "*** " << Word << " -> " << temp << " *** case B"; 
          }
        }
// Case C
        for (size_t i = 0; i < Word.length(); i++) {
          string temp = Word;
          char x = Word[i];
          char y = Word[i+1];
          temp[i]= y;
          temp[i+1] = x;
          if(hash_table.Contains(temp)){
              cout << "\n" << "*** " << Word << " -> " << temp << " *** case C"; 
          }
        }
        cout << endl;
      }
    }
  }
}




/**
 * @brief Acts as the main function 
 * @param argument_count 
 * @param argument_list 
 * @return int 
 */
int testSpellingWrapper(int argument_count, char** argument_list) {
    const string document_filename(argument_list[1]);
    const string dictionary_filename(argument_list[2]);

    // Call functions implementing the assignment requirements.
     HashTableDouble<string> dictionary;
     MakeDictionary(dictionary, dictionary_filename);
     SpellChecker(dictionary, document_filename);

    return 0;
}

// Sample main for program spell_check.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testSpellingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>"
             << endl;
        return 0;
    }

    testSpellingWrapper(argc, argv);

    return 0;
}

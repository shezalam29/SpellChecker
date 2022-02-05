// Shezan Alam

// References used
// - Data Structures and Algorithm Analysis in C++ by Mark A. Weiss
// - Sven Dietrich CSCI 335 Slides
// In this file, I added statements to TestFunctionForHashTable
// to print out information including number of elements in the table, 
// the size of the table, the load factor, the average number of collisions,
// and the total number of collisons.
// Rvalue was specificied. 


#include "quadratic_probing.h"
#include "linear_probing.h"
#include "double_hashing.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/**
 * @brief Checks whether each word in a given file is in the hash table 
 * then prints the corresponding output depending on whether the word is found 
 * or not and how many probes it took to find 
 * @tparam HashTableType 
 * @param hash_table 
 * @param words_filename - txt file that contains list of words
 * @param query_filename - txt file that contains words to be searched 
 */ 
template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
  //cout << "TestFunctionForHashTables..." << endl;
  //cout << "Words filename: " << words_filename << endl;
  //cout << "Query filename: " << query_filename << endl;
  hash_table.MakeEmpty();  
  //..Insert your own code

  std:: ifstream word_file(words_filename);
  std:: ifstream query_words(query_filename);

  string inp;
  while (getline(word_file, inp)){
      hash_table.Insert(inp);
  }
  

  cout << "number_of_elements: " << hash_table.getNumElements() << endl;
  cout << "size_of_table: " << hash_table.getArraySize() << endl;
  cout << "load_factor: " << hash_table.load_factor() << endl;
  cout << "average_collisions: " << hash_table.avg_collisions() << endl;
  cout << "total_collisions: " << hash_table.getNumCollisions() << endl;

  cout << "\n";

  while(getline(query_words, inp)) 
  {
    hash_table.clear_();
    cout << inp << " ";
    
    if (!hash_table.Contains(inp)) 
    {
      cout << "Not_Found " << hash_table.getNumCollisions() + 1 << "\n";
    }
    else
    {
      cout << "Found " << hash_table.getNumCollisions() + 1 << "\n";
    } 
  }
}



// namespace PROTECTED {

/**
 * @brief Essentially acts as the main function
 * calls the TestFuctionForHashTable function based on if 
 * user specifies flag 
 * @param argument_count 
 * @param argument_list 
 */
int testHashingWrapper(int argument_count, char **argument_list) {
    const string words_filename(argument_list[1]);
    const string query_filename(argument_list[2]);
    const string param_flag(argument_list[3]);
    
    int R = 97;
    if (argument_count == 5) {
      const string rvalue(argument_list[4]);
      R = stoi(rvalue);
    }

    if (param_flag == "linear") {
        //cout << "Linear:" << endl;
        HashTableLinear<string> linear_probing_table;
        TestFunctionForHashTable(linear_probing_table, words_filename,
                                query_filename);
    } else if (param_flag == "quadratic") {
        //cout << "Quadratic:" << endl;
        HashTable<string> quadratic_probing_table;
        TestFunctionForHashTable(quadratic_probing_table, words_filename,
                                 query_filename);
    } else if (param_flag == "double") {
        //cout << "Double:" << endl;
        cout << "r_value: " << R << endl;
        HashTableDouble<string> double_probing_table;
        TestFunctionForHashTable(double_probing_table, words_filename,
                                 query_filename);
    } else {
        cout << "Unknown hash type " << param_flag
             << " (User should provide linear, quadratic, or double)" << endl;
    }
    return 0;
}

// Sample main for program create_and_test_hash.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testHashingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char **argv) {
    if (argc != 4 and argc != 5) {
        cout << "Usage: " << argv[0]
             << " <wordsfilename> <queryfilename> <flag>" << endl;
        cout << "or Usage: " << argv[0]
             << " <wordsfilename> <queryfilename> <flag> <rvalue>" << endl;
        return 0;
    }

    testHashingWrapper(argc, argv);
    return 0;
}

//};  // namespace PROTECTED


// Shezan Alam

// References used
// - Data Structures and Algorithm Analysis in C++ by Mark A. Weiss
// - Sven Dietrich CSCI 335 Slides 
// In this file, again code is very similar and attributed to the already provided 
// quadratic_probing.h file.
// Again, there are getter and calculation functions for the print statements 
// in create_and_test_hash.cc 
// Tweaks were made in the FindPos() 
// Rvalue I set to 97, a prime number less than the size of the table 
// A second internal hash function is added based on the hashing technique described in the textbook

#ifndef DOUBLE_HASHING_H
#define DOUBLE_HASHING_H

#include <vector>
#include <algorithm>
#include <functional>
#include "quadratic_probing.h"

template <typename HashedObj>
class HashTableDouble {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableDouble(size_t size = 101, size_t rVal = 97) : array_(NextPrime(size)), rVal{rVal}
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

  // Code added here 

  /**
   * @brief Returns number of elements 
   * @return current_size_ of HashTable
   */
  size_t getNumElements() const {
    return current_size_;
  }
  
  /**
   * @brief Returns load factor 
   * @return float 
   */
  float load_factor() const {
    float load_factor = (float)current_size_ / (float)array_.size();
    return load_factor;
  }
  
  /**
   * @brief Returns average # of collisions 
   * @return float 
   */
  float avg_collisions() const {
    float average_collisions = (float)getNumCollisions() / (float)getNumElements();
    return average_collisions;
  }
  
  /**
   * @brief size of table 
   * @return size_t 
   */
  size_t getArraySize() const {
    return array_.size();
  }
  
  /**
   * @brief Returns the number of collisions 
   * @return size_t 
   */
  size_t getNumCollisions() const {
    return collisions_;
  }
  
  /**
   * @brief collisions_ variable set to 0 
   */
  void clear_() {
    collisions_ = 0;
  }

 private:        
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    

  std::vector<HashEntry> array_;
  size_t current_size_;
  mutable size_t collisions_ = 0; // needs to be mutable because at line 181 we are assigning 
                                //a non-static data member within const member function FindPos

  size_t rVal = 97;

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }
  /**
   * @brief Edited FindPos function to iterate collisions by 1 in 
   *        the while loop which checks if it was not EMPTY.
   *        Offset based on second internal hash function 
   *        
   * 
   * @param x 
   * @return size_t current_pos
   */
  size_t FindPos(const HashedObj & x) const {
    size_t const offset = InternalHash_(x);
    size_t current_pos = InternalHash(x);
      
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      
      collisions_ += 1; 
      current_pos += offset;  // Compute ith probe.
      if (current_pos >= array_.size())
	current_pos -= array_.size();
    }
    return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }

  /**
   * @brief Second internal hash 
   * @param x 
   * @return size_t 
   */
  size_t InternalHash_(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return (rVal - (hf(x) % rVal)); // array_.size();
    // calculation based on hashing technique described in the textbook 
    // hash2(x) = R - (xmodR)
  }
};

#endif  // DOUBLE_HASHING_H

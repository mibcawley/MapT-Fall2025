//
// Created by we7289 on 4/19/2021.
//

#include "MapT.h"
#include <unordered_map>
//using namespace std;

const int DEFAULT_BUCKETS = 10;
const double DEFAULT_LOAD = 1.0;

template<class K, class T>
MapT<K, T>::MapT() {
    buckets = new forward_list<pair<K,T>>[DEFAULT_BUCKETS];
    numBuckets = DEFAULT_BUCKETS;
    numKeys = 0;

    // 1.0 means that on average, 1 value per bucket
    maxLoad = DEFAULT_LOAD;
}

template <class K, class T>
MapT<K, T>::MapT(int numBucks)
{
    buckets = new forward_list<pair<K,T>>[numBucks];
    numBuckets = numBucks;
    numKeys = 0;

    // 1.0 means that on average, 1 value per bucket
    maxLoad = DEFAULT_LOAD;
}

template<class K, class T>
void MapT<K, T>::Add(K key, T value) {

    Remove(key); // Remove the key/value pair first (if it is in the map)

    // Find the appropriate bucket that
    // key lives in
    int bucket = GetHashIndex(key);

    // // Check to see if the key already exists
    // // If we do find it, replace the value
    // //             forward list beginning         forward list end       moves to next item in list
    // for (auto it = buckets[bucket].begin(); it != buckets[bucket].end(); ++it)
    // {
    //     if (it->first == key)
    //     {
    //         it->second = value; // change the value associated with key
    //         return;
    //     }
    // }

    pair<K, T> keyVal;
    keyVal.first = key;  // first stores the key
    keyVal.second = value; // second stores the value

    // buckets[bucket] is a forward list
    // Adds the key value pair to the forward list at the bucket
    buckets[bucket].push_front(keyVal);

    ++numKeys; // bookkeeping

    if (LoadFactor() > maxLoad)
    {
        Rehash(2*numBuckets);
    }
}

template<class K, class T>
void MapT<K, T>::Remove(K key) {
    // Find the appropriate bucket that key lives in
    int bucket = GetHashIndex(key);

    // Check to see if the key already exists
    // If we do find it, remove the key value pair
    //             forward list beginning         forward list end       moves to next item in list
    for (auto it = buckets[bucket].begin(); it != buckets[bucket].end(); ++it)
    {
        if (it->first == key)
        {
            //                     *it is the pair we are removing
            buckets[bucket].remove(*it); // remove the pair from the bucket
            --numKeys;
            return;
        }
    }
}

template<class K, class T>
bool MapT<K, T>::Contains(K key) {
    // Find the appropriate bucket that key lives in
    int bucket = GetHashIndex(key);

    // Check to see if the key exists
    // If we do find it, return true
    //             forward list beginning         forward list end       moves to next item in list
    for (auto it = buckets[bucket].begin(); it != buckets[bucket].end(); ++it)
    {
        if (it->first == key)
        {
            return true;
        }
    }

    return false;
}

template<class K, class T>
T MapT<K, T>::operator[](K key) {
    int bucket = GetHashIndex(key);

    // Check to see if the key exists
    // If we do find it, return the associated value
    //             forward list beginning         forward list end       moves to next item in list
    for (auto it = buckets[bucket].begin(); it != buckets[bucket].end(); ++it)
    {
        if (it->first == key)
        {
            return it->second;
        }
    }

    throw KeyDoesNotExist();
}

template <class K, class T>
MapT<K, T>& MapT<K, T>::operator=(MapT const& other)
{
    delete [] buckets;
    numBuckets = other.numBuckets;
    buckets = new forward_list<pair<K, T>>[numBuckets];
    numKeys = 0;

    // Iterate over every single value in other
    for (int b = 0;  b < other.numBuckets; b++)
    {
        for (auto it = other.buckets[b].begin(); it != other.buckets[b].end(); ++it)
        {
            Add(it->first, it->second);
        }
    }

    return *this;
}

template<class K, class T>
double MapT<K, T>::LoadFactor() {
    return static_cast<double>(numKeys)/numBuckets;
}

template<class K, class T>
void MapT<K, T>::SetMaxLoad(double maxLoad) {
    this->maxLoad = maxLoad;
}

template<class K, class T>
void MapT<K, T>::Rehash(int numBuckets) {
    MapT<K, T> newMap(numBuckets);  // Need to copy over all elements to newMap

    for (int b = 0;  b < numBuckets; b++)
    {
        for (auto it = buckets[b].begin(); it != buckets[b].end(); ++it)
        {
            newMap.Add(it->first, it->second);
        }
    }

    *this = newMap;
}

template<class K, class T>
void MapT<K, T>::ResetIterator() {
    mapIter = buckets[0].begin();
    currBucket = 0;
}

template<class K, class T>
pair<K,T> MapT<K, T>::GetNextPair() {
    pair<K,T> currVal;

    // Is the iterator at the end?
    while (mapIter == buckets[currBucket].end())
    {
        currBucket++;
        mapIter = buckets[currBucket].begin();
    }

    currVal = *mapIter;
    ++mapIter; // Move to next node in the current forward list

    return currVal;
}

template<class K, class T>
int MapT<K, T>::GetHashIndex(const K &key) {
    unordered_map<K,T> mapper;
    typename unordered_map<K,T>::hasher hashFunction = mapper.hash_function();
    return static_cast<int>(hashFunction(key) % numBuckets);
}




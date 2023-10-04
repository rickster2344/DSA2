#include "hash.h"

hashTable::hashTable(int size)
{
    capacity = getPrime(size); 
    filled = 0; //initializes the number of items filled as 0
    data.resize(capacity); //data is stored in a std::vector
}

int hashTable::insert(const std::string &key, void *pv)
{
    //first check if we need to resize the vector. typically best to keep loadfactor small,<0.5.
    //then take item passed into insert and put it into the hashtable
    if (filled > capacity/2){
        if (!rehash()){return 2;}//runs rehash if needed, if rehash fails, return 2
    }

    int i = hash(key);
    //now insert string into the data vector, making sure to check for and resolve collisions
    
    while (data[i].isOccupied ==true)
    { //loop while the element in the vector is occupied
        //check if the key already exists inside the table
        if(data[i].key == key){
            //check if the key is deleted 
            if(data[i].isDeleted == true){
                data[i].isDeleted = false; // undelete the deleted item
                return 0; //if key exists but is deleted, undelete it and then return 0 for a successful insertion 
            }
            return 1; //if key exists and is not deleted, return 1.
        }
        //use lin probing method to find the next empty slot or find the key in the table already
        i++;
        i=i%capacity;
    }
    //insert key into the empty slot, (empty slot definitely exists because we rehashed)
    data[i].isOccupied = true;
    data[i].isDeleted = false;
    data[i].key = key;
    data[i].pv = pv; //optional pointer
    filled ++; //filled one more item in the vector
    return 0;
}

bool hashTable::contains(const std::string &key){
    //functionally the same as findPos, so may as well use it. 
    if(findPos(key)==-1){
        return false;
    } 
    return true;
}

void *hashTable::getPointer(const std::string &key, bool *b)
{
    int index = findPos(key);
    if(index==-1){
        if(b != nullptr){
            *b = false;
        }
        return nullptr;
    }

    if(b !=nullptr){
        *b = true;
    }
    return data[index].pv;
}

int hashTable::setPointer(const std::string &key, void *pv){
    int index = findPos(key);
    if(index == -1){
        return 1;
    }
    data[index].pv = pv;
    return 0;
} 

//useful for debugging
void hashTable::printVals(const std::string &key)
{
    std::cout<< data[findPos(key)].key<<std::endl;
    std::cout<< &data[findPos(key)]<<std::endl;
}

bool hashTable::remove(const std::string &key){
    int i = findPos(key);
    if(i==-1){
      return false;//entry is not found in hashtable
    }
    data[i].isDeleted = true; //lazy delete the entry
    filled--;
    return true;
}

int hashTable::hash(const std::string &key){
    unsigned int hashValue = 0;
    for(char ch : key){
        // computing polynomial with x=31 whos coeffs are the integer values of ch using Horners rule
        hashValue = 31*hashValue + ch; 
    }
    return hashValue % capacity;
}

int hashTable::findPos(const std::string &key){ 
    int i = hash(key);
    
    while(data[i].isOccupied){
        if(data[i].key==key){
            if(data[i].isDeleted){ //if the key has been lazy deleted, return false
                return -1;
            }
            return i; //key exists and is not deleted, return its index;
        }
        i++;
        i = i%capacity;
    }
    return -1; //key doesn't exist in the table since we hit unnocupied space
}

bool hashTable::rehash(){//dynamic memory allocation for rehash function...
    std::vector<hashItem> temp = data;//current data is stored in temp
    int newCap = getPrime(capacity);

    try{
        data.resize(newCap);//new vector of size = next prime number
    }
    catch (std::bad_alloc&){
        std::cerr << "Error: Unable to allocate memory for new hash table"<<std::endl;
        return false;
    }
    // initializing the new vector
    filled = 0;
    for(int i = 0; i<newCap; i++){
        data[i].isOccupied = false; //making .isOccupied false is enough since the insert function will fill it in
    }

    for(int i =0; i< capacity; i++){
        if(temp[i].isOccupied && !temp[i].isDeleted){
            insert(temp[i].key, temp[i].pv); //need to insert the pointer associated with the key as well if there is one.
        }
    }

    capacity = newCap;
    return true;
}


unsigned int hashTable::getPrime(int size)//gets the next biggest prime number
{
    int primes[] = { 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741 };
    int i = 0;
    while (primes[i] <= size && i<26){
        i++;
    }
    return primes[i];
}

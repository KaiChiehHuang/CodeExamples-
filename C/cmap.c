/*
 * File: cvector.c
 * Author: Kai-Chieh Huang 
 * ----------------------
 *
 */

#include "cmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

// a suggested value to use when given capacity_hint is 0
#define DEFAULT_CAPACITY 1023

/* Type: struct CMapImplementation
 * -------------------------------
 * This definition completes the CMap type that was declared in
 * cmap.h. You fill in the struct with your chosen fields.
 */
struct CMapImplementation {
    // include your desired fields here
    void** buckets;
    size_t nbuckets;
    size_t valueSize;
    size_t numEntries;
    CleanupValueFn cleanupValueFn;
};


/* The NOT_YET_IMPLEMENTED macro is used as the body for all functions
 * to remind you about which operations you haven't yet implemented.
 * It wil report an error if a call is made to an not-yet-implemented
 * function (this is preferable to returning garbage or silently
 * ignoring the call).  Remove the call to this macro as you implement
 * each function and finally you can remove this macro and comment
 * when no longer needed.
 */
#define NOT_YET_IMPLEMENTED printf("%s() not yet implemented!\n", __func__); raise(SIGKILL); exit(107);


//=========================HELPER FUNCTIONS===================================//
/* Function: hash
 * --------------
 * This function adapted from Eric Roberts' _The Art and Science of C_
 * It takes a string and uses it to derive a "hash code," which
 * is an integer in the range [0..nbuckets-1]. The hash code is computed
 * using a method called "linear congruence." A similar function using this
 * method is described on page 144 of Kernighan and Ritchie. The choice of
 * the value for the multiplier can have a significant effort on the
 * performance of the algorithm, but not on its correctness.
 * The computed hash value is stable, e.g. passing the same string and
 * nbuckets to function again will always return the same code.
 * The hash is case-sensitive, "ZELENSKI" and "Zelenski" are
 * not guaranteed to hash to same code.
 */
static int hash(const char *s, int nbuckets)
{
   const unsigned long MULTIPLIER = 2630849305L; // magic number
   unsigned long hashcode = 0;
   for (int i = 0; s[i] != '\0'; i++)
      hashcode = hashcode * MULTIPLIER + s[i];
   return hashcode % nbuckets;
}

/* Function: initBuckets(CMap* cm)
 * ------------------------------------------
 * This function takes in a pointer to a CMap
 * and initialize all the void* in the buckets
 * to NULL. 
 */
void initBuckets(CMap* cm){
     for(int i=0; i<cm->nbuckets; i++) {
        cm->buckets[i] = NULL;
     }
}

/* Function: getKey(void* cell)
 * --------------------------------------------
 * This function takes in the pointer to a cell
 * and get the pointer of the key by adding the 
 * pointer offset. 
 * -------------------------------------------
 * The cell is stored as: 
 * void*(8 bytes) + key(strlen(key)+1 bytes) + value(valueSize bytes)
 */
static char* getKey(void* cell){
    char* key = (char*) cell + sizeof(void*);
    return key;
}

/* Function: getValue(void* cell)
 * ---------------------------------------------
 * This function takes in the pointer to a cell
 * and get the pointer of the value by adding the 
 * pointer offset. 
 * -------------------------------------------
 * The cell is stored as: 
 * void*(8 bytes) + key(strlen(key)+1 bytes) + value(valueSize bytes)*
 */
static void* getValue(void* cell){
    char* key = getKey(cell);
    void* value = key + strlen(key) + 1; // extra 1 for null terminator 
    return value;
}

/* Function: setKey(void* cell, const char* key)
 * -----------------------------------------------------
 * This function takes in the pointer to a cell and the
 * key to set at this cell, then copy the characters to
 * the memory in the cell to hold the key.
 */
void setKey(void* cell, const char* key){
    void* keyStart = getKey(cell);
    memcpy(keyStart,key,strlen(key)+1);
}

/* Function: setValue(CMap* cm, void* cell, const void* valueAddr)
 * -----------------------------------------------------------------
 * This function takes in the pointer to a cell and the value to set 
 * at this cell, then copy the content of the value to the memory in 
 * the cell to hold the value.
 */
void setValue(CMap* cm, void* cell, const void* valueAddr){
    void* valueStart = getValue(cell);
    memcpy(valueStart,valueAddr,cm->valueSize);
} 

/* Function: addCell(CMap* cm, int bucketIndex, void* newCell) 
 * ------------------------------------------------------------
 * This function takes in a pointer to a CMap, the bucket index
 * to add the new cell, and the pointer to the new cell and then
 * make this new cell as the head of the link list at this bucket
 * index. 
 */
void addCell(CMap* cm, int bucketIndex, void* newCell){
    void* head = cm->buckets[bucketIndex];
    if(head==NULL) {
        cm->buckets[bucketIndex] = newCell;
    }else {
        // copy the old head adress into the first 8 bytes of newCell
        memcpy(newCell,&head,sizeof(void*));
        // Add the new cell as head of the link list  
        cm->buckets[bucketIndex] = newCell;
    }
}

/* Function: findCell(CMap* cm, int bucketIndex, const char* key)
 * ---------------------------------------------------------------
 * This function takes in a pointer CMap, the bucket index where 
 * the key is at, and the key and then returns the pointer of the
 * cell that holds this key. (Note that this function also takes 
 * in a bool which controls the return value. When returnPrev is 
 * true the function returns the pointer to the cell previous to
 * the cell containing this key)
 */
void* findCell(const CMap* cm, int bucketIndex, const char* key, bool returnPrev) {
    void* cell = cm->buckets[bucketIndex];
    void* prev = NULL;
    while(cell!=NULL && strcmp(getKey(cell),key)!=0) {
        prev = cell;
        memcpy(&cell,cell,sizeof(void*)); 
    }
    return (returnPrev==true) ? prev : cell;
}

//============================================================================//

CMap *cmap_create(size_t valuesz, size_t capacity_hint, CleanupValueFn fn){ 
    assert(valuesz>0);
    CMap* myMap = malloc(sizeof(CMap));
    assert(myMap!=NULL);
    myMap->nbuckets = (capacity_hint==0) ? DEFAULT_CAPACITY : capacity_hint;
    myMap->valueSize = valuesz;
    myMap->buckets = malloc(sizeof(void*)*myMap->nbuckets); 
    assert(myMap->buckets!=NULL);
    myMap->cleanupValueFn = fn;
    myMap->numEntries = 0;
    initBuckets(myMap);
    return myMap;
}

void cmap_dispose(CMap *cm){ 
    void* prevCell = NULL;
    for(const char* key=cmap_first(cm); key!=NULL; key=cmap_next(cm,key)){
        void* currCell = (char*) key - sizeof(void*);
        if(cm->cleanupValueFn != NULL) {
            void* value = getValue(currCell);
            cm->cleanupValueFn(value);
        }
        if(prevCell!=NULL) free(prevCell);
        prevCell = currCell;
    }
    free(prevCell);
    free(cm->buckets);
    free(cm);
}

int cmap_count(const CMap *cm){ 
    return cm->numEntries;
}

void cmap_put(CMap *cm, const char *key, const void *addr){
    int code = hash(key,cm->nbuckets);
    void* cellToPut = findCell(cm,code,key,false);
    if(cellToPut==NULL)  {
        void* newCell = malloc(sizeof(void*)+strlen(key)+1+cm->valueSize);
        assert(newCell!=NULL);
        memcpy(newCell,&cm->buckets[code],sizeof(void*)); //initialize the first 8 bytes to NULL
        setKey(newCell,key);
        setValue(cm,newCell,addr);
        addCell(cm,code,newCell);
        cm->numEntries += 1;
    }else{
        // Call the cleanup function of the value first
        if(cm->cleanupValueFn != NULL) cm->cleanupValueFn(cmap_get(cm,key)); 
        // Overwrites the original value at this key if key already exists
        setValue(cm,cellToPut,addr); 
    } 
}

void *cmap_get(const CMap *cm, const char *key){ 
    int code = hash(key,cm->nbuckets);
    void* cell = findCell(cm,code,key,false);  
    if(cell==NULL) return NULL;
    return getValue(cell);
}

void cmap_remove(CMap *cm, const char *key)
{
    int code = hash(key,cm->nbuckets); 
    void* cell = findCell(cm,code,key,false);
    if(cell==NULL) return; // Ignore the opperation if cannot find the key
    void* cellToFree = cell;
    if(cm->cleanupValueFn != NULL) cm->cleanupValueFn(cmap_get(cm,key)); 
    if(cell==cm->buckets[code]) {
        // If the cell to delete is the head then update the head
        memcpy(&cm->buckets[code],cell,sizeof(void*));
    }else{
        // If the cell to delete is in the middle of the linklist then relink the linklist
        void* prev = findCell(cm,code,key,true);
        memcpy(prev,cell,sizeof(void*));
    }     
    free(cellToFree);
    cm->numEntries -= 1;
}

const char *cmap_first(const CMap *cm)
{ 
    if(cm->numEntries==0) return NULL;
    void* cell = cm->buckets[0];
    int bucketIndex = 0;
    while(cell==NULL) {
        bucketIndex += 1;
        if(bucketIndex==cm->nbuckets)  return NULL;
        cell = cm->buckets[bucketIndex];
    }
    return getKey(cell);
}

const char *cmap_next(const CMap *cm, const char *prevkey)
{ 
    int code = hash(prevkey,cm->nbuckets);
    void* nextCell = NULL;
    const void* nextCellAddr = prevkey - sizeof(void*);
    memcpy(&nextCell,nextCellAddr,sizeof(void*));
    // If we hit the end of this bucket, go to the next bucket that is not NULL 
    while(nextCell==NULL) {
        code += 1;
        if(code==cm->nbuckets)  return NULL;
        nextCell = cm->buckets[code];
    } 
    return getKey(nextCell);
}

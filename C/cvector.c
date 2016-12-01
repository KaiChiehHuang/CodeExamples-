/*
 * File: cvector.c
 * Author: Kai-Chieh Huang
 * ----------------------
 *
 */

#include "cvector.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <search.h>
#include <assert.h>

// a suggested value to use when given capacity_hint is 0
#define DEFAULT_CAPACITY 16

/* Type: struct CVectorImplementation
 * ----------------------------------
 * This definition completes the CVector type that was declared in
 * cvector.h. You fill in the struct with your chosen fields.
 */
struct CVectorImplementation {
    // include your desired fields here
    void* start;
    size_t numElems;
    size_t elemSize;
    size_t capacity;
    CleanupElemFn cleanupFn;
};


/* The NOT_YET_IMPLEMENTED macro is used as the body for all functions
 * to remind you about which operations you haven't yet implemented.
 * It will report a fatal error if a call is made to an not-yet-implemented
 * function (this is preferable to returning garbage or silently
 * ignoring the call).  Remove the call to this macro as you implement
 * each function and finally you can remove this macro and comment
 * when no longer needed.
 */
#define NOT_YET_IMPLEMENTED printf("%s() not yet implemented!\n", __func__); raise(SIGKILL); exit(107);

//==============HELPER FUNCTIONS===================//
/* Function: resize(CVector* cv)
 * This function takes in a pointer to a CVector and
 * resize it with double capacity.
 */
void resize(CVector* cv)
{ 
    void* newStart = malloc(cv->elemSize*cv->capacity*2);
    assert(newStart!=NULL);
    memcpy(newStart,cv->start,cv->capacity*cv->elemSize);
    cv->capacity = cv->capacity*2;
    free(cv->start);
    cv->start = newStart;
}
//=================================================//

CVector *cvec_create(size_t elemsz, size_t capacity_hint, CleanupElemFn fn)
{  
    assert(elemsz>0);
    CVector* myVector = malloc(sizeof(CVector));
    assert(myVector!=NULL);
    myVector->capacity = (capacity_hint==0) ? DEFAULT_CAPACITY : capacity_hint;
    myVector->numElems = 0;
    myVector->elemSize = elemsz;
    myVector->start = malloc(myVector->elemSize * myVector->capacity);
    assert(myVector->start!=NULL);
    myVector->cleanupFn = fn;
    return myVector;
}

void cvec_dispose(CVector *cv)
{ 
    if(cv->cleanupFn != NULL) {
        for(int i=0; i<cv->numElems; i++) 
            cv->cleanupFn(cvec_nth(cv,i));
    }
    free(cv->start);
    free(cv);
}

int cvec_count(const CVector *cv)
{ 
    return cv->numElems;
}

void *cvec_nth(const CVector *cv, int index)
{ 
    assert( index >= 0 && index<cvec_count(cv));
    void* nth = (char*) cv->start + index*cv->elemSize;
    return nth;
}

void cvec_insert(CVector *cv, const void *addr, int index)
{ 
    assert( index >= 0 && index<=cvec_count(cv));
    if(cv->numElems == cv->capacity) resize(cv);
    void* nth = (char*) cv->start + index*cv->elemSize;
    void* nthPlusOne = (char*) cv->start + (index+1)*cv->elemSize;  
    memmove(nthPlusOne,nth,cv->elemSize*(cv->numElems-index));
    memcpy(nth,addr,cv->elemSize);
    cv->numElems += 1;
}

void cvec_append(CVector *cv, const void *addr)
{  
    if(cv->numElems == cv->capacity) resize(cv);
    void* newElem = (char*) cv->start + cv->numElems*cv->elemSize;
    memcpy(newElem,addr,cv->elemSize);
    cv->numElems += 1;
}

void cvec_replace(CVector *cv, const void *addr, int index)
{ 
    assert( index >= 0 && index<cvec_count(cv));
    void* nth = cvec_nth(cv,index);  
    if(cv->cleanupFn!=NULL) cv->cleanupFn(nth);
    memcpy(nth,addr,cv->elemSize);
}

void cvec_remove(CVector *cv, int index)
{ 
    assert( index >= 0 && index<cvec_count(cv)); 
    void* nth = (char*) cv->start + index*cv->elemSize;
    void* nthPlusOne = (char*) cv->start + (index+1)*cv->elemSize;  
    if(cv->cleanupFn!=NULL) cv->cleanupFn(nth);
    memmove(nth,nthPlusOne,cv->elemSize*(cv->numElems-index-1));
    cv->numElems -= 1;
}

int cvec_search(const CVector *cv, const void *key, CompareFn cmp, int start, bool sorted)
{ 
   assert( start >= 0 && start < cvec_count(cv));  
   int keyIndex;
   void* elemFound;
   void* searchPoint = cvec_nth(cv,start);
   size_t numElemsToSearch = cv->numElems - start;
   if(sorted){
       elemFound = bsearch(key,searchPoint,cv->numElems-start,cv->elemSize,cmp);
   }else{
       elemFound = lfind(key,searchPoint,&(numElemsToSearch),cv->elemSize,cmp);
   }
   if(elemFound==NULL) return -1;
   keyIndex = ((char*) elemFound - (char*) cv->start)/cv->elemSize;
   return keyIndex; 
}

void cvec_sort(CVector *cv, CompareFn cmp)
{ 
    qsort(cv->start,cv->numElems,cv->elemSize,cmp);
}

void *cvec_first(const CVector *cv)
{  
    return cv->start;
}

void *cvec_next(const CVector *cv, const void *prev)
{ 
    void* next = (char*) prev + cv->elemSize;
    void* end = (char*) cv->start + cv->numElems*cv->elemSize; 
    return (next>=end) ? NULL : next;
}

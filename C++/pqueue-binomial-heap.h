/*  Implementation of priority queue with binomial heap
 *  Kai-Chieh Huang / 2, 20, 2015
 */

#ifndef _binomial_heap_pqueue_
#define _binomial_heap_pqueue_

#include "pqueue.h"
#include "vector.h"
#include "queue.h"
#include <string>
using namespace std;

class BinomialHeapPQueue : public PQueue {
public:
	BinomialHeapPQueue();
	~BinomialHeapPQueue();
	
	static BinomialHeapPQueue *merge(BinomialHeapPQueue *one, BinomialHeapPQueue *two);
	
    void enqueue(const string& elem);
    string extractMin();
    string peek() const;
	
private:
    struct Node{
        string value;
        Vector<Node*> children;
    };
    /*
     * Private member: binaryExpressions
     * ------------------------------------------------------------------
     * Express the binomial heap as the bit value at each binary positions
     */
     Vector<Node*> binaryExpressions;
    /*
     * Private function: findMinElemIndex
     * Usage: minElem = findMinElemIndex();
     * ------------------------------------------------------------------
     * Returns the minimum element index in the vector: binaryExpressions.
     */
     int findMinElemIndex() const;
    /*
     * Private function: sumTwoBiTrees
     * Usage: Node* carrier = sumTwoBiTrees(Node* one, Node* two);
     * ------------------------------------------------------------------
     * Add two same order binomial heap and return the next order binomial
     * heap as the carrier.
     */
    static Node* sumTwoBiTrees(Node* one, Node* two);
    /*
     * Private function: binaryAddition
     * Usage: Vector<Node*> binarySum = binaryAddition(Vector<Node*> one, Vector<Node*> two);
     * ------------------------------------------------------------------
     * Do the binary addition of two binary expressions and returns the sum.
     */
    static Vector<Node*> binaryAddition(Vector<Node*> one, Vector<Node*> two);
    /*
     * Private function: zeroPad
     * Usage: zeroPad(Vector<Node*>& one, int numberOfZeros);
     * ------------------------------------------------------------------
     * Pad zeros to the binary expression to ensure the bits size is the
     * same with othres.
     */
    static void zeroPad(Vector<Node*>& one, int numberOfZeros);
    void deleteALLNodes(Vector<Node*>& binaryExpressions);
    void deleteNode(Node* node);
};

#endif

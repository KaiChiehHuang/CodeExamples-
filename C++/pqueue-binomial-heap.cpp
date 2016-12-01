#include "pqueue-binomial-heap.h"
#include "strlib.h"
#include <algorithm>

BinomialHeapPQueue::BinomialHeapPQueue()
{
    binaryExpressions.add(NULL);
}

void BinomialHeapPQueue::deleteNode(Node* node)
{
    if(node==NULL)
    {
        return;
    }
    for(int position = 0; position<node->children.size(); position++)
    {
        deleteNode(node->children[position]);
    }

    delete node;
}

void BinomialHeapPQueue::deleteALLNodes(Vector<Node*>& binaryExpressions)
{
    for(int position = 0; position<binaryExpressions.size(); position++)
    {
        deleteNode(binaryExpressions[position]);
    }
}

BinomialHeapPQueue::~BinomialHeapPQueue()
{
    deleteALLNodes(binaryExpressions);
}

int BinomialHeapPQueue::findMinElemIndex() const
{
    int minElemIndex = -1;
    string minElem = "";

    for(int i = 0; i<binaryExpressions.size(); i++)
    {
        if(binaryExpressions[i]!=NULL)
        {
            // find the first none NULL elems in the binaryExpressions
            if(minElemIndex == -1)
            {
                minElem = binaryExpressions[i]->value;
                minElemIndex = i;
            }
            // find the smallest elems in the binaryExpressions
            else if(binaryExpressions[i]->value<minElem)
            {
                minElem = binaryExpressions[i]->value;
                minElemIndex = i;
            }
        }
    }

    return minElemIndex;
}

BinomialHeapPQueue::Node* BinomialHeapPQueue::sumTwoBiTrees(Node* one, Node* two)
{
    Node* carrier = NULL;
    // Compare the roots of the two same order binomial heap and set the smaller one
    // as the carrier root and add the other one as a children to this root
    if(one->value<two->value)
    {
        carrier = one;
        carrier->children.add(two);
    }
    else
    {
        carrier = two;
        carrier->children.add(one);
    }
    return carrier;
}

void BinomialHeapPQueue::zeroPad(Vector<Node*>& one, int numberOfZeros)
{
    for(int i = 0; i<numberOfZeros; i++)
    {
        one.add(NULL);
    }
}

Vector<BinomialHeapPQueue::Node*> BinomialHeapPQueue::binaryAddition(Vector<Node*> one, Vector<Node*> two)
{
    if(one.size()<two.size())
    {
        zeroPad(one,two.size()-one.size());
    }
    else if(one.size()>two.size())
    {
        zeroPad(two,one.size()-two.size());
    }
    Node* carrier = NULL;
    Vector<Node*>  binarySum(one.size(),NULL);
    Queue<Node*>  binaryValuesAtPosition;
    for(int position=0; position<one.size(); position++)
    {
        if(carrier!=NULL) binaryValuesAtPosition.enqueue(carrier);
        if(one[position]!=NULL) binaryValuesAtPosition.enqueue(one[position]);
        if(two[position]!=NULL) binaryValuesAtPosition.enqueue(two[position]);
        if(binaryValuesAtPosition.size()==0)
        {
            binarySum[position] = NULL;
            carrier = NULL;
        }
        else if(binaryValuesAtPosition.size()==1)
        {
            binarySum[position] = binaryValuesAtPosition.dequeue();
            carrier = NULL;
        }
        else if(binaryValuesAtPosition.size()==2)
        {
            binarySum[position] = NULL;
            carrier = sumTwoBiTrees(binaryValuesAtPosition.dequeue(),binaryValuesAtPosition.dequeue());
        }
        else
        {
            binarySum[position] = binaryValuesAtPosition.dequeue();
            carrier = sumTwoBiTrees(binaryValuesAtPosition.dequeue(),binaryValuesAtPosition.dequeue());
        }
    }
    if(carrier!=NULL)
    {
        binarySum.add(carrier);
    }
    return binarySum;
}

string BinomialHeapPQueue::peek() const
{   
    if(isEmpty())
    {
        error("Attempt to peek an empty priority queue!");
    }
    string minElem = binaryExpressions[findMinElemIndex()]->value;

    return minElem;
}

string BinomialHeapPQueue::extractMin()
{
    if(isEmpty())
    {
        error("Attempt to extract an empty priority queue!");
    }
    int minElemIndex = findMinElemIndex();
    string minElem = binaryExpressions[minElemIndex]->value;
    Vector<Node*> childrenOfMinElem =  binaryExpressions[minElemIndex]->children;
    // remove the min element
    delete binaryExpressions[minElemIndex];
    binaryExpressions[minElemIndex] = NULL;

    // add all the children into the binomial heap
    binaryExpressions = binaryAddition(binaryExpressions,childrenOfMinElem);

    logSize -= 1;
    return minElem;
}

void BinomialHeapPQueue::enqueue(const string& elem)
{
    Node* newElem = new Node;
    newElem->value = elem;
    Vector<Node*> newElemBinaryExpression;

    newElemBinaryExpression.add(newElem);
    binaryExpressions = binaryAddition(newElemBinaryExpression,binaryExpressions);

    logSize += 1;
}

BinomialHeapPQueue *BinomialHeapPQueue::merge(BinomialHeapPQueue *one , BinomialHeapPQueue *two)
{
    BinomialHeapPQueue* newBinomialHeapPQueue = new BinomialHeapPQueue;
    newBinomialHeapPQueue->logSize = one->size() + two->size();
    newBinomialHeapPQueue->binaryExpressions = binaryAddition(one->binaryExpressions,two->binaryExpressions);
    Vector<Node*> empty(one->size(),NULL);
    one->binaryExpressions = two->binaryExpressions = empty;
    delete one;
    delete two;

    return newBinomialHeapPQueue;
}

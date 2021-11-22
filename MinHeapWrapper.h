
#ifndef MINHEAPWRAPPER_H
#define MINHEAPWRAPPER_H

#include "Node.h"
#include <queue>
#include <vector>
template<typename K, typename V> class MinHeapWrapper {

  private:
    /**
     * @brief Defines the comparison of two nodes in `minHeap`.
     * @see minHeap
     */
    class CompareNodesInMinHeap {
      public:
        bool operator()(Node<K, V> *leftNode, Node<K, V> *rightNode) {
            return (leftNode > rightNode);
        }
    };

  private:
    std::priority_queue<Node<K, V> *, std::vector<Node<K, V> *>,
                        CompareNodesInMinHeap>
            minHeap;

  public:
    std::priority_queue<Node<K, V> *, std::vector<Node<K, V> *>,
                        CompareNodesInMinHeap> &
    getMinHeap() {
        return minHeap;
    }

  public:
    explicit MinHeapWrapper(std::vector<Entry<K, V> *> entryVector) {
        for (Entry<K, V> *entry : entryVector) {
            minHeap.push(new Node<K, V>(entry));
        }
    }

  public:
    virtual ~MinHeapWrapper() { deleteTree(minHeap.top()); }

  private:
    static void deleteTree(Node<K, V> *node) {
        if (node == nullptr) { return; }
        if (node->getEntry() == nullptr) { return; }
        deleteTree(node->getLeftNode());
        deleteTree(node->getRightNode());
        delete node;
    }
};

#endif // MINHEAPWRAPPER_H

//
// Created by Tal on 21-Nov-21.
//

#ifndef NODE_H
#define NODE_H

#include "Entry.h"
template<typename K, typename V> class Node {

  private:
    Entry<K, V> *entry = nullptr;

  private:
    Node *leftNode = nullptr;

  private:
    Node *rightNode = nullptr;

  public:
    explicit Node(Entry<K, V> *entry) : entry(entry) {}

  public:
    Node(Entry<K, V> *entry, Node *leftNode, Node *rightNode)
        : entry(entry), leftNode(leftNode), rightNode(rightNode) {}

  public:
    virtual ~Node() { delete getEntry(); }

  public:
    Entry<K, V> *getEntry() { return entry; }

  public:
    void setEntry(Entry<K, V> *entry) { Node::entry = entry; }

  public:
    Node *getLeftNode() { return leftNode; }

  public:
    void setLeftNode(Node *leftNode) { Node::leftNode = leftNode; }

  public:
    Node *getRightNode() { return rightNode; }

  public:
    void setRightNode(Node *rightNode) { Node::rightNode = rightNode; }

  public:
    bool operator<(const Node &rhs) const {
        return entry->getKey() < rhs.entry->getKey();
    }
    bool operator>(const Node &rhs) const { return rhs < *this; }
    bool operator<=(const Node &rhs) const { return !(rhs < *this); }
    bool operator>=(const Node &rhs) const { return !(*this < rhs); }
};

#endif // NODE_H

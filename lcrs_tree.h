#pragma once

#include "ptr_macro.h"

// Standard library
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include <cassert>

// Test
#include <iostream>


template <typename T>
class TreeNode {
public:
    TreeNode(int32_t id, const char* label);
    ~TreeNode();

    // Getters
    int32_t GetId() const { return m_Id; }
    const char* GetLabel() const { return m_Label.c_str(); }
    const TreeNode* GetParent() const { return m_Parent; }
    const TreeNode* GetLeftChild() const { return m_LeftChild; }
    const TreeNode* GetRightSibling() const { return m_RightSibling; }
    const TreeNode* GetLeftSibling() const { return m_LeftSibling; }

    int32_t GetChildCount() const;
    int32_t GetDepth() const;

private:
    int32_t m_Id;
    std::string m_Label;
    std::unique_ptr<T> m_Data;
    TreeNode* m_Parent;
    TreeNode* m_LeftChild;
    TreeNode* m_RightSibling;
    TreeNode* m_LeftSibling;

    // All template instances of LcrsTree will be friends with this class.
    template <typename U>
    friend class LcrsTree;
};


DECLARE_TEMPLATE_PTR(LcrsTree)
template <typename T>
class LcrsTree {
public:
    static LcrsTreeUPtr<T> New(int32_t id, const char* label, std::unique_ptr<T> data = nullptr);
    ~LcrsTree();

    // Getters
    const TreeNode<T>* GetRoot() const { return m_Root; }
    const TreeNode<T>* GetTreeNodeById(int32_t id) const;
    const T* GetDataById(int32_t id) const;
    
    // If the parent is nullptr, the root will be used.
    TreeNode<T>* InsertItem(int32_t id, const char* label, std::unique_ptr<T> data = nullptr, 
        TreeNode<T>* parent = nullptr);
    bool DeleteItem(TreeNode<T>* node);

    // Traverse all tree nodes
    // The callback function will be called for each node.
    void TraverseTree(std::function<void(TreeNode<T>*, void*)> callback, 
        TreeNode<T>* startNode = nullptr, void* userData = nullptr);
    void TraverseTree(std::function<void(const TreeNode<T>*, void*)> callback, 
        const TreeNode<T>* startNode = nullptr, void* userData = nullptr) const;

private:
    LcrsTree();

    TreeNode<T>* m_Root;

    bool createRoot(int32_t id, const char* label, std::unique_ptr<T> data = nullptr);
    void traverseTreeRecursive(std::function<void(TreeNode<T>*, void*)> callback, 
        TreeNode<T>* node, void* userData);
    void traverseTreeRecursive(std::function<void(const TreeNode<T>*, void*)> callback, 
        const TreeNode<T>* node, void* userData) const;
    void deleteItemRecursive(TreeNode<T>* node);
    bool isExistingId(int32_t id) const;
};


// Implementation of TreeNode

template <typename T>
TreeNode<T>::TreeNode(int32_t id, const char* label)
    : m_Id(id)
    , m_Label(label)
    , m_Parent(nullptr)
    , m_LeftChild(nullptr)
    , m_RightSibling(nullptr)
    , m_LeftSibling(nullptr)
{
    assert(label && "Label cannot be empty.");
}

template <typename T>
TreeNode<T>::~TreeNode() {
    // NOTE:
    // The node links are managed by the LcrsTree class,
    // so it is not need to rearrange the links here.
    // The user data is managed by the user,
    // so it is not need to delete the user data here.
}

template <typename T>
int32_t TreeNode<T>::GetChildCount() const {
    int32_t count = 0;
    TreeNode* child = m_LeftChild;

    // Traverse the right siblings to count the children
    while (child != nullptr) {
        ++count;
        child = child->m_RightSibling;
    }

    return count;
}

template <typename T>
int32_t TreeNode<T>::GetDepth() const {
    int32_t depth = 0;
    TreeNode* parent = m_Parent;

    // Traverse up the tree to count the depth
    while (parent != nullptr) {
        ++depth;
        parent = parent->m_Parent;
    }

    return depth;
}


// Implementation of LcrsTree

template <typename T>
LcrsTreeUPtr<T> LcrsTree<T>::New(int32_t id, const char* label, std::unique_ptr<T> data) {
    if (label == nullptr) {
        return nullptr;
    }

    // Create a new LcrsTree instance
    LcrsTreeUPtr<T> tree = LcrsTreeUPtr<T>(new LcrsTree<T>());
    if (!tree->createRoot(id, label, std::move(data))) {
        return nullptr;
    }
    
    return std::move(tree);
}

template <typename T>
LcrsTree<T>::LcrsTree()
    : m_Root(nullptr)
{
}

template <typename T>
LcrsTree<T>::~LcrsTree() {
    if (m_Root) {
        // Delete all nodes starting from the root
        deleteItemRecursive(m_Root);
        m_Root = nullptr;
    }
}

template <typename T>
bool LcrsTree<T>::createRoot(int32_t id, const char* label, std::unique_ptr<T> data) {
    assert(m_Root == nullptr && "Root already exists.");

    if (isExistingId(id)) {
        return false;
    }

    m_Root = new TreeNode<T>(id, label);
    if (data) {
        m_Root->m_Data = std::move(data);
    }

    return true;
}

template <typename T>
TreeNode<T>* LcrsTree<T>::InsertItem(int32_t id, const char* label, 
    std::unique_ptr<T> data, TreeNode<T>* parent)
{
    if (isExistingId(id)) {
        return nullptr;
    }

    // If the parent is nullptr, insert item under the root.
    if (parent == nullptr) {
        parent = m_Root;
    }

    TreeNode<T>* newNode = new TreeNode<T>(id, label);
    if (data) {
        newNode->m_Data = std::move(data);
    }

    if (parent->m_LeftChild == nullptr) {
        // If the parent has no children, set the new node as the left child
        parent->m_LeftChild = newNode;
        newNode->m_Parent = parent;
    }
    else {
        // Otherwise, find the rightmost sibling and set the new node as the right sibling
        TreeNode<T>* sibling = parent->m_LeftChild;
        while (sibling->m_RightSibling != nullptr) {
            sibling = sibling->m_RightSibling;
        }
        sibling->m_RightSibling = newNode;
        newNode->m_LeftSibling = sibling;
        newNode->m_Parent = parent;
    }

    return newNode;
}

template <typename T>
bool LcrsTree<T>::DeleteItem(TreeNode<T>* node) {
    if (node == nullptr) {
        return false;
    }

    if (node == m_Root) {
        // Cannot delete the root node
        return false;
    }

    // Update links
    TreeNode<T>* parent = node->m_Parent;
    TreeNode<T>* rightSibling = node->m_RightSibling;
    TreeNode<T>* leftSibling = node->m_LeftSibling;

    if (parent->m_LeftChild == node) {
        // If the node is the left child of its parent
        parent->m_LeftChild = rightSibling;
    }

    if (leftSibling) {
        // If the node has a left sibling
        leftSibling->m_RightSibling = rightSibling;
    }

    if (rightSibling) {
        // If the node has a right sibling
        rightSibling->m_LeftSibling = leftSibling;
    }

    deleteItemRecursive(node);

    return true;
}

template <typename T>
void LcrsTree<T>::TraverseTree(std::function<void(TreeNode<T>*, void*)> callback, 
    TreeNode<T>* startNode, void* userData)
{
    if (callback == nullptr) {
        return;
    }

    // If startNode is nullptr, start from the root.
    if (startNode == nullptr) {
        startNode = m_Root;
    }

    // Traverse the tree recursively
    traverseTreeRecursive(callback, startNode, userData);
}

template <typename T>
void LcrsTree<T>::TraverseTree(std::function<void(const TreeNode<T>*, void*)> callback, 
    const TreeNode<T>* startNode, void* userData) const
{
    if (callback == nullptr) {
        return;
    }

    // If startNode is nullptr, start from the root.
    if (startNode == nullptr) {
        startNode = m_Root;
    }

    // Traverse the tree recursively
    traverseTreeRecursive(callback, startNode, userData);
}

template <typename T>
void LcrsTree<T>::traverseTreeRecursive(std::function<void(TreeNode<T>*, void*)> callback,
    TreeNode<T>* node, void* userData)
{
    if (!node || !callback) {
        return;
    }

    // Call the callback function for the current node
    callback(node, userData);

    // Traverse all children
    TreeNode<T>* child = node->m_LeftChild;
    while (child != nullptr) {
        traverseTreeRecursive(callback, child, userData);
        child = child->m_RightSibling;
    }
}

template <typename T>
void LcrsTree<T>::traverseTreeRecursive(std::function<void(const TreeNode<T>*, void*)> callback,
    const TreeNode<T>* node, void* userData) const
{
    if (!node || !callback) {
        return;
    }

    // Call the callback function for the current node
    callback(node, userData);

    // Traverse all children
    const TreeNode<T>* child = node->m_LeftChild;
    while (child != nullptr) {
        traverseTreeRecursive(callback, child, userData);
        child = child->m_RightSibling;
    }
}

template <typename T>
void LcrsTree<T>::deleteItemRecursive(TreeNode<T>* node) {
    if (node == nullptr) {
        return;
    }

    // Recursively delete all children
    TreeNode<T>* child = node->m_LeftChild;
    while (child != nullptr) {
        TreeNode<T>* nextChild = child->m_RightSibling;
        deleteItemRecursive(child);
        child = nextChild;
    }

    // Delete the current node
    delete node;
}

template <typename T>
const TreeNode<T>* LcrsTree<T>::GetTreeNodeById(int32_t id) const {
    if (m_Root == nullptr) {
        return nullptr;
    }

    const TreeNode<T>* foundNode = nullptr;

    // Traverse the tree to find the node with the specified id
    traverseTreeRecursive([&foundNode, id](const TreeNode<T>* node, void*) {
        if (node->GetId() == id) {
            foundNode = node;
            return;
        }
    }, m_Root, nullptr);

    return foundNode;
}

template <typename T>
bool LcrsTree<T>::isExistingId(int32_t id) const {
    if (GetTreeNodeById(id) != nullptr) {
        assert(false && "Id already exists.");
        return true;
    }
    return false;
}

template <typename T>
const T* LcrsTree<T>::GetDataById(int32_t id) const {
    const TreeNode<T>* node = GetTreeNodeById(id);
    if (node) {
        return node->m_Data.get();
    }
    return nullptr;
}
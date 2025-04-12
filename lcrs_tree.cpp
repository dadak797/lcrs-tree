#include "lcrs_tree.h"

// Standard library
#include <cassert>
#include <iostream>


TreeNode::TreeNode(int32_t id, const char* text)
    : m_Id(id)
    , m_Text(text)
    // , m_UserData(nullptr)
    , m_Parent(nullptr)
    , m_LeftChild(nullptr)
    , m_RightSibling(nullptr)
    , m_LeftSibling(nullptr)
{
    assert(text && "Text cannot be empty.");
}

TreeNode::~TreeNode() {
    // NOTE:
    // The node links are managed by the LcrsTree class,
    // so it is not need to rearrange the links here.
    // The user data is managed by the user,
    // so it is not need to delete the user data here.
    std::cout << "Deleting node: " << m_Text << std::endl;
}

int32_t TreeNode::GetChildCount() const {
    int32_t count = 0;
    TreeNode* child = m_LeftChild;

    // Traverse the right siblings to count the children
    while (child != nullptr) {
        ++count;
        child = child->m_RightSibling;
    }

    return count;
}

int32_t TreeNode::GetDepth() const {
    int32_t depth = 0;
    TreeNode* parent = m_Parent;

    // Traverse up the tree to count the depth
    while (parent != nullptr) {
        ++depth;
        parent = parent->m_Parent;
    }

    return depth;
}


int32_t LcrsTree::s_NextId = 0;

LcrsTreeUPtr LcrsTree::New(const char* text) {
    if (text == nullptr) {
        return nullptr;
    }

    // Create a new LcrsTree instance
    LcrsTreeUPtr tree = LcrsTreeUPtr(new LcrsTree());
    if (!tree->createRoot(text)) {
        return nullptr;
    }
    
    return std::move(tree);
}

LcrsTree::LcrsTree()
    : m_Root(nullptr)
{
}

LcrsTree::~LcrsTree() {
    if (m_Root == nullptr) {
        // Delete all nodes starting from the root
        deleteItemRecursive(m_Root);
        m_Root = nullptr;
    }
}

bool LcrsTree::createRoot(const char* text) {
    assert(m_Root == nullptr && "Root already exists.");

    m_Root = new TreeNode(s_NextId++, text);
    return true;
}

TreeNode* LcrsTree::InsertItem(const char* text, TreeNode* parent) {
    // If the parent is nullptr, insert item under the root.
    if (parent == nullptr) {
        parent = m_Root;
    }

    TreeNode* newNode = new TreeNode(s_NextId++, text);

    if (parent->m_LeftChild == nullptr) {
        // If the parent has no children, set the new node as the left child
        parent->m_LeftChild = newNode;
        newNode->m_Parent = parent;
    }
    else {
        // Otherwise, find the rightmost sibling and set the new node as the right sibling
        TreeNode* sibling = parent->m_LeftChild;
        while (sibling->m_RightSibling != nullptr) {
            sibling = sibling->m_RightSibling;
        }
        sibling->m_RightSibling = newNode;
        newNode->m_LeftSibling = sibling;
        newNode->m_Parent = parent;
    }

    return newNode;
}

bool LcrsTree::DeleteItem(TreeNode* node) {
    if (node == nullptr) {
        return false;
    }

    if (node == m_Root) {
        // Cannot delete the root node
        return false;
    }

    // Update links
    TreeNode* parent = node->m_Parent;
    TreeNode* rightSibling = node->m_RightSibling;
    TreeNode* leftSibling = node->m_LeftSibling;

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
    setNextId();  // Update the next id after deletion

    return true;
}

void LcrsTree::TraverseTree(std::function<void(TreeNode*, void*)> callback, TreeNode* startNode, void* userData) {
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

void LcrsTree::traverseTreeRecursive(std::function<void(TreeNode*, void*)> callback, TreeNode* node, void* userData) {
    if (!node || !callback) {
        return;
    }

    // Call the callback function for the current node
    callback(node, userData);

    // Traverse all children
    TreeNode* child = node->m_LeftChild;
    while (child != nullptr) {
        traverseTreeRecursive(callback, child, userData);
        child = child->m_RightSibling;
    }
}

void LcrsTree::deleteItemRecursive(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    // Recursively delete all children
    TreeNode* child = node->m_LeftChild;
    while (child != nullptr) {
        TreeNode* nextChild = child->m_RightSibling;
        deleteItemRecursive(child);
        child = nextChild;
    }

    // Delete the current node
    delete node;
}

void LcrsTree::setNextId() {
    int32_t maxId = 0;

    TreeNode* startNode = m_Root;
    traverseTreeRecursive([&maxId](TreeNode* node, void*) {
        if (node->m_Id > maxId) {
            maxId = node->m_Id;
        }
    }, startNode, nullptr);

    s_NextId = maxId + 1;
}

const TreeNode* LcrsTree::GetNodeById(int32_t id) {
    if (m_Root == nullptr) {
        return nullptr;
    }

    TreeNode* foundNode = nullptr;

    // Traverse the tree to find the node with the specified id
    traverseTreeRecursive([&foundNode, id](TreeNode* node, void*) {
        if (node->m_Id == id) {
            foundNode = node;
            return;  // Stop traversing once the node is found
        }
    }, m_Root, nullptr);

    return foundNode;
}
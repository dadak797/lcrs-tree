#pragma once

#include "ptr_macro.h"

// Standard library
#include <string>
#include <functional>
#include <cassert>
#include <memory>

// FOR DEBUGGING
#include <iostream>


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
    TreeNode* m_Parent;
    TreeNode* m_LeftChild;
    TreeNode* m_RightSibling;
    TreeNode* m_LeftSibling;

    // All template instances of LcrsTree will be friends with this class.
    friend class LcrsTree;
};


DECLARE_PTR(LcrsTree)
class LcrsTree {
public:
    // Factory method to create a new LcrsTree instance
    // The new Id should be assigned by the caller.
    static LcrsTreeUPtr New(int32_t id, const char* label);
    ~LcrsTree();

    // Getters
    const TreeNode* GetRoot() const { return m_Root; }
    const TreeNode* GetTreeNodeById(int32_t id) const;
    
    // If the parent is nullptr, the root will be used.
    // If the id already exists or the label is empty, the item will not be inserted and return nullptr.
    TreeNode* InsertItem(int32_t id, const char* label, TreeNode* parent = nullptr);
    bool DeleteItem(TreeNode* node);

    // Traverse all tree nodes
    // The callback function will be called for each node.
    // The userData parameter can be used to pass additional data to the callback function.
    void TraverseTree(std::function<void(const TreeNode*, void*)> callback, 
        const TreeNode* startNode = nullptr, void* userData = nullptr) const;
    void TraverseTreeMutable(std::function<void(TreeNode*, void*)> callback, 
        TreeNode* startNode = nullptr, void* userData = nullptr);

private:
    LcrsTree();

    TreeNode* m_Root;

    bool createRoot(int32_t id, const char* label);
    void traverseTreeRecursive(std::function<void(const TreeNode*, void*)> callback, 
        const TreeNode* node, void* userData) const;
    void traverseTreeRecursiveMutable(std::function<void(TreeNode*, void*)> callback, 
        TreeNode* node, void* userData);
    void deleteItemRecursive(TreeNode* node);
    bool isExistingId(int32_t id) const;
};

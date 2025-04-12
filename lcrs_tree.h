#pragma once

#include "ptr_macro.h"

// Standard library
#include <string>
#include <vector>
#include <memory>
#include <functional>


class TreeNode {
public:
    TreeNode(int32_t id, const char* text);
    ~TreeNode();

    // Setters
    // void SetText(const char* text) { m_Text = text; }
    // void SetUserData(void* data) { m_UserData = data; }

    // Getters
    int32_t GetId() const { return m_Id; }
    const char* GetText() const { return m_Text.c_str(); }
    // const void* GetUserData() const { return m_UserData; }
    const TreeNode* GetParent() const { return m_Parent; }
    const TreeNode* GetLeftChild() const { return m_LeftChild; }
    const TreeNode* GetRightSibling() const { return m_RightSibling; }
    const TreeNode* GetLeftSibling() const { return m_LeftSibling; }

    int32_t GetChildCount() const;
    int32_t GetDepth() const;

private:
    int32_t m_Id;
    std::string m_Text;
    // void* m_UserData;
    TreeNode* m_Parent;
    TreeNode* m_LeftChild;
    TreeNode* m_RightSibling;
    TreeNode* m_LeftSibling;

    friend class LcrsTree;
};


DECLARE_PTR(LcrsTree)
class LcrsTree {
public:
    static LcrsTreeUPtr New(const char* text);
    ~LcrsTree();

    // Getters
    const TreeNode* GetRoot() const { return m_Root; }
    const TreeNode* GetNodeById(int32_t id);

    // If the parent is nullptr, the root will be used.
    TreeNode* InsertItem(const char* text, TreeNode* parent = nullptr);
    bool DeleteItem(TreeNode* node);

    // Traverse all tree nodes
    // The callback function will be called for each node.
    void TraverseTree(std::function<void(TreeNode*, void*)> callback, TreeNode* startNode = nullptr, void* userData = nullptr);

private:
    LcrsTree();

    TreeNode* m_Root;
    static int32_t s_NextId;

    bool createRoot(const char* text);
    void traverseTreeRecursive(std::function<void(TreeNode*, void*)> callback, TreeNode* node, void* userData);
    void deleteItemRecursive(TreeNode* node);

public:
    // Helper function to set the next id for a new node
    // When the tree is loaded from a file, the next id is set to (the maximum id + 1) in the file.
    void setNextId();
};
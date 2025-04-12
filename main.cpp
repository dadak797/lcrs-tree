#include "lcrs_tree.h"
#include <iostream>
#include <string>
#include <cstdint>


int main() {
    LcrsTreeUPtr geomTree = LcrsTree::New("0");
    if (!geomTree) {
        std::cerr << "Failed to create LCRS tree." << std::endl;
        return -1;
    }

    // Insert items into the tree
    TreeNode* node01 = geomTree->InsertItem("0-1");
    TreeNode* node02 = geomTree->InsertItem("0-2");
    TreeNode* node03 = geomTree->InsertItem("0-3");
    TreeNode* node11 = geomTree->InsertItem("1-1", node01);
    TreeNode* node12 = geomTree->InsertItem("1-2", node01);
    TreeNode* node21 = geomTree->InsertItem("2-1", node02);
    TreeNode* node22 = geomTree->InsertItem("2-2", node02);
    TreeNode* node31 = geomTree->InsertItem("3-1", node03);
    TreeNode* node32 = geomTree->InsertItem("3-2", node03);
    TreeNode* node33 = geomTree->InsertItem("3-3", node03);
    TreeNode* node111 = geomTree->InsertItem("1-1-1", node11);
    TreeNode* node112 = geomTree->InsertItem("1-1-2", node11);

    geomTree->TraverseTree([](TreeNode* node, void* userData) {
        for (int i = 0; i < node->GetDepth(); i++) {
            std::cout << "--";
        }

        std::cout << "[Node]: " << node->GetText() << ", [ID]: " << node->GetId();
        std::cout << ", [Parent]: " << (node->GetParent() ? node->GetParent()->GetText() : "NULL");
        std::cout << ", [Left Child]: " << (node->GetLeftChild() ? node->GetLeftChild()->GetText(): "NULL");
        std::cout << ", [Right Sibling]: " << (node->GetRightSibling() ? node->GetRightSibling()->GetText() : "NULL");
        std::cout << ", [Left Sibling]: " << (node->GetLeftSibling() ? node->GetLeftSibling()->GetText() : "NULL");
        std::cout << std::endl;
    });

    const TreeNode* node = geomTree->GetNodeById(11);
    if (node) {
        std::cout << "Found node with Id 11: " << node->GetText() << std::endl;
    } else {
        std::cout << "Node with Id 11 not found." << std::endl;
    }

    geomTree->DeleteItem(node11);
    std::cout << "--- After deleting node11 ---" << std::endl;

    geomTree->TraverseTree([](TreeNode* node, void* userData) {
        for (int i = 0; i < node->GetDepth(); i++) {
            std::cout << "--";
        }

        std::cout << "[Node]: " << node->GetText() << ", [ID]: " << node->GetId();
        std::cout << ", [Parent]: " << (node->GetParent() ? node->GetParent()->GetText() : "NULL");
        std::cout << ", [Left Child]: " << (node->GetLeftChild() ? node->GetLeftChild()->GetText(): "NULL");
        std::cout << ", [Right Sibling]: " << (node->GetRightSibling() ? node->GetRightSibling()->GetText() : "NULL");
        std::cout << ", [Left Sibling]: " << (node->GetLeftSibling() ? node->GetLeftSibling()->GetText() : "NULL");
        std::cout << std::endl;
    });

    geomTree->DeleteItem(node33);
    std::cout << "--- After deleting node33 ---" << std::endl;

    geomTree->TraverseTree([](TreeNode* node, void* userData) {
        for (int i = 0; i < node->GetDepth(); i++) {
            std::cout << "--";
        }

        std::cout << "[Node]: " << node->GetText() << ", [ID]: " << node->GetId();
        std::cout << ", [Parent]: " << (node->GetParent() ? node->GetParent()->GetText() : "NULL");
        std::cout << ", [Left Child]: " << (node->GetLeftChild() ? node->GetLeftChild()->GetText(): "NULL");
        std::cout << ", [Right Sibling]: " << (node->GetRightSibling() ? node->GetRightSibling()->GetText() : "NULL");
        std::cout << ", [Left Sibling]: " << (node->GetLeftSibling() ? node->GetLeftSibling()->GetText() : "NULL");
        std::cout << std::endl;
    });

    return 0;
}
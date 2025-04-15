#include "lcrs_tree.h"
#include <iostream>
#include <string>
#include <cstdint>
#include <unordered_map>


class Mesh {
public:
    Mesh(int32_t id, const char* name, int32_t nodeCount, int32_t faceCount)
        : m_Id(id), m_Name(name), m_NodeCount(nodeCount), m_FaceCount(faceCount)
    {
        std::cout << "Creating Mesh - [Id]: " << m_Id << std::endl;
    }
    
    ~Mesh() {
        std::cout << "Destroying Mesh - [Id]: " << m_Id << std::endl;
    }

    const int32_t GetId() const { return m_Id; }
    const char* GetName() const { return m_Name.c_str(); }
    const int32_t GetNodeCount() const { return m_NodeCount; }
    const int32_t GetFaceCount() const { return m_FaceCount; }

private:
    int32_t m_Id;
    std::string m_Name;
    int32_t m_NodeCount;
    int32_t m_FaceCount;
};


int main() {
    std::unique_ptr<Mesh> pMesh0 = std::make_unique<Mesh>(0, "Mesh0", 100, 200);
    LcrsTreeUPtr meshTree = LcrsTree::New(pMesh0->GetId(), pMesh0->GetName());

    std::unique_ptr<Mesh> pMesh1 = std::make_unique<Mesh>(1, "Mesh1", 150, 250);
    std::unique_ptr<Mesh> pMesh11 = std::make_unique<Mesh>(2, "Mesh11", 200, 300);
    std::unique_ptr<Mesh> pMesh12 = std::make_unique<Mesh>(3, "Mesh12", 250, 350);
    std::unique_ptr<Mesh> pMesh2 = std::make_unique<Mesh>(4, "Mesh2", 300, 400);
    std::unique_ptr<Mesh> pMesh3 = std::make_unique<Mesh>(5, "Mesh3", 350, 450);

    TreeNode* node1 = meshTree->InsertItem(pMesh1->GetId(), pMesh1->GetName());
    meshTree->InsertItem(pMesh11->GetId(), pMesh11->GetName(), node1);
    meshTree->InsertItem(pMesh12->GetId(), pMesh12->GetName(), node1);
    TreeNode* node2 = meshTree->InsertItem(pMesh2->GetId(), pMesh2->GetName());
    TreeNode* node3 = meshTree->InsertItem(pMesh3->GetId(), pMesh3->GetName());

    meshTree->TraverseTree([](const TreeNode* node, void* userData) {
        for (int i = 0; i < node->GetDepth(); ++i) {
            std::cout << "--";
        }

        std::cout << "[Node]: " << node->GetLabel() << ", [ID]: " << node->GetId();
        std::cout << ", [Parent]: " << (node->GetParent() ? node->GetParent()->GetLabel() : "NULL");
        std::cout << ", [Left Child]: " << (node->GetLeftChild() ? node->GetLeftChild()->GetLabel(): "NULL");
        std::cout << ", [Right Sibling]: " << (node->GetRightSibling() ? node->GetRightSibling()->GetLabel() : "NULL");
        std::cout << ", [Left Sibling]: " << (node->GetLeftSibling() ? node->GetLeftSibling()->GetLabel() : "NULL");
        std::cout << std::endl;
    });

    meshTree->DeleteItem(node1);

    std::cout << "After deleting node1:" << std::endl;
    meshTree->TraverseTree([](const TreeNode* node, void* userData) {
        for (int i = 0; i < node->GetDepth(); ++i) {
            std::cout << "--";
        }

        std::cout << "[Node]: " << node->GetLabel() << ", [ID]: " << node->GetId();
        std::cout << ", [Parent]: " << (node->GetParent() ? node->GetParent()->GetLabel() : "NULL");
        std::cout << ", [Left Child]: " << (node->GetLeftChild() ? node->GetLeftChild()->GetLabel(): "NULL");
        std::cout << ", [Right Sibling]: " << (node->GetRightSibling() ? node->GetRightSibling()->GetLabel() : "NULL");
        std::cout << ", [Left Sibling]: " << (node->GetLeftSibling() ? node->GetLeftSibling()->GetLabel() : "NULL");
        std::cout << std::endl;
    });

    return 0;
}
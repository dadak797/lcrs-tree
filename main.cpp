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
    LcrsTreeUPtr<Mesh> geomTree = LcrsTree<Mesh>::New(pMesh0->GetId(), pMesh0->GetName(), std::move(pMesh0));

    std::unique_ptr<Mesh> pMesh1 = std::make_unique<Mesh>(1, "Mesh1", 150, 250);
    std::unique_ptr<Mesh> pMesh11 = std::make_unique<Mesh>(2, "Mesh11", 200, 300);
    std::unique_ptr<Mesh> pMesh12 = std::make_unique<Mesh>(3, "Mesh12", 250, 350);
    std::unique_ptr<Mesh> pMesh2 = std::make_unique<Mesh>(4, "Mesh2", 300, 400);
    std::unique_ptr<Mesh> pMesh3 = std::make_unique<Mesh>(5, "Mesh3", 350, 450);

    TreeNode<Mesh>* node1 = geomTree->InsertItem(pMesh1->GetId(), pMesh1->GetName(), std::move(pMesh1));
    geomTree->InsertItem(pMesh11->GetId(), pMesh11->GetName(), std::move(pMesh11), node1);
    geomTree->InsertItem(pMesh12->GetId(), pMesh12->GetName(), std::move(pMesh12), node1);
    TreeNode<Mesh>* node2 = geomTree->InsertItem(pMesh2->GetId(), pMesh2->GetName(), std::move(pMesh2));
    TreeNode<Mesh>* node3 = geomTree->InsertItem(pMesh3->GetId(), pMesh3->GetName(), std::move(pMesh3));

    geomTree->TraverseTree([](TreeNode<Mesh>* node, void* userData) {
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

    geomTree->DeleteItem(node1);

    std::cout << "After deleting node1:" << std::endl;
    geomTree->TraverseTree([](TreeNode<Mesh>* node, void* userData) {
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
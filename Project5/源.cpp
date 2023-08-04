#include <iostream>
#include <vector>
#include <string>
#include <openssl/sha.h>

// Merkle���ڵ�
struct MerkleNode {
    std::string value;
    bool isLeaf;
};

// ������������ַ����Ĺ�ϣ����
std::string computeHash(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);
    std::string output(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);
    return output;
}

// �ݹ鹹��Merkle��
void buildMerkleTree(const std::vector<std::string>& data, std::vector<MerkleNode>& tree) {
    int n = data.size();
    if (n == 0) return;

    // ����Ҷ�ӽڵ�
    for (const auto& item : data) {
        MerkleNode node;
        node.value = computeHash(item);
        node.isLeaf = true;
        tree.push_back(node);
    }

    // �����м�ڵ�
    while (tree.size() != 1) {
        if (tree.size() % 2 != 0) {
            tree.push_back(tree.back());
        }

        std::vector<MerkleNode> newLevel;
        for (int i = 0; i < tree.size(); i += 2) {
            MerkleNode parentNode;
            parentNode.value = computeHash(tree[i].value + tree[i + 1].value);
            parentNode.isLeaf = false;
            newLevel.push_back(parentNode);
        }

        tree = newLevel;
    }
}

// ���ɸ���Ҷ�����������·��
std::vector<std::string> generateAuditPath(const std::vector<MerkleNode>& tree, int leafIndex) {
    std::vector<std::string> auditPath;

    int currentIndex = leafIndex;
    int currentLevel = 0;
    while (currentIndex < tree.size() - 1) {
        int siblingIndex = (currentIndex % 2 == 0) ? (currentIndex + 1) : (currentIndex - 1);
        std::string siblingHash = (currentIndex % 2 == 0) ? tree[currentIndex + 1].value : tree[currentIndex - 1].value;
        auditPath.push_back(siblingHash);

        currentIndex = (currentIndex / 2) + (tree.size() >> (currentLevel + 1));
        currentLevel++;
    }

    return auditPath;
}

// ��֤����Ҷ�����ݡ�Merkle�������·��
bool verifyAuditPath(const std::string& leafData, const std::string& merkleRoot, const std::vector<std::string>& auditPath) {
    std::string hash = computeHash(leafData);
    for (const auto& siblingHash : auditPath) {
        hash = computeHash((hash + siblingHash));
    }
    return (hash == merkleRoot);
}

int main() {
    std::vector<std::string> data = { "data1", "data2", "data3", "data4", "data5" };
    std::vector<MerkleNode> tree;

    buildMerkleTree(data, tree);

    // ��ӡMerkle��
    std::cout << "Merkle����" << std::endl;
    for (const auto& node : tree) {
        std::cout << "ֵ��" << node.value << "���Ƿ�Ҷ�ӽڵ㣺" << node.isLeaf << std::endl;
    }

    // ���ɲ���ӡҶ������Ϊ1�����·��
    int leafIndex = 1;
    std::vector<std::string> auditPath = generateAuditPath(tree, leafIndex);
    std::cout << "Ҷ������Ϊ " << leafIndex << " �����·����" << std::endl;
    for (const auto& siblingHash : auditPath) {
        std::cout << siblingHash << std::endl;
    }

    // ��֤���·��
    std::string merkleRoot = tree[0].value;
    bool isVerified = verifyAuditPath(data[leafIndex], merkleRoot, auditPath);
    std::cout << "���·����֤��" << (isVerified ? "ͨ��" : "ʧ��") << std::endl;

    return 0;
}

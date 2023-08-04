#include <iostream>
#include <vector>
#include <string>
#include <openssl/sha.h>

// Merkle树节点
struct MerkleNode {
    std::string value;
    bool isLeaf;
};

// 计算给定输入字符串的哈希函数
std::string computeHash(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);
    std::string output(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);
    return output;
}

// 递归构建Merkle树
void buildMerkleTree(const std::vector<std::string>& data, std::vector<MerkleNode>& tree) {
    int n = data.size();
    if (n == 0) return;

    // 构建叶子节点
    for (const auto& item : data) {
        MerkleNode node;
        node.value = computeHash(item);
        node.isLeaf = true;
        tree.push_back(node);
    }

    // 构建中间节点
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

// 生成给定叶子索引的审计路径
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

// 验证给定叶子数据、Merkle根和审计路径
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

    // 打印Merkle树
    std::cout << "Merkle树：" << std::endl;
    for (const auto& node : tree) {
        std::cout << "值：" << node.value << "，是否叶子节点：" << node.isLeaf << std::endl;
    }

    // 生成并打印叶子索引为1的审计路径
    int leafIndex = 1;
    std::vector<std::string> auditPath = generateAuditPath(tree, leafIndex);
    std::cout << "叶子索引为 " << leafIndex << " 的审计路径：" << std::endl;
    for (const auto& siblingHash : auditPath) {
        std::cout << siblingHash << std::endl;
    }

    // 验证审计路径
    std::string merkleRoot = tree[0].value;
    bool isVerified = verifyAuditPath(data[leafIndex], merkleRoot, auditPath);
    std::cout << "审计路径验证：" << (isVerified ? "通过" : "失败") << std::endl;

    return 0;
}

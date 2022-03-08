#include "precomp.h"

struct HuffmanTreeNode
{
    HuffmanTreeNode(char data, int freq)
    {
        this->data = data;
        this->freq = freq;
    }

    HuffmanTreeNode(
        char data,
        int freq,
        std::shared_ptr<HuffmanTreeNode>& left,
        std::shared_ptr<HuffmanTreeNode>& right)
    {
        this->data = data;
        this->freq = freq;
        this->left = left;
        this->right = right;
    }

    ~HuffmanTreeNode() = default;

    bool operator>(HuffmanTreeNode& rhs)
    {
        return this->freq > rhs.freq;
    }

    char data;
    int freq;
    std::shared_ptr<HuffmanTreeNode> left;
    std::shared_ptr<HuffmanTreeNode> right;
};

template <typename T>
struct SharedComparer
{
    bool operator()(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) const
    {
        return (*lhs) > (*rhs);
    }
};

class HuffmanTreeCompression
{
public:
    HuffmanTreeCompression(const std::vector<std::pair<char, int>>& freq)
    {
        this->freq_ = freq;
        this->root_ = this->BuildHuffmanTree();
    }

    void CompressText(std::string inputText,
                      std::vector<bool>& compressedBits)
    {
        std::map<char, std::vector<bool>> codes = this->GetHuffmanCodes();

        for (auto input : inputText) {
            compressedBits.insert(compressedBits.end(), codes[input].begin(), codes[input].end());
        }
    }

    void DecompressText(const std::vector<bool>& compressedBits,
                        std::string& decompressedText)
    {
        std::stringstream outputStream;
        HuffmanTreeNode* current = root_.get();

        for (auto compressedBit : compressedBits) {
            current = compressedBit ? current->right.get() :
                                      current->left.get();
            if (current->data != '$') {
                outputStream << current->data;
                current = root_.get();
            }
        }

        decompressedText = outputStream.str();
    }

private:
    std::shared_ptr<HuffmanTreeNode> BuildHuffmanTree()
    {
        std::priority_queue<std::shared_ptr<HuffmanTreeNode>,
                            std::vector<std::shared_ptr<HuffmanTreeNode>>,
                            SharedComparer<HuffmanTreeNode>> priority_queue;

        for (auto& freq: this->freq_) {
            priority_queue.push(std::make_shared<HuffmanTreeNode>(freq.first, freq.second));
        }

        while (priority_queue.size() > 1) {
            auto left = priority_queue.top();
            priority_queue.pop();

            auto right = priority_queue.top();
            priority_queue.pop();

            priority_queue.push(std::make_shared<HuffmanTreeNode>(
                '$',
                left->freq + right->freq,
                left,
                right
            ));
        }

        auto root = priority_queue.top();
        priority_queue.pop();

        return root;
    }

    std::map<char, std::vector<bool>> GetHuffmanCodes()
    {
        std::vector<bool> codes;
        std::map<char, std::vector<bool>> encodings;
        this->GetHuffmanCodes(root_, encodings, codes);

        return encodings;
    }

    void GetHuffmanCodes(std::shared_ptr<HuffmanTreeNode>& root,
                         std::map<char, std::vector<bool>>& encodings,
                         std::vector<bool>& codes)
    {
        if (root->left != nullptr) {
            codes.push_back(false);
            this->GetHuffmanCodes(root->left, encodings, codes);
            codes.pop_back();
        }

        if (root->right != nullptr) {
            codes.push_back(true);
            this->GetHuffmanCodes(root->right, encodings, codes);
            codes.pop_back();
        }

        if (root->data != '$') {
            encodings[root->data] = codes;
        }
    }

private:
    std::vector<std::pair<char, int>> freq_;
    std::shared_ptr<HuffmanTreeNode> root_;
};

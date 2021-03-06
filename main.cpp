#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <string>

struct TreeNode {
    int val;
    std::shared_ptr<TreeNode> left, right;
    TreeNode(int v, const std::shared_ptr<TreeNode> & lchild = nullptr, const std::shared_ptr<TreeNode> & rchild = nullptr)
        : val(v), left(lchild), right(rchild) {}

    void print() {
        if (left != nullptr || right != nullptr) {
            std::cout << "val = " << val
                      << "\t -> left = " << (left != nullptr ? std::to_string(left->val) : "null") << std::endl
                      << "\t -> right = " << (right != nullptr ? std::to_string(right->val) : "null") << std::endl;
        }
    }
};

using pNode = std::shared_ptr<TreeNode>;

pNode buildTree(const std::vector<int> & nums) {
    pNode root = nullptr;
    if (nums.empty())
        return root;
    if (nums.size() == 1)
        root = std::make_shared<TreeNode>(nums[0]);
    else {
        size_t mid = nums.size()/2;
        root = std::make_shared<TreeNode>(nums[mid]);
        root->left = buildTree(std::vector<int>(nums.begin(), nums.begin()+mid));
        root->right = buildTree(std::vector<int>(nums.begin()+mid+1, nums.end()));
    }
    return root;
}

void printTree(const pNode & node) {
    if (node != nullptr) {
        printTree(node->left);
        std::cout << node->val << " ";
        printTree(node->right);
    }
}

void printBFS(const pNode & root) {
    std::queue<pNode> qu;
    qu.push(root);
    while (!qu.empty()) {
        auto curr = qu.front();
        qu.pop();
        if (curr != nullptr) {
            curr->print();
            qu.push(curr->left);
            qu.push(curr->right);
        }
    }
}

pNode getParent(const pNode & root, const pNode & node) {
    if (root != nullptr) {
        std::queue<pNode> qu;
        qu.push(root);
        while (!qu.empty()) {
            auto curr = qu.front();
            qu.pop();
            if (curr != nullptr) {
                if (curr->left == node || curr->right == node)
                    return curr;
                else {
                    qu.push(curr->left);
                    qu.push(curr->right);
                }
            }
        }
    }
    return nullptr;
}

pNode inorderNext(const pNode & root, const pNode & curr) {
    pNode next = nullptr;
    if (curr->right != nullptr) {
        next = curr->right;
        while (next->left != nullptr)
            next = next->left;
    }
    else {
        pNode parent = getParent(root, curr);
        if (parent != nullptr) {
            if (parent->left == curr)
                next = parent;
            else {
                pNode grandparent = getParent(root, parent);
                while (grandparent != nullptr && parent == grandparent->right) {
                    parent = grandparent;
                    grandparent = getParent(root, parent);
                }
                next = grandparent;
            }
        }
    }
    return next;
}

class solution {
  public:
    void recover(const pNode & root) {
      pNode leftmost = root;
      while (leftmost->left != nullptr)
	leftmost = leftmost->left;
      pNode first = nullptr, second = nullptr, curr, next;
      curr = leftmost;
      next = inorderNext(root, curr);
      while (curr != nullptr && next != nullptr && curr->val < next->val) {
	curr = next;
	next = inorderNext(root, curr);
      }
      if (curr != nullptr && next == nullptr)
	return;
      auto temp = inorderNext(root, next);
      if (temp == nullptr) {
	first = curr;
	second = next;
      }
      else {
	if (curr->val < temp->val) {
	  first = curr;
	  second = next;
	}
	else {
	  first = curr;
	  curr = next;
	  next = inorderNext(root, curr);
	  while (curr != nullptr && next != nullptr && curr->val < next->val) {
	    curr = next;
	    next = inorderNext(root, curr);
	  }
	  second = next;
	}
      }
      std::cout << "first = " << first->val << ", second = " << second->val << std::endl;
      std::swap(first->val, second->val);
    }
};


int main() {
  std::vector<int> nums{1,2,3,4,5,6,7,8};
    srand(time(0));
    size_t ix1 = rand()%nums.size(), ix2 = rand()%nums.size();
    while (ix1 == ix2) {
      ix1 = rand()%nums.size();
      ix2 = rand()%nums.size();
    }
      std::swap(nums[ix1], nums[ix2]);

    pNode root = buildTree(nums);

    std::cout << "Before recovering:\n";
    printTree(root);
    std::cout << std::endl;
    // printBFS(root);
    // std::cout << "Iterative:\n";
    // pNode leftmost = root;
    // while (leftmost->left != nullptr)
    //     leftmost = leftmost->left;
    // while (leftmost != nullptr) {
    //     std::cout << leftmost->val << " ";
    //     leftmost = inorderNext(root, leftmost);
    // }
    // std::cout << std::endl;

    solution soln;
    soln.recover(root);
    std::cout << "After recovering:\n";
    printTree(root);
    std::cout << std::endl;
    // printBFS(root);
}

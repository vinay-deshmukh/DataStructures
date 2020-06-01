
#include <algorithm>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <tuple>

#define watch(x) std::cout << (#x) << "=" << x << std::endl;

class AVLNode {
public:
    int val;
    int height;
    AVLNode* left;
    AVLNode* right;

    /**
    * Constructor
    */
    explicit AVLNode(int v, AVLNode* l = nullptr, AVLNode* r = nullptr);

    /**
    * Operator<< Overload to print repr(node)
    */
    friend std::ostream& operator<<(std::ostream& out, const AVLNode* node);

};


AVLNode::AVLNode(int v, AVLNode* l, AVLNode* r)
    : val(v), height(0), left(l), right(r) {
}


int height(AVLNode* node) {
    if(node) {
        return node->height;
    } else {
        return -1;
    }
}


std::ostream& operator<<(std::ostream& out, const AVLNode* node) {
    if(node == nullptr) {
        out << "null";
    } else {
        std::string s = "<" + 
            std::to_string(node->val) 
            + "," 
            + std::to_string(node->height)
            + ">"
            ;
        out << s;
    }
    return out;
}


class AVLTree {
public:
    AVLNode* root;

    /**
    * Constructor
    */
    AVLTree();

    /**
    * Insert the given value into the tree, 
    */
    void insert(int val);

    /**
    * Prints inorder traversal to std::cout
    */
    void printInorder(AVLNode* node);


    /**
    * Print the level order traversal to the ostream parameter.
    * By default, it prints to std::cout
    */
    void prettyPrint(std::ostream& out = std::cout);

    /**
    * Deletes the given value from the tree
    * Returns a bool to denote success of deletion
    */
    bool remove(int val);

    /**
    * Return a pointer to the first node with value == `val`
    * If such node doesn't exist, return `nullptr`
    */
    AVLNode* search(int val);


private:
    /**
    * Recursive insertion function called by `insert()`
    */
    void insertNode(AVLNode* & node, int val);

    /**
    * Populates the argument std::vector `levels` with tuples where each
    * tuple holds information about location of an `AVLNode*` in the tree.
    * (See Parameters for more info.)
    *
    * This function is called only by the `prettyPrint()` function,
    * to pretty print the binary tree.
    *
    * Parameters:
        depth  : distance from root to current node, where depth(root) = 0
        ai     : is index of node in an array which holds a complete binary tree
                 if ai is index of current node, then
                    left child  = 2 * ai + 1
                    right child = 2* ai + 2
        levels : A std::vector of std::tuple where each tuple contains
                   (depth, ai, AVLNode* t)
    */
    void preorderLevels(
        AVLNode* t, 
        int depth, 
        int ai, 
        std::vector<std::tuple<int, int, AVLNode*>>& levels
    );

    /**
    * Recursive deletion function called by `remove()`
    */
    bool removeNode(AVLNode* & node, int val);

    /**
    * Rotate LL on critical node
    * Src: Reema Thareja - Data Structures using C, 2nd Edition
    *
    *   Before:                After:
    *          Cr                 Cn
    *         /  \               /  \ 
    *       Cn    T3           T1    Cr
    *      /  \                     /  \
    *    T1   T2                   T2  T3
    *
    * where Cr: is the critcal node, and argument to this function
    *       Cn: left child of critical node
    *
    */
    void rotateLL(AVLNode* & node);

    /**
    * Rotate RR on critical node
    * Src: Reema Thareja - Data Structures using C, 2nd Edition
    *
    *   Before:                After:
    *          Cr                 Cn
    *         /  \               /  \ 
    *       T1    Cn           Cr    T3
    *            /  \         /  \
    *          T2   T3      T1    T2
    *
    * where Cr: is the critcal node, and argument to this function
    *       Cn: left child of critical node
    *
    */
    void rotateRR(AVLNode* & node);

    /**
    * Recursive search function called by `search()`.
    */
    AVLNode* searchNode(AVLNode* node, int val);

};


AVLTree::AVLTree() : root(nullptr) {
}


void AVLTree::insert(int val) {
    insertNode(root, val);
}


void AVLTree::insertNode(AVLNode* & node, int val) {

    if(node == nullptr) {
        node = new AVLNode(val);
    } else if (val < node->val) {
        insertNode(node->left, val);
    } else {
        insertNode(node->right, val);
    }

    int hdf = height(node->right) - height(node->left);
    if (hdf > 1) {
        // Rebalance right heavy

        if(val >= node->right->val) {
            // RR
            rotateRR(node);
        } else {
            // RL
            rotateLL(node->right);
            rotateRR(node);
        }
    } else if (hdf < -1) {
        // Rebalance left heavy

        if(val < node->left->val) {
            // LL
            watch("LL");
            rotateLL(node);
        } else {
            // LR
            rotateRR(node->left);
            rotateLL(node);
        }
    }

    // Update height of critical node
    node->height = std::max(height(node->left), height(node->right)) + 1;
}


bool AVLTree::remove(int val) {
    return removeNode(root, val);
}


bool AVLTree::removeNode(AVLNode* & node, int val) {
    if(node == nullptr)
        return false;

    bool res = false;

    if(val < node->val) {
        res = removeNode(node->left, val);
    } else if (val > node->val) {
        res = removeNode(node->right, val);
    } else {
        // Found node, now delete it

        res = true;
        if(node->left == nullptr && node->right == nullptr) {
            // Leaf node
            delete node;
            node = nullptr;
        } else if (node->left == nullptr || node->right == nullptr) {
            // One child
            AVLNode* temp = node;
            delete temp;
            node = (node->left != nullptr) ? node->left : node->right;
        } else {
            // Two children

            // Find inorder predecessor
            // `find` will contain inorder predecessor
            AVLNode* find = node->left; 

            while(find->right != nullptr) {
                find = find->right;
            }

            // Assign inorder predecessor here
            node->val = find->val;

            // Delete the inorder predecessor
            res = removeNode(node->left, find->val);
        }
    }

    if(node != nullptr) {
        // `node` will only be `nullptr`, when it is a leaf.
        // The parents of a leaf will automatically be balanced as
        // as we go back up the recursion stack.

        int hdf = height(node->right) - height(node->left);
        if (hdf > 1) {
            // Rebalance right heavy

            int c = height(node->right) - height(node->left);
            if(c >= 0) {
                // RR
                rotateRR(node);
            } else {
                // RL
                rotateLL(node->right);
                rotateRR(node);
            }
        } else if (hdf < -1) {
            // Rebalance left heavy

            int c = height(node->right) - height(node->left);
            if(c < 0) {
                // LL
                rotateLL(node);
            } else {
                // LR
                rotateRR(node->left);
                rotateLL(node);
            }
        }

        // Update height of critical node
        node->height = std::max(height(node->left), height(node->right)) + 1;

    }
    return res;
}


void AVLTree::rotateLL(AVLNode* & node) {

    AVLNode* t2 = node->left->right;

    AVLNode* temp = node->left;
    temp->right = node;
    node->left = t2;
    node = temp;

    node->height = 1 + std::max(height(node->left), height(node->right));
    AVLNode* orig = node->right;
    orig->height = 1 + std::max(height(orig->left), height(orig->right));
}


void AVLTree::rotateRR(AVLNode* & node) {

    AVLNode* t2 = node->right->left;

    AVLNode* temp = node->right;
    temp->left = node;
    node->right = t2;
    node = temp;

    node->height = 1 + std::max(height(node->left), height(node->right));
    AVLNode* orig = node->left;
    orig->height = 1 + std::max(height(orig->left), height(orig->right));
}


AVLNode* AVLTree::search(int val) {
    return searchNode(root, val);
}


AVLNode* AVLTree::searchNode(AVLNode* node, int val) {
    if(node == nullptr)
        return nullptr;
    if(val < node->val) {
        return searchNode(node->left, val);
    } else if (val > node->val) {
        return searchNode(node->right, val);
    } else {
        return node;
    }
}


void AVLTree::printInorder(AVLNode* n) {
    if(n != nullptr) {
        printInorder(n->left);
        std::cout << n->val << " ";
        printInorder(n->right);
    }
}


void AVLTree::preorderLevels(
    AVLNode* t, 
    int depth, 
    int ai, 
    std::vector<std::tuple<int, int, AVLNode*>>& levels
    ) {

    if(t != nullptr) {
        levels.push_back(std::tuple<int,int, AVLNode*>(depth, ai, t));
        preorderLevels(t->left , depth + 1, 2 * ai + 1, levels);
        preorderLevels(t->right, depth + 1, 2 * ai + 2, levels);
    }
}

void AVLTree::prettyPrint(std::ostream& out) {

    std::vector<std::tuple<int, int, AVLNode*>> levels;
    // Stores pairs of (depth, xval, node)
    /* where depth: is distance from root, where depth(root) = 0
             ai   : is index of node if this was a complete binary tree,
                        stored in an array
             node : is a pointer to the node
    */

    preorderLevels(root, 0, 0, levels);

    // Sort all the tuples by their depth
    // if depth is equal, then sort on the `ai` value.
    std::sort(levels.begin(), levels.end(), 
        [](std::tuple<int, int, AVLNode*>& p1, std::tuple<int, int, AVLNode*>& p2) {
            if(std::get<0>(p1) == std::get<0>(p2))
                return std::get<1>(p1) < std::get<1>(p2);
            return std::get<0>(p1) < std::get<0>(p2);
        }
    );

    // Get maxDepth from the last tuple of `levels`
    int maxDepth = std::get<0>(levels.back());

    // NOTE: Both have to be odd, to center properly
    // TODO: Calculate width, based on max size of string of node or some heuristic
    const int WIDTH = 5; // Width of each node
    const int SPACE = 1; // Space between node at deepest level
    // Works with SPACE = 1
    /*
        Seems to work properly with
        width space
        3     1
        5     1
        7     1
    */

    const int p2n = 1 << maxDepth; // No of nodes at maxDepth ie 2^maxDepth

    const int R = maxDepth + 1;                     // Rows for print string
    const int C = p2n * WIDTH + (p2n - 1) * SPACE;  // Cols for print string

    // Change this to a non blank character to see
    // where the nodes will be placed.
    const char PLACEHOLDER = ' '; 

    // Allocation
    const char BLANK = ' ';
    char** arr = new char*[R];
    for(int i = 0; i < R; ++i) {
        arr[i] = new char[C+1];
        memset(arr[i], BLANK, C * sizeof(char));
        arr[i][C] = '\0';
    }

    const int totalNodes = (1 << (maxDepth + 1)) - 1;
    std::vector<int> locations(totalNodes, -1);
    // Stores the x-location of a node in it's depth row
    // locations[5] = 12;
    // means index=5 node in a complete binary tree, at depth <appropriate depth>
    // is to be printed at char 12 onwards


    for(int r = R-1; r >= 0; --r) {

        int start(0);
        if(maxDepth - r != 0) {
            // Nodes under this node, but only left half
            int underNodes = 1 << (maxDepth - r - 1); 

            start = underNodes * WIDTH + (underNodes-1) * SPACE - WIDTH/2 ;
        }

        int gap(0);
        if(maxDepth - r == 0) {
            gap = SPACE;
        } else {
            gap = 2 * start + 1;
        }

        // Index of node in a complete binary tree as array
        int ai = (1 << r) - 1;
        for(int i = start; i + WIDTH - 1< C;) {
            locations[ai] = i;
            ai++;

            // Uncomment to show where nodes will be placed
            // if all nodes in complete binary tree existed.
            // memset(arr[r] + i, PLACEHOLDER, WIDTH);
            
            i = i + WIDTH + gap;
        }


    } // row loop in reverse


    for(int i = 0; i < levels.size(); ++i) {
        int depth, ai;
        AVLNode* node;
        std::tie(depth, ai, node) = levels[i];

        int xval = locations[ai];


        std::stringstream sv;
        sv << node;
        std::string temp;
        sv >> temp;

        int inputLength = temp.size();
        // https://stackoverflow.com/questions/17512825/align-text-to-center-in-string-c
        int extra = (WIDTH - inputLength)/2;

        // Pads the repr(node) if it's shorter than `WIDTH`
        // Set to a visible character like 'P' to see the padding
        const char PAD = BLANK;
        
        std::string res;
        if(extra > 0) {
            std::string pad = std::string(extra, PAD);
            res = pad + temp + pad;
        }
        else {
            res = temp;
        }

        // Makes sure a large repr() of a node doesn't exceed the allotted width
        std::strncpy(arr[depth] + xval, res.c_str(), WIDTH);
    }

    // Formatted Pretty Print of the tree
    for(int i = 0; i < R; ++i) {
        std::cout << arr[i] << std::endl;
    }


    // Clean up
    for(int i = 0; i < R; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
}


int main() {
    AVLTree tree;

    // When tree empty
    auto f = tree.search(5);
    std::cout << "search when tree is empty" << std::endl;
    std::cout << "Result: " << f << std::endl;

    // More or less balanced
    tree.insert(5);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(6);
    tree.insert(7);
    tree.insert(1);
    tree.insert(8);
    tree.insert(9);

    // auto g = tree.search(1);
    // watch("search when present");
    // watch(g);

    // auto h = tree.search(99);
    // watch("search when absent")
    // watch(h);

    /*
    Delete individually and check
    1 leaf (left child)
    4 leaf (right child)
    2 one child left
    6 one child right
    3 two child (left child of root)
    7 two child (right child of root)
    5 two child root
    */

    int del = 5;
    bool y = tree.remove(del);
    std::cout << "delete " << del << std::endl;
    std::cout << "Was deletion a success? " << std::boolalpha << y << std::endl;
    tree.prettyPrint();




    // Right ladder
    // tree.insert(1);
    // tree.insert(2);
    // tree.insert(3);
    // tree.insert(4);
    // tree.insert(5);
    // tree.insert(6);

    // Left ladder
    // tree.insert(6);
    // tree.insert(5);
    // tree.insert(4);
    // tree.insert(3);
    // tree.insert(2);
    // tree.insert(1);
    
    

    // printInorder(tree.root);

    // If tree gets too deep, the pretty print will go out of your screen
    // tree.prettyPrint();

    return 0;
}

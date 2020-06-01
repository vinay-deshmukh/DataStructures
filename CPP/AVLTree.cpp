
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
    * Destructor
    * (to make sure deallocation is happeing)
    */
    // ~AVLNode();

    /**
    * Operator<< Overload to print repr(node)
    */
    friend std::ostream& operator<<(std::ostream& out, const AVLNode* node);

};

AVLNode::AVLNode(int v, AVLNode* l, AVLNode* r)
    : val(v), height(0), left(l), right(r) {
}

// AVLNode::~AVLNode() {
//     std::cout << std::string(10, '*') << std::endl;
//     std::cout << val  << " was destroyed!"<< std::endl;
//     std::cout << std::string(10, '*') << std::endl;
// }

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
    * Deletes the given value from the tree
    * Returns a bool to denote success of deletion
    */
    bool remove(int val);

    /**
    * Return a pointer to the first node with value == `val`
    * If such node doesn't exist, return `nullptr`
    */
    AVLNode* search(int val);


    /**
    * Print the level order traversal to the ostream parameter.
    */
    void levelOrder(std::ostream& out = std::cout);

private:
    /**
    * Recursive insertion function called by `insert()`
    */
    void insertNode(AVLNode* & node, int val);

    /**
    * Recursive deletion function called by `remove()`
    */
    bool removeNode(AVLNode* & node, int val);

    /**
    * Rotate LL on critical node
    */
    void rotateLL(AVLNode* & node);

    /**
    * Rotate RR on critical node
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

    std::cout << "inserted " << val << std::endl;
    levelOrder();
    std::cout << std::endl;
}

void AVLTree::insertNode(AVLNode* & node, int val) {

    if(node == nullptr) {
        node = new AVLNode(val);
    } else if (val < node->val) {
        insertNode(node->left, val);
    } else { //if (val >= node->val) {
        insertNode(node->right, val);
    }

    int hdf = height(node->right) - height(node->left);
    if (hdf > 1) {
        // Rebalance right heavy
        watch("right");

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
        watch("left");

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

    node->height = std::max(height(node->left), height(node->right)) + 1;
}

bool AVLTree::remove(int val) {
    return removeNode(root, val);
}

bool AVLTree::removeNode(AVLNode* & node, int val) {
    if(node == nullptr)
        return false;
    watch(node->val);
    bool res = false;

    if(val < node->val) {
        res = removeNode(node->left, val);
    } else if (val > node->val) {
        res = removeNode(node->right, val);
    } else {
        // Found node, now delete it
        // return node;

        res = true;
        if(node->left == nullptr && node->right == nullptr) {
            // Leaf node
            watch("no child")
            delete node;
            node = nullptr;
        } else if (node->left == nullptr || node->right == nullptr) {
            // One child
            watch("one child")
            AVLNode* temp = node;
            delete temp;
            node = (node->left != nullptr) ? node->left : node->right;
        } else { //if(node->left != nullptr && node->right != nullptr) {
            // Two children
            watch("two child")
            watch(node)

            // Preserve the child pointers
            // AVLNode* noderight = node->right;
            AVLNode* nodeleft = node->left;

            // Find inorder predecessor
            // `find` will contain inorder predecessor
            AVLNode* find = nodeleft; 

            watch(find)
            watch(find->right)

            // AVLNode* prev = nullptr;
            watch("begin loop")
            while(find->right != nullptr) {
                // prev = find;
                find = find->right;
            }


            // Assign inorder predecessor here
            node->val = find->val;

            watch("Start deletion of inorder child now")
            res = removeNode(node->left, find->val);
        }
    }


    if(node != nullptr) {
        // Make sure that node isn't a nullptr
        // Node will become a nullptr, only when node was a leaf
        // which has been removed now.
        // Hence, node being `nullptr` can't be rebalanced, 
        // so this code block will not be applicable for a deleted leaf

        watch("balancing")
        watch(node)
        
        int hdf = height(node->right) - height(node->left);
        watch(node)
        watch(height(node->right));
        watch(height(node->left));
        watch(hdf)
        if (hdf > 1) {
            // Rebalance right heavy
            watch("right");

            int c = height(node->right) - height(node->left);
            if(c >= 0) {//val >= node->right->val) {
                // RR
                rotateRR(node);
            } else {
                // RL
                rotateLL(node->right);
                rotateRR(node);
            }
        } else if (hdf < -1) {
            // Rebalance left heavy
            watch("left");

            int c = height(node->right) - height(node->left);
            if(c < 0) {
                // LL
                watch("LL");
                rotateLL(node);
            } else {
                // LR
                watch("LR")
                rotateRR(node->left);
                watch("RRdone")
                rotateLL(node);
            }
        }
        node->height = std::max(height(node->left), height(node->right)) + 1;

    }
    return res;
}



void AVLTree::rotateLL(AVLNode* & node) {

    // AVLNode* t2 = nullptr; //node->left->right;
    // if(node->left != nullptr) {
    AVLNode* t2 = node->left->right;
    // }


    watch("LL start");
    watch(node)
    watch(node->left)
    watch(node->right)

    AVLNode* temp = node->left;
    temp->right = node;
    node->left = t2;
    node = temp;
    watch("after swap")
    watch(node)

    node->height = 1 + std::max(height(node->left), height(node->right));
    AVLNode* orig = node->right;
    orig->height = 1 + std::max(height(orig->left), height(orig->right));
    watch("LL end")
}

void AVLTree::rotateRR(AVLNode* & node) {

    watch("tt")
    // AVLNode* t2 = nullptr;//node->right->left;
    watch("defined")

        watch("node right not null")
    AVLNode* t2 = node->right->left;

    AVLNode* temp = node->right;
    temp->left = node;
    node->right = t2;
    node = temp;

    node->height = 1 + std::max(height(node->left), height(node->right));
    AVLNode* orig = node->left;
    orig->height = 1 + std::max(height(orig->left), height(orig->right));
    watch("RR end")
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

void preorderLevels(AVLNode* t, int depth, int ai, std::vector<std::tuple<int, int, AVLNode*>>& levels) {
    // depth is from root to current node, where depth(root) = 0
    // ai is index of node in an array which holds a complete binary tree
    // if ai is index of current node, then
    // left child = 2 * ai + 1
    // right child = 2* ai + 2

    if(t != nullptr) {
        
        // std::cout << n->val << " ";
        levels.push_back(std::tuple<int,int, AVLNode*>(depth, ai, t));
        preorderLevels(t->left , depth + 1, 2 * ai + 1, levels);
        preorderLevels(t->right, depth + 1, 2 * ai + 2, levels);
    }
}

void AVLTree::levelOrder(std::ostream& out) {

    std::vector<std::tuple<int, int, AVLNode*>> levels;
    // Stores pairs of (depth, xval, node)
    /* where depth is y-coordinate
             xval is index of node if this was a complete binary tree
             node is a pointer to the node
    */

    preorderLevels(root, 0, 0, levels);


    std::sort(levels.begin(), levels.end(), 
        [](std::tuple<int, int, AVLNode*>& p1, std::tuple<int, int, AVLNode*>& p2) {
            if(std::get<0>(p1) == std::get<0>(p2))
                return std::get<2>(p1)->val < std::get<2>(p2)->val;
            return std::get<0>(p1) < std::get<0>(p2);
        }
    );

    // for(auto& a: levels) {
    //     std::cout << std::get<0>(a) 
    //     << "->" << std::get<1>(a)
    //     << "->" << std::get<2>(a)
    //     << "\n";
    // }
    // std::cout << std::endl;

    // std::tuple<int,int,int>*
    auto x = std::max_element(levels.begin(), levels.end(), 
        [](std::tuple<int, int, AVLNode*>& p1, std::tuple<int, int, AVLNode*>& p2) {
            return std::get<0>(p1) < std::get<0>(p2);
        }
    );
    int maxDepth = std::get<0>(*x);

    // std::cout << "maxDepth" << maxDepth << '\n';
    // std::cout << "where depth of root is 0\n";

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

    // watch(p2n);

    const int R = maxDepth + 1;                     // Rows for print string
    const int C = p2n * WIDTH + (p2n - 1) * SPACE;  // Cols for print string
    const char PLACEHOLDER = ' ';
    // watch(R);
    // watch(C);

    // Allocation
    const char BLANK = ' ';
    char** arr = new char*[R];
    for(int i = 0; i < R; ++i) {
        arr[i] = new char[C+1];
        memset(arr[i], BLANK, C * sizeof(char));
        arr[i][C] = '\0';
    }

    // Do Work
    // last Row
    // for(int i = 0; i < C - WIDTH + 1; i=i+WIDTH+SPACE) {
    //     memset(arr[R-1] + i, 'x', WIDTH * sizeof(char));
    // }

    // std::cout << "total nodes:" << ( ) << std::endl;
    const int totalNodes = (1 << (maxDepth + 1)) - 1;
    std::vector<int> locations(totalNodes, -1);
    // Stores the x-location of a node in it's depth row
    // locations[5] = 12;
    // means index=5 node in a complete binary tree, at depth <appropriate depth>
    // is to be printed at char 12 onwards


    for(int r = R-1; r >= 0; --r) {

        int start = [&](){
            if(maxDepth - r == 0) {
                return 0;
            }

            int underNodes = 1 << (maxDepth - r - 1); 
            // Nodes under this node, but only left half

            int space = underNodes * WIDTH + (underNodes-1) * SPACE - WIDTH/2 ;
            return space;
        }();

        // watch(r);
        // watch(maxDepth - r);
        // watch(start);

        auto gap = [&](){ // between each node
            if(maxDepth - r == 0) {
                return SPACE;
            }
            return 2 * start + 1;
            // return 2 * start + SPACE;
        };


        int u = gap();

        // Index of node in a complete binary tree as array
        int ai = (1 << r) - 1;
        for(int i = start; i + WIDTH - 1< C;) {
            locations[ai] = i;
            ai++;

            // Uncomment to show where nodes will be placed
            // if all nodes in complete binary tree existed.
            memset(arr[r] + i, PLACEHOLDER, WIDTH);
            
            i = i + WIDTH + u;
        }


    } // row loop in reverse


    // std::cout << "locations\n";
    // for(auto& a: locations) {
    //     std::cout << a << std::endl;
    // }

    for(int i = 0; i < levels.size(); ++i) {
        int depth, ai;
        AVLNode* node;
        std::tie(depth, ai, node) = levels[i];

        int xval = locations[ai];

        // TODO: increase buffer width/handle 
        char buf[100] = {'\0'};

        std::stringstream sv;
        sv << node;
        char temp[100];
        sv >> temp;

        int inputLength = std::strlen(temp);
        // https://stackoverflow.com/questions/17512825/align-text-to-center-in-string-c
        int extra = (WIDTH - inputLength)/2;
        const char PAD = 'P';
        
        std::string res;
        if(extra > 0) {
            std::string pad = std::string(extra, PAD);
            res = pad + std::string(temp) + pad;
        }
        else {
            res = std::string(temp);
        }

        // Makes sure a large repr() of a node doesn't exceed the allotted width
        std::strncpy(arr[depth] + xval, res.c_str(), WIDTH);

    }



    // Print
    for(int i = 0; i < R; ++i) {
        // std::cout << arr[i] << std::endl;
        for(int c = 0; c < C; ++c) {
            if(arr[i][c] != '\0')
                printf("%c", arr[i][c]);
            else
                printf("%c", 'X');
        }
        printf("\n");
    }


    // Clean up
    for(int i = 0; i < R; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
}

void inorder(AVLNode* n) {
    if(n != nullptr) {
        inorder(n->left);
        std::cout << n->val << " ";
        inorder(n->right);
    }
}

int main() {
    AVLTree tree;

    // DONT INSERT 0
    
    // When tree empty
    auto f = tree.search(5);
    watch("search when empty");
    watch(f);

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
    watch(y)
    tree.levelOrder();




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
    
    

    // inorder(tree.root);

    // If tree gets too deep, the pretty print will go out of your screen
    // tree.levelOrder();

    return 0;
}

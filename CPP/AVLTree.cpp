
#include <algorithm>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

#define watch(x) std::cout << (#x) << "=" << x << std::endl;

class AVLNode {
public:
    int val;
    int balance;
    AVLNode* left;
    AVLNode* right;

    /**
    * Constructor
    */
    AVLNode(int v, AVLNode* l = nullptr, AVLNode* r = nullptr);

};

AVLNode::AVLNode(int v, AVLNode* l, AVLNode* r)
    : val(v), balance(0), left(l), right(r) {
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
    * and return a pointer to newly inserted node.
    */
    AVLNode* insert(int val);
    //TODO: functions

    /**
    * Print the level order traversal to the ostream parameter.
    */
    void levelOrder(std::ostream& out = std::cout);

private:
    /**
    * Recursive insertion function called by `insert()`.
    * Returns a pointer to the newly inserted node.
    */
    AVLNode* insertNode(AVLNode* node, int val);

    /**
    * TODO:
    */
    // void explore(AVLNode* node, std::queue<AVLNode*>& bfs);

};

AVLTree::AVLTree() : root(nullptr) {
}

// void AVLTree::explore(AVLNode* node, std::queue<AVLNode*>& bfs) {
//     if(node != nullptr) {
//         bfs.push(node->left);
//         bfs.push(node->right);
//     }
// }

AVLNode* AVLTree::insert(int val) {
    if(root == nullptr) {
        root = new AVLNode(val);
        return root;
    } else {
        return insertNode(root, val);
    }
    return nullptr;
}

AVLNode* AVLTree::insertNode(AVLNode* node, int val) {
    AVLNode* res;
    if (val < node->val) {
        if(node->left){
            res = insertNode(node->left, val);
        } else {
            res = node->left = new AVLNode(val);
        }
    } else if (val >= node->val) {
        if(node->right) {
            res = insertNode(node->right, val);
        } else {
            res = node->right = new AVLNode(val);
        }
    } else {
        // TODO: remove this block
        res = node;
        std::cout << "already in" << std::endl;
    }
    return res;
}

void preorderLevels(AVLNode* t, int depth, int ai, std::vector<std::tuple<int, int, int>>& levels) {
    // depth is from root to current node, where depth(root) = 0
    // ai is index of node in an array which holds a complete binary tree
    // if ai is index of current node, then
    // left child = 2 * ai + 1
    // right child = 2* ai + 2

    if(t != nullptr) {
        
        // std::cout << n->val << " ";
        levels.push_back(std::tuple<int,int, int>(depth, ai, t->val));
        preorderLevels(t->left , depth + 1, 2 * ai + 1, levels);
        preorderLevels(t->right, depth + 1, 2 * ai + 2, levels);
    }
}

void preorderFunction(AVLNode* n, std::function<void(AVLNode*)> func) {
    if(n != nullptr) {
        func(n);
        preorderFunction(n->left, func);
        preorderFunction(n->right, func);
    }
}


void AVLTree::levelOrder(std::ostream& out) {
    // TODO:
    // if tree is left ladder or right ladder,
    // then shave off all whitespace on one side of the root and return

    out << "yes" << std::endl;
    
    
    std::vector<std::tuple<int, int, int>> levels;
    // Stores pairs of (depth, xval, value)
    /* where depth is y-coordinate
             xval is x-coordinate,
                root is 0, nodes ot lef
    */

    preorderLevels(root, 0, 0,levels);


    std::sort(levels.begin(), levels.end(), 
        [](std::tuple<int, int, int>& p1, std::tuple<int, int, int>& p2) {
            if(std::get<0>(p1) == std::get<0>(p2))
                return std::get<2>(p1) < std::get<2>(p2);
            return std::get<0>(p1) < std::get<0>(p2);
        }
    );

    for(auto& a: levels) {
        std::cout << std::get<0>(a) 
        << "->" << std::get<1>(a)
        << "->" << std::get<2>(a)
        << "\n";
    }
    std::cout << std::endl;

    // std::tuple<int,int,int>*
    auto x = std::max_element(levels.begin(), levels.end(), 
        [](std::tuple<int, int, int>& p1, std::tuple<int, int, int>& p2) {
            return std::get<0>(p1) < std::get<0>(p2);
        }
    );
    int maxDepth = std::get<0>(*x);

    std::cout << "maxDepth" << maxDepth << '\n';
    std::cout << "where depth of root is 0\n";

    // NOTE: Both have to be odd, to center properly
    const int WIDTH = 3; // Width of each node
    const int SPACE = 1; // Space between node at deepest level

    // Can seem to change width and it works
    // But only works with SPACE = 1

    const int p2n = 1 << maxDepth; // No of nodes at maxDepth ie 2^maxDepth

    // watch(p2n);

    const int R = maxDepth + 1;                     // Rows for print string
    const int C = p2n * WIDTH + (p2n - 1) * SPACE;  // Cols for print string

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
            // memset(arr[r] + i, 'd', WIDTH);
            
            i = i + WIDTH + u;
        }


    } // row loop in reverse


    // std::cout << "locations\n";
    // for(auto& a: locations) {
    //     std::cout << a << std::endl;
    // }

    for(int i = 0; i < levels.size(); ++i) {
        int depth, ai, val;
        std::tie(depth, ai, val) = levels[i];

        int xval = locations[ai];

        // TODO: increase buffer width/handle 
        char buf[100] = {'\0'};

        // TODO: print Balance val as well, maybe overload << to get repr of node
        int nc = snprintf(buf, WIDTH + 1, "<%d>", val);
        // Makes sure a large number doesn't offset the other nodes
        
        std::strncpy(arr[depth] + xval, buf, WIDTH);

    }



    // Print
    for(int i = 0; i < R; ++i) {
        std::cout << arr[i] << std::endl;
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
    std::tuple<int, int, int, int> t;

    // DONT INSERT 0
    
    // More or less balanced
    tree.insert(5);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(6);
    tree.insert(7);
    tree.insert(1);

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
    
    


    std::cout << "preorderFunction\n";
    preorderFunction(tree.root, 
        [](AVLNode* n) {
            std::cout << "preorderFunction:";
            std:: cout << n->val << std::endl;
        }
    );


    // inorder(tree.root);

    // If tree gets too deep, the pretty print will go out of your screen
    tree.levelOrder();

    return 0;
}

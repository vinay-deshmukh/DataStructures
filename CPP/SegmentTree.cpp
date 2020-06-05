
#include <cassert>
#include <iostream>
#include <functional>
#include <vector>

class SegmentTree {
public:
    /**
    * Constructor takes the following arguments:
    *     v   : a vector of integers
    *     func: a bivariate, associative function,
    *           for e.g. addition, multiplication.
    *     def_val: a default value to return when query range
    *           goes out of bounds
    */
    explicit SegmentTree(
        std::vector<int> v, 
        std::function<int(const int&, const int&)> func,
        int def_val
        );

    /** 
    * Returns the result of a query on the interval [tl, tr]
    * based on the `func` passed in the constructor
    */
    int query(size_t tl, size_t tr);

    /*
    * Updates the value of `index`, where `index` is an index in
    * the vector passed to the constructor
    * The index is translated into the appropriate binary tree index
    * while recursing down to the leaf.
    */
    void update(size_t index, int val);


private:
    /*
    * Internal representation of a binary tree
    */
    std::vector<int> arr;

    /**
    * Value to be used when query parameters go out of bounds
    */
    int default_value;
    
    /**
    * A Bivariate, associative function which is the basis of query operations.
    */
    std::function<int(int,int)> func;


    /**
    * Count of the number of leaves
    */
    size_t vn;

    /**
    * Recursively construct the tree, where
    *    v          : input vector
    *    vi         : index of current node within an array representing a binary tree
    *                   vi for root = 0
    *                   vi for left  child of root = 2 * 0 + 1 = 1
    *                   vi for right child of root = 2 * 0 + 2 = 2
    *    start, end : The inclusive bounds of the current node.
    *       For reference, look at the diagram in main()
    */
    void buildTree(std::vector<int>& v, size_t vi, size_t start, size_t end);

    /**
    * Recursively query the tree, where
    *    vi         : index of current node within an array representing a binary tree
    *                   vi for root                            = 0
    *                   vi for left  child of root = 2 * 0 + 1 = 1
    *                   vi for right child of root = 2 * 0 + 2 = 2
    *    tl, tr     : inclusive bounds of the query parameters
    *    start, end : The inclusive bounds of the current node.
    *       For reference, look at the diagram in main()
    */
    int queryRecurse(size_t vi, size_t tl, size_t tr, size_t start, size_t end);

    /**
    * Recursively update the tree, where
    *    index      : Index (within the original input vector) to assign the given 
    *                   `val` at.
    *    val        : The new value for the index
    *    start, end : The inclusive bounds of the current node.
    *       For reference, look at the diagram in main()
    */
    void updateRecurse(size_t index, int val, size_t vi, size_t start, size_t end);
};


SegmentTree::SegmentTree(
    std::vector<int> v, 
    std::function<int(const int&, const int&)> func, 
    int def_val
    ) 
    : default_value(def_val), 
      func(func) {

    vn = v.size();
    arr = std::vector<int>(4 * vn, 0);
    
    buildTree(v, 0, 0, vn - 1);
}


void SegmentTree::buildTree(std::vector<int>& v, size_t vi, size_t start, size_t end) {

    if(start == end) {
        arr[vi] = v.at(start);
        return;
    }

    size_t mid = (start + end) / 2;
    size_t vi1 = (vi << 1) + 1;
    size_t vi2 = (vi << 1) + 2;
    buildTree(v, vi1, start, mid);
    buildTree(v, vi2, mid + 1, end);

    arr[vi] = func(arr[vi1], arr[vi2]);
}


int SegmentTree::query(size_t tl, size_t tr) {
    return queryRecurse(0, tl, tr, 0, vn - 1);
}

int SegmentTree::queryRecurse(size_t vi, size_t tl, size_t tr, size_t start, size_t end) {

    if(tr < tl) {
        return default_value;
    }

    if(tl <= start && end <= tr) {
        return arr.at(vi);
    }

    size_t mid = (start + end) / 2;
    size_t vi1 = (vi << 1) + 1;
    size_t vi2 = (vi << 1) + 2;

    int a = queryRecurse(vi1, tl, std::min(mid, tr), start, mid);
    int b = queryRecurse(vi2, std::max(mid + 1, tl), tr, mid + 1, end);

    return func(a, b);
}


void SegmentTree::update(size_t index, int val) {
    updateRecurse(index, val, 0, 0, vn - 1);
}

void SegmentTree::updateRecurse(size_t index, int val, size_t vi, size_t start, size_t end) {

    if(start == end) {
        // at leaf
        arr.at(vi) = val;
        return;
    }

    size_t mid = (start + end) / 2;
    size_t vi1 = (vi << 1) + 1;
    size_t vi2 = (vi << 1) + 2;

    if(index <= mid) {
        // go left child
        updateRecurse(index, val, vi1, start, mid);
    } else {
        // go right child
        updateRecurse(index, val, vi2, mid + 1, end);

    }

    arr[vi] = func(arr[vi1], arr[vi2]);
}


int main() {

    std::vector<int> v = {1, 2, 3, 4, 5};

    auto add = [](int a, int b) {return a + b;};
    int default_add = 0;
    SegmentTree st(v, add, default_add);
    /*
                       [0:4]=15
                      /        \
              [0:2]= 6         [3:4]= 9
               / \             /    \
        [0:1]=3   [2]=3    [3]=4   [4]=5
         /   \
    [0]=1   [1]=2

    */


    // Addition tree
    assert(st.query(0, 2) == 6);
    assert(st.query(1, 3) == 9);
    assert(st.query(4, 4) == 5);
    assert(st.query(0, 0) == 1);

    st.update(2, 7);

    assert(st.query(2, 4) == 16);

    std::cout << "Addition works correctly" << std::endl;


    // Multiplication
    std::vector<int> mv = {1, 2, 3, 4, 5};
    auto mul = [](int a, int b) {return a * b;};
    int mul_default = 1;
    SegmentTree mst(mv, mul, mul_default);
    /*
                        [0:4]=120
                      /        \
              [0:2]= 6         [3:4]= 20
               / \             /    \
        [0:1]=2   [2]=3    [3]=4   [4]=5
         /   \
    [0]=1   [1]=2
    */

    assert(mst.query(0, 2) == 6);
    assert(mst.query(1, 3) == 24);
    assert(mst.query(0, 4) == 120);
    assert(mst.query(1, 2) == 6);

    mst.update(1, 7);
    assert(mst.query(1, 4) == 420);

    std::cout << "Multiplication works correctly" << std::endl;

    return 0;
}

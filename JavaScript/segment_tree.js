/** A class to represent a Segment Tree.
 */
class SegmentTree {
    /**
     * @param {Array} arr   The segment tree will be built using this iterable
     * @param {function} func  The function that will be used to built the tree
     * @param {number} def     The default value with which the tree array is initialized
     */
    constructor(arr, func, def) {
        if(arr != null) {
            this.arr = arr;
            this.len_arr = this.arr.length;
        }
        else
            throw TypeError("Argument arr should not be null");

        if(func != null) {
            this.func = func;
        }
        else
            throw TypeError("Argument func should not be null");

        this.default = def;
        this.build_tree();
    }

    /**
     * Function to return the next number which is a power of 2.
     *
     * @param n
     * @returns number
     */
    static next_power_of_2(n) {
        // Checks if n is already a power of 2
        if((n & n - 1) === 0)
            return n;

        let val = 1;

        // Loop will run until it give us the value which is greater than n.
        while (val <= n) {
            // val will be left shifted by 1 in each iteration.
            val <<= 1;
        }

        return val;

    }

    /**Function to build the segment tree using the function provided.
     */
    build_tree() {
        // If number of leaves in a full binary tree is n,
        // total number of internal nodes in the tree is n - 1.
        // Hence, total number of nodes in the tree will be 2n - 1.
        this.n_leaves = SegmentTree.next_power_of_2(this.len_arr);
        const n = this.n_leaves << 1;

        // initializing all nodes of the tree to 0
        this.tree = [];
        for (let i = 0; i < n; i++)
            this.tree.push(this.default);

        // copying array to leaves of the tree
        for (let i = 0; i < this.len_arr; i++)
            this.tree[this.len_arr + i] = this.arr[i];

        // updating all the nodes of the tree
        for (let i = this.len_arr - 1; i > 0; i--)
            this.tree[i] = this.func(this.tree[i << 1], this.tree[i << 1 | 1])

    }

    /**
     * Function to update the element at the given index with new value
     *
     * @param i
     * @param value
     */
    update(i, value) {
        // check if the index is within bounds
        if(i > this.len_arr)
            throw new Error('Index out of bounds');

        i += this.len_arr;
        this.tree[i] = value;
        let p = i >> 1;

        // updating the upper level nodes of the tree
        while (p > 0) {
            this.tree[p] = this.func(this.tree[p << 1], this.tree[p << 1 | 1]);
            p >>= 1;
        }
    }

    /**
     * Returns the self.func(arr[l..r]) equivalent in O(log(n)) time.
     *
     * @param left
     * @param right
     * @returns {*}
     */
    query(left, right, l=1, r=this.n_leaves, i=1) {
        if (left < 0 || right > this.len_arr || left > right) {
            throw new Error('Index out of bounds');
        }

        if (r <= right && left <= l)
            return this.tree[i - 1];
        else if (r < left || right < l)
            return this.default;
        else {
            let mid = (l + r) >> 1;
            return this.func(
                this.query(
                    left, right,
                    l, mid,
                    i << 1
                ),
                this.query(
                    left, right,
                    mid + 1, r,
                    (i << 1) | 1
                )
            )
        }
    }
}

// Sample test cases to test the implementation of Segment Tree
const main = function () {
    const arr = [1, 2, 3, 5, 5];

    // Testing segment tree for sum of ranges
    const add_func = function (x, y) {
        return x + y;
    };
    const sgt1 = new SegmentTree(arr, add_func, 0);
    console.log(sgt1.query(1, 3) === 10);
    sgt1.update(2, 5); // changing the element at index 2 to 5
    console.log(sgt1.query(1, 3) === 12);

    // Testing segment tree for multiplication of ranges
    const mult_func = function (x, y) {
        return x * y;
    };
    const sgt2 = new SegmentTree(arr, mult_func, 1);
    console.log(sgt2.query(1, 3) === 30);
    sgt2.update(2, 5); // changing the element at index 2 to 5
    console.log(sgt2.query(1, 3) === 50);

};

main();

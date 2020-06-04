import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/** A class to represent a Segment Tree.
 */
public class SegmentTree {
    /**
     * @param {List} arr   The segment tree will be built using this iterable
     * @param {function} func  The function that will be used to built the tree
     * @param {number} def     The default value with which the tree array is initialized
     */
    List<Integer> arr;
    Operations func;
    int def, n_leaves, len_arr;
    List<Integer> tree;

    public SegmentTree(List<Integer> arr, Operations func, int def) {
        if (arr == null)
            throw new NullPointerException("Argument arr should not be null");
        this.arr = arr;
        this.len_arr = this.arr.size();

        if (func == null)
            throw new NullPointerException("Argument func should not be null");
        this.func = func;
        this.def = def;

        buildTree();
    }

    /**
     * Function to return the next number which is a power of 2.
     *
     * @param n
     * @returns number
     */
    private int next_power_of_2(int n) {
        // Checks if n is already a power of 2
        if ((n & n-1) == 0)
            return n;
        int val = 1;

        // Loop will run until it give us the value which is greater than n.
        while (val <= n)
            // val will be left shifted by 1 in each iteration.
            val <<= 1;

        return val;
    }

    /**Function to build the segment tree using the function provided.
     */
    private void buildTree() {
        // If number of leaves in a full binary tree is n,
        // total number of internal nodes in the tree is n - 1.
        // Hence, total number of nodes in the tree will be 2n - 1.
        this.n_leaves = next_power_of_2(this.len_arr);
        int n = this.n_leaves << 1;

        // initializing all nodes of the tree to 0
        this.tree = new ArrayList<>();
        for (int i = 0; i < n; i++)
            this.tree.add(this.def);

        // copying array to leaves of the tree
        for (int i = 0; i < this.len_arr; i++)
            this.tree.set(this.len_arr+i, this.arr.get(i));

        // updating all the nodes of the tree
        for (int i = this.len_arr - 1; i > 0; i--)
            this.tree.set(i, this.func.operate(this.tree.get(i << 1), this.tree.get(i << 1 | 1)));
    }

    /**
     * Function to update the element at the given index with new value
     *
     * @param i
     * @param value
     */
    public void update(int i, int value) {
        if (i > this.len_arr)
            throw new IndexOutOfBoundsException("Index i should be less than length of the arr");

        i += this.len_arr;
        this.tree.set(i, value);
        int p = i >> 1;

        while (p > 0) {
            this.tree.set(p, this.func.operate(this.tree.get(p << 1), this.tree.get(p << 1 | 1)));
            p >>= 1;
        }
    }

    /**
     * Returns the self.func(arr[l..r]) equivalent in O(log(n)) time.
     *
     * @param left
     * @param right
     * @returns number
     */
    public int query(int left, int right) {
        return query(left, right, 1, this.n_leaves, 1);
    }

    private int query(int left, int right, int l, int r, int i) {
        if (left < 0 || right > this.len_arr || left > right)
            throw new IndexOutOfBoundsException();

        if (r <= right && left <= l)
            return this.tree.get(i - 1);
        else if (r < left || right < l)
            return this.def;
        else {
            int mid = (l + r) >> 1;
            return this.func.operate(
                query(
                    left, right,
                    l, mid,
                    i << 1
                ),
                query(
                    left, right,
                    mid + 1, r,
                    i << 1 | 1
                )
            );
        }
    }

}

@FunctionalInterface
interface Operations {
    int operate(int x, int y);
}

// Sample test cases to test the implementation of Segment Tree
class App {
    public static void main(String[] args) {
        List<Integer> arr = new ArrayList<>(Arrays.asList(1, 2, 3, 5, 5));

        // Testing segment tree for sum of ranges
        Operations add = (x, y) -> x + y;
        SegmentTree sgt1 = new SegmentTree(arr, add, 0);
        assert sgt1.query(1, 3) == 10;
        sgt1.update(2, 5); // changing the element at index 2 to 5
        assert sgt1.query(1, 3) == 12;

        // Testing segment tree for multiplication of ranges
        Operations mult = (x, y) -> x * y;
        SegmentTree sgt2 = new SegmentTree(arr, mult, 1);
        assert sgt2.query(1, 3) == 30;
        sgt2.update(2, 5); // changing the element at index 2 to 5
        assert sgt2.query(1, 3) == 50;

    }
}

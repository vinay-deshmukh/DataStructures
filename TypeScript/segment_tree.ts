// returns the number of bits required to represent
// the given number in binary format.
// The function assumes the input to be right.
// It doesn't check for specifically data type
function bit_length(n: number): number {
    let n_bits = 0

    // following will run till the time n is not 0.
    // Don't know how it will behave with negative numbers.
    // input should be a whole number.
    while (n > 0) {
        n_bits += 1
        n >>= 1
    }
    return n_bits;
}


// returns first number greater than or equal to the input number n
// which is a power of 2.
function nearest_pow_2(n: number): number {

    // n & n - 1 flips the last set bit.
    // 2's power is a number with only a single bit set.
    // flipping only bit set will return 0.
    // hence, if number after flipping the last set bit is 0, it will be a power of 2.
    if ((n & n - 1) == 0)
        return n

    // since the control reached here, the number is not a power of 2.
    return bit_length(n) // analogous to python's int.bit_length()
}


// NOTE: 1-based indexing of segment tree is allowed.
/**
 *  initializes segment tree with given array of objects.
 *  arr: arr is a collection of objects.
 *       arr should be an iterable which supports each function.
 *  func: a bivariate function.
 *        must work with all objects in the collection provided (arr)
 *  fallback: default value of the leaves which won't affect the
 *            computation of bivariate func
 *            when value of only one parameter is given.
 *   For example:
 *       In case if we are building tree to answer min queries,
 *           fallback = inf will always yield the right answer.
 *       If segment tree is required to answer max queries,
 *           fallback = -inf
 *       if we are answering sum queries,
 *           fallback = 0
 */
class SegmentTree{
    fallback: any

    // func should be an associative function for a correct result.
    // Answer might not be as expected for a non-associative function.
    func: (x: any, y: any) => any

    internalArr: any[];
    nLeaves: number = 0


    /**
     * sets all the internal nodes of the segment tree.
     * This method requires func argument to be set
     * internal array will be a representation of a binary tree.
     * leaves of the tree is the array elements.
     * since segment tree is a full binary tree,
     *     the number of leaves should be a power of 2.
     * Hence, for a lossless tree conversion, we take number of elements
     *     as pow of 2 just >= the current arr size.
     *
     * If number of leaves in a full binary tree is n,
     *   total number of internal nodes in the tree is n - 1
     * Hence, total number of nodes in the tree will be 2n - 1
     * but we will use 2n elements and use a 1 based indexing.
     */
    constructor(arr: any[], fallback: any, func: (x: any, y: any) => any) {
        this.fallback = fallback
        this.func = func
        this.nLeaves = nearest_pow_2(arr.length);
        var n = this.nLeaves << 1;

        this.internalArr = []

        // initializing all internal nodes with the default value.
        for (let i = 0; i < n; i++){
            this.internalArr.push(fallback)
        }

        // copying array to leaves of the tree.
        for (let i = this.nLeaves; i < this.nLeaves + arr.length; i++) {
            this.internalArr[i] = arr[i - this.nLeaves]
        }

        // updating all the nodes of the tree
        // the for loop doesn't update zeroth index.
        for (var i = this.nLeaves - 1; i > 0; i--) {
            this.internalArr[i] = func(this.internalArr[i << 1], this.internalArr[i << 1 | 1])
        }
    }

    // updating the array element with new_val
    // idx in (0..arr.size)
    update(idx: number, new_val: number) {
        // idx is 1-based
        idx += this.nLeaves - 1

        // Note: this will allow updating indices
        //     that were not in original array  but is
        //     less than next power of 2 of arr.size.
        if (idx >= this.internalArr.length) {
            throw new RangeError;
        }

        // update the leaf of the tree with the new value.
        this.internalArr[idx] = new_val

        // p represent the parent of current node.
        var p = idx >> 1

        // since we are using a 1-based indexing,
        //     p=0 is an invalid state.
        while (p > 0) {
            // recomputing current parent's value because 
            //     it's child might have invalidated it's old value.
            this.internalArr[p] = this.func(this.internalArr[p << 1], this.internalArr[p << 1 | 1])

            // move to parent of the current parent.
            p >>= 1
        }
    }

    query(
        query_l: any, query_r: any,  // left and right bounds of the query.
                           //     right inclusive.
        segment_l = 1,
        segment_r = this.nLeaves,
        i = 1
    ): any {
        if (query_l <= segment_l && segment_r <= query_r) {
            // current segment is subsumed by the query segment.
            return this.internalArr[i]
        }
        else if (segment_r < query_l || query_r < segment_l) {
            // disjoint segments.
            return this.fallback
        }
        else {
            const mid = (segment_l + segment_r) >> 1
            return this.func(
                this.query(
                    query_l, query_r,
                    segment_l, mid,
                    i << 1
                ),
                this.query(
                    query_l, query_r,
                    mid + 1, segment_r,
                    (i << 1) | 1
                )
            )
        }
    }

}


function main() {
    // Demonstrates how the TrieTree class can be used.
    const add = (x: number, y: number) => x + y
    var st = new SegmentTree("hello world".split(''), "", add)
    console.assert(st.query(1, 4) == "hell", "Segment Tree Faulty")
}


main()

import math


class SegmentTree:
    '''Class representation of Segment Tree
    
    ...

    Attributes:
    ----------
    arr: Any iterable
        the segment tree will be built using this iterable
    len-arr: int
        length of the iterable arr
    func: method/function
        the function that will be used to built the tree
    default: Any
        the default value with which the tree array is initialized
    tree: list
        the representation of the tree

    Note: Indexing is 0 based in this implementation
    '''
    def __init__(self, arr, func, default):

        # Checking if iterable passed is of NoneType
        if arr is not None:
            self.arr = arr
            self.len_arr = len(self.arr)
        else:
            raise TypeError('Argument arr should not be null')

        # Checking if function passed is of NoneType
        if func is not None:
            self.func = func
        else:
            raise TypeError('Argument func should not be null')

        self.default = default
        self._build_tree()

    def _next_power_of_2(self, n):
        '''Function to return the next number which is a power of 2'''

        # Checks if n is already a power of 2
        if (n & n - 1) == 0:
            return n

        val = 1

        # Loop will run until it give us the value which is greater than n.
        while val <= n:

            # val will be left shifted by 1 in each iteration.
            val = val << 1
        return val

    def _build_tree(self):
        '''Function to build the segment tree using the function provided.'''

        # If number of leaves in a full binary tree is n,
        # total number of internal nodes in the tree is n - 1
        # Hence, total number of nodes in the tree will be 2n - 1.
        self.n_leaves = self._next_power_of_2(self.len_arr)
        n = self.n_leaves << 1

        # initializing all nodes of the tree to 0
        self.tree = [self.default for i in range(n)]

        # copying array to leaves of the tree.
        for i in range(self.len_arr):
            self.tree[self.len_arr + i] = self.arr[i]

        # updating all the nodes of the tree.
        for i in range(self.len_arr-1, 0, -1):
            self.tree[i] = self.func(self.tree[i << 1], self.tree[i << 1 | 1])

    def update(self, i, value):
        '''Function to update the element at the given index with new value'''

        # check if the index is within bounds
        if i > self.len_arr:
            raise IndexError('Index out of bounds')

        i += self.len_arr
        self.tree[i] = value
        p = i >> 1

        # updating the upper level nodes of the tree
        while p > 0:
            self.tree[p] = self.func(self.tree[p << 1], self.tree[p << 1 | 1])
            p >>= 1

    def query(self, left, right, l=1, r=None, i=1):
        '''Returns the self.func(arr[l..r]) equivalent in O(log(n)) time.'''

        # checking whether the range is out of bounds.
        if left < 0 or right > self.len_arr or left > right:
            raise IndexError('Index is out of bounds')

        if r is None:
            r = self.n_leaves

        if left <= l and r <= right:
            return self.tree[i-1]
        elif r < left or right < l:
            return self.default
        else:
            mid = (l + r) >> 1
            return self.func(
                self.query(
                    left, right,
                    l = l, r = mid,
                    i = i << 1
                ),
                self.query(
                    left, right,
                    l = mid + 1, r = r,
                    i = (i << 1) | 1
                )
            )


# Sample test cases to test the implementation of Segment Tree
def main():
    arr = [1, 2, 3, 5, 5]
    
    # Testing segment tree for sum of ranges
    add_func = lambda x, y: x + y
    sgt1 = SegmentTree(arr, add_func, default=0)

    assert sgt1.query(1, 3) == 10
    sgt1.update(2, 5) # changing the element at index 2 to 5
    assert sgt1.query(1, 3) == 12

    # Testing segment tree for multiplication of ranges
    mult_func = lambda x, y: x * y
    sgt2 = SegmentTree(arr, mult_func, default=1)
    
    assert sgt2.query(1, 3) == 30
    sgt2.update(2, 5) # changing the element at index 2 to 5
    assert sgt2.query(1, 3) == 50

if __name__ == '__main__':
    main()

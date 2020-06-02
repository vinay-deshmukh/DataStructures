# returns the number of bits required to store n.
# n should be an integer.
def get_bit_length(n)
    if n == 0
        # we require no bit to store 0.
        # That is, absence of bit is considered 0.
        return 0
    else 
        return 1 + get_bit_length(n >> 1)
    end
end


def nearest_pow_2(n)
    # returns next number which is a power of 2.
    if n & (n - 1) == 0
        # n is already a power of 2.
        return n
    end

    # current number is not already power of 2.
    # answer is pow(2, ⌈log2(n)⌉)
    # number of bits required to represent n is ⌈log2(n)⌉.
    return 1 << get_bit_length(n)
end


class SegmentTree
    
    attr_accessor :internal_arr, :n_leaves, :func
    # full binary tree represented in array form.
    @internal_arr = nil

    @n_leaves = 0

    # bivariate function which determines 
    #    the type of query segment tree will handle.
    @func = nil

    # initializes segment tree with given array of objects.
    # arr: arr is a collection of objects. 
    #      arr should be an iterable which supports each function.
    # func: a bivariate function.
    #       must work with all objects in the collection provided (arr)
    # fallback: default value of the leaves which won't affect the 
    #           computation of bivariate func 
    #           when value of only one parameter is given.
    #    For example:
    #      In case if we are building tree to answer min queries,
    #          fallback = inf will always yield the right answer.
    #      If segment tree is required to answer max queries,
    #          fallback = -inf
    #      if we are answering sum queries,
    #          fallback = 0
    def initialize(arr, func, fallback)
        # check if the input array is an iterable 
        #       that supports each function.
        unless arr.respond_to? :each
            # raise TypeError, "The object must be an iterable."
        end
        
        @func = func
        @fallback = fallback

        self.build_tree(arr)
    end

    def build_tree(arr)
        # sets all the internal nodes of the segment tree.
        # This method requires func argument to be set.

        unless arr != nil
            raise ArgumentError, "Uninitialized array object (arr)"
        end

        # internal array will be a representation of a binary tree.
        # leaves of the tree is the array elements.
        # since segment tree is a full binary tree, 
        #     the number of leaves should be a power of 2.
        # Hence, for a lossless tree conversion, we take number of elements 
        #     as pow of 2 just >= the current arr size.
        #
        # If number of leaves in a full binary tree is n,
        #   total number of internal nodes in the tree is n - 1
        # Hence, total number of nodes in the tree will be 2n - 1
        # but we will use 2n elements and use a 1 based indexing.
        @n_leaves = nearest_pow_2(arr.size)
        n = @n_leaves << 1

        @internal_arr = Array.new(n, @fallback)
        # copying array to leaves of the tree.
        # ignore magic numbers 1 and 3 below. Will explain later.
        for i in (@n_leaves .. @n_leaves + arr.size - 1)
            @internal_arr[i] = arr[i - @n_leaves]
        end
 
        # updating all the nodes of the tree.
        (1 - @n_leaves .. -1).each do |i|
            i = -i
            @internal_arr[i] = @func.call(@internal_arr[i << 1], @internal_arr[i << 1 | 1])
        end
    end

    # updating the array element with new_val
    # idx in (0..arr.size)
    def update(idx, new_val)
        # idx is 1-based
        idx += @n_leaves - 1

        # Note: this will allow updating indices 
        #     that were not in original array  but is 
        #     less than next power of 2 of arr.size.
        unless idx < @internal_arr.size
            raise KeyError, "Index Out of Bounds Error"
        end
        @internal_arr[idx] = new_val
        
        p = idx >> 1
        
        # since we are using a 1-based indexing, 
        #     p=0 is an invalid state.
        while p > 0 do
            @internal_arr[p] = @func.call(@internal_arr[p << 1], @internal_arr[p << 1 | 1])
            p >>= 1
        end
    end

    # The most important part of segment tree.
    # returns @func(arr[l..r]) equivalent in O(log(n)) time.
    # Args:
    #    
    def query(
        query_l, query_r,  # left and right bounds of the query.
                           #     right inclusive.
        segment_l = 1,
        segment_r = @n_leaves, 
        i = 1
    )
        if query_l <= segment_l and segment_r <= query_r
            # current segment is subsumed by the query segment.
            return @internal_arr[i]
        elsif segment_r < query_l or query_r < segment_l
            # disjoint segments.
            return @fallback
        else
            mid = (segment_l + segment_r) >> 1
            return @func.call(
                query(
                    query_l, query_r,
                    segment_l, mid,
                    i << 1
                ),
                query(
                    query_l, query_r,
                    mid + 1, segment_r,
                    (i << 1) | 1
                )
            )
        end
    end
end




class ValueError < StandardError
end


def main
    # Function for demonstrating and testing the Segment Tree.

    arr = [1, 2, 3, 5, 5]
    add_func = Proc.new {|x, y| x + y}
    min_func = Proc.new {|x, y| [x, y][x < y ? 0 : 1]}
    
    
    # segment tree for answering range sum query:
    sum_st = SegmentTree.new(arr, func=add_func, fallback=0)
    raise ValueError, "Segment Tree Faulty" unless sum_st.query(1, 1) == 1
    raise ValueError, "Segment Tree Faulty" unless sum_st.query(2, 4) == arr[1..3].inject(0, :+)
    raise ValueError, "Segment Tree Faulty" unless sum_st.query(5, 4) == fallback
    sum_st.update(4, 4)
    raise ValueError, "Segment Tree Faulty" unless sum_st.query(1, 5) == 15   # 1 + 2 + 3 + 4 + 5 = 5 * 6 >> 1 = 15


    # segment tree for answering range minimum query:
    arr = [1, 2, 3, 5, 5]
    fallback = 1.0 / 0     # inf since min(obj, inf) = obj
    min_st = SegmentTree.new(arr, func=min_func, fallback)
    raise ValueError, "Segment Tree Faulty" unless min_st.query(1, 1) == 1
    raise ValueError, "Segment Tree Faulty" unless min_st.query(2, 5) == 2
    min_st.update(3, -1) # setting third element to -1
    raise ValueError, "Segment Tree Faulty" unless min_st.query(1, 5)== -1
end

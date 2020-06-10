from math import sqrt, ceil, floor


class VanEmdeBoas:
    '''A class to represent Van Emde Boas Tree.

    ...

    Attributes
    ----------
    u : int
        size of the universe
    min: int
        minimum element present in the tree.
    max: int
        maximum element present in the tree.
    clusters: List[VanEmdeBoas]
        array of high(sqrt(u)) pointers to VanEmdeBoas(low(sqrt(u))) trees.
    summary: VanEmdeBoas
        a pointer summary to  a VanEmdeBoas(high(sqrt(u))) tree.
    '''
    def __init__(self, u):
        if u < 0:
            raise ValueError('u should be greater than 0')

        self.min = None
        self.max = None
        self.u = 2

        while self.u < u:
            self.u = self.u * self.u

        if u > 2:
            self.clusters = [None for _ in range(self.high(self.u))]
            self.summary = None

    def get_min(self):
        return self.min

    def get_max(self):
        return self.max

    def high(self, x):
        return floor(x / sqrt(self.u))

    def low(self, x):
        return x % ceil(sqrt(self.u))

    def index(self, x, y):
        return x * floor(sqrt(self.u)) + y

    def is_member(self, x):
        # base case, where x is the minimum or maximum element
        if self.min == x or self.max == x:
            return True

        elif self.u <= 2:
            return False

        else:
            cluster = self.clusters[self.high(x)]
            if cluster is not None:
                return cluster.is_member(self.low(x))
            else:
                return False

    def empty_veb_insert(self, x):
        '''Function to insert x when vEB tree is empty'''
        self.min = x
        self.max = x

    def insert(self, x):
        if self.min is None:
            self.empty_veb_insert(x)
        else:
            # here we get a new minimum element.
            # Since we don't want to lose the original min,
            # however, and so we need to insert it into one of V ’s clusters.
            # Therefore we swap x with min, so that we insert
            # original min into one of the clusters.
            if x < self.min:
                # temp = x
                # x = self.min
                # self.min = temp
                x, self.min = self.min, x

            if self.u > 2:
                cluster_id = self.high(x)
                high_u = self.high(self.u)
                low = self.low(x)

                # checks whether the cluster that x will go
                # into is empty.
                if self.clusters[cluster_id] is None:
                    self.clusters[cluster_id] = VanEmdeBoas(high_u)

                # checking whether the summary is empty
                if self.summary is None:
                    self.summary = VanEmdeBoas(high_u)

                # inserts x’s cluster number into the summary
                if self.clusters[cluster_id].min is None:
                    self.summary.insert(cluster_id)
                    self.clusters[cluster_id].empty_veb_insert(low)

                else:
                    # If x’s cluster is not currently empty, then
                    # inserts x into its cluster
                    self.clusters[cluster_id].insert(low)

            # taking care of updating max
            if x > self.max:
                self.max = x

    def delete(self, x):
        # base case to check if there is only one
        # element present in the tree.
        if self.min == self.max:
            self.min = None
            self.max = None

        elif self.u == 2:
            if x == 0:
                self.min = 1
            else:
                self.min = 0
            self.max = self.min

        else:
            high = self.high(x)
            low = self.low(x)
            # In this case, we will have to delete an element from a cluster.
            # The element we delete from a cluster might not be x,
            # however, because if x equals min, then once we have
            # deleted x, some other element within one of V ’s clusters 
            # becomes the new min, and we have to delete that other element 
            # from its cluster.
            if x == self.min:
                # sets first-cluster to the number of the cluster
                # that contains the lowest element other than min
                first_cluster = self.summary.min
                x = self.index(first_cluster, self.clusters[first_cluster].min)
                self.min = x

            # we need to delete element x from its cluster,
            # whether x was the value originally passed to delete() or 
            # x is the element becoming the new minimum
            self.clusters[self.high(x)].delete(self.low(x))

            if self.clusters[self.high(x)].min is None:
                # cluster is empty and now we delete x from its summary.
                self.summary.delete(self.high(x))

                # updating max
                # checking if we are deleting maximum element
                if x == self.max:
                    summary_max = self.summary.max

                    # if all clusters are empty, then only minimum value
                    # remain that value is been assigned to max
                    if summary_max is None:
                        self.max = self.min

                    else:
                        # sets max to the maximum element in the 
                        # highest-numbered cluster
                        self.max = self.index(summary_max, 
                                            self.clusters[summary_max].max)

            # case in which x’s cluster did not become empty
            # due to x being deleted. Although we do not 
            # have to update the summary in this case,
            # we might have to update max
            elif x == self.max:
                self.max = self.index(self.high(x), self.clusters[self.high(x)].max)

    def predecessor(self, x):
        if self.u <= 2:
            # base case where we are finding predecessor
            # of 1 and 0 is the minimum element
            if x == 1 and self.min == 0:
                return 0
            return None

        # if x is strictly greater than the maximum element
        elif self.max is not None and x > self.max:
            return self.max

        else:
            high = self.high(x)
            low = self.low(x)
            high_cluster = self.clusters[high]
            min_low = None

            if high_cluster is not None:
                min_low = high_cluster.min

            if min_low is not None and low > min_low:
                offset = high_cluster.predecessor(low)
                if offset is not None:
                    return self.index(self.high(x), offset)
                else:
                    offset = 0
                    return self.index(self.high(x), offset)

            else:
                pred_cluster = None
                if self.summary is not None:
                    pred_cluster = self.summary.predecessor(high)

                # when x’s predecessor, if it exists,
                # does not reside in x’s cluster
                if pred_cluster is None:

                    # x’s predecessor is the minimum value in vEB
                    # tree V , then the successor resides in 
                    # no cluster at all.
                    if self.min is not None and x > self.min:
                        return self.min
                    else:
                        return None

                else:
                    offset = self.clusters[pred_cluster].max
                    return self.index(pred_cluster, offset)

    def successor(self, x):
        '''Function to return the next element in tree after x'''
        if self.u <= 2:

            # base case, where we are trying to find successor of 0
            # and 1 is present in the vEB tree.
            if x == 0 and self.max == 1:
                return 1
            return None
        
        # if x is strictly less than min, return the min element
        elif self.min is not None and x < self.min:
            return self.min

        else:
            high = self.high(x)
            low = self.low(x)
            high_cluster = self.clusters[high]
            max_low = None

            # max_low = maximum element in x's cluster
            if high_cluster is not None:
                max_low = high_cluster.max

            # If x’s cluster contains some element 
            # that is greater than x, then we know 
            # that x’s successor lies somewhere within x’s cluster
            if max_low is not None and low < max_low:
                offset = high_cluster.successor(low)
                return self.index(self.high(x), offset)

            else:
                # if x is greater than or equal to the 
                # greatest element in its cluster.
                succ_cluster = None
                if self.summary is not None:
                    succ_cluster = self.summary.successor(high)

                if succ_cluster is None:
                    return None

                else:
                    offset = self.clusters[succ_cluster].min
                    return self.index(succ_cluster, offset)


def main():
    veb = VanEmdeBoas(u=1000)

    # inserting some elements into vEB Tree.
    veb.insert(3)
    veb.insert(5)
    veb.insert(10)

    assert veb.is_member(4) == False
    assert veb.is_member(3) == True

    assert veb.successor(3) == 5
    assert veb.predecessor(5) == 3

    veb.delete(3) # deleting element 3

    assert veb.is_member(3) == False
    assert veb.is_member(10) == True

    assert veb.get_min() == 5
    assert veb.get_max() == 10

    assert veb.successor(5) == 10
    assert veb.predecessor(10) == 5

if __name__ == '__main__':
    main()
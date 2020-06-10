import java.util.ArrayList;
import java.util.List;

class VanEmdeBoas {
    // Size of the universe
    Integer u;

    // Minimum element present in the tree.
    Integer min;

    // Maximum element present in the tree.
    Integer max;

    // array of high(sqrt(u)) pointers to VanEmdeBoas(low(sqrt(u))) trees.
    List<VanEmdeBoas> clusters = new ArrayList<>();

    // a pointer summary to  a VanEmdeBoas(high(sqrt(u))) tree.
    VanEmdeBoas summary;

    public VanEmdeBoas(long u) throws IllegalArgumentException  {
        if (u < 0)
            throw new IllegalArgumentException("value of u cannot be less than 0");

        this.u = 2;
        this.min = null;
        this.max = null;

        while (this.u < u)
            this.u *= this.u;

        if (u > 2) {
            for (int i = 0; i < high(this.u); i++)
                 this.clusters.add(null);
            this.summary = null;
        }
    }

    public int getMax() {
        return this.max;
    }

    public int getMin() {
        return this.min;
    }

    private int high(int x) {
        return (int) Math.floor(x / Math.sqrt(this.u));
    }


    private int index(int x, int y) {
        return (int) (x * Math.floor(Math.sqrt(this.u)) + y);
    }

    private int low(int x) {
        return (int) (x % Math.ceil(Math.sqrt(this.u)));
    }

    public boolean isMember(int x) {
        // base case, where x is the minimum or maximum element
        if (this.min != null && this.max != null) {
            return this.min.equals(x) || this.max.equals(x);
        }
        else if (this.u == 2)
            return false;
        else {
            VanEmdeBoas cluster = this.clusters.get(high(x));
            if (cluster != null)
                return cluster.isMember(low(x));
            return false;
        }
    }

    // Function to insert x when vEB tree is empty
    private void emptyVEBInsert(int x) {
        this.min = this.max = x;
    }

    public void insert(int x) throws IllegalArgumentException {
        if (this.min == null) {
            emptyVEBInsert(x);
        }
        else {
            // here we get a new minimum element.
            // Since we don't want to lose the original min,
            // however, and so we need to insert it into one of V ’s clusters.
            // Therefore we swap x with min, so that we insert
            // original min into one of the clusters.

            if (x < this.min)
                // swapping x and this.min
                x = x ^ this.min ^ (this.min = x);

            if (this.u > 2) {
                int clusterId = high(x);
                int highU = high(this.u);
                int high = high(x);
                int low = low(x);

                // checks whether the cluster that x will go
                // into is empty.
                if (this.clusters.get(clusterId) == null)
                    this.clusters.set(clusterId, new VanEmdeBoas(highU));

                // checking whether summary is empty
                if (this.summary == null)
                    this.summary = new VanEmdeBoas(highU);

                // inserts x’s cluster number into the summary
                if (this.clusters.get(clusterId).min == null) {
                    this.summary.insert(high);
                    this.clusters.get(clusterId).emptyVEBInsert(low);
                }
                else
                    // If x’s cluster is not currently empty, then
                    // inserts x into its cluster
                    this.clusters.get(clusterId).insert(low);
            }

            // updating max
            if (x > this.max)
                this.max = x;
        }

    }

    public void delete(int x) {
        // base case to check if there is only one
        // element present in the tree.
        if (this.min.equals(this.max))
            this.min = this.max = null;

        else if (this.u == 2) {
            if (x == 0)
                this.min = 1;
            else
                this.min = 0;
            this.max = this.min;
        }

        else {
            // In this case, we will have to delete an element from a cluster.
            // The element we delete from a cluster might not be x,
            // however, because if x equals min, then once we have
            // deleted x, some other element within one of V ’s clusters
            // becomes the new min, and we have to delete that other element
            // from its cluster.
            int high = high(x);
            int low = low(x);

            if (x == this.min) {
                int first_cluster = this.summary.min;
                x = index(first_cluster,
                        this.clusters.get(first_cluster).min);
                this.min = x;
            }

            // we need to delete element x from its cluster,
            // whether x was the value originally passed to delete() or
            // x is the element becoming the new minimum
            this.clusters.get(high(x)).delete(low(x));

            if (this.clusters.get(high(x)).min == null) {
                // cluster is empty and now we delete x from its summary.
                this.summary.delete(high(x));

                // updating max
                // checking if we are deleting maximum element
                if (x == this.max) {
                    Integer summaryMax = this.summary.max;

                    // if all clusters are empty, then only minimum value
                    // remain that value is been assigned to max
                    if (summaryMax == null)
                        this.max = this.min;
                    else
                        // sets max to the maximum element in the
                        // highest-numbered cluster
                        this.max = index(summaryMax,
                                        this.clusters.get(summaryMax).min);
                }
            }

            // case in which x’s cluster did not become empty
            // Due to x being deleted. Although we do not
            // have to update the summary in this case,
            // we might have to update max
            else if (x == this.max)
                this.max = index(high(x), this.clusters.get(high(x)).max);
        }
    }

    // Function to return the next element in tree after x
    public Integer successor(int x) {
        if (this.u == 2) {
            // base case, where we are trying to find successor of 0
            // and 1 is present in the vEB tree.
            if (x == 0 && this.max == 1)
                return 1;
            return null;
        }

        // if x is strictly less than min, return the min element
        else if (this.min != null && x < this.min)
            return this.min;

        else {
            int high = high(x);
            int low = low(x);
            Integer maxLow = null;

            // max_low = maximum element in x's cluster
            if (this.clusters.get(high) != null)
                maxLow = this.clusters.get(high).max;

            // If x’s cluster contains some element
            // that is greater than x, then we know
            // that x’s successor lies somewhere within x’s cluster
            if (maxLow != null && low < maxLow) {

                int offset = this.clusters.get(high).successor(low);
                return index(high, offset);
            }
            else {
                // if x is greater than or equal to the
                // greatest element in its cluster.
                Integer succCluster = null;
                if (this.summary != null)
                    succCluster = this.summary.successor(high);
                if (succCluster == null)
                    return null;
                else {
                    int offset = this.clusters.get(succCluster).min;
                    return index(succCluster, offset);
                }
            }
        }
    }

    // Function to return the previous element in the tree after x
    public Integer predecessor(int x) {
        if (this.u == 2) {
            // base case where we are finding predecessor
            // of 1 and 0 is the minimum element
            if (x == 1 && this.min == 0)
                return 0;
            return null;
        }

        // if x is strictly greater than the maximum element
        else if (this.max != null && x > this.max)
            return this.max;

        else {
            int high = high(x);
            int low = low(x);
            Integer minLow = null;

            if (this.clusters.get(high) != null)
                minLow = this.clusters.get(high).min;

            if (minLow != null && low > minLow) {
                int offset = this.clusters.get(high).predecessor(low);
                return index(high, offset);
            }
            else {
                Integer predCluster = null;
                if (this.summary != null)
                    predCluster = this.summary.predecessor(high);

                // when x’s predecessor, if it exists,
                // does not reside in x’s cluster
                if (predCluster == null) {

                    // x’s predecessor is the minimum value in vEB
                    // tree V , then the successor resides in
                    // no cluster at all.
                    if (this.min != null && x > this.min)
                        return this.min;
                }
                else {
                    int offset = this.clusters.get(predCluster).max;
                    return index(predCluster, offset);
                }
            }
        }
        return null;
    }
}

class App {
    public static void main(String[] args) throws IllegalArgumentException {
        VanEmdeBoas veb = new VanEmdeBoas(1000);

        veb.insert(3);
        veb.insert(5);
        veb.insert(10);

        assert !veb.isMember(4);
        assert veb.isMember(3);

        veb.delete(3);

        assert !veb.isMember(3);

        assert veb.successor(5) == 10;
        assert veb.predecessor(10) == 5;
    }
}

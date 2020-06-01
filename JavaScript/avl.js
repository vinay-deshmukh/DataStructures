/** A class to denote a node of the AVL Tree.
 */
class AVLNode {

    /** Creates a single AVL Node with the given value.
     * @param value The value of the node.
     */
    constructor(value) {
        this.value = value;
        this.left = null;
        this.right = null;
        this.height = 1;
    }
}

/** A class to denote the AVL Tree.
 */
class AVL {

    /** Intitializes a new AVL Tree with root as null
     */
    constructor() {
        this.root = null;
    }

    static isLeaf(node) {
        return node.left == null && node.right == null;
    }

    getHeight(node) {
        if(node != null)
            return node.height;
        return 0;
    }

    getBalance(node) {
        if (node != null)
            return this.getHeight(node.left) - this.getHeight(node.right);
        return 0;
    }

    /**
     * Left Rotate Utility function
     *
     * node                                       rchild
     *   \             After left rotate          /    \
     *   rchild      ====================>       node   X
     *   /   \                                    \
     * temp   X                                    temp
     *
     * @param node
     * @return node that comes on the top after left rotation.
     */
    lrotate(node) {
        const rchild = node.right;
        const temp = rchild.left;

        rchild.left = node;
        node.right = temp;

        node.height = 1 + Math.max(this.getHeight(node.left), this.getHeight(node.right));
        rchild.height = 1 + Math.max(this.getHeight(rchild.left), this.getHeight(rchild.right));

        return rchild;
    }

    /**
     * Right Rotate Utility Function
     *
     *      node                                lchild
     *       /          After right rotate        /   \
     *    lchild      ====================>      X   node
     *      /  \                                       /
     *     X    temp                                  temp
     *
     * @param node
     * @return node that comes on the top after right rotation.
     */
    rrotate(node) {
        const lchild = node.left;
        const temp = lchild.right;

        lchild.right = node;
        node.left = temp;

        node.height = 1 + Math.max(this.getHeight(node.left), this.getHeight(node.right));
        lchild.height = 1 + Math.max(this.getHeight(lchild.left), this.getHeight(lchild.right));

        return lchild;
    }

    /**
     * Balances the node if it is unbalanced.
     * Node is balanced only when the balance factors are -1, 0 or 1.
     *
     * @param node Node which is to be balanced.
     * @param value
     * @return AVLNode
     */
    updateBalance(node, value) {
        const balance = this.getBalance(node);

        if(balance > 1 && (value < node.left.value || this.getBalance(node.left) >= 0))
        // Case 1: Right Rotate
            return this.rrotate(node);

        if(balance < -1 && (value > node.right.value || this.getBalance(node.right) <= 0))
        // Case 2: Left Rotate
            return this.lrotate(node);

        if(balance > 1 && (value > node.left.value || this.getBalance(node.left) < 0)) {
            // Case 3: Left-Right Rotation
            node.left = this.lrotate(node.left);
            return this.rrotate(node);
        }

        if(balance < -1 && (value < node.right.value || this.getBalance(node.right) > 0)) {
            // Case 4: Right-Left Rotation
            node.right = this.rrotate(node.right);
            return this.lrotate(node);
        }

        return node;
    }

    /**
     * Inserts the given value into the AVL Tree and updates the root node.
     *
     * @param value Value to be inserted.
     */
    insert(value) {
        this.root = this.insertHelper(this.root, value);
    }

    insertHelper(node, value) {
        if(node == null)
            return new AVLNode(value);
        else if(value < node.value)
            node.left = this.insertHelper(node.left, value);
        else
            node.right = this.insertHelper(node.right, value);

        node.height = 1 + Math.max(this.getHeight(node.left), this.getHeight(node.right));

        return this.updateBalance(node, value);
    }

    /**
     * Searches whether the value is present in the AVl Tree or not.
     *
     * @param value Value to be searched.
     * @return
     */
    search(value) {
        if(this.root == null)
            return null;
        return this.searchHelper(this.root, value);
    }

    searchHelper(node, value) {
        // Best case: value is present or if node does not exist

        if(node == null || node.value === value)
            return node;

        // Since AVL Tree follows property of BST, if value is lesser than current node's value,
        // it will be present in left sub tree.
        else if(value < node.value)
            return this.searchHelper(node.left, value);

        // If value is greater than current node's value then it will be present in right sub tree.
        else
            return this.searchHelper(node.right, value);
    }

    /**
     * Deletes the given value from the AVL Tree if present.
     *
     * @param value Value to be deleted.
     */
    delete(value) {
        this.root = this.deleteHelper(this.root, value);
    }

    deleteHelper(node, value) {

        if(node == null)
            return node;

        else if(value < node.value)
            node.left = this.deleteHelper(node.left, value);

        else if(value > node.value)
            node.right = this.deleteHelper(node.right, value);

        else {
            // Node has right child present or it does not have both childs
            if (node.left == null) {
                let temp = node.right;
                node = null;
                return temp;
            }

            // Node has left child present
            else if (node.right == null) {
                let temp = node.left;
                node = null;
                return temp;
            }

            // Node has both child present
            // Get the in-order successor element in the right subtree, and replace
            // it by the node to be deleted and delete the in-order successor.
            let temp = this.getInorderSuccessor(node.right);
            node.value = temp.value;
            node.right = this.deleteHelper(node.right, temp.value);
        }

        // Update the heights of the node after deletion
        node.height = 1 + Math.max(this.getHeight(node.left), this.getHeight(node.right));

        // Rebalance the node
        return this.updateBalance(node, node.value);
    }

    /**
     * Gets the In-Order successor of the given node
     * (node with the minimum value in the left subtree.)
     *
     * @param node Node whose left subtree is to be searched.
     * @return in-order successor.
     */
    getInorderSuccessor(node) {
        if(node == null || node.left == null)
            return node;
        return this.getInorderSuccessor(node.left);
    }

    /**
     * Prints the tree using In-Order traversal.
     * @return
     */
    inOrder() {
        let inOrderList = [];
        this.inOrderHelper(this.root, inOrderList);
        return inOrderList;
    }

    inOrderHelper(node, inOrderList) {
        if(node != null){
            this.inOrderHelper(node.left, inOrderList);
            inOrderList.push(node.value);
            this.inOrderHelper(node.right, inOrderList);
        }
    }

    /**
     * Prints the tree using Post-Order traversal.
     * @return
     */
    postOrder() {
        let postOrderList = [];
        this.postOrderHelper(this.root, postOrderList);
        return postOrderList;
    }

    postOrderHelper(node, postOrderList) {
        if(node != null){
            this.postOrderHelper(node.left, postOrderList);
            this.postOrderHelper(node.right, postOrderList);
            postOrderList.push(node.value);
        }
    }

    /**
     * Prints the tree using Pre-Order traversal.
     * @return
     */
    preOrder() {
        let preOrderList = [];
        this.preOrderHelper(this.root, preOrderList);
        return preOrderList;
    }


    preOrderHelper(node, preOrderList) {
        if(node != null){
            preOrderList.push(node.value);
            this.preOrderHelper(node.left, preOrderList);
            this.preOrderHelper(node.right, preOrderList);
        }
    }
}


// Sample test cases to test the AVL Tree Implementation.
const avl = new AVL();
let li = [0, 1, 2, 3, 4, 5, 6, 7, 8];
li.forEach(function (item) {
    avl.insert(item);
});

const node1 = avl.search(2);
if(node1 != null)
    console.log("Node Found")
else
    console.log("Node not Found")

const node2 = avl.search(10);
if(node2 != null)
    console.log("Node Found")
else
    console.log("Node not Found")

console.log(avl.inOrder());
console.log(avl.postOrder());
console.log(avl.preOrder());

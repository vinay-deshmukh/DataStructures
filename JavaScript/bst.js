// A class to denote a node of the BST Tree.
class BSTNode {
    constructor(value) {
        this.value = value;
        this.left = null;
        this.right = null;
        this.height = 1;
    }
}

// A class to denote the BST Tree.
class BST {

    constructor() {
        this.root = null;
    }

    static isLeaf(node) {
        return node.left == null && node.right == null;
    }

    height() {
        if (this.root != null)
            return this._getHeight(this.root);
        return 0;
    }

    _getHeight(node) {
        if(node != null)
            return node.height;
        return 0;
    }

    insert(value) {
        this.root = this.insertHelper(this.root, value);
    }

    insertHelper(node, value) {
        if(node == null)
            return new BSTNode(value);
        else if(value < node.value)
            node.left = this.insertHelper(node.left, value);
        else if(value > node.value)
            node.right = this.insertHelper(node.right, value);
        else
            return node; // node is already present

        node.height = 1 + Math.max(this._getHeight(node.left), this._getHeight(node.right));

        return node;
    }

    search(value) {
        if(this.root == null)
            return null;
        return this.searchHelper(this.root, value);
    }

    searchHelper(node, value) {
        // Best case: value is present or if node does not exist

        if(node == null || node.value === value)
            return node;

        // Since BST Tree follows property of BST, if value is lesser than current node's value,
        // it will be present in left sub tree.
        else if(value < node.value)
            return this.searchHelper(node.left, value);

        // If value is greater than current node's value then it will be present in right sub tree.
        else
            return this.searchHelper(node.right, value);
    }

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
        node.height = 1 + Math.max(this._getHeight(node.left), this._getHeight(node.right));

        return node;
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

    levelOrder(nodes = [], li = []) {
        nodes = [this.root];
        li.push(this.root.value);
        for (const node of nodes) {
            if (node.left != null) {
                li.push(node.left.value);
                nodes.push(node.left);
            }

            if (node.right != null) {
                li.push(node.right.value);
                nodes.push(node.right);
            }
        }
        return li;
    }
}


// Sample test cases to test the BST Tree Implementation.
const main = function () {
    const bst = new BST();
    let li = [9, 3, 15, 4, 2, 11, 17];
    li.forEach(function (item) {
        bst.insert(item);
    });

    console.log(bst.inOrder());
    console.log(bst.preOrder());
    console.log(bst.postOrder());
    console.log(bst.levelOrder());

    const node1 = bst.search(2);
    if(node1 != null)
        console.log("Node Found");
    else
        console.log("Node not Found");

    const node2 = bst.search(10);
    if(node2 != null)
        console.log("Node Found");
    else
        console.log("Node not Found");

    bst.delete(9); // deleting node with value 9/

    console.log(bst.inOrder());
    console.log(bst.height());
};

main();

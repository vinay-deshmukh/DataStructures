import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/** A class to denote a node of the AVL Tree.
 */
class AVLNode {
    /** AVL Node hold's this value.
     */
    public int value;

    /** Pointer to left child of the AVL Node.
     */
    public AVLNode left;

    /** Pointer to right child of the AVL Node.
     */
    public AVLNode right;

    /** Height of the AVL Node.
     */
    public int height;

    /** Creates a single AVL Node with the given value.
     * @param value The value of the node.
     */
    AVLNode(int value) {
        this.value = value;
        this.left = null;
        this.right = null;
        this.height = 1;
    }

    @Override
    public String toString() {
        return "AVLNode{" +
                "value=" + value +
                ", height=" + height +
                '}';
    }
}

/** A class to denote the AVL Tree.
 */
public class AVL {
    public AVLNode root;

    /** Intitializes a new AVL Tree with root as null
     */
    public AVL() {
        this.root = null;
    }

    public boolean isLeaf(AVLNode node) {
        return node.left == null && node.right == null;
    }

    public int getHeight(AVLNode node) {
        if(node != null)
            return node.height;
        return 0;
    }

    public int getBalance(AVLNode node) {
        if(node != null)
            return getHeight(node.left) - getHeight(node.right);
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
    private AVLNode lrotate(AVLNode node) {
        AVLNode rchild = node.right;
        AVLNode temp = rchild.left;

        rchild.left = node;
        node.right = temp;

        node.height = 1 + Math.max(getHeight(node.left), getHeight(node.right));
        rchild.height = 1 + Math.max(getHeight(rchild.left), getHeight(rchild.right));

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
    private AVLNode rrotate(AVLNode node) {
        AVLNode lchild = node.left;
        AVLNode temp = lchild.right;

        lchild.right = node;
        node.left = temp;

        node.height = 1 + Math.max(getHeight(node.left), getHeight(node.right));
        lchild.height = 1 + Math.max(getHeight(lchild.left), getHeight(lchild.right));

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
    private AVLNode updateBalance(AVLNode node, int value) {
        int balance = getBalance(node);

        if(balance > 1 && (value < node.left.value || getBalance(node.left) >= 0))
            // Case 1: Right Rotate
            return rrotate(node);

        if(balance < -1 && (value > node.right.value || getBalance(node.right) <= 0))
            // Case 2: Left Rotate
            return lrotate(node);

        if(balance > 1 && (value > node.left.value || getBalance(node.left) < 0)) {
            // Case 3: Left-Right Rotation
            node.left = lrotate(node.left);
            return rrotate(node);
        }

        if(balance < -1 && (value < node.right.value || getBalance(node.right) > 0)) {
            // Case 4: Right-Left Rotation
            node.right = rrotate(node.right);
            return lrotate(node);
        }

        return node;
    }

    /**
     * Inserts the given value into the AVL Tree and updates the root node.
     *
     * @param value Value to be inserted.
     */
    public void insert(int value) {
        this.root = insert(this.root, value);
    }

    private AVLNode insert(AVLNode node, int value) {
        if(node == null)
            return new AVLNode(value);
        else if(value < node.value)
            node.left = insert(node.left, value);
        else
            node.right = insert(node.right, value);

        node.height = 1 + Math.max(getHeight(node.left), getHeight(node.right));

        return updateBalance(node, value);
    }

    /**
     * Searches whether the value is present in the AVl Tree or not.
     *
     * @param value Value to be searched.
     * @return
     */
    public AVLNode search(int value) {
        if(this.root == null)
            return null;
        return search(this.root, value);
    }

    private AVLNode search(AVLNode node, int value) {
        // Best case: value is present or if node does not exist
        if(node == null || node.value == value)
            return node;

        // Since AVL Tree follows property of BST, if value is lesser than current node's value,
        // it will be present in left sub tree.
        else if(value < node.value)
            return search(node.left, value);

        // If value is greater than current node's value then it will be present in right sub tree.
        else
            return search(node.right, value);
    }

    /**
     * Deletes the given value from the AVL Tree if present.
     *
     * @param value Value to be deleted.
     */
    public void delete(int value) {
        this.root = delete(this.root, value);
    }

    private AVLNode delete(AVLNode node, int value) {
        if(node == null)
            return node;

        else if(value < node.value)
            node.left = delete(node.left, value);

        else if(value > node.value)
            node.right = delete(node.right, value);

        else {
            // Node has right child present or it does not have both childs
            if (node.left == null) {
                AVLNode temp = node.right;
                node = null;
                return temp;
            }

            // Node has left child present
            else if (node.right == null) {
                AVLNode temp = node.left;
                node = null;
                return temp;
            }

            // Node has both child present
            // Get the in-order successor element in the right subtree, and replace
            // it by the node to be deleted and delete the in-order successor.
            AVLNode temp = getInorderSuccessor(node.right);
            node.value = temp.value;
            node.right = delete(node.right, temp.value);
        }

        // Update the heights of the node after deletion
        node.height = 1 + Math.max(getHeight(node.left), getHeight(node.right));

        // Rebalance the node
        return updateBalance(node, node.value);
    }

    /**
     * Gets the In-Order successor of the given node
     * (node with the minimum value in the left subtree.)
     *
     * @param node Node whose left subtree is to be searched.
     * @return in-order successor.
     */
    private AVLNode getInorderSuccessor(AVLNode node) {
        if(node == null || node.left == null)
            return node;
        return getInorderSuccessor(node.left);
    }

    /**
     * Prints the tree using In-Order traversal.
     * @return
     */
    public List<Integer> inOrder() {
        List<Integer> inOrderList = new ArrayList<>();
        inOrder(this.root, inOrderList);
        return inOrderList;
    }

    public void inOrder(AVLNode node, List<Integer> inOrderList) {
        if(node != null){
            inOrder(node.left, inOrderList);
            inOrderList.add(node.value);
            inOrder(node.right, inOrderList);
        }
    }

    /**
     * Prints the tree using Pre-Order traversal.
     * @return
     */
    public List<Integer> preOrder() {
        List<Integer> postOrderList = new ArrayList<>();
        preOrder(this.root, postOrderList);
        return postOrderList;
    }

    public void preOrder(AVLNode node, List<Integer> postOrderList) {
        if(node != null){
            postOrderList.add(node.value);
            preOrder(node.left, postOrderList);
            preOrder(node.right, postOrderList);
        }
    }

    /**
     * Prints the tree using Post-Order traversal.
     * @return
     */
    public List<Integer> postOrder() {
        List<Integer> preOrderList = new ArrayList<>();
        postOrder(this.root, preOrderList);
        return preOrderList;
    }

    public void postOrder(AVLNode node, List<Integer> postOrderList) {
        if(node != null){
            postOrder(node.left, postOrderList);
            postOrder(node.right, postOrderList);
            postOrderList.add(node.value);
        }
    }
}

/**
 * Utility class to pretty print the AVL Tree.
 *
 * https://stackoverflow.com/questions/4965335/how-to-print-binary-tree-diagram/29704252
 *
 */
class TreePrinter {

//    public static <T extends Comparable<?>> void printNode(AVLNode<T> root) {
    public static void printNode(AVLNode root) {
        int maxLevel = TreePrinter.maxLevel(root);

        printNodeInternal(Collections.singletonList(root), 1, maxLevel);
    }

    private static void printNodeInternal(List<AVLNode> nodes, int level, int maxLevel) {
        if (nodes.isEmpty() || TreePrinter.isAllElementsNull(nodes))
            return;

        int floor = maxLevel - level;
        int edgeLines = (int) Math.pow(2, (Math.max(floor - 1, 0)));
        int firstSpaces = (int) Math.pow(2, (floor)) - 1;
        int betweenSpaces = (int) Math.pow(2, (floor + 1)) - 1;

        TreePrinter.printWhitespaces(firstSpaces);

        List<AVLNode> newNodes = new ArrayList<AVLNode>();
        for (AVLNode node : nodes) {
            if (node != null) {
                System.out.print(node.value);
                newNodes.add(node.left);
                newNodes.add(node.right);
            } else {
                newNodes.add(null);
                newNodes.add(null);
                System.out.print(" ");
            }

            TreePrinter.printWhitespaces(betweenSpaces);
        }
        System.out.println("");

        for (int i = 1; i <= edgeLines; i++) {
            for (AVLNode node : nodes) {
                TreePrinter.printWhitespaces(firstSpaces - i);
                if (node == null) {
                    TreePrinter.printWhitespaces(edgeLines + edgeLines + i + 1);
                    continue;
                }

                if (node.left != null)
                    System.out.print("/");
                else
                    TreePrinter.printWhitespaces(1);

                TreePrinter.printWhitespaces(i + i - 1);

                if (node.right != null)
                    System.out.print("\\");
                else
                    TreePrinter.printWhitespaces(1);

                TreePrinter.printWhitespaces(edgeLines + edgeLines - i);
            }

            System.out.println("");
        }

        printNodeInternal(newNodes, level + 1, maxLevel);
    }

    private static void printWhitespaces(int count) {
        for (int i = 0; i < count; i++)
            System.out.print(" ");
    }

    private static int maxLevel(AVLNode node) {
        if (node == null)
            return 0;

        return Math.max(TreePrinter.maxLevel(node.left), TreePrinter.maxLevel(node.right)) + 1;
    }

    private static boolean isAllElementsNull(List list) {
        for (Object object : list) {
            if (object != null)
                return false;
        }
        return true;
    }

}

// Sample Test case to test the AVL Tree Implementation.
class App {
    public static void main(String[] args) {
        AVL avlTree = new AVL();
        ArrayList<Integer> arrayList = new ArrayList<>(List.of(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

        for(int x: arrayList)
            avlTree.insert(x);

        System.out.println(avlTree.inOrder());

        AVLNode node1 = avlTree.search(0);  // True
        if(node1 != null)
            System.out.println("Node found");
        else
            System.out.println("Node not found");

        AVLNode node2 = avlTree.search(10); // False
        if(node2 != null)
            System.out.println("Node found");
        else
            System.out.println("Node not found");

        avlTree.delete(3);

        TreePrinter.printNode(avlTree.root);
    }

}

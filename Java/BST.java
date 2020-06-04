import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.LinkedList; 
import java.util.Queue; 


/** A class to denote a node of the BST.
 */
class BSTNode {
    public int value;
    public BSTNode left;
    public BSTNode right;
    public int height;

    BSTNode(int value) {
        this.value = value;
        this.left = null;
        this.right = null;
        this.height = 1;
    }

    @Override
    public String toString() {
        return "BSTNode{" +
                "value=" + value +
                ", height=" + height +
                '}';
    }
}


public class BST {
    public BSTNode root;

    public BST() {
        this.root = null;
    }

    public int height() {
        if (this.root != null)
            return getHeight(this.root);
        return 0;
    }

    private int getHeight(BSTNode node) {
        if (node != null)
            return node.height;
        return 0;
    }

    public void insert(int value) {
        this.root = insert(this.root, value);
    }

    private BSTNode insert(BSTNode node, int value) {
        if(node == null)
            return new BSTNode(value);
        else if(value < node.value)
            node.left = insert(node.left, value);
        else if(value > node.value)
            node.right = insert(node.right, value);
        else
            return node; // node is already present.

        node.height = 1 + Math.max(getHeight(node.left), getHeight(node.right));

        return node;
    }

    public BSTNode search(int value) {
        if(this.root == null)
            return null;
        return search(this.root, value);
    }

    private BSTNode search(BSTNode node, int value) {
        // Best case: value is present or if node does not exist
        if(node == null || node.value == value)
            return node;

            // If value is lesser than current node's value,
            // it will be present in left sub tree.
        else if(value < node.value)
            return search(node.left, value);

        // If value is greater than current node's value,
        //then it will be present in right sub tree.
        else
            return search(node.right, value);
    } 

    public void delete(int value) {
        this.root = delete(this.root, value);
    }

    private BSTNode delete(BSTNode node, int value) {
        if(node == null)
            return node;

        else if(value < node.value)
            node.left = delete(node.left, value);

        else if(value > node.value)
            node.right = delete(node.right, value);

        else {
            // Node has right child present or it does not have both childs
            if (node.left == null) {
                BSTNode temp = node.right;
                node = null;
                return temp;
            }

            // Node has left child present
            else if (node.right == null) {
                BSTNode temp = node.left;
                node = null;
                return temp;
            }

            // Node has both child present
            // Get the in-order successor element in the right subtree, and replace
            // it by the node to be deleted and delete the in-order successor.
            BSTNode temp = getInorderSuccessor(node.right);
            node.value = temp.value;
            node.right = delete(node.right, temp.value);
        }

        // Update the heights of the node after deletion
        node.height = 1 + Math.max(getHeight(node.left), getHeight(node.right));

        return node;
    }

    /**
     * Gets the In-Order successor of the given node
     * (node with the minimum value in the left subtree.)
     *
     * @param node Node whose left subtree is to be searched.
     * @return in-order successor.
     */
    private BSTNode getInorderSuccessor(BSTNode node) {
        if(node == null || node.left == null)
            return node;
        return getInorderSuccessor(node.left);
    }

    /**
     * Prints the tree using In-Order traversal.
     * @return list This list contains all nodes in In-Order.
     */
    public List<Integer> inOrder() {
        List<Integer> inOrderList = new ArrayList<>();
        inOrder(this.root, inOrderList);
        return inOrderList;
    }

    public void inOrder(BSTNode node, List<Integer> inOrderList) {
        if(node != null){
            inOrder(node.left, inOrderList);
            inOrderList.add(node.value);
            inOrder(node.right, inOrderList);
        }
    }

    public List<Integer> preOrder() {
        List<Integer> postOrderList = new ArrayList<>();
        preOrder(this.root, postOrderList);
        return postOrderList;
    }

    public void preOrder(BSTNode node, List<Integer> postOrderList) {
        if(node != null){
            postOrderList.add(node.value);
            preOrder(node.left, postOrderList);
            preOrder(node.right, postOrderList);
        }
    }

    public List<Integer> postOrder() {
        List<Integer> preOrderList = new ArrayList<>();
        postOrder(this.root, preOrderList);
        return preOrderList;
    }

    public void postOrder(BSTNode node, List<Integer> postOrderList) {
        if(node != null){
            postOrder(node.left, postOrderList);
            postOrder(node.right, postOrderList);
            postOrderList.add(node.value);
        }
    }

    public List<Integer> levelOrder() {
        List<Integer> list = new ArrayList<>();
        if (this.root == null)
            return list;

        Queue<BSTNode> queue = new LinkedList<>();
        queue.add(this.root);

        while (!queue.isEmpty()) {
            BSTNode node = queue.poll();
            list.add(node.value);
            if (node.left != null)
                queue.add(node.left);
            if (node.right != null)
                queue.add(node.right);
        }
        return list;
    }
}

/**
 * Utility class to pretty print the BST.
 *
 * https://stackoverflow.com/questions/4965335/how-to-print-binary-tree-diagram/29704252
 *
 */
class TreePrinter {

    public static void printNode(BSTNode root) {
        int maxLevel = TreePrinter.maxLevel(root);

        printNodeInternal(Collections.singletonList(root), 1, maxLevel);
    }

   private static void printNodeInternal(List<BSTNode> nodes, int level, int maxLevel) {
        if (nodes.isEmpty() || TreePrinter.isAllElementsNull(nodes))
            return;

        int floor = maxLevel - level;
        int edgeLines = (int) Math.pow(2, (Math.max(floor - 1, 0)));
        int firstSpaces = (int) Math.pow(2, (floor)) - 1;
        int betweenSpaces = (int) Math.pow(2, (floor + 1)) - 1;

        TreePrinter.printWhitespaces(firstSpaces);

        List<BSTNode> newNodes = new ArrayList<BSTNode>();
        for (BSTNode node : nodes) {
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
            for (BSTNode node : nodes) {
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

    private static int maxLevel(BSTNode node) {
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

class App {
    public static void main(String[] args) {
        BST bst = new BST();
        ArrayList<Integer> arrayList = new ArrayList<>(Arrays.asList(9, 3, 15, 4, 2, 11, 17));

        for(int x: arrayList)
           bst.insert(x);

        System.out.println(bst.inOrder());
        System.out.println(bst.preOrder());
        System.out.println(bst.postOrder());
        System.out.println(bst.levelOrder());

        BSTNode node1 = bst.search(2);
        BSTNode node2 = bst.search(10);

        assert node1.value == 2 : true;
        assert node2 == null : true;

        TreePrinter.printNode(bst.root);

        bst.delete(3);

        TreePrinter.printNode(bst.root);
    }
}

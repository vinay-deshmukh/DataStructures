# A class to represent a node in a BST.
class BSTNode:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None
        self.height = 1

    def __repr__(self):
        return f'BSTNode(value={self.value}, height={self.height})'

class BST:
    def __init__(self, args=None):
        '''
        Args:
            args: optional parameter to pass a list of values that will directly be added in
                  in the BST.
        '''
        self.root = None
        if args:
            for i in args:
                self.insert(i)

    def height(self):
        if self.root is not None:
            return self._get_height(self.root)
        return 0

    def _get_height(self, node):
        '''Returns the height of the given node'''
        if node:
            return node.height
        return 0

    def insert(self, value):
        '''Function to insert a value in BST.'''
        self.root = self._insert(self.root, value)

    def _insert(self, node, value):
        '''Recursive function that inserts a value in an BST.'''
        if not node:
            return BSTNode(value)
        elif value < node.value:
            node.left = self._insert(node.left, value)
        elif value > node.value:
            node.right = self._insert(node.right, value)
        else:
            return node # node is already present.

        node.height = max(self._get_height(node.left), self._get_height(node.right)) + 1

        return node

    def search(self, value):
        '''Function to search whether the node is present in the BST or not.'''
        if self.root is None:
            return False
        node = self._search(self.root, value)
        if node:
            return node
        else:
            return False

    def _search(self, node, value):
        '''Recursive function to search the value in the BST.'''

        # Best case: value is present or if node does not exist
        if node is None or node.value == value:
            return node

        # Since AVL Tree follows property of BST, if value is lesser than current node's value,
        # it will be present in left sub tree.
        elif value < node.value:
            return self._search(node.left, value)

        # If value is greater than current node's value then it will be present in right sub tree.
        else:
            return self._search(node.right, value)

    def delete(self, value):
        '''Function to delete a value from the BST.'''
        self.root = self._delete(self.root, value)

    def _delete(self, node, value):
        '''Recursive function to delete a node from the BST.'''
        if node is None:
            return node
        elif value < node.value:
            node.left = self._delete(node.left, value)
        elif value > node.value:
            node.right = self._delete(node.right, value)
        else:
            # Node has right child present or it does not have both childs
            if node.left is None:
                temp = node.right
                node = None
                return temp

            # Node has left child present.
            elif node.right is None:
                temp = node.left
                node = None
                return temp

            # Node has both child present
            # Get the in-order successor element in the right subtree, and replace
            # it by the node to be deleted and delete the in-order successor.
            temp = self._get_inorder_successor(node.right)
            node.value = temp.value
            node.right = self._delete(node.right, temp.value)

        # Update the heights of the node after deletion
        node.height = 1 + max(self._get_height(node.left), self._get_height(node.right))

        # Rebalance the node
        return node

    def _get_inorder_successor(self, node):
        '''Returns the minimum node in the tree from a given node.'''
        if node is None or node.left is None:
            return node
        return self._get_inorder_successor(node.left)

    def in_order(self):
        in_order_list = []
        self._in_order(self.root, in_order_list)
        return in_order_list

    def _in_order(self, node, in_order_list):
        '''Recursive In Order traversal of the tree'''
        if node is not None:
            self._in_order(node.left, in_order_list)
            in_order_list.append(node.value)
            self._in_order(node.right, in_order_list)

    def post_order(self):
        post_order_list = []
        self._post_order(self.root, post_order_list)
        return post_order_list

    def _post_order(self, node, post_order_list):
        '''Recursive Post Order traversal of the tree'''
        if node is not None:
            self._post_order(node.left, post_order_list)
            self._post_order(node.right, post_order_list)
            post_order_list.append(node.value)

    def pre_order(self):
        pre_order_list = []
        self._pre_order(self.root, pre_order_list)
        return pre_order_list

    def _pre_order(self, node, pre_order_list):
        '''Recursive Pre Order traversal of the tree'''
        if node is not None:
            pre_order_list.append(node.value)
            self._pre_order(node.left, pre_order_list)
            self._pre_order(node.right, pre_order_list)

    def level_order(self, li = []):
        if self.root is None:
            return li

        queue = []
        queue.append(self.root)
        while len(queue) > 0:
            li.append(queue[0].value)
            node = queue.pop(0)

            if node.left is not None:
                queue.append(node.left)

            if node.right is not None:
                queue.append(node.right)

        return li

    def __repr__(self):
        '''Function to pretty print the tree.
        
        src: https://github.com/pgrafov/python-avl-tree/blob/master/pyavltree.py#L448
        '''
        start_node = self.root
        sym, spaces_count = "-", 80
        op = ""
        init_str  = sym * spaces_count + "\n" 

        if not start_node:
            return "BST is empty"

        else:
            level = [start_node]
            while (len([i for i in level if i is not None]) > 0):
                level_string = init_str

                for i in range(len(level)):
                    j = (i+1)* spaces_count // (len(level)+1)
                    level_string = level_string[:j] + (str(level[i].value) if level[i] else sym) + level_string[j+1:]
                
                level_next = []

                for i in level:
                    level_next += ([i.left, i.right] if i else [None, None])
                
                level = level_next
                op += level_string

        return op


def main():
    b = [9, 3, 15, 4, 2, 11, 17]
    bst = BST(b)

    print(bst)
    
    assert bst.search(4).value == 4
    assert bst.search(10) == False

    bst.delete(9) # deleting the node 9 (in this case the root node)
    print(bst)

    bst.insert(8)
    print(bst)

    print(bst.in_order())
    print(bst.post_order())
    print(bst.pre_order())
    print(bst.level_order())

    assert bst.height() == 4 # printing the height of the tree

if __name__ == '__main__':
    main()

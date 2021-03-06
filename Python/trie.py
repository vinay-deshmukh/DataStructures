class TrieNode:
    def __init__(self, value: str):
        self.children : Dict[str, TrieNode] = {}
        self.value = value
        self.is_end = False

class Trie:
    def __init__(self):
        self.root = TrieNode(None)

    def insert(self, s: str) -> None:
        '''Function to insert a string in a Trie Tree

        Args:
            s: String to insert.
        '''
        curr = self.root
        for char in s:
            if char not in curr.children:
                curr.children[char] = TrieNode(char)
            curr = curr.children[char]

        curr.is_end = True

    def search(self, s: str, allow_prefix: bool = True) -> bool:
        '''Function to search a given string exists inside a Trie Tree

        Args:
            s: String to search.
            allow_prefix: parameter to check word is prefix of words inserted.
        '''
        curr = self.root
        for char in s:
            if char not in curr.children:
                return False
            curr = curr.children[char]
        return allow_prefix or curr.is_end

def main():
    trie = Trie()

    '''Sample test cases to test the Trie Tree'''
    trie.insert('abc')
    trie.insert('adc')
    print(trie.search('abc', allow_prefix=False)) # True
    print(trie.search('ab', allow_prefix=True))   # True
    print(trie.search('cbv', allow_prefix=False)) # False
    print(trie.search('cv', allow_prefix=True))   # False


if __name__ == '__main__':
    main()

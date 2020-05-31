class TrieNode {
    char: string  // using a string since ts doesn't have a char datatype.
    children: { [char: string]: TrieNode; } = {}
    isSentinel: boolean = false
    constructor(char: string) {
        this.char = char
    }
}


class TrieTree {
    root: TrieNode = new TrieNode('');

    insert(word: string) {
        var curr = this.root;
        for (let i = 0, char; i < word.length, char = word.charAt(i); i++) {
            if (!(char in curr.children)) {
                curr.children[char] = new TrieNode(char);
            }
            curr = curr.children[char]
        }
        curr.isSentinel = true
    }

    check(word: string, allowPrefix:boolean=true): boolean {
        var curr = this.root;
        for (let i = 0, char; i < word.length, char = word.charAt(i); i++) {
            if (!(char in curr.children)) {
                return false
            }
            curr = curr.children[char]
        }
        return allowPrefix || curr.isSentinel
    }
}

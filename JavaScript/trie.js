class TrieNode{
    constructor(value){
        this.children = {};
        this.value = value;
        this.is_end = false;
    }
}

class Trie{
    constructor(){
        this.root = new TrieNode(null);
    }

    insert(s){
        // Function to insert a string in a Trie Tree
        var curr = this.root;
        for(var i = 0; i < s.length; i++){
            var char = s.charAt(i);
            if(!(char in curr.children)){
                curr.children[char] = new TrieNode(char);
            }
            curr = curr.children[char];
        }
        curr.is_end = true;
    }

    search(s, allow_prefix){
        // Function to search whether a given string exists inside a Trie Tree
        var curr = this.root;
        for(var i = 0; i < s.length; i++){
            var char = s.charAt(i);
            if(!(char in curr.children)){
                return false;
            }
            curr = curr.children[char];
        }
        return allow_prefix || curr.is_end;
    }
}

// Sample test cases to test the Trie Tree.
const trie = new Trie();
trie.insert("abc");
trie.insert("adc");
console.log(trie.search("abc", false)); // True
console.log(trie.search("ab", true));   // True
console.log(trie.search("cbv", false)); // False
console.log(trie.search("cv", true));   // False
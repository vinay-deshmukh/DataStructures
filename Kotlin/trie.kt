// Sample implementation of Trie Tree in Kotlin


/**
 * @property char: alphabet of the current node.
 * @property isSentinel: true if current node is 
 *                       end of any inserted word.
 * By default, the node will not have any children nodes.
 */
class TrieNode(val char: Char, var isSentinel: Boolean = false) {
    var children = HashMap<Char, TrieNode>()
}


/**
 * Trie Tree is also called as a Prefix Tree
 * Following implementation provides implementation of tries keyed by chars.
 * Tries are often used to represent a dictionary of words with same prefixes.
 */
class TrieTree() {
    // \u0000 is the smallest possible character possible using Char datatype.
    // It symbolizes a null character.
    var root = TrieNode('\u0000')
    
    /**
    * Inserting a word in the trie tree.
    * For all characters in the given string, it traverses the tree. 
    *        If the current character is already inserted 
    *        Else it adds a new node for the current char.
    */
    fun insert(word: String) {
        var curr: TrieNode = root
        for (char in word) {
            if (!curr.children.containsKey(char)) {
                curr.children[char] = TrieNode(char)
            }
            curr = curr.children[char]!!  // curr should not be null.
        }
        curr.isSentinel = true
    }
    
    /*
    * Checks if the provided word is already inserted in the tree.
    * if allowPrefix is set, 
    *     the word to be searched needn't be inserted in the past. 
    *     It will return true when 
    *         any of the previous words startswith given word. 
    */
    fun check(word: String, allowPrefix: Boolean = true): Boolean {
        var curr = root
        for (char in word) {
            if (!curr.children.containsKey(char)){
                return false
            }
            curr = curr.children[char]!!
        }
        return allowPrefix or curr.isSentinel
    }
}


fun main() {
    // Demonstrates how the TrieTree class can be used.
    val tt = TrieTree()
    tt.insert("card")
    tt.insert("cart")
     
    assert(tt.check("", allowPrefix=false) == false) {
        "empty string was not inserted but trie tree says it was."
    }
    assert(tt.check("car") == true) {
        "car prefix was not found in any words"
    }
    assert(tt.check("cardi") == false){
        "cardi was mysteriously found in the tree"
    }
}

import java.util.HashMap;
import java.util.TreeMap;

class TrieNode{
    public HashMap<Character, TrieNode> children;
    public Character value;
    public boolean is_end;

    TrieNode(Character value){
        this.children = new HashMap<>();
        this.value = value;
        this.is_end = false;
    }
}

public class Trie {
    public TrieNode root;

    public Trie(){
        this.root = new TrieNode(null);
    }

    public void insert(String s){
        /**
         * Function to insert a string into Trie tree.
         *
         * @param s String to insert
         * @return null
         */
        TrieNode curr = this.root;
        for(int i = 0; i < s.length(); i++){
            char ch = s.charAt(i);
            if(!curr.children.containsKey(ch)){
                curr.children.put(ch, new TrieNode(ch));
            }
            curr = curr.children.get(ch);
        }
        curr.is_end = true;
    }

    public boolean search(String s, boolean allow_prefix){
        /**
         * Function to check whether the string is present inside Trie tree.
         *
         * @param s String to insert
         * @param allow_prefix Parameter to check word is prefix of words inserted.
         * @return boolean
         */
        TrieNode curr = this.root;
        for(int i = 0; i < s.length(); i++){
            char ch = s.charAt(i);
            if(!curr.children.containsKey(ch)){
                return false;
            }
            curr = curr.children.get(ch);
        }
        return allow_prefix | curr.is_end;
    }
}

// Sample Test case to test Trie Tree.
class App{
    public static void main(String[] args) {
        Trie trie = new Trie();

        trie.insert("abc");
        trie.insert("adc");
        System.out.println(trie.search("abc", false)); // true
        System.out.println(trie.search("ab", true));   // true
        System.out.println(trie.search("cbv", false)); // false
        System.out.println(trie.search("cv", true));   // false
    }
}

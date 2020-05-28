#include <cassert>
#include <unordered_map>

class TrieNode {
private:
    char val;
    std::unordered_map<char, TrieNode*> children;
    bool isEnd;
public:
    /**
     * Constructor
     */
    TrieNode(char v, bool end = false);

    /**
     * Creates a new TrieNode and inserts it as a child node to the current node.
     * If the child node already exists, does nothing.
     * Finally, it returns a pointer to the child node.
     */
    TrieNode* addChild(char c);

    /**
     * Sets the current TrieNode as leaf node, by setting 
     * it's `isEnd` attribute.
     */
    void setEnd();

    /**
     * Gets the value of `isEnd` attribute.
     */
    bool getEnd();

    /**
     * Returns a pointer to the child node if it exists, otherwise return `nullptr`.
     */
    TrieNode* hasChild(char c);
};

TrieNode::TrieNode(char v, bool end) : val(v), isEnd(end) {
}

TrieNode* TrieNode::addChild(char c) {
    TrieNode* t;
    if(this->children.count(c)) {
        t = this->children[c];
    } else {
        t = new TrieNode(c);
        // this->children[c].insert(std::make_pair<char, TrieNode*>(c, t));
        this->children[c] = t;
    }
    return t;
}

void TrieNode::setEnd() {
    this->isEnd = true;
}

bool TrieNode::getEnd() {
    return isEnd;
}

TrieNode* TrieNode::hasChild(char c) {
    TrieNode* t = nullptr;
    if(this->children.count(c)){
        t = this->children[c];
    }
    return t;
}

class Trie {
private:
    TrieNode* root;
public:
    /**
     * Default constructor
     */
    Trie();

    /**
     * Inserts the `std::string` into the Trie
     */
    void insert(std::string s);

    /**
     * Returns `true` if this `std::string` `s` exists in the Trie.
     * or, if the `std::string` `s` is present in the Trie as a substring 
     * of a longer string when `allowPrefix` is `true`.
     */
    bool search(std::string s, bool allowPrefix = false);
};

Trie::Trie(){
    root = new TrieNode(0);
}

void Trie::insert(std::string s) {
    TrieNode* t = root;
    for(char& a: s) {
        t = t->addChild(a);
    }
    t->setEnd();
}

bool Trie::search(std::string s, bool allowPrefix) {
    TrieNode* t = root;
    for(char& a: s) {
        t = t->hasChild(a);
        if(t == nullptr) {
            return false;
        }
    }
    return allowPrefix || t->getEnd();
}

int main() {
    Trie t;

    t.insert("abcd");
    t.insert("ab");
    assert(t.search("abc") == false);
    assert(t.search("abc", true) == true);
    assert(t.search("ab")  ==  true);
    assert(t.search("xyz") == false);
    assert(t.search("xyz", true) == false);

    return 0;
}

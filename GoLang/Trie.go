package main

import "fmt"

type TrieNode struct {
	val      rune
	children map[rune]*TrieNode
	isEnd    bool
}

// NewTrieNode is used to construct an instance of TrieNode
func NewTrieNode(val rune) *TrieNode {
	tn := new(TrieNode)
	tn.val = val
	tn.children = make(map[rune]*TrieNode)
	tn.isEnd = false
	return tn
}

// AddChild creates a new TrieNode and inserts it as a
// child node to the current node.
// If the child node already exists, does nothing.
// Finally, it returns a pointer to the child node.
func (this *TrieNode) AddChild(c rune) *TrieNode {
	var t *TrieNode
	if val, ok := this.children[c]; ok {
		t = val
	} else {
		t = NewTrieNode(c)
		this.children[c] = t
	}
	return t
}

// HasChild returns a pointer to the child node if it exists,
// otherwise return `nil`.
func (this *TrieNode) HasChild(c rune) *TrieNode {
	var t *TrieNode = nil
	if val, ok := this.children[c]; ok {
		t = val
	}
	return t
}

// GetEnd gets the value of `isEnd` attribute.
func (this *TrieNode) GetEnd() bool {
	return this.isEnd
}

// SetEnd sets the current TrieNode as leaf node, by setting
// it's `isEnd` attribute.
func (this *TrieNode) SetEnd(b bool) {
	this.isEnd = b
}

type Trie struct {
	root *TrieNode
}

// NewTrie constructs an instance of Trie and returns a pointer to it
func NewTrie() *Trie {
	var t *Trie = new(Trie)
	t.root = NewTrieNode(0)
	return t
}

// Insert inserts the string into the Trie
func (this *Trie) Insert(s string) {
	var t *TrieNode = this.root
	for _, a := range s {
		t = t.AddChild(a)
	}
	t.SetEnd(true)
}

// Search returns `true` if the string `s` exists in the Trie.
// or, if the string `s` is present in the Trie as a substring
// of a longer string when `allowPrefix` is `true`.
func (this *Trie) Search(s string, allowPrefix bool) bool {
	var t *TrieNode = this.root
	for _, a := range s {
		t = t.HasChild(a)
		if t == nil {
			return false
		}
	}

	return allowPrefix || t.GetEnd()
}

// PanicIf works as an equivalent of the C++ assert()
func PanicIf(b bool) {
	if !b {
		panic(b)
	}
}

func main() {

	var t *Trie = NewTrie()
	t.Insert("abcd")
	t.Insert("ab")
	PanicIf(t.Search("abc", false) == false)
	PanicIf(t.Search("abc", true) == true)
	PanicIf(t.Search("ab", false) == true)
	PanicIf(t.Search("xyz", false) == false)
	PanicIf(t.Search("xyz", true) == false)

	fmt.Println("Trie works correctly")
}

#!/usr/bin/env ruby


class TrieNode
    attr_accessor :children, :char, :is_sentinel
    def initialize(char)
        @children = {}
        @char = char
        @is_sentinel = false   # denotes current char is end of the word.
    end
end


class TrieTree
    attr_accessor :root

    def initialize()
        @root = TrieNode.new(nil)
    end
    
    def insert(word)
        # inserts the given word in the trie tree.
        # word: a string to insert in the trie tree.
        curr = @root
        word.each_char{ |char|
            if curr.children[char] == nil
                curr.children[char] = TrieNode.new(char)
            end
            curr = curr.children[char]
        }
        curr.is_sentinel = true  # setting last char as end of word
    end

    def check(word, allow_prefix=true)
        # checks if given word exists in the tree.
        # allow_suffix: if set, it will return true if given word is 
        #                the prefix of words entered in the tree.
        curr = @root
        word.each_char{|char|
            if curr.children[char] == nil
                return false
            else
                curr = curr.children[char]
            end
        }
        return allow_prefix | curr.is_sentinel
    end
end


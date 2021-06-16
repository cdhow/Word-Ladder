# Word-Ladder
A word ladder puzzle has two words in the ladder, one at the bottom and one at the top. You must form a sequence of words (a word ladder). On every step of the ladder you must place a new word that only differs one letter than the previous word. All words must exist in the dictionary.txt
file.


# Algorithm
Two algorithms have been implemented to solve this problem. <br>
The first algorithm checks if the source word can be converted to the target word by changing non-matching letters in 
the source word only one time each. If this algorithm fails to find a link between the source and target word, a bi-directional breadth-first-search (BFS) is used.

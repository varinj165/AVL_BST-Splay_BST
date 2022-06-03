1) I first tested my code with 1000 strings of length 8, The results were:
1000 insertions
AVL: 0.00112809 seconds
Splay: 0.00199469 seconds
Splay expensive: 59

Then I tested my code with 1000 strings of length 10, the results were:
1000 insertions
AVL: 0.00109237 seconds
Splay: 0.00214448 seconds
Splay expensive: 56

Then I tested my code with 5000 strings of length 10, the results were:
5000 insertions
AVL: 0.00679549 seconds
Splay: 0.0139461 seconds
Splay expensive: 205

2) The length of strings did not affect time per repition that much. However, 
changing the number of strings from 1000 to 5000 increased the time a lot. So
the number of strings were the determining factor.

3) The more number of insertions there are, the bigger the tree is, so it takes
longer to insert a new node.

4) An unsorted list would have constant time inserts so it would be faster than 
splay and AVL trees.

A sorted list would be slower than Splay and AVL as the fastest sorting algorithm is
O(n log n) and AVL and Splayyyyyy inserts are Theta(log n).

A BST insert would be faster than Splay and AVL as it is the same just without rotations
so it would take less time.

Hash Table would be faster on average as they have an average runtime of O(1).
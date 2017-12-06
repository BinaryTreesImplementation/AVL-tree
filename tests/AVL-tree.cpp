#include <catch.hpp>

#include "AVL-tree.hpp"

/*           10            */
TEST_CASE("insert1", "[root]") 
{
   AVL_tree<int> tree;
   tree.insert(10);
   REQUIRE(tree.getLeft(10) == nullptr);
   REQUIRE(tree.getRight(10) == nullptr);
   REQUIRE(tree.getParent(10) == nullptr);
   REQUIRE(tree.getRoot() == tree.search(10));
   REQUIRE(tree.height_(tree.search(10)) == 1);
   REQUIRE(tree.count_() == 1);
}

/*        10             9
         /             /   \       
        9      ->     8     10        
      /
     8                           */
TEST_CASE("insert2", "[rotate_right]") 
{
   AVL_tree<int> tree;
   tree.insert(10);
   tree.insert(9);
   tree.insert(8);
   Node<int> * node = tree.search(9);
   REQUIRE(tree.getLeft(9) == tree.search(8));
   REQUIRE(tree.getRight(9) == tree.search(10));
   REQUIRE(tree.getParent(9) == nullptr);
   REQUIRE(tree.getRoot() == node);
   REQUIRE(tree.height_(node) == 2);
   REQUIRE(tree.getParent(8) == node);
   REQUIRE(tree.getParent(10) == node);
   REQUIRE(tree.height_(tree.search(8)) == 1);
   REQUIRE(tree.height_(tree.search(10)) == 1);
   REQUIRE(tree.count_() == 3);
}

/*  8                        9
      \                    /   \       
        9           ->    8     10        
          \
           10                     */
TEST_CASE("insert3", "[rotate_left]") 
{
   AVL_tree<int> tree;
   tree.insert(8);
   tree.insert(9);
   tree.insert(10);
   REQUIRE(tree.getLeft(9) == tree.search(8));
   REQUIRE(tree.getRight(9) == tree.search(10));
   REQUIRE(tree.getParent(9) == nullptr);
   REQUIRE(tree.getRoot() == tree.search(9));
   REQUIRE(tree.height_(tree.search(9)) == 2);
   REQUIRE(tree.getParent(8) == node);
   REQUIRE(tree.getParent(10) == node);
   REQUIRE(tree.height_(tree.search(8)) == 1);
   REQUIRE(tree.height_(tree.search(10)) == 1);
   REQUIRE(tree.count_() == 3);
}

/*        9                9
        /   \     ->     /
       8     10         8        */
TEST_CASE("remove1", "[leaf]") 
{
   AVL_tree<int> tree;
   tree.insert(8);
   tree.insert(9);
   tree.insert(10);
   REQUIRE(tree.count_() == 3);
   tree.remove(10);
   REQUIRE(tree.getRoot() == tree.search(9));
   REQUIRE(tree.height_(tree.search(8)) == 1);
   REQUIRE(tree.height_(tree.search(9)) == 2);
   REQUIRE(tree.getRight(9) == nullptr);
   REQUIRE(tree.search(10) == nullptr);
   REQUIRE(tree.count_() == 2);
}

/*        9                9
        /   \     ->     /   \
       8     10         7     10    
      /                          
     7                            */
TEST_CASE("remove2", "[node->left, !node->right]") 
{
   AVL_tree<int> tree;
   tree.insert(8);
   tree.insert(9);
   tree.insert(10);
   tree.insert(7);
   REQUIRE(tree.height_(tree.search(7)) == 1);
   REQUIRE(tree.height_(tree.search(10)) == 1);
   REQUIRE(tree.height_(tree.search(8)) == 2);
   REQUIRE(tree.height_(tree.search(9)) == 3);
   REQUIRE(tree.count_() == 4);
   tree.remove(8);
   REQUIRE(tree.getRoot() == tree.search(9));
   REQUIRE(tree.height_(tree.search(7)) == 1);
   REQUIRE(tree.height_(tree.search(10)) == 1);
   REQUIRE(tree.height_(tree.search(9)) == 2);
   REQUIRE(tree.getLeft(9) == tree.search(7));
   REQUIRE(tree.search(8) == nullptr);
   REQUIRE(tree.count_() == 3);
}

/*        9                10
        /   \     ->     /
       8     10         8        */
TEST_CASE("remove3", "[node->right]") 
{
   AVL_tree<int> tree;
   tree.insert(8);
   tree.insert(9);
   tree.insert(10);
   REQUIRE(tree.count_() == 3);
   tree.remove(9);
   REQUIRE(tree.getRoot() == tree.search(10));
   REQUIRE(tree.height_(tree.search(8)) == 1);
   REQUIRE(tree.height_(tree.search(10)) == 2);
   REQUIRE(tree.getLeft(10) == tree.search(8));
   REQUIRE(tree.getRight(10) == nullptr);
   REQUIRE(tree.getParent(10) == nullptr);
   REQUIRE(tree.search(9) == nullptr);
   REQUIRE(tree.count_() == 2);
}

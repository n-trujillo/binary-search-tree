#include "BSTree.h"

#include <iostream>
#include <stack>
#include <queue>

using namespace std;

///////////////////////////////////
// Already Implemented Functions //
///////////////////////////////////

// These functions are already implemented for you. Scroll below to
// see the functions that you have to implement 


// constructors
BSTree::BSTree() : size(0), root(nullptr) {}

// input / output operators
ostream& operator<<(ostream& out, BSTree& tree)
{
    tree.print_level_by_level(out);
    return out;
}

ostream& operator<<(ostream& out, Node& node)
{
    return out << node.value << "[" << node.search_time << "]";
}

istream& operator>>(istream& in, BSTree& tree)
{
    /*
      take input from the in stream, and build your tree
      input will look like
      4 
      2 
      6 
      1 
      3 
      5 
      7
    */
    int next;
    while(in >> next) {
        tree.insert(next);
    }
    return in;
}

// Example recursive function
// If you try to use it without care, you will get a memory leak.
void BSTree::copy_helper(Node*& newNode, const Node* sourceNode) {
    //Don't copy if the node is nullptr
    if(sourceNode == nullptr)
        return;

    //Change the new node to a copy of sourceNode
    newNode = new Node(sourceNode->value);
    //Copy over the search cost
    newNode->search_time = sourceNode->search_time;

    //Copy left subtree
    if (sourceNode->left != nullptr)
        copy_helper(newNode->left, sourceNode->left);
    //Copy right subtree
    if(sourceNode->right != nullptr)
        copy_helper(newNode->right, sourceNode->right);
}

// recursive function
int BSTree::get_total_search_time(Node* node)
{
  if (node != nullptr) {
    return node->search_time + get_total_search_time(node->left) + get_total_search_time(node->right);
  }
  return 0;
}

// destructor helper
void BSTree::delete_all_nodes(Node* node) {

    // Don't delete if the node is nullptr
    if(node == nullptr) {
        return;
    }

    // Delete left subtree
    delete_all_nodes(node->left);

    // Delete right subtree
    delete_all_nodes(node->right);

    // delete the node
    delete node;
    node = nullptr;
}

// // insert helper 
// Node* BSTree::insert_helper(int obj, Node* node, int count) {

//     if (root == nullptr) { // end of the line
//         Node* newNode = new Node(obj);
//         newNode->search_time = count;

//         // if it was the root that was empty
//         if (node == nullptr) {
//             node = newNode;
//         }

//         return newNode;
//     }

//     if (obj > node->value) { // greater than
//         count++;
//         node->right = insert_helper(obj, node->right, count);
//     }

//     else { // less than
//         count++;
//         node->left = insert_helper(obj, node->left, count);
//     }

//     return node;
// }

// implemented
float BSTree::get_average_search_time()
{
  int total_search_time = get_total_search_time(root);
  if(total_search_time == 0)
    return -1;
	
  return ((float)total_search_time)/size;
}


///////////////////////////////////
//     Functions to Implement    //
///////////////////////////////////

// These are the functions you should implement
// Feel free to call the functions above or create new helper functions

// copy constructor
BSTree::BSTree(const BSTree& other)
{

    // set this to be empty
    root = nullptr;
    size  = 0;

    // copy from empty tree
    if (other.root == nullptr) {
        return;
    }

    // copy from non-empty tree
    if (other.get_root() != nullptr) {
        copy_helper(root,other.get_root());

        // update root and size
        this->size = other.size;
    }
}

// move constructor
BSTree::BSTree(BSTree&& other)
{
    // check for self assignment
    if (this != &other) {
        // copy over assets from other
        this->root = other.root;
        this->size = other.size;

        // delete the other
        other.size = 0;
        other.root = nullptr;
    }
}

//copy assignment
BSTree& BSTree::operator=(const BSTree& other)
{

    // check for self assignment
    if (this != &other) {
        // set this to be empty

        if (root) {
            this->delete_all_nodes(root);
        }

        root = nullptr;
        size  = 0;

        // copy from empty tree
        if (other.root == nullptr) {
            return *this;
        }

        // copy from non-empty tree
        if (other.get_root() != nullptr) {
            copy_helper(root,other.get_root());

            // update root and size
            this->size = other.size;
        }
    }

    return *this;
}

// move assignment
BSTree& BSTree::operator=(BSTree&& other)
{
    // check for self assignment
    if (this != &other) {
        // set this to be empty

        if (root) {
            this->delete_all_nodes(root);
        }

        root = nullptr;
        size  = 0;

        // copy over assets from other
        this->root = other.root;
        this->size = other.size;

        // delete the other
        other.size = 0;
        other.root = nullptr;
    }

    return *this;
}

// destructor
BSTree::~BSTree()
{
    // Make sure to call delete on every node of the tree
    // You can use a recursive helper function to do this
    delete_all_nodes(root);
    root = nullptr;
    size = 0;
}

Node* BSTree::insert(int obj)
{
    size++;
    // return insert_helper(obj, root, 1);

    int searchCount = 1;
    Node* marker = root;
    Node* parent = nullptr;
 
    // if tree is empty
    if (root == nullptr)
    {
        root = new Node(obj);
        root->search_time = 1;
        return root;
    }
    
    // iterate though the tree
    while (marker != nullptr)
    {
        parent = marker;
 
        if (obj < marker->value) { // less than
            marker = marker->left;
        }
        else { // greater than
            marker = marker->right;
        }

        searchCount++;
    }
 
    if (obj < parent->value) {
        Node* newNode = new Node(obj);
        parent->left = newNode;
        newNode->search_time = searchCount;
        return newNode;
    }
    else {
        Node* newNode = new Node(obj);
        parent->right = newNode;
        newNode->search_time = searchCount;
        return newNode;
    }


}

Node* BSTree::search(int obj)
{
    // temp value for root
    Node* t = root;

    while (t != nullptr) {
        if (obj < t->value) {
            t = t->left;
        } else if (obj > t->value) {
            t = t->right;
        } else {
            return t; // found
        }
    }
    return nullptr;
}

void BSTree::update_search_times()
{
    /* do a BFS or DFS of your tree and update the search times of all nodes at once
      The root has a search time of 1, and each child is 1 more than its parent */


    std::queue<Node*> q;
    int searchCost = 1;

    q.push(root);
    int elementsInLevel = 1;
    bool nonNullChild = false;

    while (elementsInLevel > 0) {
        Node* node = q.front();
        q.pop();
        elementsInLevel--;
        if (node != nullptr) {
            // update searchCost
            node->search_time = searchCost;

            // enqueue the children of node into q
            if (node->left != nullptr) {
                q.push(node->left);
            } else {
                q.push(nullptr);
            }

            if (node->right != nullptr) {
                q.push(node->right);
            } else {
                q.push(nullptr);
            }

            if (node->right != nullptr || node->left != nullptr) {
                nonNullChild = true;
            }

        } else {
            // out << 'X';
            q.push(nullptr);
            q.push(nullptr);
        }

        if (elementsInLevel == 0) { // reached the end of the level
            // increment the search cost
            searchCost++;

            if (nonNullChild == true) {
                nonNullChild = false;
                elementsInLevel = q.size();
            }
        }
    }

}


void BSTree::inorder(ostream& out) {

    Node* current = root;
    stack<Node*> stack;
 
    while (!stack.empty() || current != nullptr) {

        while (current !=  nullptr) {
            stack.push(current);
            current = current->left;
        }
 
        current = stack.top();
        stack.pop();
        out << *current << " ";
        current = current->right;
    }
}

void BSTree::print_level_by_level(ostream& out)
{
    /* Print the tree using a BFS so that each level contains the values for a level of the tree.
    Use X to mark empty positions. 
    
      if our tree looks like this:

       4
     2   6
    1   5 7
           9

    it should output:

    4[1]
    2[2] 6[2]
    1[3] X 5[3] 7[3]
    X X X X X X X 9[4]

    it might be helpful to do this part with the std::queue data structure.
    Hint: the Nth level contains 2^(N-1) nodes (with a value or with an X). Watch out
    for nodes like the left child of 6 above, and print the descendents of an empty
    node as also empty
    You can use the << operator to print nodes in the format shown */

    std::queue<Node*> q;

    q.push(root);
    int elementsInLevel = 1;
    bool nonNullChild = false;

    while (elementsInLevel > 0) {
        Node* node = q.front();
        q.pop();
        elementsInLevel--;
        if (node != nullptr) {
            out << (*node) << " ";

            // enqueue the children of node into q
            if (node->left != nullptr) {
                q.push(node->left);
            } else {
                q.push(nullptr);
            }

            if (node->right != nullptr) {
                q.push(node->right);
            } else {
                q.push(nullptr);
            }

            if (node->right != nullptr || node->left != nullptr) {
                nonNullChild = true;
            }

        } else {
            out << 'X';
            q.push(nullptr);
            q.push(nullptr);
        }

        if (elementsInLevel == 0) {
            out << endl;
            if (nonNullChild == true) {
                nonNullChild = false;
                elementsInLevel = q.size();
            }
        }
    }
}

#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;

class MaxHeapNode {
  public:
    MaxHeapNode *left, *right, *parent;
    std::string key;
    int value;

    MaxHeapNode(std::string k, int v, MaxHeapNode *p):left(NULL), right(NULL), key(k), value(v), parent(p){};

    ~MaxHeapNode(){
      if(left != NULL) {
        //cout << "delete left of" << value << "which is " << left->value << endl;
        delete left;
      }
      if(right != NULL) {
        //cout << "delete right of" << value <<  "which is " << right->value << endl;
        delete right;
      }
    };

    void insert_node(int bin_path, int total_num_nodes, MaxHeapNode* new_node) {
      if (total_num_nodes & bin_path) {
        bin_path >>= 1;
        if (right == NULL) {
          //cout << new_node->value << "inserted right" << endl;
          //you are here, INSERT
          right = new_node;
          new_node->parent = this;
        }
        else {
          //cout <<  new_node->value << "went right" << endl;
          right->insert_node(bin_path, total_num_nodes, new_node);
        }
      }
      else {
        bin_path >>= 1;
        if (left == NULL) {
          //cout << new_node->value << "inserted left" << endl;
          //you are here, INSERT
          left = new_node;
          new_node->parent = this;
        }
        else {
          //cout <<  new_node->value << "went left" << endl;
          left->insert_node(bin_path, total_num_nodes, new_node);
        }
      }
    };
    //voithitiki synarthsh
    void print2DUtil(int space)  {

          // Increase distance between levels
            space += 4;

            // Process right child first
            if(right != NULL)
                right->print2DUtil(space);

            // Print current node after space
            // count
            cout<<endl;
            for (int i = 4; i < space; i++)
                cout<<" ";
            cout << key << ":" << value << "\n";

            // Process left child
            if(left != NULL)
                left->print2DUtil(space);
    };

    MaxHeapNode* heap_check() {
      if(parent == NULL) {
        return this;
      }
      if (value > parent->value) {
        //cout << value << ">" << parent->value << endl; 
        //swap with parent
        MaxHeapNode *my_left = left, *my_right = right, *old_parent = parent;

        //update old parent's parent
        if(old_parent->parent != NULL) {
          bool old_parent_is_left_child = old_parent == old_parent->parent->left ? 1 : 0;
          if(old_parent_is_left_child) {
            old_parent->parent->left = this;
          }
          else {
            old_parent->parent->right = this;
          }
        }

        bool is_left_of_parent = parent->left == this ? 1 : 0;
        if(is_left_of_parent) {
          //update me
          parent = parent->parent;
          left = old_parent;
          right = old_parent->right;
          //update old_parent's right
          if(old_parent->right != NULL) {
            old_parent->right->parent = this;
          }
        }
        else {
          //update me
          parent = old_parent->parent;
          right = old_parent;
          left = old_parent->left;
          //update old_parent's left
          if(old_parent->left != NULL)
            old_parent->left->parent = this;
        }
        //update parent
        old_parent->parent = this;
        old_parent->left = my_left;
        old_parent->right = my_right;
        //update my_left and my_right
        if(my_right != NULL) {
          my_right->parent = old_parent;
        }
        if(my_left != NULL) {
          my_left->parent = old_parent;
        }
      }
      //call again until you get to root
      if(parent != NULL) {
        return parent->heap_check();
      }
      else {
        return this;
      }
    };

    void get_node(int bin_path, int total_num_nodes, MaxHeapNode* new_node) {
      if (total_num_nodes & bin_path) {
        bin_path >>= 1;
        if (right == NULL) {
          cout << "this node doesnt exist" << endl;
        }
        else {
          cout <<  new_node->value << "went right" << endl;
          right->insert_node(bin_path, total_num_nodes, new_node);
        }
      }
      else {
        bin_path >>= 1;
        if (left == NULL) {
          cout << new_node->value << "inserted left" << endl;
          //you are here, INSERT
          left = new_node;
          new_node->parent = this;
        }
        else {
          cout <<  new_node->value << "went left" << endl;
          left->insert_node(bin_path, total_num_nodes, new_node);
        }
      }
    };

    MaxHeapNode* heap_check_pop() {
      //find max child
      MaxHeapNode *max_child = NULL;
      if(left == NULL && right == NULL) {
        return this; //no change
      }
      else if(left == NULL && right != NULL) {
        max_child = right;
      }
      else if(left != NULL && right == NULL) {
        max_child = left;
      }
      else{
        if(left->value > right->value) {
          max_child = left;
        }
        else {
          max_child = right;
        }
        
      }
      //check if child is bigger
      if(max_child->value > value) {
        //swap
        MaxHeapNode *old_parent = this;
        //update old_parent parent
        if(old_parent->parent != NULL) {
          if(old_parent->parent->left == old_parent) {
            old_parent->parent->left = max_child;
          }
          else {
            old_parent->parent->right = max_child;
          }
        }
        //update max_child left
        if(max_child->left != NULL) {
          max_child->left->parent = old_parent;
        }
        //update max_child right
        if(max_child->right != NULL) {
          max_child->right->parent = old_parent;
        }
        MaxHeapNode *max_child_left = max_child->left, *max_child_right = max_child->right;
        //update max_child
        if(max_child == old_parent->right) {
          max_child->right = old_parent;
          max_child->left = old_parent->left;
          max_child->parent = old_parent->parent;
          //update old_parent left
          if(old_parent->left != NULL) {
            old_parent->left->parent = max_child;
          }
        }
        else {
          max_child->left = old_parent;
          max_child->right = old_parent->right;
          max_child->parent = old_parent->parent;
          //update old_parent right
          if(old_parent->right != NULL) {
            old_parent->right->parent = max_child;
          }
        }
        //update old_parent
        old_parent->parent = max_child;
        old_parent->left = max_child_left;
        old_parent->right = max_child_right;
        //call again for old_parent
        this->heap_check_pop();
        return max_child;
      }
      else {
        return this; //no change
      }

    };
};

class MaxHeap {
public:
  MaxHeapNode *root;
  int total_num_nodes;

  //Constructor
  MaxHeap(std::string k, int v):root(new MaxHeapNode(k,v,NULL)), total_num_nodes(1){};

  //Destructor
  ~MaxHeap(){
    delete root;
  };

  void insert(std::string k, int v) {
    total_num_nodes++;
    int binary_path = total_num_nodes;
    //delete first bit of path
    while (binary_path & (binary_path - 1))
        binary_path &= binary_path - 1;
    binary_path >>= 1;

    MaxHeapNode* new_node = new MaxHeapNode(k,v,NULL);
    root->insert_node(binary_path, total_num_nodes, new_node);

    root = new_node->heap_check();
  };

  MaxHeapNode* getMax() {
    /*********swap root with last node******/
    MaxHeapNode* last_node = getTargetNode(total_num_nodes);
    MaxHeapNode *old_root = root;
    //update last_node->parent
    if(last_node->parent->left == last_node) {
      last_node->parent->left = NULL;
    }
    else {
      last_node->parent->right = NULL;
    }
    //set last_node as root
    root = last_node;
    root->right = old_root->right;
    root->left = old_root->left;
    root->parent = NULL; //root has no parent
    //old_root update left and right. their parent changed
    if(old_root->left != NULL)
      old_root->left->parent = root;
    if(old_root->right != NULL)
      old_root->right->parent = root;
    /****check heap properties after the pop*****/
    root = root->heap_check_pop();
    //return old_root
    old_root->left = NULL;
    old_root->right = NULL;
    total_num_nodes--;
    return old_root;
  }

  MaxHeapNode* getTargetNode(int targetNodeIndex) {
    if(targetNodeIndex > total_num_nodes || targetNodeIndex < 1) {
      cout << "bad target node index " <<  targetNodeIndex << ". The heap has " << total_num_nodes << " nodes in total." << endl; 
      return NULL;
    }
    int binary_path = targetNodeIndex;
    //delete first bit of path
    while (binary_path & (binary_path - 1))
        binary_path &= binary_path - 1;
    binary_path >>= 1;
  

    MaxHeapNode* target = root;

    while(binary_path) {
      if(binary_path & targetNodeIndex) {
        if(target->right == NULL) {
          cout << "Something went wrong. Couldnt find target node " << targetNodeIndex << endl;
          root->print2DUtil(4);
        }
        else {
          target = target->right;
        }
      }
      else {
        if(target->left == NULL) {
          cout << "Something went wrong. Couldnt find target node " << targetNodeIndex << endl;
          root->print2DUtil(4);
        }
        else {
          target = target->left;
        }
      }
      binary_path >>= 1;
    }
    return target;
  };

  void print(int space) {
    root->print2DUtil(space);
  }

};

#endif

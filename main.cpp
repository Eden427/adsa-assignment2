/*
 * @Author: Zichu Zhao 
 * @Date: 2025-09-11
 * @LastEditors: Zichu Zhao 965927155@qq.com
 * @LastEditTime: 2025-09-21 22:50:51
 * @FilePath: /main.cpp
 * @Description: AVL Tree implementation with insert, delete, and traversal
 *               Assignment code with some debug traces for testing.
 */

# include <iostream>
# include <vector>
# include <string>

using namespace std;

// Node Structure for ALV tree
struct Node{
    int data;
    Node* left;
    Node* right;
    int height;
};

// Create a new node with given value
Node *createNode(int val){
    Node* node = new Node();
    node->data = val;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1; // height of a new node is 1
    // cout << "[debug]create node: " << val << endl;
    return node;
}

// return height of a node, or 0 if node is null
int getHeight(Node* n){
    if (n ==nullptr){
        return 0;
    } else{
        return n->height;
    }
}

// return maximum of two numbers
int getMax(int a, int b){
    if (a > b){
        return a;
    } else{
        return b;
    }
}

// update the height of a node based on its children's heights
void updateHeight(Node* n){
    if (n != nullptr) {
        int lh = getHeight(n->left);
        int rh = getHeight(n->right);
        n->height = 1 + getMax(lh, rh);
        // cout << "[debug]update height of node " << n->data << " to " << n->height << endl;
    }
}

// compute balance factor = left height - right height
int getBalance(Node* n){
    if (n == nullptr){
        return 0;
    }
    return getHeight(n->left) - getHeight(n->right);
}

// right rotation(LL case)
Node * rotateRight(Node* y){
    Node* x = y->left;
    Node* T2 = x->right;

    // perform rotation
    x->right = y;
    y->left = T2;

    // update heights
    updateHeight(y);
    updateHeight(x);

    // return new root
    // cout << "[debug]right rotate on node " << y->data << endl;
    return x;
}

// left rotation(RR case)
Node * rotateLeft(Node* x){
    Node* y = x->right;
    Node* T2 = y->left;

    // perform rotation
    y->left = x;
    x->right = T2;

    // update heights
    updateHeight(x);
    updateHeight(y);

    // return new root
    // cout << "[debug]left rotate on node " << x->data << endl;
    return y;
}

// Insert a node into the AVL tree
// 1.Insert like BST
// 2.Upade height
// 3.check balance factor
// 4.Rotate if need

Node* insertNode(Node* root, int val){
    // Base case: found null position, insert new node here
    if (root == nullptr){
        return createNode(val);
    }
    // Perform normal BST insertion
    if (val < root->data){
        root->left = insertNode(root->left, val);
    } else if (val > root->data){
        root->right = insertNode(root->right, val);
    } else{
        // cout<<"[debug] duplicate igored:"<<val<<endl;
        return root;
    }
    
    updateHeight(root);
    int bf = getBalance(root);

    // Four imbalance cases
    if (bf > 1 && val < root->left->data) return rotateRight(root);  // LL
    if (bf < -1 && val > root->right->data) return rotateLeft(root); // RR
    if (bf > 1 && val > root->left->data) {                          // LR
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (bf < -1 && val < root->right->data) {                        // RL
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;

}

// find the minimum node(used for deletion)
Node* findMin(Node* root){
    while (root != nullptr && root->left != nullptr){
        root = root->left;
    }
    return root;
}

// Delete a node from AVL tree
Node* deleteNode(Node* root, int val){
    if (root == nullptr){
        // cout << "[debug]value not found for deletion: " << val << endl;
        return root;
    }

if (val < root->data) {
        root->left = deleteNode(root->left, val);
    } else if (val > root->data) {
        root->right = deleteNode(root->right, val);
    } else {
        // Node found
        if (root->left == nullptr || root->right == nullptr) {
            Node* child;
            if (root->left != nullptr) {
                child = root->left;
            } else {
                child = root->right;
            }

            if (child == nullptr) {
                // no child
                // cout << "[DEBUG] delete leaf: " << root->data << endl;
                delete root;
                return nullptr;
            } else {
                // one child -> copy
                *root = *child;
                delete child;
            }
        } else {
            // two children
            Node* succ = findMin(root->right);
            root->data = succ->data;
            root->right = deleteNode(root->right, succ->data);
        }
    }

    updateHeight(root);
    int bf = getBalance(root);

    if (bf > 1 && getBalance(root->left) >= 0) return rotateRight(root);
    if (bf > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (bf < -1 && getBalance(root->right) <= 0) return rotateLeft(root);
    if (bf < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
          
}

// Traversal functions
void preOrder(Node* root, vector<int> &out){
    if (root == nullptr) return;
    out.push_back(root->data);
    preOrder(root->left, out);
    preOrder(root->right, out);
}

void inOrder(Node* root, vector<int> &out){
    if (root == nullptr) return;
    inOrder(root->left, out);
    out.push_back(root->data);
    inOrder(root->right, out);
}

void postOrder(Node* root, vector<int> &out){
    if (root == nullptr) return;
    postOrder(root->left, out);
    postOrder(root->right, out);
    out.push_back(root->data);
}

// test function(used during coding, not for submission)
void testTree() {
    Node* root = nullptr;
    root = insertNode(root, 30);
    root = insertNode(root, 20);
    root = insertNode(root, 40); 
    root = insertNode(root, 10);
    root = insertNode(root, 25);

    vector<int> res;
    preOrder(root, res);
    cout << "Pre-order: ";
    for (int i: res) cout << i << " ";
    cout << endl;

    res.clear();
    root = deleteNode(root, 40);
    preOrder(root, res);
    cout << "After deleting 40, Pre-order: ";
    for (int i: res) cout << i << " ";
    cout << endl;
}

int main() {
    // testTree(); // for debugging only
    
    Node* root = nullptr;
    vector<string> cmds;
    string s;

    // keep reading until we meet PRE/IN/POST
    while (cin >> s) {
        cmds.push_back(s);
        if (s == "PRE" || s == "IN" || s == "POST") {
            break; 
        }
    }

    // if no input at all
    if (cmds.empty()) {
        cout << "EMPTY" << endl;
        return 0;
    }

    // the last one is traversal type
    string traversal = cmds.back();

    // process insert A and delete D commands
    for (int i = 0; i < (int)cmds.size() - 1; i++) {
        char op = cmds[i][0];      // first char: A or D
        int val = stoi(cmds[i].substr(1)); // number part
        if (op == 'A') {
            root = insertNode(root, val);
        } else if (op == 'D') {
            root = deleteNode(root, val);
        }
    }

    // do traversal
    vector<int> out;
    if (traversal == "PRE") {
        preOrder(root, out);
    } else if (traversal == "IN") {
        inOrder(root, out);
    } else if (traversal == "POST") {
        postOrder(root, out);
    }

    // print result
    if (out.empty()) {
        cout << "EMPTY" << endl;
    } else {
        for (int i = 0; i < (int)out.size(); i++) {
            if (i > 0) cout << " ";
            cout << out[i];
        }
        cout << endl;
    }

    return 0;
}







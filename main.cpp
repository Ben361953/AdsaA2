#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

struct Node{
    int value;
    Node *left;
    Node *right;
    Node(int value) {
        this->value = value;
        left = nullptr;
        right = nullptr;
    }
};

int height(Node *cur){
    if(cur == nullptr){
        return 0;
    }
    return max(height(cur->left), height(cur->right)) + 1;
}

Node *rotate_left(Node *cur){
    if(cur == nullptr || cur->right == nullptr){
        return cur;
    }
    Node *tmp = cur->right;
    cur->right = tmp->left;
    tmp->left = cur;
    return tmp;
}

Node *rotate_right(Node *cur){
    if(cur == nullptr || cur->left == nullptr){
        return cur;
    }
    Node *tmp = cur->left;
    cur->left = tmp->right;
    tmp->right = cur;
    return tmp;
}

Node *balance(Node *root){
    if(root == nullptr){
        return nullptr;
    }
    int left_height = height(root->left);
    int right_height = height(root->right);
    if (right_height - left_height > 1) {
        if (height(root->right->left) > height(root->right->right)){
            root->right = rotate_right(root->right);
        }
        root = rotate_left(root);
    } else if(left_height - right_height > 1) {
        if (height(root->left->right) > height(root->left->left)){
            root->left = rotate_left(root->left);
        }
        root = rotate_right(root);
    }
    return root;
}

Node *insert_value(Node *root, int value){
    if (root == nullptr){
        return new Node(value);
    }
    if (root->value == value){
        return root;
    }
    // Insert into left or right subtree
    if (value < root->value){
        root->left = insert_value(root->left, value);
    } else {
        root->right = insert_value(root->right, value);
    }
    // Balance
    return balance(root);
}

Node *delete_value(Node *root, int value){
    if (root == nullptr){
        return nullptr;
    }
    if(value < root->value){
        root->left = delete_value(root->left, value);
    } else if(value > root->value) {
        root->right = delete_value(root->right, value);
    } else {
        if(root->left == nullptr && root->right == nullptr){
            delete root;
            root = nullptr;
        } else if (root->left == nullptr){
            Node *root_new = root->right;
            delete root;
            root = root_new;
        } else if (root->right == nullptr){
            Node *root_new = root->left;
            delete root;
            root = root_new;
        } else {
            Node *cur = root->right;
            while(cur->left != nullptr){
                cur = cur->left;
            }
            int right_min = cur->value;
            root->right = delete_value(root->right, right_min);
            Node *root_new = new Node(right_min);
            root_new->left = root->left;
            root_new->right = root->right;
            delete root;
            root = root_new;
        }
    }
    if (root == nullptr){
        return nullptr;
    }
    // Balance
    return balance(root);
}

void pre(Node *cur, vector<int> &res){
    if(cur == nullptr){
        return;
    }
    res.push_back(cur->value);
    pre(cur->left, res);
    pre(cur->right, res);
}

void in(Node *cur, vector<int> &res){
    if(cur == nullptr){
        return;
    }
    in(cur->left, res);
    res.push_back(cur->value);
    in(cur->right, res);
}

void post(Node *cur, vector<int> &res){
    if(cur == nullptr){
        return;
    }
    post(cur->left, res);
    post(cur->right, res);
    res.push_back(cur->value);
}

int main(){
    string str;
    Node *root = nullptr;
    vector<int> res;
    while(cin>>str){
        if(str == "PRE"){
            pre(root, res);
            break;
        }
        if(str == "IN"){
            in(root, res);
            break;
        }
        if(str == "POST"){
            post(root, res);
            break;
        }
        int value = 0;
        for(size_t i=1;i<str.size();i++){
            value = value*10 + str[i] - '0';
        }
        if(str[0] == 'A'){
            root = insert_value(root, value);
        } else {
            root = delete_value(root, value);
        }
    }
    if(res.empty()){
        cout<<"EMPTY"<<endl;
        return 0;
    }
    for(size_t i=0;i<res.size();i++){
        cout<<res[i];
        if(i==res.size()-1){
            cout<<endl;
        }else{
            cout<<" ";
        }
    }
    return 0;
}

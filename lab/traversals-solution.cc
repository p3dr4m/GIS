// Starter code for lab 04 of COMP 8042
// Copyright Hassan S. Shavarani
// You can compile and run this file using the following command:
//			g++ -std=c++11 -pthread traversals.cc libgtest.a
#include <iostream>
#include <gtest/gtest.h>
#include <vector>
#include <stack>
#include "helper.hh"

using namespace std;

template <typename T>
vector<T> preorderTraversalNoRecursion(Node<T>* root) {
	vector<T> preorder;
	if (root == NULL)
		return preorder;
	stack<Node<T>*> traversal;
	traversal.push(root);
	while(!traversal.empty()){
		Node<T>* next = traversal.top();
		preorder.push_back(next->value);
		traversal.pop();
		Node<T>* left = next -> left;
		Node<T>* right = next -> right;
		if (right != NULL)
			traversal.push(right);
		if (left != NULL)
			traversal.push(left);
	}
	return preorder;
}

template <typename T>
vector<T> inorderTraversalNoRecursion(Node<T>* root) {
	vector<T> inorder;
	if (root == NULL)
		return inorder;
	stack<Node<T>*> traversal;
	Node<T>* current = root;
	while(current != NULL){
		traversal.push(current);
		current = current -> left;
		while(!traversal.empty() && current == NULL){
			Node<T>* next = traversal.top();
			inorder.push_back(next->value);
			traversal.pop();
			Node<T>* right = next -> right;
			if (right != NULL){
				current = right;
			}
		}
	}
	return inorder;
}

void testPreorder() {
  Node<int>* root = buildTree(vector<int>({1}), vector<int>({1}));
  vector<int> preorder = preorderTraversalNoRecursion(root);
  ASSERT_EQ(vector<int>({1}), preorder);
  delete(root);
  
  root = buildTree(vector<int>({10, 8, 3, 5, 2, 7}), vector<int>({3, 8, 5, 10, 7, 2}));
  preorder = preorderTraversalNoRecursion(root);
  ASSERT_EQ(vector<int>({10, 8, 3, 5, 2, 7}), preorder);
  delete(root);

  root = buildTree(vector<int>({1, 2, 3, 4, 5}), vector<int>({1, 4, 3, 2, 5}));
  preorder = preorderTraversalNoRecursion(root);
  ASSERT_EQ(vector<int>({1, 2, 3, 4, 5}), preorder);
  delete(root);
}


void testInorder() {
  Node<int>* root = buildTree(vector<int>({1}), vector<int>({1}));
  vector<int> inorder = inorderTraversalNoRecursion(root);
  ASSERT_EQ(vector<int>({1}), inorder);
  delete(root);
  
  root = buildTree(vector<int>({10, 8, 3, 5, 2, 7}), vector<int>({3, 8, 5, 10, 7, 2}));
  inorder = inorderTraversalNoRecursion(root);
  ASSERT_EQ(vector<int>({3, 8, 5, 10, 7, 2}), inorder);
  delete(root);
  
  root = buildTree(vector<int>({1, 2, 3, 4, 5}), vector<int>({1, 4, 3, 2, 5}));
  inorder = inorderTraversalNoRecursion(root);
  ASSERT_EQ(vector<int>({1, 4, 3, 2, 5}), inorder);
  delete(root);
}


int main() {
  testBuildTree();
  testPreorder();
  testInorder();
  
  Node<int>* root = buildTree(vector<int>({10, 8, 3, 5, 2, 7}), vector<int>({3, 8, 5, 10, 7, 2}));
  vector<int> preorder = preorderTraversalNoRecursion(root);
  vector<int> inorder = inorderTraversalNoRecursion(root);
  cout << "Preorder: " << preorder << endl
       << "Inorder: " << inorder << endl;
  cout << "Finished succesfully!" << endl;
}


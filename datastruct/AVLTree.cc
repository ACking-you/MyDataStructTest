//
// Created by Alone on 2022-11-29.
//

#include "AVLTree.h"
#include <algorithm>
#include<iostream>
#include<cassert>
/*Rotate*/
node *AVLTree::rotateRight(node *root) {//右旋
	node *son = root->lchild;
	root->lchild = son->rchild;
	son->rchild = root;
	update_depth(root);//更新深度(右旋只会对这两结点产生影响
	update_depth(son);
	return son;
}

node *AVLTree::rotateLeft(node *root) {
	node *son = root->rchild;
	root->rchild = son->lchild;
	son->lchild = root;
	update_depth(root);
	update_depth(son);
	return son;
}

node *AVLTree::rotateLeftRight(node *root) {
	root->lchild = rotateLeft(root->lchild);
	return rotateRight(root);
}

 node * AVLTree::rotateRightLeft( node *root) {
	root->rchild = rotateRight(root->rchild);
	return rotateLeft(root);
}


/*Destruct*/
void  AVLTree::destroy( node *root) {
	if (root == nullptr)
		return;
	destroy(root->lchild);
	destroy(root->rchild);
	delete root;
	root = nullptr;
}


/*Getter*/
 node * AVLTree::getNext( node *root,  node *p) {
	if (root == nullptr || p == nullptr) return nullptr;
	if (p->val >= root->val) {
		return getNext(root->rchild, p);
	} else {
		node *left = getNext(root->lchild, p);
		return left ? left : root;
	}
}

 node * AVLTree::getPre( node *root,  node *p) {
	if (root == nullptr || p == nullptr)return nullptr;
	if (p->val <= root->val) {
		return getPre(root->lchild, p);
	} else {
		node *right = getPre(root->rchild, p);
		return right ? right : root;
	}
}

 node * AVLTree::getMinNode( node *root) {
	if (root == nullptr)
		return nullptr;
	while (root->lchild != nullptr)
		root = root->lchild;
	return root;
}

 node * AVLTree::getMaxNode( node *root) {
	if (root == nullptr)
		return nullptr;
	while (root->rchild != nullptr)
		root = root->rchild;
	return root;
}

int  AVLTree::get_depth( node *root) {
	if (root == nullptr)
		return 0;
	return root->depth;
}

void  AVLTree::update_depth( node *root) {
	if (root == nullptr)
		return;
	root->depth = std::max(get_depth(root->lchild), get_depth(root->rchild)) + 1;
}


/*Insert&remove*/
 node * AVLTree::Insert(int x,  node *root, int &size) {
	if (root == nullptr) {
		root = new node(x);
		size++;//创建结点后size++
	} else if (x < root->val) {
		root->lchild = Insert(x, root->lchild, size);
		//由于在更新该root结点之前，当平衡度未达到该要求之前肯定以及是进行了update_depth操作
		if (get_depth(root->lchild) - get_depth(root->rchild) == 2)
			root = x < root->lchild->val ? rotateRight(root) : rotateLeftRight(root);
	} else if (x > root->val) {
		root->rchild = Insert(x, root->rchild, size);
		if (get_depth(root->lchild) - get_depth(root->rchild) == -2)
			root = x > root->rchild->val ? rotateLeft(root) : rotateRightLeft(root);
	}
	update_depth(root);
	return root;
}

 node * AVLTree::remove(int x,  node *root, int &size) {
	if (root == nullptr)
		return nullptr;

	if (x == root->val) {
		/*左右子树均不为空---用中序的前驱或者后继来进行替换*/
		if (root->lchild != nullptr && root->rchild != nullptr) {
			/*根据左右子树的深度来选择删除替换哪边的*/
			if (get_depth(root->lchild) > get_depth(root->rchild)) {
				node *t = getMaxNode(root->lchild);
				root->val = t->val;
				root->lchild = remove(t->val, root->lchild, size);
			} else {
				node *t = getMinNode(root->rchild);
				root->val = t->val;
				root->rchild = remove(t->val, root->rchild, size);
			}
		}
			/*左右子树至少有一个为空的情况，直接往下走一步即可*/
		else {
			node *tmp = root->lchild == nullptr ? root->rchild : nullptr;
			if (tmp != nullptr) {
				*root = *tmp;
				delete tmp;
			} else {
				delete root;
				root = nullptr;
			}
			//删除时size--
			size--;
		}
	} else if (x < root->val) {
		root->lchild = remove(x, root->lchild, size);
		if (get_depth(root->lchild) - get_depth(root->rchild) == -2)
			root = get_depth(root->rchild->lchild) > get_depth(root->rchild->rchild) ? rotateRightLeft(root)
																					 : rotateLeft(root);
	} else {
		root->rchild = remove(x, root->rchild, size);
		if (get_depth(root->lchild) - get_depth(root->rchild) == 2)
			root = get_depth(root->lchild->rchild) > get_depth(root->lchild->lchild) ? rotateLeftRight(root)
																					 : rotateRight(root);
	}
	return root;
}


/*print part*/
void  AVLTree::inorder( node *root) {
	if (root != nullptr) {
		inorder(root->lchild);
		printf("%d ", root->val);
		inorder(root->rchild);
	}
}


/*clear&isEmpty*/
void  AVLTree::clear() {
	destroy(head);
}

bool  AVLTree::isEmpty() {
	return head == nullptr;
}


/*find*/
bool  AVLTree::find(int x) {
	//查找直接迭代方式即可
	node *f = head;
	while (f != nullptr) {
		if (x == f->val)
			return true;
		else if (x < f->val)
			f = f->lchild;
		else
			f = f->rchild;
	}
	return false;
}


/*insert&remove*/
void  AVLTree::insert(int x) {
	head = Insert(x, head, length);
}

void  AVLTree::remove(int x) {
	assert(length != 0);
	head = remove(x, head, length);
}

int  AVLTree::size() {
	return length;
}


/*begin&end*/
 AVLTree::iterator  AVLTree::begin() {
	node *min = getMinNode(head);
	return iterator(head, min);
}

 AVLTree::iterator  AVLTree::end() {
	return iterator(head, nullptr);
}

/*print*/
void  AVLTree::inorder_print() {
	printf("Inorder:# ");
	inorder(head);
}


/*iterator implement*/
bool  AVLTree::iterator::operator==(const  AVLTree::iterator &x) {
	return this->root == x.root;
}

bool  AVLTree::iterator::operator!=(const  AVLTree::iterator &x) {
	return this->root != x.root;
}

 AVLTree::iterator  AVLTree::iterator::operator++(int) {
	iterator t = *this;
	root = getNext(head, root);
	return t;
}

 AVLTree::iterator & AVLTree::iterator::operator--() {
	root = getPre(head, root);
	return *this;
}

 AVLTree::iterator  AVLTree::iterator::operator--(int) {
	iterator t = *this;
	root = getPre(head, root);
	return t;
}

int  AVLTree::iterator::operator*() {
	return root->val;
}

 AVLTree::iterator & AVLTree::iterator::operator++() {
	root = getNext(head, root);
	return *this;
}
﻿// Copyright 2017 zhaochenyou16@gmail.com

#ifndef DATASTRUCTURE_AVLTREE_HPP_
#define DATASTRUCTURE_AVLTREE_HPP_

#include <algorithm>
#include <assert.h>

struct AVLTreeNode {
  /*节点下标号*/
  int index;
  /*节点高度值*/
  int depth;
  /*左右孩子节点*/
  AVLTreeNode *left;
  AVLTreeNode *right;
};

struct AVLTree {
  AVLTreeNode *root;
};

auto RotateLL(AVLTreeNode **e) -> void;
auto RotateRR(AVLTreeNode **e) -> void;
auto RotateLR(AVLTreeNode **e) -> void;
auto RotateRL(AVLTreeNode **e) -> void;
auto NodeFree(AVLTreeNode *e) -> void;
auto NodeInsert(AVLTreeNode **e, int index) -> void;
auto NodeFind(AVLTreeNode **e, int index) -> int;
auto NodeErase(AVLTreeNode **e, int index) -> void;
auto NodeDepth(AVLTreeNode *e) -> int;

auto AVLTreeNew() -> AVLTree* {
  AVLTree *t = new AVLTree();
  if (!t)
    return NULL;
  t->root = NULL;
  return t;
}

auto AVLTreeFree(AVLTree *t) -> void {
  NodeFree(t->root);
}

auto AVLTreeInsert(AVLTree *t, int index) -> void {
  if (t->root) {
    NodeInsert( &(t->root), index);
    return;
  }

  t->root = new AVLTreeNode();
  t->root->left = NULL;
  t->root->right = NULL;
  t->root->index = index;
  t->root->depth = 0;
}

auto AVLTreeFind(AVLTree *t, int index) -> int {
  return NodeFind( &(t->root), index);
}

auto AVLTreeErase(AVLTree *t, int index) -> void {
  NodeErase( &(t->root), index);
}

auto AVLTreeDepth(AVLTree *t) -> int {
  if (!t->root)
    return 0;
  return t->root->depth;
}

auto NodeFree(AVLTreeNode *e) -> void {
  if (!e)
    return;
  NodeFree(e->left);
  NodeFree(e->right);
  delete e;
}

auto RotateLL(AVLTreeNode **e) -> void {
  AVLTreeNode *e1;

  e1 = (*e)->left;
  (*e)->left = e1->right;
  e1->right = (*e);

  (*e)->depth = std::max( NodeDepth((*e)->left), NodeDepth((*e)->right) ) + 1;
  e1->depth = std::max( NodeDepth(e1->left), (*e)->depth ) + 1;
  (*e) = e1;
}

auto RotateRR(AVLTreeNode **e) -> void {
  AVLTreeNode *e1;

  e1 = (*e)->right;
  (*e)->right = e1->left;
  e1->left = (*e);

  (*e)->depth = std::max( NodeDepth((*e)->left) , NodeDepth((*e)->right) ) + 1;
  e1->depth = std::max( NodeDepth(e1->right), (*e)->depth ) + 1;
  (*e) = e1;
}

auto RotateLR(AVLTreeNode **e) -> void {
  RotateRR( &((*e)->left) );
  RotateLL( e );
}

auto RotateRL(AVLTreeNode **e) -> void {
  RotateLL( &((*e)->right) );
  RotateRR( e );
}

auto NodeInsert(AVLTreeNode **e, int index) -> void {
  assert(e);
  assert(*e);
  /*二分插入*/
  if ( (*e)->index > index ) {
    /*若做孩子节点为空节点 创建新的节点*/
    if ((*e)->left == NULL) {
      (*e)->left = new AVLTreeNode();
      (*e)->left->left = NULL;
      (*e)->left->right = NULL;
      (*e)->left->index = index;
      (*e)->left->depth = 0;
    } else {
      NodeInsert( &((*e)->left), index );
    }
    if ( NodeDepth((*e)->left) - NodeDepth((*e)->right) >= 2 ) {
      if ( (*e)->left->index > index ) {
        RotateLL( e );
      } else {
        RotateLR( e );
      }
    }
  } else if ( (*e)->index < index ) {
      /*若右孩子节点为空节点 创建新的节点*/
      if ((*e)->right == NULL) {
        (*e)->right = new AVLTreeNode();
        (*e)->right->left = NULL;
        (*e)->right->right = NULL;
        (*e)->right->index = index;
        (*e)->right->depth = 0;
      } else {
        NodeInsert(&((*e)->right), index);
      }
      if ( NodeDepth((*e)->right) - NodeDepth((*e)->left) >= 2 ) {
        if ( (*e)->right->index < index ) {
          RotateRR( e );
        } else {
          RotateRL( e );
        }
      }
  }

  (*e)->depth = std::max( NodeDepth((*e)->left), NodeDepth((*e)->right) ) + 1;
}

auto NodeFind(AVLTreeNode **e, int index) -> int {
  if (*e == NULL)
    return 0;
  /*二分查找*/
  if ( (*e)->index == index ) {
    return 1;
  } else if ( (*e)->index > index ) {
    return NodeFind( &((*e)->left), index );
  } else {
    return NodeFind( &((*e)->right) , index );
  }
}

auto NodeErase(AVLTreeNode **e, int index) -> void {
  if ( (*e)->index > index ) {
    NodeErase( &((*e)->left), index );

    if ( NodeDepth((*e)->right) - NodeDepth((*e)->left) >= 2 ) {
      if ( (*e)->right->left != NULL and ((*e)->right->left->depth > (*e)->right->right->depth) ) {
        RotateRL( e );
      } else {
        RotateRR( e );
      }
    }
  } else if ( (*e)->index < index ) {
    NodeErase( &((*e)->right), index );

    if ( NodeDepth((*e)->left) - NodeDepth((*e)->right) >= 2 ) {
      if ( (*e)->right->left != NULL and ((*e)->left->right->depth >(*e)->left->left->depth) ) {
        RotateLR( e );
      } else {
        RotateLL( e );
      }
    }
  } else {
    /* (*e)->index == index */
    if ( (*e)->left and (*e)->right ) {
      AVLTreeNode* temp = (*e)->right;

      /*temp指向节点的右儿子*/
      /*找到中序遍历的后继节点*/
      while ( temp->left != NULL )
        temp = temp->left;

      (*e)->index = temp->index; /*调整节点数据信息*/

      /*删除边缘节点*/
      NodeErase( &((*e)->right), temp->index );
      if ( NodeDepth((*e)->left) - NodeDepth((*e)->right) >= 2 ) {
        if ( (*e)->left->right != NULL and (NodeDepth((*e)->left->right) > NodeDepth((*e)->left->left)) ) {
          RotateLR( e );
        } else {
          RotateLL( e );
        }
      }
    } else {
      AVLTreeNode* temp = (*e);
      if( (*e)->left == NULL )
        (*e) = (*e)->right;
      else if ( (*e)->right == NULL )
        (*e) = (*e)->left;
      delete temp;
    }
  }

  if ( (*e) == NULL)
    return;

  (*e)->depth = std::max(NodeDepth((*e)->left), NodeDepth((*e)->right)) + 1;
  return;
}

/*空节点的高度值depth=-1*/
/*叶子节点的高度值depth=0*/
auto NodeDepth(AVLTreeNode *e) -> int {
  return e ? e->depth : -1;
}


#endif  // SRC_DATASTRUCTURE_AVLTREE_HPP_

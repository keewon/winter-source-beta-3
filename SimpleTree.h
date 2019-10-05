// SimpleTree.h: interface for the SimpleTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLETREE_H__E7CA741D_2C54_4D5C_AC49_6083F2FD403B__INCLUDED_)
#define AFX_SIMPLETREE_H__E7CA741D_2C54_4D5C_AC49_6083F2FD403B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SimpleTreeNode
{
	int data;
	SimpleTreeNode *left, *right;
};

class SimpleTree  
{
public:
	SimpleTree() { root=new SimpleTreeNode; root->left=root->right=0; current=root; }
	virtual ~SimpleTree();
	void SetCurrentdata(int x) { current->data=x; }
	void Deletecurrent();
private:
	SimpleTreeNode *root;
	SimpleTreeNode *current;
};

#endif // !defined(AFX_SIMPLETREE_H__E7CA741D_2C54_4D5C_AC49_6083F2FD403B__INCLUDED_)

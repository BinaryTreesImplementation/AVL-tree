#include<iostream>

template<typename T>
struct Node
{
	Node<T> * left;
	Node<T> * right;
	Node<T> * parent;
	T key;
	unsigned int height;
	Node(T const& value) : left{ nullptr }, right{ nullptr }, parent{ nullptr }, key{ value }, height{ 1 } {};
	Node(T const& value, Node<T> * parent) : left{ nullptr }, right{ nullptr }, parent{ parent }, key{ value }, height{ 1 } {};
};

template<typename T>
class AVL_tree
{
private:
	Node<T> * root;
	unsigned int count;

	void deleteNode(Node<T> * node);

	int bfactor(Node<T> * node);
	void fixheight(Node<T> * node);

	void rotate_left(Node<T> * node);
	void rotate_right(Node<T> * node);

	void balance(Node<T> * node);

	void insert(Node<T>* & node, const T& value);

	Node<T> * find_min(Node<T> * node);
	Node<T> * remove_min(Node<T> * node);
	void remove(Node<T>* & node, const T& value);

	void print(Node<T> * node, std::ostream & stream, size_t level)const;
public:
	AVL_tree();
	~AVL_tree();

	void insert(const T& value);

	void remove(const T& value);
	
	Node<T> * search(const T& value)const;

	void print(std::ostream & stream);
	
	unsigned int height_(Node<T> * node);
	
	Node<T> * getLeft(T key)
	{
		Node<T> * node = search(key);
		if (node)
			return node->left;
		return nullptr;
	}
	Node<T> * getRight(T key)
	{
		Node<T> * node = search(key);
		if (node)
			return node->right;
		return nullptr;
	}
	Node<T> * getParent(T key)
	{
		Node<T> * node = search(key);
		if (node)
			return node->parent;
		return nullptr;
	}
	Node<T> * getRoot()
	{
		return root;
	}
	unsigned int count_()
	{
		return count;
	}
};

template<typename T>
AVL_tree<T>::AVL_tree() : root{ nullptr }, count{ 0 } {};
template<typename T>
AVL_tree<T>::~AVL_tree()
{
	deleteNode(root);
}
template<typename T>
void AVL_tree<T>::deleteNode(Node<T> * node)
{
	if (node == nullptr) return;
	deleteNode(node->left);
	deleteNode(node->right);
	delete node;
}
template<typename T>
unsigned int AVL_tree<T>::height_(Node<T> * node)
{
	if (node)
		return node->height;
	return 0;
}
template<typename T>
int AVL_tree<T>::bfactor(Node<T> * node)
{
	return height_(node->right) - height_(node->left);
}
template<typename T>
void AVL_tree<T>::fixheight(Node<T> * node)
{
	unsigned int h_l =  height_(node->left);
	unsigned int h_r = height_(node->right);
	node->height = (h_l > h_r ? h_l : h_r) + 1;
}
template<typename T>
void AVL_tree<T>::rotate_left(Node<T> * node)
{
	Node<T> * r = node->right;
	if (node == root)
		root = r;
	node->right = r->left;
	if (r->left)
		r->left->parent = node;
	r->parent = node->parent;
	if (node->parent == nullptr)
		root = r;
	else if (node == node->parent->left)
		node->parent->left = r;
	else node->parent->right = r;
	r->left = node;
	node->parent = r;
	
	fixheight(node);
	fixheight(r);
}
template<typename T>
void AVL_tree<T>::rotate_right(Node<T> * node)
{
	Node<T> * l = node->left;
	if (node == root)
		root = l;
	node->left = l->right;
	if (l->right)
		l->right->parent = node;
	l->parent = node->parent;
	if (node->parent == nullptr)
		root = l;
	else if (node == node->parent->right)
		node->parent->right = l;
	else node->parent->left = l;
	l->right = node;
	node->parent = l;

	fixheight(node);
	fixheight(l);
}
template<typename T>
void  AVL_tree<T>::balance(Node<T> * node)
{
	fixheight(node);
	if (bfactor(node) == 2)
	{
		if (bfactor(node->right) < 0)
			rotate_right(node->right);
		rotate_left(node);
	}
	if (bfactor(node) == -2)
	{
		if (bfactor(node->left) > 0)
			rotate_left(node->left);
		rotate_right(node);
	}
}
template<typename T>
void AVL_tree<T>::insert(const T& value)
{
	insert(root, value);
}
template<typename T>
void AVL_tree<T>::insert(Node<T> * & node, const T& value)
{
	Node<T> * parent = nullptr;
	Node<T> * cur = node;
	while (cur)
	{
		parent = cur;
		if (value == cur->key)
			throw std::logic_error("There is this element in the tree\n");
		if (value < cur->key)
			cur = cur->left;
		else //if (value > node->value)
			cur = cur->right;
	}
	cur = new Node<T>(value, parent);
	count++;
	if (cur->parent == nullptr)
		root = cur;
	else
	{
		if (value < parent->key)
			parent->left = cur;
		else parent->right = cur;
	}
	while (cur && cur->parent)
	{
		balance(cur->parent);
		cur = cur->parent;
	}
}
template<typename T>
Node<T> * AVL_tree<T>::find_min(Node<T> * node)
{
	return node->left ? find_min(node->left) : node;
}
template<typename T>
void AVL_tree<T>::remove(const T& value)
{
	remove(root, value);
}
template<typename T>
void AVL_tree<T>::remove(Node<T>* & node, const T& value)
{
	if (node)
	{
		if (value < node->key)
			remove(node->left, value);
		else if (value > node->key)
			remove(node->right, value);
		else if (value == node->key)
		{
			Node<T> * cur_p = node->parent;
			if (!node->left && !node->right)
			{
				count--;
				if (node->parent->left == node)
				{
					node->parent->left = nullptr;
					//node->parent->height = 1;
					do
					{
						balance(cur_p);
						cur_p = cur_p->parent;
					} while (cur_p);
				}
				else
				{
					node->parent->right = nullptr;
					//node->parent->height = 1;
					do
					{
						balance(cur_p);
						cur_p = cur_p->parent;
					} while (cur_p);
				}
			}

			else if (node->left && !node->right)
			{
				count--;
				if (node->parent->left == node)
				{
					Node<T> * parent = node->parent;
					node->parent->left = node->left;
					node->parent = parent;
					while (cur_p)
					{
						balance(cur_p);
						cur_p = cur_p->parent;
					}
				}
				else
				{
					node->parent->right = node->left;
					node->left->parent = node->parent;
					while (cur_p)
					{
						balance(cur_p);
						cur_p = cur_p->parent;
					}
				}
			}
			else if (node->right)
			{
				Node<T> * min = find_min(node->right);
				T m = min->key;
				remove(min->key);
				node->key = m;
				while (cur_p)
				{
					balance(cur_p);
					cur_p = cur_p->parent;
				}
			}
		}
		else throw std::logic_error("Element isn't find/n");
	}
}
template<typename T>
Node<T> * AVL_tree<T>::search(const T& value)const
{
	Node<T> * curEl = root;
	while (curEl != nullptr)
	{
		if (curEl->key == value)
			break;
		else
		{
			if (value > curEl->key)
				curEl = curEl->right;
			else curEl = curEl->left;
		}
	}
	return curEl;
}
template<typename T>
void AVL_tree<T>::print(std::ostream & stream)
{
	print(root, stream, 0);
}
template<typename T>
void AVL_tree<T>::print(Node<T> * node, std::ostream & stream, size_t level)const
{
	Node<T> * curEl = node;
	if (curEl != nullptr)
	{
		print(curEl->right, stream, level + 1);
		for (unsigned int i = 0; i < level; ++i)
			stream << '-';
		stream << curEl->key << " " << curEl->height << std::endl;
		print(curEl->left, stream, level + 1);
	}
}

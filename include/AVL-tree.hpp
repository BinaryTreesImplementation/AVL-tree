#include<iostream>

template<typename T>
struct Node
{
	Node<T> * left_;
	Node<T> * right_;
	Node<T> * parent_;
	T key_;
	unsigned int height_;
	Node(T const& key) : left_{ nullptr }, right_{ nullptr }, parent_{ nullptr }, key_{ key }, height_{ 1 } {};
	Node(T const& key, Node<T> * parent_) : left_{ nullptr }, right_{ nullptr }, parent_{ parent_ }, key_{ key }, height_{ 1 } {};
};

template<typename T>
class AVL_tree
{
private:
	Node<T> * root_;
	unsigned int count_;

	void deleteNode_(Node<T> * node);

	int balanceFactor_(Node<T> * node);
	void fixHeight_(Node<T> * node);

	void rotateLeft_(Node<T> * node);
	void rotateRight_(Node<T> * node);

	void balance_(Node<T> * node);

	void insert_(Node<T>* & node, const T& key);

	Node<T> * findMin_(Node<T> * node);
	Node<T> * deleteMin_(Node<T> * node);
	void deleteElement_(Node<T>* & node, const T& key);

	std::ostream & print_(std::ostream & stream);
	std::ostream & print_(Node<T> * node, std::ostream & stream, size_t level)const;
public:
	AVL_tree();
	~AVL_tree();

	void insert(const T& key);

	void deleteElement(const T& key);

	Node<T> * search(const T& key)const;

	friend std::ostream & operator << (std::ostream&stream, AVL_tree<T> & tree)
	{
		return tree.print_(std::cout);
	}

	unsigned int height(Node<T> * node);

	Node<T> * left(T key_)
	{
		Node<T> * node = search(key_);
		if (node)
			return node->left_;
		return nullptr;
	}
	Node<T> * right(T key_)
	{
		Node<T> * node = search(key_);
		if (node)
			return node->right_;
		return nullptr;
	}
	Node<T> * parent(T key_)
	{
		Node<T> * node = search(key_);
		if (node)
			return node->parent_;
		return nullptr;
	}
	Node<T> * root()
	{
		return root_;
	}
	unsigned int count()
	{
		return count_;
	}
};

template<typename T>
AVL_tree<T>::AVL_tree() : root_{ nullptr }, count_{ 0 } {};
template<typename T>
AVL_tree<T>::~AVL_tree()
{
	deleteNode_(root_);
}
template<typename T>
void AVL_tree<T>::deleteNode_(Node<T> * node)
{
	if (node == nullptr) return;
	deleteNode_(node->left_);
	deleteNode_(node->right_);
	delete node;
}
template<typename T>
unsigned int AVL_tree<T>::height(Node<T> * node)
{
	if (node)
		return node->height_;
	return 0;
}
template<typename T>
int AVL_tree<T>::balanceFactor_(Node<T> * node)
{
	return height(node->right_) - height(node->left_);
}
template<typename T>
void AVL_tree<T>::fixHeight_(Node<T> * node)
{
	unsigned int leftHeight = height(node->left_);
	unsigned int rightHeight = height(node->right_);
	node->height_ = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}
template<typename T>
void AVL_tree<T>::rotateLeft_(Node<T> * node)
{
	Node<T> * right = node->right_;
	if (node == root_)
		root_ = right;
	node->right_ = right->left_;
	if (right->left_)
		right->left_->parent_ = node;
	right->parent_ = node->parent_;
	if (node->parent_ == nullptr)
		root_ = right;
	else if (node == node->parent_->left_)
		node->parent_->left_ = right;
	else node->parent_->right_ = right;
	right->left_ = node;
	node->parent_ = right;

	fixHeight_(node);
	fixHeight_(right);
}
template<typename T>
void AVL_tree<T>::rotateRight_(Node<T> * node)
{
	Node<T> * left = node->left_;
	if (node == root_)
		root_ = left;
	node->left_ = left->right_;
	if (left->right_)
		left->right_->parent_ = node;
	left->parent_ = node->parent_;
	if (node->parent_ == nullptr)
		root_ = left;
	else if (node == node->parent_->right_)
		node->parent_->right_ = left;
	else node->parent_->left_ = left;
	left->right_ = node;
	node->parent_ = left;

	fixHeight_(node);
	fixHeight_(left);
}
template<typename T>
void  AVL_tree<T>::balance_(Node<T> * node)
{
	fixHeight_(node);
	if (balanceFactor_(node) == 2)
	{
		if (balanceFactor_(node->right_) < 0)
			rotateRight_(node->right_);
		rotateLeft_(node);
	}
	if (balanceFactor_(node) == -2)
	{
		if (balanceFactor_(node->left_) > 0)
			rotateLeft_(node->left_);
		rotateRight_(node);
	}
}
template<typename T>
void AVL_tree<T>::insert(const T& key)
{
	insert_(root_, key);
}
template<typename T>
void AVL_tree<T>::insert_(Node<T> * & node, const T& key)
{
	Node<T> * parent = nullptr;
	Node<T> * cur = node;
	while (cur)
	{
		parent = cur;
		if (key == cur->key_)
			throw std::logic_error("There is this element in the tree\n");
		if (key < cur->key_)
			cur = cur->left_;
		else //if (key > node->key)
			cur = cur->right_;
	}
	cur = new Node<T>(key, parent);
	count_++;
	if (cur->parent_ == nullptr)
		root_ = cur;
	else
	{
		if (key < parent->key_)
			parent->left_ = cur;
		else parent->right_ = cur;
	}
	while (cur && cur->parent_)
	{
		balance_(cur->parent_);
		cur = cur->parent_;
	}
}
template<typename T>
Node<T> * AVL_tree<T>::findMin_(Node<T> * node)
{
	return node->left_ ? findMin_(node->left_) : node;
}
template<typename T>
void AVL_tree<T>::deleteElement(const T& key)
{
	deleteElement_(root_, key);
}
template<typename T>
void AVL_tree<T>::deleteElement_(Node<T>* & node, const T& key)
{
	if (node)
	{
		if (key < node->key_)
			deleteElement_(node->left_, key);
		else if (key > node->key_)
			deleteElement_(node->right_, key);
		else if (key == node->key_)
		{
			Node<T> * parent = node->parent_;
			if (!node->left_ && !node->right_)
			{
				count_--;
				if (node->parent_->left_ == node)
				{
					node->parent_->left_ = nullptr;
					//node->parent_->height_ = 1;
					do
					{
						balance_(parent);
						parent = parent->parent_;
					} while (parent);
				}
				else
				{
					node->parent_->right_ = nullptr;
					//node->parent_->height_ = 1;
					do
					{
						balance_(parent);
						parent = parent->parent_;
					} while (parent);
				}
			}

			else if (node->left_ && !node->right_)
			{
				count_--;
				if (node->parent_->left_ == node)
				{
					Node<T> * parent_ = node->parent_;
					node->parent_->left_ = node->left_;
					node->parent_ = parent_;
					while (parent)
					{
						balance_(parent);
						parent = parent->parent_;
					}
				}
				else
				{
					node->parent_->right_ = node->left_;
					node->left_->parent_ = node->parent_;
					while (parent)
					{
						balance_(parent);
						parent = parent->parent_;
					}
				}
			}
			else if (node->right_)
			{
				Node<T> * min = findMin_(node->right_);
				T minKey = min->key_;
				deleteElement(min->key_);
				node->key_ = minKey;
				while (parent)
				{
					balance_(parent);
					parent = parent->parent_;
				}
			}
		}
		else throw std::logic_error("Element isn't find/n");
	}
}
template<typename T>
Node<T> * AVL_tree<T>::search(const T& key)const
{
	Node<T> * cur = root_;
	while (cur != nullptr)
	{
		if (cur->key_ == key)
			break;
		else
		{
			if (key > cur->key_)
				cur = cur->right_;
			else cur = cur->left_;
		}
	}
	return cur;
}
template<typename T>
std::ostream & AVL_tree<T>::print_(std::ostream & stream)
{
	print_(root_, stream, 0);
	return stream;
}
template<typename T>
std::ostream & AVL_tree<T>::print_(Node<T> * node, std::ostream & stream, size_t level)const
{
	Node<T> * cur = node;
	if (cur != nullptr)
	{
		print_(cur->right_, stream, level + 1);
		for (unsigned int i = 0; i < level; ++i)
			stream << '-';
		stream << cur->key_ << " " << cur->height_ << std::endl;
		print_(cur->left_, stream, level + 1);
	}
	return stream;
}

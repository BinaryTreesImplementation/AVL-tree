#include<iostream>
#include "Counter.hpp"


template<typename T>
class AVL_tree
{
private:
	struct Node : public Counter
	{
		Node* left_;
		Node* right_;
		Node* parent_;
		T key_;
		unsigned int height_;
		Node(T const& key, Node* parent) : left_(nullptr), right_(nullptr), parent_(parent), key_(key), height_(1) { };
		~Node() {  }
		size_t getCount()
		{
			return Count();
		}
	}*root_;

	void deleteNode_(Node* node)
	{
		if (node == nullptr) return;
		deleteNode_(node->left_);
		deleteNode_(node->right_);
		delete node;
	}

	int balanceFactor_(Node* node)
	{
		return height(node->right_) - height(node->left_);
	}

	void fixHeight_(Node* node)
	{
		unsigned int leftHeight = height(node->left_);
		unsigned int rightHeight = height(node->right_);
		node->height_ = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
	}

	void rotateLeft_(Node* node)
	{
		Node* right = node->right_;
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

	void rotateRight_(Node* node)
	{
		Node* left = node->left_;
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

	void balance_(Node* node)
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

	void insert_(Node* & node, const T& key)
	{
		Node* parent = nullptr;
		Node* cur = node;
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
		cur = new Node(key, parent);
		//count_++;
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

	Node* findMin_(Node* node)
	{
		return node->left_ ? findMin_(node->left_) : node;
	}




	std::ostream & print_(std::ostream & stream)
	{
		print_(root_, stream, 0);
		return stream;
	}

	std::ostream & print_(Node* node, std::ostream & stream, size_t level)const
	{
		Node* cur = node;
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
public:
	AVL_tree() : root_{ nullptr } {};

	~AVL_tree() { deleteNode_(root_); }

	void insert(const T& key)
	{
		insert_(root_, key);
	}

	void deleteElement(const T& key)
	{
		Node *removing = search(key);
		if (removing)
		{
			Node* parent = removing->parent_;
			if (!removing->left_ && !removing->right_)
			{
				if (removing != root_)
				{
					if (removing->parent_->left_ == removing)
					{
						removing->parent_->left_ = nullptr;
					}
					else
					{
						removing->parent_->right_ = nullptr;
					}
					do
					{
						balance_(parent);
						parent = parent->parent_;
					} while (parent);
				}
				else
				{
					root_ = nullptr;
				}
				delete removing;
				return;
			}
			else if (removing->left_ && !removing->right_)
			{
				if (removing->parent_->left_ == removing)
				{
					removing->parent_->left_ = removing->left_;
					removing->left_->parent_ = parent;
				}
				else
				{
					removing->parent_->right_ = removing->left_;
					removing->left_->parent_ = removing->parent_;
				}
				while (parent)
				{
					balance_(parent);
					parent = parent->parent_;
				}
				delete removing;
				return;
			}
			else if (removing->right_)
			{
				Node* min = findMin_(removing->right_);
				T minKey = min->key_;
				deleteElement(min->key_);
				removing->key_ = minKey;
				while (parent)
				{
					balance_(parent);
					parent = parent->parent_;
				}
			}
		}
	}

	Node* search(const T& key)const
	{
		Node* cur = root_;
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

	friend std::ostream & operator << (std::ostream&stream, AVL_tree<T> & tree)
	{
		return tree.print_(std::cout);
	}

	unsigned int height(Node* node)
	{
		if (node)
			return node->height_;
		return 0;
	}

	Node* left(T key_)
	{
		Node* node = search(key_);
		if (node)
			return node->left_;
		return nullptr;
	}

	Node* right(T key_)
	{
		Node* node = search(key_);
		if (node)
			return node->right_;
		return nullptr;
	}

	Node* parent(T key_)
	{
		Node* node = search(key_);
		if (node)
			return node->parent_;
		return nullptr;
	}

	Node* root()
	{
		return root_;
	}

	size_t count()
	{
		return root_->getCount();
	}
};

#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED
#include <vector>
#include <iostream>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
private:
    struct Node {
        KeyType key;
        std::vector<ValueType> values;
        Node* less;
        Node* greater;

        Node(KeyType insertkey, ValueType value) { //constructor for creating new Node with key and value
            key = insertkey;
            values.push_back(value);
            less = nullptr;
            greater = nullptr;
        }
    };
    Node* root;

    void deleteNode(Node* killMe) { //deletes every Node in the tree
        if (killMe == nullptr)
            return;

        deleteNode(killMe->less);
        deleteNode(killMe->greater);
        delete killMe;
    }
  public:
    class Iterator
    {
      public:
        Iterator() //creates invalid Iterator
        {
            m_ptr = nullptr;
            index = -1;
        }

        Iterator(Node* pt) { //creates Iterator pointing at first value of Node* pt
            m_ptr = pt;
            index = 0;
        }

        ValueType& get_value() const 
        {
            return m_ptr->values[index];
        }

        bool is_valid() const
        {
            return index != -1;
        }

        void advance() 
        {
            if (m_ptr == nullptr || index == m_ptr->values.size() - 1) { //makes sure that Iterator isn't pointing at end of the vector of values
                index = -1;
                return;
            }
            index++;
        }

      private:
          Node* m_ptr;
          int index;
    };

    TreeMultimap()
    {
        root = nullptr;
    }

    ~TreeMultimap()
    {
        Node* cur = root;
        deleteNode(cur); //calls helper method to delete
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        Node* add;
        if (root == nullptr) { //adds root node if there is nothing in tree
            add = new Node(key, value);
            root = add;
            return;
        }

        Node* cur = root;
        for (;;) { //continues till add
            if (key == cur->key) { //only add to value vector if key alr exists in tree
                cur->values.push_back(value);
                return;
            }
            
            //add in right spot of tree
            if (key < cur->key) { 
                if (cur->less != nullptr)
                    cur = cur->less;
                else {
                    cur->less = new Node(key, value);
                    return;
                }
            }
            else if (key > cur->key) {
                if (cur->greater != nullptr)
                    cur = cur->greater;
                else {
                    cur->greater = new Node(key, value);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* cur = root;
        while (cur != nullptr) {
            if (cur->key == key)
                return Iterator(cur); //return Iterator pointing to values of cur if key is found in tree
            else if (key < cur->key) {
                cur = cur->less;
            }
            else
                cur = cur->greater;
        }
        return Iterator(); //return invalid iterator 
    }
};

#endif // TREEMULTIMAP_INCLUDED

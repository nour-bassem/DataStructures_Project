#include <typeinfo>
#include <string>
using namespace std;

#define SIZE 5

class HashTable
{
private:
    typedef string Key;
    typedef string Value;
    class Node {
    public:

        Key key;
        Value value;
        Node* next;

        Node()
            : next(0)
        {}

        Node(const Key& k, const Value& v) 
            : key(k), value(v), next(nullptr)
        {}
    };

    typedef Node* NodePtr;
    NodePtr myTable[SIZE];
public:

    HashTable();

    ~HashTable();

    int hashfunc(const Key& key) const;

    Node* find(const Key& key) ;

    bool empty() const;

    void insert(const Key& key, const Value& value);

    Value* getValue(const Key& key);

    bool remove(const Key& key);

    void display(ostream& out) const;
};



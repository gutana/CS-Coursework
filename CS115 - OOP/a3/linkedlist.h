#ifndef linkedlist_h
#define linkedlist_h

template <class T>
class LinkedList;

template <class T>
class ListElement
{
    T datum;
    ListElement* next;

    ListElement (T const&, ListElement*);
public:
    T const& Datum () const;
    ListElement const* Next () const;

    friend LinkedList<T>;
};

template <class T>
class LinkedList
{
    ListElement<T>* head;
    ListElement<T>* tail;
public:
    /* Constructor and Destructor */
    LinkedList ();
    ~LinkedList ();

    /* Copy Constructor and =operator overloading */
    LinkedList (LinkedList const&);
    LinkedList& operator = (LinkedList const&);

    /**
     * Head() returns a pointer point to the first ListElement 
     * Tail() returns a pointer point to the last ListElement
     * IsEmpty() returns boolean, which used for checking IsEmpty
     * First() returns an element stored in the first ListElement
     * Last() returns an element stored in the last ListElement
     */
    ListElement<T> const* Head () const;
    ListElement<T> const* Tail () const;
    bool IsEmpty () const;
    T const& First () const;
    T const& Last () const;

    /**
     * Prepend func adds a new element at the front-end of the list
     * Append func adds a new element at the tail-end of the list
     * Extract func searches an element and deletes that element
     * Purge() traverses all the elements and deletes each of them one by one
     * InsertAfter func takes two arguments: 1. a point to an element; 2. a reference to the item to be inserted into the list. The InsertAfter func is almost identical to Append func.
     * InsertBefore func is similar to InsertAfter func takes two arguments: 1. a point to an element; 2. a reference to the item to be inserted into the list. And it inserts that element before the current ListElement.
     */
    void Prepend (T const&);
    void Append (T const&);
    void Extract (T const&);
    void Purge ();
    void InsertAfter (ListElement<T> const*, T const&);
    void InsertBefore (ListElement<T> const*, T const&);
};

#endif

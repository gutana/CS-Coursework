#ifndef container_h
#define container_h

class Container
{
protected:
    unsigned int count;

    Container();

public:
    virtual unsigned int Count() const;
    virtual bool IsEmpty() const;
    virtual bool IsFull() const;

    /* Purge() deletes the elements in the container. */
    virtual void Purge() = 0;

    /* ... */
};

#endif

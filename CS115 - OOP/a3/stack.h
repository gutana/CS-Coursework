#ifndef stack_h
#define stack_h

#include "container.h"
#include "rainfall.h"

class Rainfall;

class Stack : public virtual Container
{
public:
    /**
     * Top func returns the top object of the Stack.
     * Push func pushes Rainfall object into the Stack.
     * Pop func pops Rainfall object out of the Stack.
     */
    virtual Rainfall& Top () const = 0;
    virtual void Push (Rainfall&) = 0;
    virtual void Pop () = 0;
};
#endif /* stack_h */

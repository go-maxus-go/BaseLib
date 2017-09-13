#pragma once

#include <list>

namespace Obs {

template<typename OBJ> class Obs;

template<typename OBJ>
class Chain
{
public:
    Chain(Obs<OBJ> * parent, const OBJ * obj)
        : m_parent(parent), m_copy(*obj)
    {}
    ~Chain()
    {
        // send change from m_copy to m_origin
    }

    OBJ * operator->() { return &m_copy; }
    operator OBJ * ()  { return &m_copy; }
private:
    const OBJ * m_origin;
    OBJ m_copy;
    Obs<OBJ> * m_parent;
};

template<typename OBJ>
class Obs
{
public:
    Obs()
    {
        // register observer
    }
public:
    Chain<OBJ> add(OBJ * obj = nullptr)
    {
        // add object
        return std::move(Chain(obj));
    }
    Chain<OBJ> change(OBJ * obj = nullptr)
    {
        return std::move(Chain(obj));
    }
    void remove(OBJ * obj = nullptr)
    {

    }

    const OBJ * object() const
        { return *m_objects.begin();}
    std::list<const OBJ *> objects() const
        { return m_objects;}

    void observe(const OBJ * obj)
    {

    }
    void observeAll() {}
    void forget(const OBJ * obj)
    {

    }
    void forgetAll() {}
protected:
    virtual void added  (const OBJ *) {}
    virtual void removed(const OBJ *) {}
    virtual void changed(const OBJ *) {}
    virtual void changed(const OBJ * from, const OBJ * to) {}
private:
    std::list<OBJ*> m_objects;
};

}

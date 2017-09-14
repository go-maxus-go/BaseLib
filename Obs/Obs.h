#pragma once

#include <set>
#include <iterator>
#include <algorithm>

#include "PubSub/Subscriber.h"
#include "PubSub/Publisher.h"

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

template<typename OBJ> struct AddRequest {};
template<typename OBJ> struct AddedNotification
{
    AddedNotification(OBJ * obj) : obj(obj) {}
    OBJ * obj = nullptr;
};

template<typename OBJ>
class Obs : PubSub::Subscriber<AddRequest<OBJ>, AddedNotification<OBJ>>
{
public:
    Obs(bool ctrl = false) : m_ctrl(ctrl) {}
public:
    Chain<OBJ> add()
    {
        m_waitAddNotification = true;
        PubSub::publish(AddRequest<OBJ>());
        m_waitAddNotification = false;
        auto obj = m_addedObj;
        m_addedObj = nullptr;
        if (obj)
            m_objects.insert(obj);
        return std::move(Chain<OBJ>(this, obj));
    }
    Chain<OBJ> change(OBJ * obj = nullptr)
    {
        return std::move(Chain<OBJ>(this, obj));
    }
    void remove(const OBJ * obj)
    {

    }

    const OBJ * object() const
        { return *m_objects.begin();}
    std::list<const OBJ *> objects() const
    {
        std::list<const OBJ *> res;
        std::copy(m_objects.cbegin(), m_objects.end(), std::back_inserter(res));
        return std::move(res);
    }

    void observe(const OBJ * obj) {}
    void observeAll() {}
    void forget(const OBJ * obj) {}
    void forgetAll() {}
protected:
    void notify(const AddRequest<OBJ> & r) final
        { if(m_ctrl) addRequested(nullptr); }
    void notify(const AddedNotification<OBJ> & r) final
    {
        if (m_waitAddNotification)
            m_addedObj = r.obj;
        added(r.obj);
    }
protected:
    virtual void added  (const OBJ *) {}
    virtual void removed(const OBJ *) {}
    virtual void changed(const OBJ *) {}
    virtual void changed(const OBJ * from, const OBJ * to) {}
protected:
    virtual void addRequested(OBJ * obj)
    {
        if (obj == nullptr)
            obj = new OBJ;
        m_objects.insert(obj);
        PubSub::publish(AddedNotification<OBJ>(obj));
    }
    virtual void changeRequested(OBJ *, OBJ * to) {}
    virtual void removeRequested(OBJ *) {}
private:
    std::set<OBJ*> m_objects;
    const bool m_ctrl = false;

    bool m_waitAddNotification = false;
    OBJ * m_addedObj = nullptr;
};

}

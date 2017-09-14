#pragma once

#include <set>
#include <cassert>
#include <iterator>
#include <algorithm>

#include "PubSub/Subscriber.h"
#include "PubSub/Publisher.h"

namespace Obs {

template<typename OBJ> struct AddRequest {};
template<typename OBJ> struct AddedNotification
{
    AddedNotification(OBJ * obj) : obj(obj) {}
    OBJ * obj = nullptr;
};

template<typename OBJ> struct ChangeRequest
{
    ChangeRequest(const OBJ * from, OBJ * to) : from(from), to(to) {}
    const OBJ * from = nullptr;
    OBJ * to = nullptr;
};
template<typename OBJ> struct ChangedNotification
{
    ChangedNotification(const OBJ * from, const OBJ * to)
        : from(from), to(to)
    {}
    const OBJ * from = nullptr;
    const OBJ * to   = nullptr;
};

template<typename OBJ> struct RemoveRequest
{
    RemoveRequest(const OBJ * obj) : obj(obj) {}
    const OBJ * obj = nullptr;
};
template<typename OBJ> struct RemovedNotification
{
    RemovedNotification(const OBJ * obj) : obj(obj) {}
    const OBJ * obj = nullptr;
};

template<typename OBJ>
class Chain : public OBJ
{
public:
    Chain(const OBJ * obj) : OBJ(*obj), m_origin(obj) {}
    ~Chain() { PubSub::publish(ChangeRequest<OBJ>(m_origin, this)); }

    OBJ * operator -> () { return this;}
private:
    const OBJ * m_origin = nullptr;
};

template<typename OBJ>
class Obs : PubSub::Subscriber<
    AddRequest   <OBJ>, AddedNotification  <OBJ>,
    ChangeRequest<OBJ>, ChangedNotification<OBJ>,
    RemoveRequest<OBJ>, RemovedNotification<OBJ>>
{
public:
    Obs(bool ctrl = false) : m_ctrl(ctrl) {}
    ~Obs() { assert(m_objects.empty()); }
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
        return Chain<OBJ>(obj);
    }
    Chain<OBJ> change(const OBJ * obj = nullptr)
    {
        if (!obj)
            obj = object();
        return Chain<OBJ>(obj);
    }
    void remove(const OBJ * obj)
    {
        m_objects.erase(m_objects.find(obj));
        PubSub::publish(RemoveRequest<OBJ>(obj));
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
    void notify(const AddRequest<OBJ> &) final
        { if(m_ctrl) addRequested(nullptr); }
    void notify(const AddedNotification<OBJ> & r) final
    {
        if (m_waitAddNotification)
            m_addedObj = r.obj;
        added(r.obj);
    }
    void notify(const ChangeRequest<OBJ> & r) final
        { if (m_ctrl) changeRequested(r.from, r.to); }
    void notify(const ChangedNotification<OBJ> & r) final
        { changed(r.from, r.to); changed(r.to); }
    void notify(const RemoveRequest<OBJ> & r) final
        { if (m_ctrl) removeRequested(r.obj, false); }
    void notify(const RemovedNotification<OBJ> & r) final
        { removed(r.obj); }
protected:
    virtual void added  (const OBJ *) {}
    virtual void removed(const OBJ *) {}
    virtual void changed(const OBJ *) {}
    virtual void changed(const OBJ * /*from*/, const OBJ * /*to*/) {}
protected:
    virtual void addRequested(OBJ * obj)
    {
        if (obj == nullptr)
            obj = new OBJ;
        m_objects.insert(obj);
        PubSub::publish(AddedNotification<OBJ>(obj));
    }
    virtual void changeRequested(const OBJ * from, OBJ * to)
    {
        auto f = const_cast<OBJ*>(from);
        auto temp = *f;
        *f = *to;
        *to = temp;
        PubSub::publish(ChangedNotification<OBJ>(to, from));
    }
    virtual void removeRequested(const OBJ * obj, bool isDeleted)
    {
        m_objects.erase(obj);
        if (!isDeleted)
            delete obj;
        PubSub::publish(RemovedNotification<OBJ>(obj));
    }
private:
    std::set<const OBJ*> m_objects;
    const bool m_ctrl = false;

    bool m_waitAddNotification = false;
    const OBJ * m_addedObj = nullptr;
};

}

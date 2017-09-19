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
    ChangeRequest(const OBJ * obj, const OBJ & to) : obj(obj), to(to) {}
    const OBJ * obj = nullptr;
    const OBJ & to;
};
template<typename OBJ> struct ChangedNotification
{
    ChangedNotification(const OBJ * obj, const OBJ * from, const OBJ * to)
        : obj(obj), from(from), to(to)
    {}
    const OBJ * obj  = nullptr;
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
class Controller : PubSub::Subscriber<AddRequest<OBJ>,
    ChangeRequest<OBJ>, RemoveRequest<OBJ>>
{
public:
    virtual ~Controller()
        { assert(m_objects.empty()); }
protected:
    virtual OBJ * addRequested(OBJ * obj = nullptr)
    {
        if (obj == nullptr)
            obj = new OBJ;
        m_objects.insert(obj);
        PubSub::publish(AddedNotification<OBJ>(obj));
        return obj;
    }
    virtual void changeRequested(OBJ * obj, const OBJ & to)
    {
        const OBJ from = *obj;
        *obj = to;
        PubSub::publish(ChangedNotification<OBJ>(obj, &from, &to));
    }
    virtual void removeRequested(const OBJ * obj, bool del  = true)
    {
        m_objects.erase(obj);
        if (del)
            delete obj;
        PubSub::publish(RemovedNotification<OBJ>(obj));
    }

    const OBJ * object() const { return *m_objects.begin(); }
    std::set<const OBJ*> objects() const { return m_objects; }
private:
    void notify(const AddRequest<OBJ> &) final
        { addRequested(); }
    void notify(const ChangeRequest<OBJ> & r) final
        { changeRequested(const_cast<OBJ*>(r.obj), r.to); }
    void notify(const RemoveRequest<OBJ> & r) final
        { removeRequested(r.obj); }
private:
    std::set<const OBJ*> m_objects;
};

template<typename OBJ>
class View : PubSub::Subscriber<AddedNotification <OBJ>,
    ChangedNotification<OBJ>, RemovedNotification<OBJ>>
{
public:
    virtual ~View() { assert(m_objects.empty()); }
public:
    const OBJ * add()
    {
        m_waitAdd = true;
        PubSub::publish(AddRequest<OBJ>());
        m_waitAdd = false;
        auto obj = m_addedObj;
        m_addedObj = nullptr;
        if (obj)
            m_objects.insert(obj);
        return obj;
    }
    void change(const OBJ * from, const OBJ & to)
    {
        assert(from && m_objects.find(from) != m_objects.end());
        m_waitChange = true;
        PubSub::publish(ChangeRequest<OBJ>(from, to));
        m_waitChange = false;
    }
    void remove(const OBJ * obj)
    {
        m_objects.erase(obj);
        m_waitRemove = true;
        PubSub::publish(RemoveRequest<OBJ>(obj));
        m_waitRemove = false;
    }

    const OBJ * object() const
        { return *m_objects.begin();}
    std::set<const OBJ *> objects() const
    {
        return m_objects;
    }

    void observe(const OBJ * obj)
        { assert(obj); m_objects.insert(obj); }
    void observeAll(bool value = false)
        { m_observeAll = value; }
    void forget(const OBJ * obj)
        { assert(obj); m_objects.erase(obj); }
    void forgetAll()
        { m_objects.clear(); }
protected:
    virtual void added  (const OBJ *) {}
    virtual void removed(const OBJ *) {}
    virtual void changed(const OBJ *) {}
    virtual void changed(const OBJ * /*obj*/,
                         const OBJ * /*from*/,
                         const OBJ * /*to*/) {}
protected:
    void notify(const AddedNotification<OBJ> & r) final
    {
        if (m_waitAdd)
        {
            m_addedObj = r.obj;
            return;
        }
        if (m_observeAll)
        {
            m_objects.insert(r.obj);
            added(r.obj);
        }
    }
    void notify(const ChangedNotification<OBJ> & r) final
    {
        if (m_waitChange)
            return;
        if (m_objects.find(r.obj) != m_objects.end())
        {
            changed(r.obj, r.from, r.to);
            changed(r.obj);
        }
    }
    void notify(const RemovedNotification<OBJ> & r) final
    {
        if (m_objects.find(r.obj) != m_objects.end())
        {
            m_objects.erase(r.obj);
            removed(r.obj);
        }
    }
private:
    std::set<const OBJ*> m_objects;
    const OBJ * m_addedObj = nullptr;
    bool m_waitAdd    = false;
    bool m_waitChange = false;
    bool m_waitRemove = false;
    bool m_observeAll = false;
};

}

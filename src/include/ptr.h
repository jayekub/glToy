#ifndef _PTR_H_
#define _PTR_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#define DEF_SHARED_PTR(type) \
    class type; \
    typedef boost::shared_ptr<type> type##Ptr; \
    typedef boost::shared_ptr<const type> type##ConstPtr; \
    typedef boost::weak_ptr<type> type##WeakPtr; \
    typedef boost::weak_ptr<const type> type##ConstWeakPtr;

#define DEF_SCOPED_PTR(type) \
    class type; \
    typedef boost::scoped_ptr<type> type##Ptr; \
    typedef boost::scoped_ptr<const type> type##ConstPtr;

#define DEF_CREATE(type) \
    static type##Ptr create() { return type##Ptr(new type()); }

#define DEF_CREATE_1(type, arg1_type, arg1) \
    static type##Ptr create(arg1_type arg1) { \
        return type##Ptr(new type(arg1)); }

#endif // _PTR_H_

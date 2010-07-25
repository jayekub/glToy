#ifndef RELOADABLE_H_
#define RELOADABLE_H_

#include <set>

#include <boost/foreach.hpp>

class Reloadable
{
public:
    Reloadable();
    virtual ~Reloadable();

    virtual void reload() = 0;

    static void reloadAll();

private:
    static std::set<Reloadable *> _reloadables;
};

#endif /* RELOADABLE_H_ */

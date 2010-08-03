#ifndef LISTENER_H_
#define LISTENER_H_

#include <set>

#include <boost/foreach.hpp>

class Listener
{
public:
    Listener();
    virtual ~Listener();

    virtual void reload() {};
    virtual void resize(int width, int height) {};

    static void reloadAll();
    static void resizeAll(int width, int height);

private:
    static std::set<Listener *> _listeners;
};

#endif /* LISTENER_H_ */

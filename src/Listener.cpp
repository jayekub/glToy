#include "Listener.h"

std::set<Listener *> Listener::_listeners;

Listener::Listener() {
    _listeners.insert(this);
};

Listener::~Listener() {
    _listeners.erase(this);
};

void Listener::reloadAll() {
    BOOST_FOREACH(Listener *listener, _listeners) {
        listener->reload();
    }
}

void Listener::resizeAll(int width, int height) {
    BOOST_FOREACH(Listener *listener, _listeners) {
        listener->resize(width, height);
    }
}

#include "Reloadable.h"

std::set<Reloadable *> Reloadable::_reloadables;

Reloadable::Reloadable() {
    _reloadables.insert(this);
};

Reloadable::~Reloadable() {
    _reloadables.erase(this);
};

void Reloadable::reloadAll() {
    BOOST_FOREACH(Reloadable *reloadable, _reloadables) {
        reloadable->reload();
    }
}

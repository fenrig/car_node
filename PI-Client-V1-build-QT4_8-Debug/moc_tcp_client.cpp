/****************************************************************************
** Meta object code from reading C++ file 'tcp_client.h'
**
** Created: Fri Mar 1 11:04:44 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../PI-Client-V1/tcp_client.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcp_client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tcp_client[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      27,   11,   11,   11, 0x08,
      39,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_tcp_client[] = {
    "tcp_client\0\0on_connected()\0read_data()\0"
    "on_disconnected()\0"
};

void tcp_client::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        tcp_client *_t = static_cast<tcp_client *>(_o);
        switch (_id) {
        case 0: _t->on_connected(); break;
        case 1: _t->read_data(); break;
        case 2: _t->on_disconnected(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData tcp_client::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject tcp_client::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_tcp_client,
      qt_meta_data_tcp_client, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &tcp_client::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *tcp_client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *tcp_client::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_tcp_client))
        return static_cast<void*>(const_cast< tcp_client*>(this));
    return QObject::qt_metacast(_clname);
}

int tcp_client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
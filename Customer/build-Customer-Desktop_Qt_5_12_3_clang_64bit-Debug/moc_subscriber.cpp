/****************************************************************************
** Meta object code from reading C++ file 'subscriber.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Customer/subscriber.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'subscriber.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Subscriber_t {
    QByteArrayData data[25];
    char stringdata0[301];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Subscriber_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Subscriber_t qt_meta_stringdata_Subscriber = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Subscriber"
QT_MOC_LITERAL(1, 11, 9), // "send_data"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 4), // "info"
QT_MOC_LITERAL(4, 27, 5), // "login"
QT_MOC_LITERAL(5, 33, 4), // "data"
QT_MOC_LITERAL(6, 38, 11), // "new_connect"
QT_MOC_LITERAL(7, 50, 12), // "read_message"
QT_MOC_LITERAL(8, 63, 12), // "send_message"
QT_MOC_LITERAL(9, 76, 13), // "display_error"
QT_MOC_LITERAL(10, 90, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(11, 119, 14), // "temperature_up"
QT_MOC_LITERAL(12, 134, 16), // "temperature_down"
QT_MOC_LITERAL(13, 151, 12), // "speed_change"
QT_MOC_LITERAL(14, 164, 12), // "state_change"
QT_MOC_LITERAL(15, 177, 11), // "mode_change"
QT_MOC_LITERAL(16, 189, 9), // "check_out"
QT_MOC_LITERAL(17, 199, 8), // "re_fresh"
QT_MOC_LITERAL(18, 208, 8), // "moni_tor"
QT_MOC_LITERAL(19, 217, 16), // "temperature_send"
QT_MOC_LITERAL(20, 234, 11), // "close_event"
QT_MOC_LITERAL(21, 246, 12), // "QCloseEvent*"
QT_MOC_LITERAL(22, 259, 5), // "event"
QT_MOC_LITERAL(23, 265, 19), // "on_set_temp_clicked"
QT_MOC_LITERAL(24, 285, 15) // "on_exit_clicked"

    },
    "Subscriber\0send_data\0\0info\0login\0data\0"
    "new_connect\0read_message\0send_message\0"
    "display_error\0QAbstractSocket::SocketError\0"
    "temperature_up\0temperature_down\0"
    "speed_change\0state_change\0mode_change\0"
    "check_out\0re_fresh\0moni_tor\0"
    "temperature_send\0close_event\0QCloseEvent*\0"
    "event\0on_set_temp_clicked\0on_exit_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Subscriber[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,  107,    2, 0x08 /* Private */,
       6,    0,  110,    2, 0x08 /* Private */,
       7,    0,  111,    2, 0x08 /* Private */,
       8,    1,  112,    2, 0x08 /* Private */,
       9,    1,  115,    2, 0x08 /* Private */,
      11,    0,  118,    2, 0x08 /* Private */,
      12,    0,  119,    2, 0x08 /* Private */,
      13,    0,  120,    2, 0x08 /* Private */,
      14,    0,  121,    2, 0x08 /* Private */,
      15,    0,  122,    2, 0x08 /* Private */,
      16,    0,  123,    2, 0x08 /* Private */,
      17,    0,  124,    2, 0x08 /* Private */,
      18,    0,  125,    2, 0x08 /* Private */,
      19,    0,  126,    2, 0x08 /* Private */,
      20,    1,  127,    2, 0x08 /* Private */,
      23,    0,  130,    2, 0x08 /* Private */,
      24,    0,  131,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Subscriber::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Subscriber *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_data((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 1: _t->login((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->new_connect(); break;
        case 3: _t->read_message(); break;
        case 4: _t->send_message((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 5: _t->display_error((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 6: _t->temperature_up(); break;
        case 7: _t->temperature_down(); break;
        case 8: _t->speed_change(); break;
        case 9: _t->state_change(); break;
        case 10: _t->mode_change(); break;
        case 11: _t->check_out(); break;
        case 12: _t->re_fresh(); break;
        case 13: _t->moni_tor(); break;
        case 14: _t->temperature_send(); break;
        case 15: _t->close_event((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 16: _t->on_set_temp_clicked(); break;
        case 17: _t->on_exit_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Subscriber::*)(QJsonObject );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Subscriber::send_data)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Subscriber::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_Subscriber.data,
    qt_meta_data_Subscriber,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Subscriber::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Subscriber::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Subscriber.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Subscriber::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void Subscriber::send_data(QJsonObject _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

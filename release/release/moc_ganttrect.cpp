/****************************************************************************
** Meta object code from reading C++ file 'ganttrect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ganttrect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ganttrect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GanttRect_t {
    QByteArrayData data[6];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GanttRect_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GanttRect_t qt_meta_stringdata_GanttRect = {
    {
QT_MOC_LITERAL(0, 0, 9), // "GanttRect"
QT_MOC_LITERAL(1, 10, 11), // "iWasClicked"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 5), // "index"
QT_MOC_LITERAL(4, 29, 10), // "hoverStart"
QT_MOC_LITERAL(5, 40, 10) // "hoverLeave"

    },
    "GanttRect\0iWasClicked\0\0index\0hoverStart\0"
    "hoverLeave"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GanttRect[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,
       5,    1,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void GanttRect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GanttRect *_t = static_cast<GanttRect *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->iWasClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->hoverStart((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->hoverLeave((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GanttRect::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GanttRect::iWasClicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (GanttRect::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GanttRect::hoverStart)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (GanttRect::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GanttRect::hoverLeave)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject GanttRect::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GanttRect.data,
      qt_meta_data_GanttRect,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GanttRect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GanttRect::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GanttRect.stringdata0))
        return static_cast<void*>(const_cast< GanttRect*>(this));
    if (!strcmp(_clname, "QGraphicsRectItem"))
        return static_cast< QGraphicsRectItem*>(const_cast< GanttRect*>(this));
    return QObject::qt_metacast(_clname);
}

int GanttRect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GanttRect::iWasClicked(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GanttRect::hoverStart(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GanttRect::hoverLeave(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

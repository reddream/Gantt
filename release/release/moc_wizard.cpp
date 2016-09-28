/****************************************************************************
** Meta object code from reading C++ file 'wizard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../wizard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Wizard_t {
    QByteArrayData data[22];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Wizard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Wizard_t qt_meta_stringdata_Wizard = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Wizard"
QT_MOC_LITERAL(1, 7, 20), // "createRepresentation"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 7), // "addFlow"
QT_MOC_LITERAL(4, 37, 9), // "unit1name"
QT_MOC_LITERAL(5, 47, 7), // "op1name"
QT_MOC_LITERAL(6, 55, 9), // "unit2name"
QT_MOC_LITERAL(7, 65, 7), // "op2name"
QT_MOC_LITERAL(8, 73, 2), // "pr"
QT_MOC_LITERAL(9, 76, 2), // "cr"
QT_MOC_LITERAL(10, 79, 6), // "amount"
QT_MOC_LITERAL(11, 86, 7), // "addTask"
QT_MOC_LITERAL(12, 94, 8), // "unitName"
QT_MOC_LITERAL(13, 103, 8), // "taskName"
QT_MOC_LITERAL(14, 112, 5), // "start"
QT_MOC_LITERAL(15, 118, 3), // "end"
QT_MOC_LITERAL(16, 122, 5), // "color"
QT_MOC_LITERAL(17, 128, 5), // "reset"
QT_MOC_LITERAL(18, 134, 15), // "loadFileClicked"
QT_MOC_LITERAL(19, 150, 17), // "loadConfigClicked"
QT_MOC_LITERAL(20, 168, 12), // "selectSysdir"
QT_MOC_LITERAL(21, 181, 17) // "saveConfiguration"

    },
    "Wizard\0createRepresentation\0\0addFlow\0"
    "unit1name\0op1name\0unit2name\0op2name\0"
    "pr\0cr\0amount\0addTask\0unitName\0taskName\0"
    "start\0end\0color\0reset\0loadFileClicked\0"
    "loadConfigClicked\0selectSysdir\0"
    "saveConfiguration"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Wizard[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    7,   60,    2, 0x06 /* Public */,
      11,    6,   75,    2, 0x06 /* Public */,
      11,    5,   88,    2, 0x26 /* Public | MethodCloned */,
      17,    0,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    0,  100,    2, 0x08 /* Private */,
      19,    0,  101,    2, 0x08 /* Private */,
      20,    0,  102,    2, 0x08 /* Private */,
      21,    0,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Float, QMetaType::Float, QMetaType::Float,    4,    5,    6,    7,    8,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::QString,   12,   13,   14,   15,   10,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Float, QMetaType::Float, QMetaType::Float,   12,   13,   14,   15,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Wizard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Wizard *_t = static_cast<Wizard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->createRepresentation(); break;
        case 1: _t->addFlow((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< float(*)>(_a[6])),(*reinterpret_cast< float(*)>(_a[7]))); break;
        case 2: _t->addTask((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6]))); break;
        case 3: _t->addTask((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5]))); break;
        case 4: _t->reset(); break;
        case 5: _t->loadFileClicked(); break;
        case 6: _t->loadConfigClicked(); break;
        case 7: _t->selectSysdir(); break;
        case 8: _t->saveConfiguration(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Wizard::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wizard::createRepresentation)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Wizard::*_t)(QString , QString , QString , QString , float , float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wizard::addFlow)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Wizard::*_t)(QString , QString , float , float , float , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wizard::addTask)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Wizard::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wizard::reset)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject Wizard::staticMetaObject = {
    { &QWizard::staticMetaObject, qt_meta_stringdata_Wizard.data,
      qt_meta_data_Wizard,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Wizard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Wizard::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Wizard.stringdata0))
        return static_cast<void*>(const_cast< Wizard*>(this));
    return QWizard::qt_metacast(_clname);
}

int Wizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizard::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Wizard::createRepresentation()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Wizard::addFlow(QString _t1, QString _t2, QString _t3, QString _t4, float _t5, float _t6, float _t7)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Wizard::addTask(QString _t1, QString _t2, float _t3, float _t4, float _t5, QString _t6)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 4
void Wizard::reset()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

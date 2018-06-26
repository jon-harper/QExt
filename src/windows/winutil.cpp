#include "winutil.h"
#include <QtCore/QVector>
#include <QtWinExtras/QtWin>
#include <qewindows/types.h>


namespace qe {
namespace windows {
namespace util {

namespace detail {

template <class Out>
using vector_converter_t = std::conditional_t<std::is_pointer_v<Out>, Out, std::add_pointer_t<Out>>;

template <class Out, class In = vector_converter_t<Out>>
QVector<Out> from_variant_vector_helper(ULONG count, In *vec)
{
    auto ret = QVector<Out>(count);
    for (uint32_t i = 0; i < count; i++) {
        ret.append(vec[i]);
    }
    return ret;
}

template <>
QVector<LONGLONG> from_variant_vector_helper<LONGLONG>(ULONG count, LARGE_INTEGER *vec)
{
    auto ret = QVector<LONGLONG>(count);
    for (uint32_t i = 0; i < count; i++) {
        ret.append(vec[i].QuadPart);
    }
    return ret;
}

template <>
QVector<ULONGLONG> from_variant_vector_helper<ULONGLONG>(ULONG count, ULARGE_INTEGER *vec)
{
    auto ret = QVector<ULONGLONG>(count);
    for (uint32_t i = 0; i < count; i++) {
        ret.append(vec[i].QuadPart);
    }
    return ret;
}

template <>
QVector<bool> from_variant_vector_helper<bool>(ULONG count, BOOL *vec)
{
    auto ret = QVector<bool>(count);
    for (uint32_t i = 0; i < count; i++) {
        ret.append(vec[i] ? true : false);
    }
    return ret;
}

template <>
QVector<QDateTime> from_variant_vector_helper<QDateTime>(ULONG count, FILETIME *vec)
{
    auto ret = QVector<QDateTime>(count);
    for (uint32_t i = 0; i < count; i++) {
        ret.append(fromFILETIME(vec[i]));
    }
    return ret;
}

template <>
QVector<QString> from_variant_vector_helper<QString>(ULONG count, BSTR *vec)
{
    auto ret = QVector<QString>(count);
    for (uint32_t i = 0; i < count; i++) {
        ret.append(fromBSTR(vec[i]));
    }
    return ret;
}

template <>
QVector<QVariant> from_variant_vector_helper<QVariant>(ULONG count, PROPVARIANT *vec)
{
    auto ret = QVector<QVariant>(count);
    for (uint32_t i = 0; i < count; i++) {
        ret.append(fromPROPVARIANT(vec[i]));
    }
    return ret;
}

template <class Out, class Container>
QVariant fromVariantVector(Container &vec)
{
    return QVariant::fromValue(from_variant_vector_helper<Out>(vec.cElems, vec.pElems));
}

//template <class In

struct bstr_data
{
    unsigned long cBytes;
    unsigned long clSize;
    wchar_t *asData;
};

} // namespace detail

QString hresultToString(HRESULT err)
{
    return QString(QStrLit("HRESULT 0x%1 %2")).arg(
                QString::number(err & 0x7FFFFFFF, 16),
                QtWin::stringFromHresult(err));
}

QString win32ErrorToString(DWORD err)
{
    return hresultToString(HRESULT_FROM_WIN32(err));
}

QString fromBSTR(BSTR str)
{
    auto data = reinterpret_cast<detail::bstr_data *>(str);
    Q_ASSERT(data);
    return QString::fromWCharArray(data->asData, data->clSize);
}

QDateTime fromFILETIME(const FILETIME &ft)
{
    QDateTime ret(QDate(1601, 1, 1), QTime(0,0,0,0), Qt::UTC);
    qint64 time = (static_cast<qint64>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
    time /= 10000;
    return ret.addMSecs(time);
}

QRect fromRECT(const RECT &r)
{
    return {r.left, r.top, r.right - r.left, r.bottom - r.top};
}

QColor fromCOLORREF(const COLORREF &ref)
{
    return QColor(GetRValue(ref), GetGValue(ref), GetBValue(ref));
}

COLORREF toCOLORREF(const QColor &color)
{
    return RGB(color.red(), color.green(), color.blue());
}

QVariant fromVARIANT(VARIANT &v)
{
    QVariant ret;
    if (v.vt & VT_ARRAY) {
        //SAFEARRAY *parray;
        //SAFEARRAY **pparray;
//            SAFEARRAY FAR*   FAR* pparray;       // VT_ARRAY|*
    } else {
        switch(v.vt) {
        case VT_I1:
            ret.setValue<BYTE>(v.bVal);
            break;
        case VT_I2:
            ret.setValue<SHORT>(v.iVal);
            break;
        case VT_I4:
            ret.setValue<LONG>(v.lVal);
            break;
        case VT_I8:
            ret.setValue<LONGLONG>(v.llVal);
            break;
        case VT_UI1:
            ret.setValue<CHAR>(v.cVal);
            break;
        case VT_UI2:
            ret.setValue<USHORT>(v.uiVal);
            break;
        case VT_UI4:
            ret.setValue<ULONG>(v.ulVal);
            break;
        case VT_UI8:
            ret.setValue<ULONGLONG>(v.ullVal);
            break;
        case VT_INT:
            ret.setValue<INT>(v.intVal);
            break;
        case VT_UINT:
            ret.setValue<UINT>(v.uintVal);
            break;
        case VT_R4:
            ret.setValue<FLOAT>(v.fltVal);
            break;
        case VT_R8:
            ret.setValue<DOUBLE>(v.dblVal);
            break;
        case VT_BOOL:
            ret.setValue<bool>(v.boolVal);
            break;
        case VT_BSTR: {
            ret = fromBSTR(v.bstrVal);
            break;
        }
        case VT_DECIMAL:
        case VT_DATE:
        case VT_CY:
            Q_UNIMPLEMENTED();
            break;
        case VT_BYREF|VT_UI1:
            ret.setValue<unsigned char>(*v.pbVal);
            break;
        case VT_BYREF|VT_I2:
            ret.setValue<short>(*v.piVal);
            break;
        case VT_BYREF|VT_I4:
            ret.setValue<long>(*v.plVal);
            break;
        case VT_BYREF|VT_R4:
            ret.setValue<float>(*v.pfltVal);
            break;
        case VT_BYREF|VT_R8:
            ret.setValue<double>(*v.pdblVal);
            break;
        case VT_BYREF|VT_BOOL:
            ret.setValue<bool>(*v.pboolVal);
            break;
        case VT_BYREF|VT_ERROR:
            ret.setValue<long>(*v.pscode);
            break;
        case VT_BYREF|VT_CY:
            Q_UNIMPLEMENTED();
            //pcyVal
            break;
        case VT_BYREF|VT_DATE:
            Q_UNIMPLEMENTED();
            //pdate
            break;
        case VT_BYREF|VT_BSTR:
            ret = fromBSTR(*v.pbstrVal);
            break;
        case VT_VARIANT | VT_BYREF:
            //this is safe from recursion, as variants can only nest once.
            ret = fromVARIANT(*v.pvarVal);
            break;
        case VT_BYREF|VT_UNKNOWN:
            ret.setValue<UnknownBasePointer>(v.punkVal);
            break;
        case VT_BYREF|VT_DISPATCH:
            ret.setValue<DispatchPointer>(v.pdispVal);
            break;
        case VT_BYREF:
            ret.setValue<void *>(v.byref);
            break;
        case VT_VARIANT: //This should never happen
        case VT_NULL | VT_BYREF: //This should never happen
            Q_UNIMPLEMENTED();
            break;
        //return a null QVariant for these values
        case VT_NULL:
        case VT_ERROR:
        case VT_EMPTY:
            break;
        case VT_DECIMAL | VT_BYREF:
            Q_UNIMPLEMENTED();
            break;
        default:
            break;
        }
    }
    return ret;
}
QVariant fromPROPVARIANT(PROPVARIANT &v)
{
    QVariant ret;
    switch(v.vt)
    {
    // Basic Types.
    case VT_EMPTY:
    case VT_NULL:
        break;
    case VT_I1:         ret.setValue<CHAR>(v.cVal);                 break;
    case VT_UI1:        ret.setValue<UCHAR>(v.bVal);                break;
    case VT_I2:         ret.setValue<SHORT>(v.iVal);                break;
    case VT_UI2:        ret.setValue<USHORT>(v.uiVal);              break;
    case VT_I4:         ret.setValue<LONG>(v.lVal);                 break;
    case VT_UI4:        ret.setValue<ULONG>(v.ulVal);               break;
    case VT_INT:        ret.setValue<INT>(v.intVal);                break;
    case VT_UINT:       ret.setValue<UINT>(v.uintVal);              break;
    case VT_DECIMAL:    Q_UNIMPLEMENTED();                          break;
    case VT_I8:         ret.setValue<LONGLONG>(v.hVal.QuadPart);    break;
    case VT_UI8:        ret.setValue<ULONGLONG>(v.uhVal.QuadPart);  break;
    case VT_R4:         ret.setValue<FLOAT>(v.fltVal);              break;
    case VT_R8:         ret.setValue<DOUBLE>(v.dblVal);             break;
    case VT_BOOL:       ret.setValue<bool>(v.boolVal ? true:false); break;
    case VT_ERROR:      ret.setValue<LONG>(v.scode);                break;
    case VT_CY:         Q_UNIMPLEMENTED();                          break;
        //CY cyVal;
    case VT_DATE:       Q_UNIMPLEMENTED();                          break;
        //DATE date;
    case VT_FILETIME:   ret = fromFILETIME(v.filetime);             break;
    //case VT_CLSID:      ret.setValue<CLSID>(*v.puuid);              break;
        //CLIPDATA * pclipdata;
    case VT_CF:         Q_UNIMPLEMENTED();                          break;
    case VT_BSTR:       ret = fromBSTR(v.bstrVal);                  break;
    case VT_BLOB:       //BLOB blob;
    case VT_BLOB_OBJECT: Q_UNIMPLEMENTED();                         break;
    case VT_LPSTR:      ret = QString(v.pszVal);                    break;
    case VT_LPWSTR:     ret = QString::fromWCharArray(v.pwszVal);   break;
    case VT_UNKNOWN:    ret.setValue<UnknownBasePointer>(v.punkVal);        break;
    case VT_DISPATCH:   ret.setValue<DispatchPointer>(v.pdispVal);      break;
    case VT_STREAM:
    case VT_STREAMED_OBJECT:
        ret.setValue<StreamPointer>(v.pStream);
        break;
    case VT_STORAGE:
    case VT_STORED_OBJECT:
        ret.setValue<StoragePointer>(v.pStorage);
        break;
    case VT_VERSIONED_STREAM:  //LPVERSIONEDSTREAM  pVersionedStream;
        Q_UNIMPLEMENTED();
        break;

    // Vectors of types
    case VT_VECTOR|VT_I1:       ret = detail::fromVariantVector<CHAR, CAC>(v.cac);          break;
    case VT_VECTOR|VT_UI1:      ret = detail::fromVariantVector<UCHAR, CAUB>(v.caub);       break;
    case VT_VECTOR|VT_I2:       ret = detail::fromVariantVector<SHORT, CAI>(v.cai);         break;
    case VT_VECTOR|VT_UI2:      ret = detail::fromVariantVector<USHORT, CAUI>(v.caui);      break;
    case VT_VECTOR|VT_I4:       ret = detail::fromVariantVector<LONG, CAL>(v.cal);          break;
    case VT_VECTOR|VT_UI4:      ret = detail::fromVariantVector<ULONG, CAUL>(v.caul);       break;
    case VT_VECTOR|VT_I8:       ret = detail::fromVariantVector<LONGLONG, CAH>(v.cah);      break;
    case VT_VECTOR|VT_UI8:      ret = detail::fromVariantVector<ULONGLONG, CAUH>(v.cauh);   break;
    case VT_VECTOR|VT_R4:       ret = detail::fromVariantVector<FLOAT, CAFLT>(v.caflt);     break;
    case VT_VECTOR|VT_R8:       ret = detail::fromVariantVector<DOUBLE, CADBL>(v.cadbl);    break;
    case VT_VECTOR|VT_BOOL:     ret = detail::fromVariantVector<BOOL, CABOOL>(v.cabool);    break;
    case VT_VECTOR|VT_ERROR:    ret = detail::fromVariantVector<SCODE, CASCODE>(v.cascode); break;
    case VT_VECTOR|VT_CY:       Q_UNIMPLEMENTED();                                          break;
        //CACY cacy;
    case VT_VECTOR|VT_DATE:     Q_UNIMPLEMENTED();                                          break;
        //CADATE cadate;
    case VT_VECTOR|VT_CLSID:    ret = detail::fromVariantVector<CLSID, CACLSID>(v.cauuid);  break;
    case VT_VECTOR|VT_CF:       Q_UNIMPLEMENTED();                                          break;
        //CACLIPDATA    caclipdata;
    case VT_VECTOR|VT_BSTR:     ret = detail::fromVariantVector<QString, CABSTR>(v.cabstr); break;
    case VT_VECTOR|VT_LPSTR:    ret = detail::fromVariantVector<QString, CALPSTR>(v.calpstr);   break;
    case VT_VECTOR|VT_LPWSTR:   ret = detail::fromVariantVector<QString, CALPWSTR>(v.calpwstr); break;
    case VT_VECTOR|VT_FILETIME:
        ret = detail::fromVariantVector<QDateTime, CAFILETIME>(v.cafiletime);
        break;
    case VT_VECTOR|VT_VARIANT:
        ret = detail::fromVariantVector<QVariant, CAPROPVARIANT>(v.capropvar);
        break;

    // ByRefs of types.
    case VT_BYREF|VT_I1:      ret.setValue<CHAR>(*v.pcVal);                     break;
    case VT_BYREF|VT_UI1:     ret.setValue<UCHAR>(*v.pbVal);                    break;
    case VT_BYREF|VT_I2:      ret.setValue<SHORT>(*v.piVal);                    break;
    case VT_BYREF|VT_UI2:     ret.setValue<USHORT>(*v.puiVal);                  break;
    case VT_BYREF|VT_I4:      ret.setValue<LONG>(*v.plVal);                     break;
    case VT_BYREF|VT_UI4:     ret.setValue<ULONG>(*v.pulVal);                   break;
    case VT_BYREF|VT_INT:     ret.setValue<INT>(*v.pintVal);                    break;
    case VT_BYREF|VT_UINT:    ret.setValue<UINT>(*v.puintVal);                  break;
    case VT_BYREF|VT_R4:      ret.setValue<FLOAT>(*v.pfltVal);                  break;
    case VT_BYREF|VT_R8:      ret.setValue<DOUBLE>(*v.pdblVal);                 break;
    case VT_BYREF|VT_BOOL:    ret.setValue<bool>(*v.pboolVal? true : false);    break;
    case VT_BYREF|VT_DECIMAL: Q_UNIMPLEMENTED();                                break;
    case VT_BYREF|VT_ERROR:   ret.setValue<LONG>(*v.pscode);                    break;
    case VT_BYREF|VT_CY:      Q_UNIMPLEMENTED();                                break;
                              //CY* pcyVal;
    case VT_BYREF|VT_DATE:    Q_UNIMPLEMENTED();                                break;
                              //DATE* pdate;
    case VT_BYREF|VT_BSTR:    ret = fromBSTR(*v.pbstrVal);                      break;
    case VT_BYREF|VT_UNKNOWN: ret.setValue<UnknownBasePointer>(*v.ppunkVal);    break;
    case VT_BYREF|VT_DISPATCH:ret.setValue<DispatchPointer>(*v.ppdispVal);      break;
    case VT_BYREF|VT_ARRAY:   Q_UNIMPLEMENTED();                                break;
                              //LPSAFEARRAY*    pparray;
    case VT_BYREF|VT_VARIANT: ret = fromPROPVARIANT(*v.pvarVal);                break;
    default:
        break;
    };
    return ret;
};

//VT_EMPTY            [V]   [P]
//VT_NULL             [V]   [P]     SQL style Null
//VT_I2               [V][T][P][S]  SHORT
//VT_I4               [V][T][P][S]  4 byte signed int
//VT_R4               [V][T][P][S]  4 byte real
//VT_R8               [V][T][P][S]  8 byte real

//VT_CY               [V][T][P][S]  currency
//VT_DATE             [V][T][P][S]  date
//VT_BSTR             [V][T][P][S]  BSTR
//VT_ERROR            [V][T][P][S]  SCODE
//VT_BOOL             [V][T][P][S]  BOOL
//VT_VARIANT          [V][T][P][S]  VARIANT *
//VT_RECORD           [V]   [P][S]  user defined type

//VT_I1               [V][T][P][s]  signed char
//VT_UI1              [V][T][P][S]  unsigned char
//VT_UI2              [V][T][P][S]  unsigned short
//VT_UI4              [V][T][P][S]  unsigned long
//VT_I8                  [T][P]     signed 64-bit int
//VT_UI8                 [T][P]     unsigned 64-bit int
//VT_INT              [V][T][P][S]  signed machine int
//VT_LPSTR               [T][P]     null terminated string
//VT_LPWSTR              [T][P]     wide null terminated string
//VT_FILETIME               [P]     FILETIME
//VT_BLOB                   [P]     Length prefixed bytes
//VT_STREAM                 [P]     Name of the stream follows
//VT_STORAGE                [P]     Name of the storage follows
//VT_STREAMED_OBJECT        [P]     Stream contains an object
//VT_STORED_OBJECT          [P]     Storage contains an object
//VT_VERSIONED_STREAM       [P]     Stream with a GUID version
//VT_BLOB_OBJECT            [P]     Blob contains an object
//VT_CF                     [P]     Clipboard format
//VT_CLSID                  [P]     A Class ID
//VT_VECTOR                 [P]     simple counted array

} // namespace util
} // namespace windows
} // namespace qe

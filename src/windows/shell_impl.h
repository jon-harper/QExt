#ifndef QE_WINDOWS_SHELL_IMPL_H
#define QE_WINDOWS_SHELL_IMPL_H

#include <utility>
#define STRICT_TYPED_ITEMIDS
#include <ShlObj.h>

struct IThumbnailProvider;
struct IQueryAssociation;
struct IFilter;

namespace qe {
namespace windows {
namespace shell {
namespace {

template <class T>
GUID bindingGuid();

template<> GUID bindingGuid<::IStream>()                { return BHID_Stream; }
template<> GUID bindingGuid<::IStorage>()               { return BHID_Storage; }
template<> GUID bindingGuid<::IEnumShellItems>()        { return BHID_StorageEnum; }
template<> GUID bindingGuid<::ITransferSource>()        { return BHID_Transfer; }
template<> GUID bindingGuid<::ITransferDestination>()   { return BHID_Transfer; }
template<> GUID bindingGuid<::IPropertyStore>()         { return BHID_PropertyStore; }
template<> GUID bindingGuid<::IPropertyStoreFactory>()  { return BHID_PropertyStore; }
template<> GUID bindingGuid<::IExtractImage>()          { return BHID_ThumbnailHandler; }
template<> GUID bindingGuid<::IThumbnailProvider>()     { return BHID_ThumbnailHandler; }
template<> GUID bindingGuid<::IDataObject>()            { return BHID_DataObject; }
template<> GUID bindingGuid<::IQueryAssociation>()      { return BHID_AssociationArray; }
template<> GUID bindingGuid<::IEnumAssocHandlers>()     { return BHID_EnumAssocHandlers; }
template<> GUID bindingGuid<::IFilter>()                { return BHID_Filter; }

} // namespace detail
} // namespace shell
} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELL_IMPL_H
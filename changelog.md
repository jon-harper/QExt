# Changelog

### 2018-05-28
* qecore/managedpointer:Merged ManagedPointer
* Adapted Qt's QScopedPointer*Deleter types
* qecore/pointer_deleters: Broke deleters off into separate header
* Updated test files, added one for `ManagedPointer`
* Doxygen now less ugly.
* qewindows/types: Proper clutter names for qe::windows type aliases.
* qewindows/shellutil: added desktop() method
* qewindows/global: Added `STRICT_TYPED_ITEMIDS` `#define`
* qewindows/compointer: `IdListPointer` is now copyable and derives from `ManagedPointer`.
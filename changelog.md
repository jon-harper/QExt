# Changelog

### 2018-05-31
* Blocked off the windows module from qext.pro with a `win32` include condition.

### 2018-05-30
* Added some basic functions to `ShellNodeInfo`.

### 2018-05-29
* Moved qe::windows::util to qe::windows::shell
* qewindows/shellutil: renamed to shell.h
* qewindows/shellnode: simplified data storage to swappable LocalData struct
* qewindows/unknownpointer: fixed a minor bug on copy
* qewindows/shellnodedata: lots of fixes on construction
* qewindows/shellnodedata: flags populate on `refresh()` now.

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
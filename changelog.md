# Changelog

### 2018-06-07
* qewindows/shell: added `createBindContext()`
* Created test_shellcache.h in the test project.
* Lots of fixes.
* Still working at auto-populating the cache on `ShellNode` creation.
* qewindows/types: renamed `ShellItemPointer` to `ShellItem2Pointer` to reflect
the obvious.
* A lot of work narrowing construction points (and thus insertions) for `ShellNode`.
* qewindows/shellcache: Worked on parent/child relationship during node construction
* qewindows/shellnode: some refactoring
* Documented some things in need of documenting.

### 2018-06-05
* qewindows/shellcache: worked on storing data as a `QMap` instead of a `QHash`.

### 2018-06-04
* qewindows/shellcache: added `keyFor` for parsing pathnames.

### 2018-06-03
* qecore/type_util: cleaned up and removed everything from the ::detail namespace.
* qewindows/shell: added idListFromUnknown free function
* qewindows/shell: added MayHaveChildren flag
* qewindows/shellnodedata: updated for MayHaveChildren flag
* qewindows/shell: NodeFlag has proper doxygen comments now

### 2018-06-02
* Major doxygen fixes, particularly with font faces
* qecore/managedpointer: Documented and cleaned up.
* qewindows/compointer: BStrPointer and WCharPointer are now template types of
ManagedPointer and can thus be copied.
* Implemented the skeletons of ShellCache and ShellNode

### 2018-06-01
* Began work on qewindows/shellcache
* Cleaned up some pointer issues
* qewindows/compointer: BStrPointer and WCharPointer are copyable now and inherit
from qe::ManagedPointer. They are also declared as metatypes.

### 2018-05-31
* Blocked off the windows module from qext.pro with a `win32` include condition.
* Created shell branch

### 2018-05-30
* Added some basic functions to `ShellNodeInfo`.
* qewindows/shellnodedata: fleshed out `refresh()`

### 2018-05-29
* Moved qe::windows::util to qe::windows::shell
* qewindows/shellutil: renamed to shell.h
* qewindows/shellnode: simplified data storage to swappable LocalData struct
* qewindows/unknownpointer: fixed a minor bug on copy
* qewindows/shellnodedata: lots of fixes on construction
* qewindows/shellnodedata: flags populate on `refresh()` now.

### 2018-05-28
* qecore/managedpointer: Merged ManagedPointer
* Adapted Qt's QScopedPointer*Deleter types
* qecore/pointer_deleters: Broke deleters off into separate header
* Updated test files, added one for `ManagedPointer`
* Doxygen now less ugly.
* qewindows/types: Proper clutter names for qe::windows type aliases.
* qewindows/shellutil: added desktop() method
* qewindows/global: Added `STRICT_TYPED_ITEMIDS` `#define`
* qewindows/compointer: `IdListPointer` is now copyable and derives from `ManagedPointer`.
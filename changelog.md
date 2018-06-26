# Changelog

### 2018-06-26
* qewindows/shellnode: added comparison operators
* qewindows/shell: working on a proper templated implementation of `compareItems`
* test/windows/*: some cleanup here, added to test_shellnode for comarison operators
* qewindows/shell: added inline getters (`retrieveItemPointer`) in shellnode.h and shellnodedata.h to retrieve IShellItem pointers from both ShellNode and ShellNodeData and their QSharedPointer typedefs.
* Prettied up the generated doxygen class files.

### 2018-06-25
* qewindows/shellnode: Lots of documentation for ShellNode
* qewindows/shellnode: Added `childCount()`, `childIndex()` and `childAt()`
* test/windows/shellnode: updated test harness

### 2018-06-24
* qewindows/shellnodebuilder: removed
* qewindows/shellcache: removed
* qewindows/shellnodedata: added equality operators, removed cruft
* qewindows/shellnode: added equality operators, removed cruft
* qewindows/idlist: added quite a bit of functionality from the sandbox and
updated unit test
* qewindows/shell: converted to IdList from IdListPointer, added `compareItems()` 
* qewindows/shellnodeinfo: updated to use IdList
* qewindows/types: removed IdListPointer, added `#include` for IdList.
* test.h: added `flag_check()` lambda and `EXPECT_FLAG()` macro for binary
AND testing strongly typed enums.
* Lots of documentation, particularly IdList.

### 2018-06-23
* Finished unaligned.h, which helps with the `__unaligned` specifier on x86-64 
Windows platforms.
* Finished idlist.h and idlist_impl.h, which are part of qewindows/idlist, to
replace the IdListPointer.

### 2018-06-22
* Pushing the few updates I've made while working on `shell::IdList`
* `UnknownPointer` is better documented.
* qewindows/shell: `desktopFolder()` and `desktopItem()` are now backed by `thread_local`
variables.
* qewindows/shell: added `itemParent()` and `idListParent()` functions.

### 2018-06-15 - 21
* Working on an alternative implementation of IdListPointer that behaves more
like a trivial, concrete type and less like a pointer.
* Need to strip out `ShellCache`, etc. and leave `ShellNode`, `ShellNodeData`, and
`ShellNodeInfo`.
* Will start this after I have a working test harness for shell::IdList

### 2018-06-13
* qewindows/shell: added `idListParent()`.

### 2018-06-12
* qewindows/shellnodebuilder: implemented most of the work for the class.
* qewindows/shell: added `itemParent()` function.
* qewindows/shellnode: delegated construction to `ShellNodeBuilder`.

### 2018-06-11
* qewindows/shellnode: added comparison operators.
* qewindows/unknownpointer: refactoring and documentation.
* Working towards a node/cache builder/manager that accepts some arguments
and constructs a new node.
* qewindows/shell: hopefully fixing `bindingGuid()`.

### 2018-06-10
* Fixed the linking error, FINALLY (missed a `STRICT_TYPED_ITEMIDS`).
* qewindows/shellcache: implemented `insert()`.
* qewindows/shellcache: moved `createNode()` to `ShellCachePrivate`.
* qewindows/compointer: removed string functions and inlined the code.
* qewindows/shell: added `knownFolderIdList()` and `knownFolderItem()`.
* qewindows/shell: added `bindTo()` and `bindToObject()`.
* added shell_impl.h to help with `qe::windows::shell::bindTo()`.
* qewindows/shellnode: started work on `enumerate()`.

### 2018-06-09
* Minor doxygen config tweaks.
* Documented a few undocumented function calls.
* qewindows/shell: Enums are properly documented now.
* Minor doxygen style updates.
* ShellNode now inherits from `QEnableSharedFromThis`.
* Still working on the linking error.

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
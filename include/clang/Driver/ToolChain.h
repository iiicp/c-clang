//===--- ToolChain.h - Collections of tools for one platform ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_DRIVER_TOOLCHAIN_H_
#define CLANG_DRIVER_TOOLCHAIN_H_

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Path.h"
#include <string>

namespace clang {
namespace driver {
  class Compilation;
  class DerivedArgList;
  class HostInfo;
  class InputArgList;
  class JobAction;
  class Tool;

/// ToolChain - Access to tools for a single platform.
class ToolChain {
public:
  typedef llvm::SmallVector<std::string, 4> path_list;

private:
  const HostInfo &Host;
  const llvm::Triple Triple;

  /// The list of toolchain specific path prefixes to search for
  /// files.
  path_list FilePaths;

  /// The list of toolchain specific path prefixes to search for
  /// programs.
  path_list ProgramPaths;

protected:
  ToolChain(const HostInfo &Host, const llvm::Triple &_Triple);

public:
  virtual ~ToolChain();

  // Accessors

  const HostInfo &getHost() const { return Host; }
  const llvm::Triple &getTriple() const { return Triple; }

  std::string getArchName() const { return Triple.getArchName(); }
  std::string getPlatform() const { return Triple.getVendorName(); }
  std::string getOS() const { return Triple.getOSName(); }

  std::string getTripleString() const {
    return Triple.getTriple();
  }

  path_list &getFilePaths() { return FilePaths; }
  const path_list &getFilePaths() const { return FilePaths; }

  path_list &getProgramPaths() { return ProgramPaths; }
  const path_list &getProgramPaths() const { return ProgramPaths; }

  // Tool access.

  /// TranslateArgs - Create a new derived argument list for any
  /// argument translations this ToolChain may wish to perform.
  virtual DerivedArgList *TranslateArgs(InputArgList &Args) const = 0;

  /// SelectTool - Choose a tool to use to handle the action \arg JA.
  virtual Tool &SelectTool(const Compilation &C, const JobAction &JA) const = 0;

  // Helper methods

  llvm::sys::Path GetFilePath(const Compilation &C, const char *Name) const;
  llvm::sys::Path GetProgramPath(const Compilation &C, const char *Name,
                                 bool WantFile = false) const;

  // Platform defaults information

  /// IsMathErrnoDefault - Does this tool chain set -fmath-errno by
  /// default.
  virtual bool IsMathErrnoDefault() const = 0;

  /// IsUnwindTablesDefault - Does this tool chain use -funwind-tables
  /// by default.
  virtual bool IsUnwindTablesDefault() const = 0;

  /// GetDefaultRelocationModel - Return the LLVM name of the default
  /// relocation model for this tool chain.
  virtual const char *GetDefaultRelocationModel() const = 0;

  /// GetForcedPicModel - Return the LLVM name of the forced PIC model
  /// for this tool chain, or 0 if this tool chain does not force a
  /// particular PIC mode.
  virtual const char *GetForcedPicModel() const = 0;
};

} // end namespace driver
} // end namespace clang

#endif

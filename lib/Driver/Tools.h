//===--- Tools.h - Tool Implementations -------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_LIB_DRIVER_TOOLS_H_
#define CLANG_LIB_DRIVER_TOOLS_H_

#include "clang/Driver/Tool.h"
#include "clang/Driver/Types.h"
#include "clang/Driver/Util.h"

#include "llvm/Support/Compiler.h"

namespace clang {
namespace driver {
  class Driver;

namespace toolchains {
  class Darwin_X86;
}

namespace tools {

  class LLVM_LIBRARY_VISIBILITY Clang : public Tool {
    void AddPreprocessingOptions(const Driver &D,
                                 const ArgList &Args,
                                 ArgStringList &CmdArgs,
                                 const InputInfo &Output,
                                 const InputInfoList &Inputs) const;

  public:
    Clang(const ToolChain &TC) : Tool("clang", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return true; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };

  /// gcc - Generic GCC tool implementations.
namespace gcc {
  class LLVM_LIBRARY_VISIBILITY Common : public Tool {
  public:
    Common(const char *Name, const ToolChain &TC) : Tool(Name, TC) {}

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;

    /// RenderExtraToolArgs - Render any arguments necessary to force
    /// the particular tool mode.
    virtual void RenderExtraToolArgs(ArgStringList &CmdArgs) const = 0;
  };

  
  class LLVM_LIBRARY_VISIBILITY Preprocess : public Common {
  public:
    Preprocess(const ToolChain &TC) : Common("gcc::Preprocess", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void RenderExtraToolArgs(ArgStringList &CmdArgs) const;
  };

  class LLVM_LIBRARY_VISIBILITY Precompile : public Common  {
  public:
    Precompile(const ToolChain &TC) : Common("gcc::Precompile", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return false; }
    virtual bool hasIntegratedCPP() const { return true; }

    virtual void RenderExtraToolArgs(ArgStringList &CmdArgs) const;
  };

  class LLVM_LIBRARY_VISIBILITY Compile : public Common  {
  public:
    Compile(const ToolChain &TC) : Common("gcc::Compile", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return true; }

    virtual void RenderExtraToolArgs(ArgStringList &CmdArgs) const;
  };

  class LLVM_LIBRARY_VISIBILITY Assemble : public Common  {
  public:
    Assemble(const ToolChain &TC) : Common("gcc::Assemble", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return false; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void RenderExtraToolArgs(ArgStringList &CmdArgs) const;
  };

  class LLVM_LIBRARY_VISIBILITY Link : public Common  {
  public:
    Link(const ToolChain &TC) : Common("gcc::Link", TC) {}

    virtual bool acceptsPipedInput() const { return false; }
    virtual bool canPipeOutput() const { return false; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void RenderExtraToolArgs(ArgStringList &CmdArgs) const;
  };
} // end namespace gcc

namespace darwin {
  class LLVM_LIBRARY_VISIBILITY CC1 : public Tool  {
  public:
    static const char *getBaseInputName(const ArgList &Args, 
                                 const InputInfoList &Input);
    static const char *getBaseInputStem(const ArgList &Args, 
                                 const InputInfoList &Input);
    static const char *getDependencyFileName(const ArgList &Args, 
                                             const InputInfoList &Inputs);

  protected:
    const char *getCC1Name(types::ID Type) const;

    void AddCC1Args(const ArgList &Args, ArgStringList &CmdArgs) const;
    void AddCC1OptionsArgs(const ArgList &Args, ArgStringList &CmdArgs,
                           const InputInfoList &Inputs,
                           const ArgStringList &OutputArgs) const;
    void AddCPPOptionsArgs(const ArgList &Args, ArgStringList &CmdArgs,
                           const InputInfoList &Inputs,
                           const ArgStringList &OutputArgs) const;
    void AddCPPUniqueOptionsArgs(const ArgList &Args, 
                                 ArgStringList &CmdArgs,
                                 const InputInfoList &Inputs) const;
    void AddCPPArgs(const ArgList &Args, ArgStringList &CmdArgs) const;

  public:
    CC1(const char *Name, const ToolChain &TC) : Tool(Name, TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return true; }
  };

  class LLVM_LIBRARY_VISIBILITY Preprocess : public CC1  {
  public:
    Preprocess(const ToolChain &TC) : CC1("darwin::Preprocess", TC) {}

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };

  class LLVM_LIBRARY_VISIBILITY Compile : public CC1  {
  public:
    Compile(const ToolChain &TC) : CC1("darwin::Compile", TC) {}

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };

  class LLVM_LIBRARY_VISIBILITY Assemble : public Tool  {
  public:
    Assemble(const ToolChain &TC) : Tool("darwin::Assemble", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return false; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };

  class LLVM_LIBRARY_VISIBILITY Link : public Tool  {
    void AddDarwinArch(const ArgList &Args, ArgStringList &CmdArgs) const;
    void AddDarwinSubArch(const ArgList &Args, ArgStringList &CmdArgs) const;
    void AddLinkArgs(const ArgList &Args, ArgStringList &CmdArgs) const;

    /// The default macosx-version-min.
    const char *MacosxVersionMin;

    const toolchains::Darwin_X86 &getDarwinToolChain() const;

  public:
    Link(const ToolChain &TC,
         const char *_MacosxVersionMin) 
      : Tool("darwin::Link", TC), MacosxVersionMin(_MacosxVersionMin) {
    }

    virtual bool acceptsPipedInput() const { return false; }
    virtual bool canPipeOutput() const { return false; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };

  class LLVM_LIBRARY_VISIBILITY Lipo : public Tool  {
  public:
    Lipo(const ToolChain &TC) : Tool("darwin::Lipo", TC) {}

    virtual bool acceptsPipedInput() const { return false; }
    virtual bool canPipeOutput() const { return false; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };
}

  /// openbsd -- Directly call GNU Binutils assembler and linker
namespace openbsd {
  class LLVM_LIBRARY_VISIBILITY Assemble : public Tool  {
  public:
    Assemble(const ToolChain &TC) : Tool("openbsd::Assemble", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };
  class LLVM_LIBRARY_VISIBILITY Link : public Tool  {
  public:
    Link(const ToolChain &TC) : Tool("openbsd::Link", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };
} // end namespace openbsd

  /// freebsd -- Directly call GNU Binutils assembler and linker
namespace freebsd {
  class LLVM_LIBRARY_VISIBILITY Assemble : public Tool  {
  public:
    Assemble(const ToolChain &TC) : Tool("freebsd::Assemble", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };
  class LLVM_LIBRARY_VISIBILITY Link : public Tool  {
  public:
    Link(const ToolChain &TC) : Tool("freebsd::Link", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };
} // end namespace freebsd

  /// auroraux -- Directly call GNU Binutils assembler and linker
namespace auroraux {
  class LLVM_LIBRARY_VISIBILITY Assemble : public Tool  {
  public:
    Assemble(const ToolChain &TC) : Tool("auroraux::Assemble", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };
  class LLVM_LIBRARY_VISIBILITY Link : public Tool  {
  public:
    Link(const ToolChain &TC) : Tool("auroraux::Link", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };
} // end namespace auroraux

  /// dragonfly -- Directly call GNU Binutils assembler and linker
namespace dragonfly {
  class LLVM_LIBRARY_VISIBILITY Assemble : public Tool  {
  public:
    Assemble(const ToolChain &TC) : Tool("dragonfly::Assemble", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };
  class LLVM_LIBRARY_VISIBILITY Link : public Tool  {
  public:
    Link(const ToolChain &TC) : Tool("dragonfly::Link", TC) {}

    virtual bool acceptsPipedInput() const { return true; }
    virtual bool canPipeOutput() const { return true; }
    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              Job &Dest,
                              const InputInfo &Output, 
                              const InputInfoList &Inputs, 
                              const ArgList &TCArgs, 
                              const char *LinkingOutput) const;
  };
} // end namespace dragonfly

} // end namespace toolchains
} // end namespace driver
} // end namespace clang

#endif // CLANG_LIB_DRIVER_TOOLS_H_

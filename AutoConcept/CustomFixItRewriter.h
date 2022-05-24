#pragma once

#include "clang/Rewrite/Frontend/FixItRewriter.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/LLVM.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Edit/Commit.h"
#include "clang/Edit/EditsReceiver.h"
#include "clang/Frontend/FrontendDiagnostic.h"
#include "clang/Rewrite/Core/RewriteBuffer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdio>
#include <memory>
#include <string>
#include <system_error>
#include <utility>

#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/LLVM.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Edit/EditedSource.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <type_traits>




namespace auto_concept {

    using namespace clang;

    // A fixit rewriter which can be silent
    struct CustomFixItRewriter : public FixItRewriter {

        clang::DiagnosticsEngine& DiagnosticsEngine;
        bool silentMode = false;

        template <typename... Args>
        CustomFixItRewriter(bool silentMode, clang::DiagnosticsEngine& DiagnosticsEngine, Args&&... args) :
            silentMode{ silentMode },
            DiagnosticsEngine{ DiagnosticsEngine },
            FixItRewriter(DiagnosticsEngine, std::forward<Args>(args)...) {}


        void HandleDiagnostic(DiagnosticsEngine::Level DiagLevel,
            const Diagnostic& Info) override {

            bool prevMode = DiagnosticsEngine.getSuppressAllDiagnostics();
            if (silentMode) DiagnosticsEngine.setSuppressAllDiagnostics(true);
            FixItRewriter::HandleDiagnostic(DiagLevel, Info);
            DiagnosticsEngine.setSuppressAllDiagnostics(prevMode);

        }

        virtual ~CustomFixItRewriter() {}
    };

}
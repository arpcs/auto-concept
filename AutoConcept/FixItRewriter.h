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


namespace auto_concept {

	class CustomFixitRewriter : public clang::FixItRewriter {
	public:
		template <typename... Args>
		CustomFixitRewriter(Args&&... args) : clang::FixItRewriter(std::forward<Args>(args)...) {
		}
		virtual ~CustomFixitRewriter() {}

		void Diag(clang::SourceLocation Loc, unsigned DiagID) {
			if (DiagID == clang::diag::note_fixit_applied) return;
			//clang::FixItRewriter::Diag(Loc, DiagID);
		}

	};
}
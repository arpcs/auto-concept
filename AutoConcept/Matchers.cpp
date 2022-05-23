#include <string>
#include <vector>
#include "Matchers.h"
#include "clang/ASTMatchers/ASTMatchers.h"

namespace auto_concept {
    using namespace llvm;
    using namespace std;
    using namespace clang::ast_matchers;

    vector<DeclarationMatcher> GetMatchersGuessed() {

        // Don't change matchers
        vector<DeclarationMatcher> matchers;

        matchers.push_back(
            functionTemplateDecl(
                unless(
                    anyOf(
                        hasParent(
                            isInStdNamespace()
                        ),
                        isInStdNamespace(),
                        isExpansionInSystemHeader()
                    )
                ),
                isExpansionInMainFile()
                ,
                forEach(templateTypeParmDecl(
                ).bind("asdf"))
            ).bind("functionTemplateDecl2")
        );


        return matchers;
    }

}
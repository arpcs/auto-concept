#include <string>
#include <vector>
#include "Matchers.h"
#include "clang/ASTMatchers/ASTMatchers.h"

namespace auto_concept {
    using namespace llvm;
    using namespace std;
    using namespace clang::ast_matchers;

    vector<DeclarationMatcher> GetMatchersGuessed() {

        vector<DeclarationMatcher> matchers;

        matchers.push_back(
            functionTemplateDecl(
                unless(
                    anyOf(
                        hasParent(
                            isInStdNamespace()
                        ),
                        isInStdNamespace(),
                        hasDescendant(
                            nonTypeTemplateParmDecl()
                        )
                    )
                )
            ).bind("Trivial FunctionTemplateDecl")
        );


        return matchers;
    }

}
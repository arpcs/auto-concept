
#include <string>
#include <vector>
#include "Matchers.h"
#include "clang/ASTMatchers/ASTMatchers.h"

namespace auto_concept {
    using namespace llvm;
    using namespace std;
    using namespace clang::ast_matchers;

    vector<DeclarationMatcher> GetMatchersDeduced(){

        // Don't change matchers
        vector<DeclarationMatcher> matchers;

        matchers.push_back(
            functionTemplateDecl(
                isExpansionInMainFile(), 
                forEachDescendant( varDecl( 
                    hasDescendant( templateSpecializationType(
                        hasDeclaration( 
                            classTemplateDecl(
                                hasName("numeric_limits"),
                                isInStdNamespace()
                            )
                        )
                    )),
                    hasDescendant( templateTypeParmType(
                        hasDeclaration(
                            templateTypeParmDecl().bind("templateTypeParmDecl arithmetic")
                        )
                    ))
                )),
                forEach(templateTypeParmDecl(
                    equalsBoundNode("templateTypeParmDecl arithmetic")
                ))
            ).bind("functionTemplateDecl rewrite")
        );


        return matchers;
    }

}

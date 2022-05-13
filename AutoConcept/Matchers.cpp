
// Linking is pretty slow and we have to change matchers frequently
// So to mitigate this we use this file to build matchers dynamically from string or we just pass the matchers
// This is controlled by the AUTOCONCEPT_FAST_DYNAMIC_BUILD macro.
// We turn on intellisense on this file nevertheless to still get errors
#if defined(__INTELLISENSE__) || !defined(AUTOCONCEPT_FAST_DYNAMIC_BUILD)

#   include <string>
#   include <vector>
#   include "Matchers.h"
#   include "clang/ASTMatchers/ASTMatchers.h"

    namespace auto_concept {
        using namespace llvm;
        using namespace std;
        using namespace clang::ast_matchers;

        vector<DeclarationMatcher> GetMatchers(){

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

#endif

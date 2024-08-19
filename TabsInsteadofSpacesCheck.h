#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_TABSINSTEADOFTSPACESCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_TABSINSTEADOFTSPACESCHECK_H

#include "clang-tidy/ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace tabsinsteadofspaces {

class TabsInsteadOfSpacesCheck : public ClangTidyCheck {
public:
    TabsInsteadOfSpacesCheck(StringRef Name, ClangTidyContext *Context);
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
    void checkForSpaces(const SourceManager &SM, FileID FID, StringRef Buffer) const;
    void checkForMixedIndentation(const SourceManager &SM, FileID FID, StringRef Buffer) const;
    void checkForTrailingWhitespace(const SourceManager &SM, FileID FID, StringRef Buffer) const;

    unsigned int SpacesPerIndent;
    unsigned int MaxLineLength;
};

} // namespace tabsinsteadofspaces
} // namespace tidy
} // namespace clang

#endif

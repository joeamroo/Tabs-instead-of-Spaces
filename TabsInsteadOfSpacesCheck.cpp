#include "TabsInsteadOfSpacesCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Regex.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace tabsinsteadofspaces {

TabsInsteadOfSpacesCheck::TabsInsteadOfSpacesCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context) {
    SpacesPerIndent = Options.get("SpacesPerIndent", 4);
    MaxLineLength = Options.get("MaxLineLength", 80);
}


void TabsInsteadOfSpacesCheck::registerMatchers(MatchFinder *Finder) {
}

void TabsInsteadOfSpacesCheck::check(const MatchFinder::MatchResult &Result) {
    const SourceManager &SM = *Result.SourceManager;
    FileID FID = SM.getMainFileID();
    StringRef Buffer = SM.getBufferData(FID);

    checkForSpaces(SM, FID, Buffer);
    checkForMixedIndentation(SM, FID, Buffer);
    checkForTrailingWhitespace(SM, FID, Buffer);
}

void TabsInsteadOfSpacesCheck::checkForSpaces(const SourceManager &SM, FileID FID, StringRef Buffer) const {
    for (size_t lineStart = 0, lineEnd = 0; lineEnd < Buffer.size(); lineStart = lineEnd + 1) {
        lineEnd = Buffer.find('\n', lineStart);
        if (lineEnd == StringRef::npos) lineEnd = Buffer.size();

        StringRef line = Buffer.slice(lineStart, lineEnd);
        for (char c : line) {
            if (c == ' ') {
                DiagnosticsEngine &DE = SM.getDiagnostics();
                unsigned ID = DE.getCustomDiagID(DiagnosticsEngine::Error, "Space character found, use tabs instead.");
                DE.Report(SM.getLocForStartOfFile(FID).getLocWithOffset(lineStart), ID);
                break;
            }
            if (c != ' ' && c != '\t') break;
        }
    }
}

void TabsInsteadOfSpacesCheck::checkForMixedIndentation(const SourceManager &SM, FileID FID, StringRef Buffer) const {
    for (size_t lineStart = 0, lineEnd = 0; lineEnd < Buffer.size(); lineStart = lineEnd + 1) {
        lineEnd = Buffer.find('\n', lineStart);
        if (lineEnd == StringRef::npos) lineEnd = Buffer.size();

        StringRef line = Buffer.slice(lineStart, lineEnd);
        bool foundTab = false, foundSpace = false;
        for (char c : line) {
            if (c == '\t') foundTab = true;
            if (c == ' ') foundSpace = true;
            if (c != '\t' && c != ' ') break;
        }
        if (foundTab && foundSpace) {
            DiagnosticsEngine &DE = SM.getDiagnostics();
            unsigned ID = DE.getCustomDiagID(DiagnosticsEngine::Warning, "Mixed tabs and spaces found in indentation.");
            DE.Report(SM.getLocForStartOfFile(FID).getLocWithOffset(lineStart), ID);
        }
    }
}

void TabsInsteadOfSpacesCheck::checkForTrailingWhitespace(const SourceManager &SM, FileID FID, StringRef Buffer) const {
    for (size_t lineStart = 0, lineEnd = 0; lineEnd < Buffer.size(); lineStart = lineEnd + 1) {
        lineEnd = Buffer.find('\n', lineStart);
        if (lineEnd == StringRef::npos) lineEnd = Buffer.size();

        StringRef line = Buffer.slice(lineStart, lineEnd);
        if (line.ends_with(" ") || line.ends_with("\t")) {
            DiagnosticsEngine &DE = SM.getDiagnostics();
            unsigned ID = DE.getCustomDiagID(DiagnosticsEngine::Warning, "Trailing whitespace found.");
            DE.Report(SM.getLocForStartOfFile(FID).getLocWithOffset(lineEnd - 1), ID);
        }
    }
}

} // namespace tabsinsteadofspaces
} // namespace tidy
} // namespace clang
